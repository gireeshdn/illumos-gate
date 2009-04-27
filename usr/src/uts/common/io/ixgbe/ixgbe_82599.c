/*
 * CDDL HEADER START
 *
 * Copyright(c) 2007-2009 Intel Corporation. All rights reserved.
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at:
 *      http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When using or redistributing this file, you may do so under the
 * License only. No other modification of this header is permitted.
 *
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */

/*
 * Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

/* IntelVersion: 1.155 v2-7-8_2009-4-7 */

#include "ixgbe_type.h"
#include "ixgbe_api.h"
#include "ixgbe_common.h"
#include "ixgbe_phy.h"

s32 ixgbe_init_ops_82599(struct ixgbe_hw *hw);
s32 ixgbe_get_link_capabilities_82599(struct ixgbe_hw *hw,
    ixgbe_link_speed *speed, bool *autoneg);
enum ixgbe_media_type ixgbe_get_media_type_82599(struct ixgbe_hw *hw);
s32 ixgbe_setup_mac_link_multispeed_fiber(struct ixgbe_hw *hw);
s32 ixgbe_setup_mac_link_speed_multispeed_fiber(struct ixgbe_hw *hw,
    ixgbe_link_speed speed, bool autoneg, bool autoneg_wait_to_complete);
s32 ixgbe_setup_mac_link_82599(struct ixgbe_hw *hw);
s32 ixgbe_check_mac_link_82599(struct ixgbe_hw *hw,
    ixgbe_link_speed *speed,
    bool *link_up, bool link_up_wait_to_complete);
s32 ixgbe_setup_mac_link_speed_82599(struct ixgbe_hw *hw,
    ixgbe_link_speed speed, bool autoneg,
    bool autoneg_wait_to_complete);
static s32 ixgbe_setup_copper_link_82599(struct ixgbe_hw *hw);
static s32 ixgbe_setup_copper_link_speed_82599(struct ixgbe_hw *hw,
    ixgbe_link_speed speed, bool autoneg,
    bool autoneg_wait_to_complete);
s32 ixgbe_setup_sfp_modules_82599(struct ixgbe_hw *hw);
void ixgbe_init_mac_link_ops_82599(struct ixgbe_hw *hw);
s32 ixgbe_reset_hw_82599(struct ixgbe_hw *hw);
s32 ixgbe_set_vmdq_82599(struct ixgbe_hw *hw, u32 rar, u32 vmdq);
s32 ixgbe_clear_vmdq_82599(struct ixgbe_hw *hw, u32 rar, u32 vmdq);
s32 ixgbe_insert_mac_addr_82599(struct ixgbe_hw *hw, u8 *addr, u32 vmdq);
s32 ixgbe_set_vfta_82599(struct ixgbe_hw *hw, u32 vlan,
    u32 vind, bool vlan_on);
s32 ixgbe_clear_vfta_82599(struct ixgbe_hw *hw);
s32 ixgbe_init_uta_tables_82599(struct ixgbe_hw *hw);
s32 ixgbe_read_analog_reg8_82599(struct ixgbe_hw *hw, u32 reg, u8 *val);
s32 ixgbe_write_analog_reg8_82599(struct ixgbe_hw *hw, u32 reg, u8 val);
s32 ixgbe_start_hw_rev_1_82599(struct ixgbe_hw *hw);
s32 ixgbe_identify_phy_82599(struct ixgbe_hw *hw);
s32 ixgbe_init_phy_ops_82599(struct ixgbe_hw *hw);
u32 ixgbe_get_supported_physical_layer_82599(struct ixgbe_hw *hw);
s32 ixgbe_enable_rx_dma_82599(struct ixgbe_hw *hw, u32 regval);
s32 ixgbe_get_san_mac_addr_offset_82599(struct ixgbe_hw *hw,
    u16 *san_mac_offset);
s32 ixgbe_get_san_mac_addr_82599(struct ixgbe_hw *hw, u8 *san_mac_addr);
s32 ixgbe_set_san_mac_addr_82599(struct ixgbe_hw *hw, u8 *san_mac_addr);
s32 ixgbe_get_device_caps_82599(struct ixgbe_hw *hw, u16 *device_caps);

void
ixgbe_init_mac_link_ops_82599(struct ixgbe_hw *hw)
{
	struct ixgbe_mac_info *mac = &hw->mac;

	DEBUGFUNC("ixgbe_init_mac_link_ops_82599");

	if (hw->phy.multispeed_fiber) {
		/* Set up dual speed SFP+ support */
		mac->ops.setup_link =
		    &ixgbe_setup_mac_link_multispeed_fiber;
		mac->ops.setup_link_speed =
		    &ixgbe_setup_mac_link_speed_multispeed_fiber;
	} else {
		mac->ops.setup_link =
		    &ixgbe_setup_mac_link_82599;
		mac->ops.setup_link_speed =
		    &ixgbe_setup_mac_link_speed_82599;
	}
}

/*
 * ixgbe_init_phy_ops_82599 - PHY/SFP specific init
 * @hw: pointer to hardware structure
 *
 * Initialize any function pointers that were not able to be
 * set during init_shared_code because the PHY/SFP type was
 * not known.  Perform the SFP init if necessary.
 *
 */
s32
ixgbe_init_phy_ops_82599(struct ixgbe_hw *hw)
{
	struct ixgbe_mac_info *mac = &hw->mac;
	struct ixgbe_phy_info *phy = &hw->phy;
	s32 ret_val = IXGBE_SUCCESS;

	DEBUGFUNC("ixgbe_init_phy_ops_82599");

	/* Identify the PHY or SFP module */
	ret_val = phy->ops.identify(hw);

	/* Setup function pointers based on detected SFP module and speeds */
	ixgbe_init_mac_link_ops_82599(hw);
	if (hw->phy.sfp_type != ixgbe_sfp_type_unknown)
		hw->phy.ops.reset = NULL;

	/* If copper media, overwrite with copper function pointers */
	if (mac->ops.get_media_type(hw) == ixgbe_media_type_copper) {
		mac->ops.setup_link = &ixgbe_setup_copper_link_82599;
		mac->ops.setup_link_speed =
		    &ixgbe_setup_copper_link_speed_82599;
		mac->ops.get_link_capabilities =
		    &ixgbe_get_copper_link_capabilities_generic;
	}

	/* Set necessary function pointers based on phy type */
	switch (hw->phy.type) {
	case ixgbe_phy_tn:
		phy->ops.check_link = &ixgbe_check_phy_link_tnx;
		phy->ops.get_firmware_version =
		    &ixgbe_get_phy_firmware_version_tnx;
		break;
	default:
		break;
	}

	return (ret_val);
}

s32
ixgbe_setup_sfp_modules_82599(struct ixgbe_hw *hw)
{
	s32 ret_val = IXGBE_SUCCESS;
	u16 list_offset, data_offset, data_value;

	DEBUGFUNC("ixgbe_setup_sfp_modules_82599");

	if (hw->phy.sfp_type != ixgbe_sfp_type_unknown) {
		ixgbe_init_mac_link_ops_82599(hw);

		hw->phy.ops.reset = NULL;

		ret_val = ixgbe_get_sfp_init_sequence_offsets(hw, &list_offset,
		    &data_offset);

		if (ret_val != IXGBE_SUCCESS)
			goto setup_sfp_out;

		hw->eeprom.ops.read(hw, ++data_offset, &data_value);
		while (data_value != 0xffff) {
			IXGBE_WRITE_REG(hw, IXGBE_CORECTL, data_value);
			IXGBE_WRITE_FLUSH(hw);
			hw->eeprom.ops.read(hw, ++data_offset, &data_value);
		}
		/* Now restart DSP */
		IXGBE_WRITE_REG(hw, IXGBE_CORECTL, 0x00000102);
		IXGBE_WRITE_REG(hw, IXGBE_CORECTL, 0x00000b1d);
		IXGBE_WRITE_FLUSH(hw);
	}

setup_sfp_out:
	return (ret_val);
}

/*
 * ixgbe_get_pcie_msix_count_82599 - Gets MSI-X vector count
 * @hw: pointer to hardware structure
 *
 * Read PCIe configuration space, and get the MSI-X vector count from
 * the capabilities table.
 */
u32
ixgbe_get_pcie_msix_count_82599(struct ixgbe_hw *hw)
{
	u32 msix_count = 64;

	if (hw->mac.msix_vectors_from_pcie) {
		msix_count = IXGBE_READ_PCIE_WORD(hw,
		    IXGBE_PCIE_MSIX_82599_CAPS);
		msix_count &= IXGBE_PCIE_MSIX_TBL_SZ_MASK;

		/*
		 * MSI-X count is zero-based in HW, so increment to give
		 * proper value
		 */
		msix_count++;
	}

	return (msix_count);
}

/*
 * ixgbe_init_ops_82599 - Inits func ptrs and MAC type
 * @hw: pointer to hardware structure
 *
 * Initialize the function pointers and assign the MAC type for 82599.
 * Does not touch the hardware.
 */

s32
ixgbe_init_ops_82599(struct ixgbe_hw *hw)
{
	struct ixgbe_mac_info *mac = &hw->mac;
	struct ixgbe_phy_info *phy = &hw->phy;
	s32 ret_val;

	ret_val = ixgbe_init_phy_ops_generic(hw);
	ret_val = ixgbe_init_ops_generic(hw);

	/* PHY */
	phy->ops.identify = &ixgbe_identify_phy_82599;
	phy->ops.init = &ixgbe_init_phy_ops_82599;

	/* MAC */
	mac->ops.reset_hw = &ixgbe_reset_hw_82599;
	mac->ops.get_media_type = &ixgbe_get_media_type_82599;
	mac->ops.get_supported_physical_layer =
	    &ixgbe_get_supported_physical_layer_82599;
	mac->ops.enable_rx_dma = &ixgbe_enable_rx_dma_82599;
	mac->ops.read_analog_reg8 = &ixgbe_read_analog_reg8_82599;
	mac->ops.write_analog_reg8 = &ixgbe_write_analog_reg8_82599;
	mac->ops.start_hw = &ixgbe_start_hw_rev_1_82599;
	mac->ops.get_san_mac_addr = &ixgbe_get_san_mac_addr_82599;
	mac->ops.set_san_mac_addr = &ixgbe_set_san_mac_addr_82599;
	mac->ops.get_device_caps = &ixgbe_get_device_caps_82599;

	/* RAR, Multicast, VLAN */
	mac->ops.set_vmdq = &ixgbe_set_vmdq_82599;
	mac->ops.clear_vmdq = &ixgbe_clear_vmdq_82599;
	mac->ops.insert_mac_addr = &ixgbe_insert_mac_addr_82599;
	mac->rar_highwater = 1;
	mac->ops.set_vfta = &ixgbe_set_vfta_82599;
	mac->ops.clear_vfta = &ixgbe_clear_vfta_82599;
	mac->ops.init_uta_tables = &ixgbe_init_uta_tables_82599;
	mac->ops.setup_sfp = &ixgbe_setup_sfp_modules_82599;

	/* Link */
	mac->ops.get_link_capabilities = &ixgbe_get_link_capabilities_82599;
	mac->ops.check_link = &ixgbe_check_mac_link_82599;
	ixgbe_init_mac_link_ops_82599(hw);

	mac->mcft_size = 128;
	mac->vft_size = 128;
	mac->num_rar_entries = 128;
	mac->max_tx_queues = 128;
	mac->max_rx_queues = 128;
	mac->max_msix_vectors = ixgbe_get_pcie_msix_count_82599(hw);

	return (ret_val);
}

/*
 * ixgbe_get_link_capabilities_82599 - Determines link capabilities
 * @hw: pointer to hardware structure
 * @speed: pointer to link speed
 * @negotiation: true when autoneg or autotry is enabled
 *
 * Determines the link capabilities by reading the AUTOC register.
 */
s32
ixgbe_get_link_capabilities_82599(struct ixgbe_hw *hw,
    ixgbe_link_speed *speed, bool *negotiation)
{
	s32 status = IXGBE_SUCCESS;
	u32 autoc = 0;

	/*
	 * Determine link capabilities based on the stored value of AUTOC,
	 * which represents EEPROM defaults.  If AUTOC value has not
	 * been stored, use the current register values.
	 */
	if (hw->mac.orig_link_settings_stored)
		autoc = hw->mac.orig_autoc;
	else
		autoc = IXGBE_READ_REG(hw, IXGBE_AUTOC);

	switch (autoc & IXGBE_AUTOC_LMS_MASK) {
	case IXGBE_AUTOC_LMS_1G_LINK_NO_AN:
		*speed = IXGBE_LINK_SPEED_1GB_FULL;
		*negotiation = false;
		break;

	case IXGBE_AUTOC_LMS_10G_LINK_NO_AN:
		*speed = IXGBE_LINK_SPEED_10GB_FULL;
		*negotiation = false;
		break;

	case IXGBE_AUTOC_LMS_1G_AN:
		*speed = IXGBE_LINK_SPEED_1GB_FULL;
		*negotiation = true;
		break;

	case IXGBE_AUTOC_LMS_10G_SERIAL:
		*speed = IXGBE_LINK_SPEED_10GB_FULL;
		*negotiation = false;
		break;

	case IXGBE_AUTOC_LMS_KX4_KX_KR:
	case IXGBE_AUTOC_LMS_KX4_KX_KR_1G_AN:
		*speed = IXGBE_LINK_SPEED_UNKNOWN;
		if (autoc & IXGBE_AUTOC_KR_SUPP)
			*speed |= IXGBE_LINK_SPEED_10GB_FULL;
		if (autoc & IXGBE_AUTOC_KX4_SUPP)
			*speed |= IXGBE_LINK_SPEED_10GB_FULL;
		if (autoc & IXGBE_AUTOC_KX_SUPP)
			*speed |= IXGBE_LINK_SPEED_1GB_FULL;
		*negotiation = true;
		break;

	case IXGBE_AUTOC_LMS_KX4_KX_KR_SGMII:
		*speed = IXGBE_LINK_SPEED_100_FULL;
		if (autoc & IXGBE_AUTOC_KR_SUPP)
			*speed |= IXGBE_LINK_SPEED_10GB_FULL;
		if (autoc & IXGBE_AUTOC_KX4_SUPP)
			*speed |= IXGBE_LINK_SPEED_10GB_FULL;
		if (autoc & IXGBE_AUTOC_KX_SUPP)
			*speed |= IXGBE_LINK_SPEED_1GB_FULL;
		*negotiation = true;
		break;

	case IXGBE_AUTOC_LMS_SGMII_1G_100M:
		*speed = IXGBE_LINK_SPEED_1GB_FULL | IXGBE_LINK_SPEED_100_FULL;
		*negotiation = false;
		break;

	default:
		status = IXGBE_ERR_LINK_SETUP;
		goto out;
	}

	if (hw->phy.multispeed_fiber) {
		*speed |= IXGBE_LINK_SPEED_10GB_FULL |
		    IXGBE_LINK_SPEED_1GB_FULL;
		*negotiation = true;
	}

out:
	return (status);
}

/*
 * ixgbe_get_media_type_82599 - Get media type
 * @hw: pointer to hardware structure
 *
 * Returns the media type (fiber, copper, backplane)
 */
enum ixgbe_media_type
ixgbe_get_media_type_82599(struct ixgbe_hw *hw)
{
	enum ixgbe_media_type media_type;

	/* Detect if there is a copper PHY attached. */
	if (hw->phy.type == ixgbe_phy_cu_unknown ||
	    hw->phy.type == ixgbe_phy_tn) {
		media_type = ixgbe_media_type_copper;
		goto out;
	}

	switch (hw->device_id) {
	case IXGBE_DEV_ID_82599_KX4:
	case IXGBE_DEV_ID_82599_KX4_SIK:
		/* Default device ID is mezzanine card KX/KX4 */
		media_type = ixgbe_media_type_backplane;
		break;
	case IXGBE_DEV_ID_82599_SFP:
	case IXGBE_DEV_ID_82599_SPW:
		media_type = ixgbe_media_type_fiber;
		break;
	case IXGBE_DEV_ID_82599_CX4:
		media_type = ixgbe_media_type_fiber;
		break;
	default:
		media_type = ixgbe_media_type_unknown;
		break;
	}
out:
	return (media_type);
}

/*
 * ixgbe_setup_mac_link_82599 - Setup MAC link settings
 * @hw: pointer to hardware structure
 *
 * Configures link settings based on values in the ixgbe_hw struct.
 * Restarts the link.  Performs autonegotiation if needed.
 */
s32
ixgbe_setup_mac_link_82599(struct ixgbe_hw *hw)
{
	u32 autoc_reg;
	u32 links_reg;
	u32 i;
	s32 status = IXGBE_SUCCESS;

	/* Restart link */
	autoc_reg = IXGBE_READ_REG(hw, IXGBE_AUTOC);
	autoc_reg |= IXGBE_AUTOC_AN_RESTART;
	IXGBE_WRITE_REG(hw, IXGBE_AUTOC, autoc_reg);

	/* Only poll for autoneg to complete if specified to do so */
	if (hw->phy.autoneg_wait_to_complete) {
		if ((autoc_reg & IXGBE_AUTOC_LMS_MASK) ==
		    IXGBE_AUTOC_LMS_KX4_KX_KR ||
		    (autoc_reg & IXGBE_AUTOC_LMS_MASK) ==
		    IXGBE_AUTOC_LMS_KX4_KX_KR_1G_AN ||
		    (autoc_reg & IXGBE_AUTOC_LMS_MASK) ==
		    IXGBE_AUTOC_LMS_KX4_KX_KR_SGMII) {
			links_reg = 0; /* Just in case Autoneg time = 0 */
			for (i = 0; i < IXGBE_AUTO_NEG_TIME; i++) {
				links_reg = IXGBE_READ_REG(hw, IXGBE_LINKS);
				if (links_reg & IXGBE_LINKS_KX_AN_COMP)
					break;
				msec_delay(100);
			}
			if (!(links_reg & IXGBE_LINKS_KX_AN_COMP)) {
				status = IXGBE_ERR_AUTONEG_NOT_COMPLETE;
				DEBUGOUT("Autoneg did not complete.\n");
			}
		}
	}

	/* Add delay to filter out noises during initial link setup */
	msec_delay(50);

	return (status);
}

/*
 * ixgbe_setup_mac_link_multispeed_fiber - Setup MAC link settings
 * @hw: pointer to hardware structure
 *
 * Configures link settings based on values in the ixgbe_hw struct.
 * Restarts the link for multi-speed fiber at 1G speed, if link
 * fails at 10G.
 * Performs autonegotiation if needed.
 */
s32
ixgbe_setup_mac_link_multispeed_fiber(struct ixgbe_hw *hw)
{
	s32 status = IXGBE_SUCCESS;
	ixgbe_link_speed link_speed = IXGBE_LINK_SPEED_82599_AUTONEG;
	DEBUGFUNC("ixgbe_setup_mac_link_multispeed_fiber");

	status = ixgbe_setup_mac_link_speed_multispeed_fiber(hw,
	    link_speed, true, true);
	return (status);
}

/*
 * ixgbe_setup_mac_link_speed_multispeed_fiber - Set MAC link speed
 * @hw: pointer to hardware structure
 * @speed: new link speed
 * @autoneg: true if autonegotiation enabled
 * @autoneg_wait_to_complete: true when waiting for completion is needed
 *
 * Set the link speed in the AUTOC register and restarts link.
 */
s32
ixgbe_setup_mac_link_speed_multispeed_fiber(struct ixgbe_hw *hw,
    ixgbe_link_speed speed, bool autoneg, bool autoneg_wait_to_complete)
{
	s32 status = IXGBE_SUCCESS;
	ixgbe_link_speed link_speed;
	ixgbe_link_speed highest_link_speed = IXGBE_LINK_SPEED_UNKNOWN;
	u32 speedcnt = 0;
	u32 esdp_reg = IXGBE_READ_REG(hw, IXGBE_ESDP);
	bool link_up = false;
	bool negotiation;

	/* Mask off requested but non-supported speeds */
	status = ixgbe_get_link_capabilities(hw, &link_speed, &negotiation);
	if (status != IXGBE_SUCCESS)
		goto out;

	speed &= link_speed;

	/*
	 * Try each speed one by one, highest priority first.  We do this in
	 * software because 10gb fiber doesn't support speed autonegotiation.
	 */
	if (speed & IXGBE_LINK_SPEED_10GB_FULL) {
		speedcnt++;
		highest_link_speed = IXGBE_LINK_SPEED_10GB_FULL;

		/* If we already have link at this speed, just jump out */
		status = ixgbe_check_link(hw, &link_speed, &link_up, false);
		if (status != IXGBE_SUCCESS)
			goto out;

		if ((link_speed == IXGBE_LINK_SPEED_10GB_FULL) && link_up)
			goto out;

		/* Set hardware SDP's */
		esdp_reg |= (IXGBE_ESDP_SDP5_DIR | IXGBE_ESDP_SDP5);
		IXGBE_WRITE_REG(hw, IXGBE_ESDP, esdp_reg);

		/* Allow module to change analog characteristics (1G->10G) */
		msec_delay(40);

		status = ixgbe_setup_mac_link_speed_82599(
		    hw, IXGBE_LINK_SPEED_10GB_FULL, autoneg,
		    autoneg_wait_to_complete);
		if (status != IXGBE_SUCCESS)
			goto out;

		msec_delay(100);

		/* If we have link, just jump out */
		status = ixgbe_check_link(hw, &link_speed, &link_up, false);
		if (status != IXGBE_SUCCESS)
			goto out;

		if (link_up)
			goto out;
	}

	if (speed & IXGBE_LINK_SPEED_1GB_FULL) {
		speedcnt++;
		if (highest_link_speed == IXGBE_LINK_SPEED_UNKNOWN)
			highest_link_speed = IXGBE_LINK_SPEED_1GB_FULL;

		/* If we already have link at this speed, just jump out */
		status = ixgbe_check_link(hw, &link_speed, &link_up, false);
		if (status != IXGBE_SUCCESS)
			goto out;

		if ((link_speed == IXGBE_LINK_SPEED_1GB_FULL) && link_up)
			goto out;

		/* Set hardware SDP's */
		esdp_reg &= ~IXGBE_ESDP_SDP5;
		esdp_reg |= IXGBE_ESDP_SDP5_DIR;
		IXGBE_WRITE_REG(hw, IXGBE_ESDP, esdp_reg);

		/* Allow module to change analog characteristics (10G->1G) */
		msec_delay(40);

		status = ixgbe_setup_mac_link_speed_82599(
		    hw, IXGBE_LINK_SPEED_1GB_FULL, autoneg,
		    autoneg_wait_to_complete);
		if (status != IXGBE_SUCCESS)
			goto out;

		msec_delay(100);

		/* If we have link, just jump out */
		status = ixgbe_check_link(hw, &link_speed, &link_up, false);
		if (status != IXGBE_SUCCESS)
			goto out;

		if (link_up)
			goto out;
	}

	/*
	 * We didn't get link.  Configure back to the highest speed we tried,
	 * (if there was more than one).  We call ourselves back with just the
	 * single highest speed that the user requested.
	 */
	if (speedcnt > 1)
		status = ixgbe_setup_mac_link_speed_multispeed_fiber(hw,
		    highest_link_speed, autoneg, autoneg_wait_to_complete);

out:
	return (status);
}

/*
 * ixgbe_check_mac_link_82599 - Determine link and speed status
 * @hw: pointer to hardware structure
 * @speed: pointer to link speed
 * @link_up: true when link is up
 * @link_up_wait_to_complete: bool used to wait for link up or not
 *
 * Reads the links register to determine if link is up and the current speed
 */
s32
ixgbe_check_mac_link_82599(struct ixgbe_hw *hw, ixgbe_link_speed *speed,
    bool *link_up, bool link_up_wait_to_complete)
{
	u32 links_reg;
	u32 i;

	links_reg = IXGBE_READ_REG(hw, IXGBE_LINKS);
	if (link_up_wait_to_complete) {
		for (i = 0; i < IXGBE_LINK_UP_TIME; i++) {
			if (links_reg & IXGBE_LINKS_UP) {
				*link_up = true;
				break;
			} else {
				*link_up = false;
			}
			msec_delay(100);
			links_reg = IXGBE_READ_REG(hw, IXGBE_LINKS);
		}
	} else {
		if (links_reg & IXGBE_LINKS_UP)
			*link_up = true;
		else
			*link_up = false;
	}

	if ((links_reg & IXGBE_LINKS_SPEED_82599) ==
	    IXGBE_LINKS_SPEED_10G_82599)
		*speed = IXGBE_LINK_SPEED_10GB_FULL;
	else if ((links_reg & IXGBE_LINKS_SPEED_82599) ==
	    IXGBE_LINKS_SPEED_1G_82599)
		*speed = IXGBE_LINK_SPEED_1GB_FULL;
	else
		*speed = IXGBE_LINK_SPEED_100_FULL;

	/* if link is down, zero out the current_mode */
	if (*link_up == false) {
		hw->fc.current_mode = ixgbe_fc_none;
		hw->fc.fc_was_autonegged = false;
	}

	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_setup_mac_link_speed_82599 - Set MAC link speed
 * @hw: pointer to hardware structure
 * @speed: new link speed
 * @autoneg: true if autonegotiation enabled
 * @autoneg_wait_to_complete: true when waiting for completion is needed
 *
 * Set the link speed in the AUTOC register and restarts link.
 */
s32
ixgbe_setup_mac_link_speed_82599(struct ixgbe_hw *hw,
    ixgbe_link_speed speed, bool autoneg, bool autoneg_wait_to_complete)
{
	s32 status = IXGBE_SUCCESS;
	u32 autoc = IXGBE_READ_REG(hw, IXGBE_AUTOC);
	u32 autoc2 = IXGBE_READ_REG(hw, IXGBE_AUTOC2);
	u32 link_mode = autoc & IXGBE_AUTOC_LMS_MASK;
	u32 pma_pmd_1g = autoc & IXGBE_AUTOC_1G_PMA_PMD_MASK;
	u32 pma_pmd_10g_serial = autoc2 & IXGBE_AUTOC2_10G_SERIAL_PMA_PMD_MASK;
	u32 links_reg;
	u32 i;
	ixgbe_link_speed link_capabilities = IXGBE_LINK_SPEED_UNKNOWN;

	/* Check to see if speed passed in is supported. */
	status = ixgbe_get_link_capabilities(hw, &link_capabilities, &autoneg);
	if (status != IXGBE_SUCCESS)
		goto out;

	speed &= link_capabilities;

	if (speed == IXGBE_LINK_SPEED_UNKNOWN) {
		status = IXGBE_ERR_LINK_SETUP;
	} else if (link_mode == IXGBE_AUTOC_LMS_KX4_KX_KR ||
	    link_mode == IXGBE_AUTOC_LMS_KX4_KX_KR_1G_AN ||
	    link_mode == IXGBE_AUTOC_LMS_KX4_KX_KR_SGMII) {
		/* Set KX4/KX/KR support according to speed requested */
		autoc &= ~(IXGBE_AUTOC_KX4_KX_SUPP_MASK | IXGBE_AUTOC_KR_SUPP);
		if (speed & IXGBE_LINK_SPEED_10GB_FULL)
			if (autoc & IXGBE_AUTOC_KX4_SUPP)
				autoc |= IXGBE_AUTOC_KX4_SUPP;
			if (autoc & IXGBE_AUTOC_KR_SUPP)
				autoc |= IXGBE_AUTOC_KR_SUPP;
		if (speed & IXGBE_LINK_SPEED_1GB_FULL)
			autoc |= IXGBE_AUTOC_KX_SUPP;
	} else if ((pma_pmd_1g == IXGBE_AUTOC_1G_SFI) &&
	    (link_mode == IXGBE_AUTOC_LMS_1G_LINK_NO_AN ||
	    link_mode == IXGBE_AUTOC_LMS_1G_AN)) {
		/* Switch from 1G SFI to 10G SFI if requested */
		if ((speed == IXGBE_LINK_SPEED_10GB_FULL) &&
		    (pma_pmd_10g_serial == IXGBE_AUTOC2_10G_SFI)) {
			autoc &= ~IXGBE_AUTOC_LMS_MASK;
			autoc |= IXGBE_AUTOC_LMS_10G_SERIAL;
		}
	} else if ((pma_pmd_10g_serial == IXGBE_AUTOC2_10G_SFI) &&
	    (link_mode == IXGBE_AUTOC_LMS_10G_SERIAL)) {
		/* Switch from 10G SFI to 1G SFI if requested */
		if ((speed == IXGBE_LINK_SPEED_1GB_FULL) &&
		    (pma_pmd_1g == IXGBE_AUTOC_1G_SFI)) {
			autoc &= ~IXGBE_AUTOC_LMS_MASK;
			if (autoneg)
				autoc |= IXGBE_AUTOC_LMS_1G_AN;
			else
				autoc |= IXGBE_AUTOC_LMS_1G_LINK_NO_AN;
			}
	}

	if (status == IXGBE_SUCCESS) {
		/* Restart link */
		autoc |= IXGBE_AUTOC_AN_RESTART;
		IXGBE_WRITE_REG(hw, IXGBE_AUTOC, autoc);

		/* Only poll for autoneg to complete if specified to do so */
		if (autoneg_wait_to_complete) {
			if (link_mode == IXGBE_AUTOC_LMS_KX4_KX_KR ||
			    link_mode == IXGBE_AUTOC_LMS_KX4_KX_KR_1G_AN ||
			    link_mode == IXGBE_AUTOC_LMS_KX4_KX_KR_SGMII) {
				links_reg = 0; /* Just in case Autoneg time=0 */
				for (i = 0; i < IXGBE_AUTO_NEG_TIME; i++) {
					links_reg =
					    IXGBE_READ_REG(hw, IXGBE_LINKS);
					if (links_reg & IXGBE_LINKS_KX_AN_COMP)
						break;
					msec_delay(100);
				}
				if (!(links_reg & IXGBE_LINKS_KX_AN_COMP)) {
					status =
					    IXGBE_ERR_AUTONEG_NOT_COMPLETE;
					DEBUGOUT("Autoneg did not complete.\n");
				}
			}
		}

		/* Add delay to filter out noises during initial link setup */
		msec_delay(50);
	}

out:
	return (status);
}

/*
 * ixgbe_setup_copper_link_82599 - Setup copper link settings
 * @hw: pointer to hardware structure
 *
 * Restarts the link on PHY and then MAC. Performs autonegotiation if needed.
 */
static s32
ixgbe_setup_copper_link_82599(struct ixgbe_hw *hw)
{
	s32 status;

	/* Restart autonegotiation on PHY */
	status = hw->phy.ops.setup_link(hw);

	/* Set up MAC */
	(void) ixgbe_setup_mac_link_82599(hw);

	return (status);
}

/*
 * ixgbe_setup_copper_link_speed_82599 - Set the PHY autoneg advertised field
 * @hw: pointer to hardware structure
 * @speed: new link speed
 * @autoneg: true if autonegotiation enabled
 * @autoneg_wait_to_complete: true if waiting is needed to complete
 *
 * Restarts link on PHY and MAC based on settings passed in.
 */
static s32
ixgbe_setup_copper_link_speed_82599(struct ixgbe_hw *hw,
    ixgbe_link_speed speed, bool autoneg, bool autoneg_wait_to_complete)
{
	s32 status;

	/* Setup the PHY according to input speed */
	status = hw->phy.ops.setup_link_speed(hw, speed, autoneg,
	    autoneg_wait_to_complete);
	/* Set up MAC */
	(void) ixgbe_setup_mac_link_82599(hw);

	return (status);
}
/*
 * ixgbe_reset_hw_82599 - Perform hardware reset
 * @hw: pointer to hardware structure
 *
 * Resets the hardware by resetting the transmit and receive units, masks
 * and clears all interrupts, perform a PHY reset, and perform a link (MAC)
 * reset.
 */
s32
ixgbe_reset_hw_82599(struct ixgbe_hw *hw)
{
	s32 status = IXGBE_SUCCESS;
	u32 ctrl, ctrl_ext;
	u32 i;
	u32 autoc;
	u32 autoc2;

	/* Call adapter stop to disable tx/rx and clear interrupts */
	hw->mac.ops.stop_adapter(hw);

	/* PHY ops must be identified and initialized prior to reset */

	/* Identify PHY and related function pointers */
	status = hw->phy.ops.init(hw);

	/* Setup SFP module if there is one present. */
	if (hw->phy.sfp_setup_needed) {
		status = hw->mac.ops.setup_sfp(hw);
		hw->phy.sfp_setup_needed = false;
	}

	/* Reset PHY */
	if (hw->phy.reset_disable == false && hw->phy.ops.reset != NULL)
		hw->phy.ops.reset(hw);

	/*
	 * Prevent the PCI-E bus from from hanging by disabling PCI-E master
	 * access and verify no pending requests before reset
	 */
	status = ixgbe_disable_pcie_master(hw);
	if (status != IXGBE_SUCCESS) {
		status = IXGBE_ERR_MASTER_REQUESTS_PENDING;
		DEBUGOUT("PCI-E Master disable polling has failed.\n");
	}

	/*
	 * Issue global reset to the MAC.  This needs to be a SW reset.
	 * If link reset is used, it might reset the MAC when mng is using it
	 */
	ctrl = IXGBE_READ_REG(hw, IXGBE_CTRL);
	IXGBE_WRITE_REG(hw, IXGBE_CTRL, (ctrl | IXGBE_CTRL_RST));
	IXGBE_WRITE_FLUSH(hw);

	/* Poll for reset bit to self-clear indicating reset is complete */
	for (i = 0; i < 10; i++) {
		usec_delay(1);
		ctrl = IXGBE_READ_REG(hw, IXGBE_CTRL);
		if (!(ctrl & IXGBE_CTRL_RST)) {
			break;
		}
	}
	if (ctrl & IXGBE_CTRL_RST) {
		status = IXGBE_ERR_RESET_FAILED;
		DEBUGOUT("Reset polling failed to complete.\n");
	}

	/* Clear PF Reset Done bit so PF/VF Mail Ops can work */
	ctrl_ext = IXGBE_READ_REG(hw, IXGBE_CTRL_EXT);
	ctrl_ext |= IXGBE_CTRL_EXT_PFRSTD;
	IXGBE_WRITE_REG(hw, IXGBE_CTRL_EXT, ctrl_ext);

	msec_delay(50);

	/*
	 * Store the original AUTOC/AUTOC2 values if they have not been
	 * stored off yet.  Otherwise restore the stored original
	 * values since the reset operation sets back to defaults.
	 */
	autoc = IXGBE_READ_REG(hw, IXGBE_AUTOC);
	autoc2 = IXGBE_READ_REG(hw, IXGBE_AUTOC2);
	if (hw->mac.orig_link_settings_stored == false) {
		hw->mac.orig_autoc = autoc;
		hw->mac.orig_autoc2 = autoc2;
		hw->mac.orig_link_settings_stored = true;
	} else {
		if (autoc != hw->mac.orig_autoc) {
			IXGBE_WRITE_REG(hw, IXGBE_AUTOC, (hw->mac.orig_autoc |
			    IXGBE_AUTOC_AN_RESTART));
		}

		if ((autoc2 & IXGBE_AUTOC2_UPPER_MASK) !=
		    (hw->mac.orig_autoc2 & IXGBE_AUTOC2_UPPER_MASK)) {
			autoc2 &= ~IXGBE_AUTOC2_UPPER_MASK;
			autoc2 |= (hw->mac.orig_autoc2 &
			    IXGBE_AUTOC2_UPPER_MASK);
			IXGBE_WRITE_REG(hw, IXGBE_AUTOC2, autoc2);
		}
	}

	/*
	 * Store MAC address from RAR0, clear receive address registers, and
	 * clear the multicast table.  Also reset num_rar_entries to 128,
	 * since we modify this value when programming the SAN MAC address.
	 */
	hw->mac.num_rar_entries = 128;
	hw->mac.ops.init_rx_addrs(hw);

	/* Store the permanent mac address */
	hw->mac.ops.get_mac_addr(hw, hw->mac.perm_addr);

	/* Add the SAN MAC address to the RAR only if it's a valid address */
	if (ixgbe_validate_mac_addr(hw->mac.san_addr) == 0) {
		hw->mac.ops.set_rar(hw, hw->mac.num_rar_entries - 1,
		    hw->mac.san_addr, 0, IXGBE_RAH_AV);

		/* Reserve the last RAR for the SAN MAC address */
		hw->mac.num_rar_entries--;
	}

	return (status);
}

/*
 * ixgbe_insert_mac_addr_82599 - Find a RAR for this mac address
 * @hw: pointer to hardware structure
 * @addr: Address to put into receive address register
 * @vmdq: VMDq pool to assign
 *
 * Puts an ethernet address into a receive address register, or
 * finds the rar that it is aleady in; adds to the pool list
 */
s32
ixgbe_insert_mac_addr_82599(struct ixgbe_hw *hw, u8 *addr, u32 vmdq)
{
	static const u32 NO_EMPTY_RAR_FOUND = 0xFFFFFFFF;
	u32 first_empty_rar = NO_EMPTY_RAR_FOUND;
	u32 rar;
	u32 rar_low, rar_high;
	u32 addr_low, addr_high;

	/* swap bytes for HW little endian */
	addr_low  = addr[0] | (addr[1] << 8)
	    | (addr[2] << 16)
	    | (addr[3] << 24);
	addr_high = addr[4] | (addr[5] << 8);

	/*
	 * Either find the mac_id in rar or find the first empty space.
	 * rar_highwater points to just after the highest currently used
	 * rar in order to shorten the search.  It grows when we add a new
	 * rar to the top.
	 */
	for (rar = 0; rar < hw->mac.rar_highwater; rar++) {
		rar_high = IXGBE_READ_REG(hw, IXGBE_RAH(rar));

		if (((IXGBE_RAH_AV & rar_high) == 0) &&
		    first_empty_rar == NO_EMPTY_RAR_FOUND) {
			first_empty_rar = rar;
		} else if ((rar_high & 0xFFFF) == addr_high) {
			rar_low = IXGBE_READ_REG(hw, IXGBE_RAL(rar));
			if (rar_low == addr_low)
				break;	  /* found it already in the rars */
		}
	}

	if (rar < hw->mac.rar_highwater) {
		/* already there so just add to the pool bits */
		(void) ixgbe_set_vmdq(hw, rar, vmdq);
	} else if (first_empty_rar != NO_EMPTY_RAR_FOUND) {
		/* stick it into first empty RAR slot we found */
		rar = first_empty_rar;
		(void) ixgbe_set_rar(hw, rar, addr, vmdq, IXGBE_RAH_AV);
	} else if (rar == hw->mac.rar_highwater) {
		/* add it to the top of the list and inc the highwater mark */
		(void) ixgbe_set_rar(hw, rar, addr, vmdq, IXGBE_RAH_AV);
		hw->mac.rar_highwater++;
	} else if (rar >= hw->mac.num_rar_entries) {
		return (IXGBE_ERR_INVALID_MAC_ADDR);
	}

	/*
	 * If we found rar[0], make sure the default pool bit (we use pool 0)
	 * remains cleared to be sure default pool packets will get delivered
	 */
	if (rar == 0)
		(void) ixgbe_clear_vmdq(hw, rar, 0);

	return (rar);
}

/*
 * ixgbe_clear_vmdq_82599 - Disassociate a VMDq pool index from a rx address
 * @hw: pointer to hardware struct
 * @rar: receive address register index to disassociate
 * @vmdq: VMDq pool index to remove from the rar
 */
s32
ixgbe_clear_vmdq_82599(struct ixgbe_hw *hw, u32 rar, u32 vmdq)
{
	u32 mpsar_lo, mpsar_hi;
	u32 rar_entries = hw->mac.num_rar_entries;

	if (rar < rar_entries) {
		mpsar_lo = IXGBE_READ_REG(hw, IXGBE_MPSAR_LO(rar));
		mpsar_hi = IXGBE_READ_REG(hw, IXGBE_MPSAR_HI(rar));

		if (!mpsar_lo && !mpsar_hi) {
			goto done;
		}

		if (vmdq == IXGBE_CLEAR_VMDQ_ALL) {
			if (mpsar_lo) {
				IXGBE_WRITE_REG(hw, IXGBE_MPSAR_LO(rar), 0);
				mpsar_lo = 0;
			}
			if (mpsar_hi) {
				IXGBE_WRITE_REG(hw, IXGBE_MPSAR_HI(rar), 0);
				mpsar_hi = 0;
			}
		} else if (vmdq < 32) {
			mpsar_lo &= ~(1 << vmdq);
			IXGBE_WRITE_REG(hw, IXGBE_MPSAR_LO(rar), mpsar_lo);
		} else {
			mpsar_hi &= ~(1 << (vmdq - 32));
			IXGBE_WRITE_REG(hw, IXGBE_MPSAR_HI(rar), mpsar_hi);
		}

		/* was that the last pool using this rar? */
		if (mpsar_lo == 0 && mpsar_hi == 0 && rar != 0) {
			hw->mac.ops.clear_rar(hw, rar);
		}
	} else {
		DEBUGOUT1("RAR index %d is out of range.\n", rar);
	}
done:
	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_set_vmdq_82599 - Associate a VMDq pool index with a rx address
 * @hw: pointer to hardware struct
 * @rar: receive address register index to associate with a VMDq index
 * @vmdq: VMDq pool index
 */
s32
ixgbe_set_vmdq_82599(struct ixgbe_hw *hw, u32 rar, u32 vmdq)
{
	u32 mpsar;
	u32 rar_entries = hw->mac.num_rar_entries;

	if (rar < rar_entries) {
		if (vmdq < 32) {
			mpsar = IXGBE_READ_REG(hw, IXGBE_MPSAR_LO(rar));
			mpsar |= 1 << vmdq;
			IXGBE_WRITE_REG(hw, IXGBE_MPSAR_LO(rar), mpsar);
		} else {
			mpsar = IXGBE_READ_REG(hw, IXGBE_MPSAR_HI(rar));
			mpsar |= 1 << (vmdq - 32);
			IXGBE_WRITE_REG(hw, IXGBE_MPSAR_HI(rar), mpsar);
		}
	} else {
		DEBUGOUT1("RAR index %d is out of range.\n", rar);
	}

	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_set_vfta_82599 - Set VLAN filter table
 * @hw: pointer to hardware structure
 * @vlan: VLAN id to write to VLAN filter
 * @vind: VMDq output index that maps queue to VLAN id in VFVFB
 * @vlan_on: boolean flag to turn on/off VLAN in VFVF
 *
 * Turn on/off specified VLAN in the VLAN filter table.
 */
s32
ixgbe_set_vfta_82599(struct ixgbe_hw *hw, u32 vlan, u32 vind, bool vlan_on)
{
	u32 regindex;
	u32 bitindex;
	u32 bits;
	u32 first_empty_slot;

	if (vlan > 4095) {
		return (IXGBE_ERR_PARAM);
	}

	/*
	 * this is a 2 part operation - first the VFTA, then the
	 * VLVF and VLVFB if vind is set
	 */

	/*
	 * Part 1
	 * The VFTA is a bitstring made up of 128 32-bit registers
	 * that enable the particular VLAN id, much like the MTA:
	 *    bits[11-5]: which register
	 *    bits[4-0]:  which bit in the register
	 */
	regindex = (vlan >> 5) & 0x7F;
	bitindex = vlan & 0x1F;
	bits = IXGBE_READ_REG(hw, IXGBE_VFTA(regindex));
	if (vlan_on) {
		bits |= (1 << bitindex);
	} else {
		bits &= ~(1 << bitindex);
	}
	IXGBE_WRITE_REG(hw, IXGBE_VFTA(regindex), bits);


	/*
	 * Part 2
	 * If the vind is set
	 *   Either vlan_on
	 *	make sure the vlan is in VLVF
	 *	set the vind bit in the matching VLVFB
	 *   Or !vlan_on
	 *	clear the pool bit and possibly the vind
	 */
	if (vind) {
		/* find the vlanid or the first empty slot */
		first_empty_slot = 0;

		for (regindex = 1; regindex < IXGBE_VLVF_ENTRIES; regindex++) {
			bits = IXGBE_READ_REG(hw, IXGBE_VLVF(regindex));
			if (!bits && !first_empty_slot)
				first_empty_slot = regindex;
			else if ((bits & 0x0FFF) == vlan)
				break;
		}

		if (regindex >= IXGBE_VLVF_ENTRIES) {
			if (first_empty_slot)
				regindex = first_empty_slot;
			else {
				DEBUGOUT("No space in VLVF.\n");
			}
		}


		if (vlan_on) {
			/* set the pool bit */
			if (vind < 32) {
				bits =
				    IXGBE_READ_REG(hw, IXGBE_VLVFB(regindex*2));
				bits |= (1 << vind);
				IXGBE_WRITE_REG(hw,
				    IXGBE_VLVFB(regindex*2), bits);
			} else {
				bits = IXGBE_READ_REG(hw,
				    IXGBE_VLVFB((regindex*2)+1));
				bits |= (1 << vind);
				IXGBE_WRITE_REG(hw,
				    IXGBE_VLVFB((regindex*2)+1), bits);
			}
		} else {
			/* clear the pool bit */
			if (vind < 32) {
				bits = IXGBE_READ_REG(hw,
				    IXGBE_VLVFB(regindex*2));
			bits &= ~(1 << vind);
				IXGBE_WRITE_REG(hw,
				    IXGBE_VLVFB(regindex*2), bits);
				bits |= IXGBE_READ_REG(hw,
				    IXGBE_VLVFB((regindex*2)+1));
			} else {
				bits = IXGBE_READ_REG(hw,
				    IXGBE_VLVFB((regindex*2)+1));
				bits &= ~(1 << vind);
				IXGBE_WRITE_REG(hw,
				    IXGBE_VLVFB((regindex*2)+1), bits);
				bits |= IXGBE_READ_REG(hw,
				    IXGBE_VLVFB(regindex*2));
			}
		}

		if (bits)
			IXGBE_WRITE_REG(hw, IXGBE_VLVF(regindex),
			    (IXGBE_VLVF_VIEN | vlan));
		else
			IXGBE_WRITE_REG(hw, IXGBE_VLVF(regindex), 0);
	}

	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_clear_vfta_82599 - Clear VLAN filter table
 * @hw: pointer to hardware structure
 *
 * Clears the VLAN filer table, and the VMDq index associated with the filter
 */
s32
ixgbe_clear_vfta_82599(struct ixgbe_hw *hw)
{
	u32 offset;

	for (offset = 0; offset < hw->mac.vft_size; offset++)
		IXGBE_WRITE_REG(hw, IXGBE_VFTA(offset), 0);

	for (offset = 0; offset < IXGBE_VLVF_ENTRIES; offset++) {
		IXGBE_WRITE_REG(hw, IXGBE_VLVF(offset), 0);
		IXGBE_WRITE_REG(hw, IXGBE_VLVFB(offset*2), 0);
		IXGBE_WRITE_REG(hw, IXGBE_VLVFB((offset*2)+1), 0);
	}

	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_init_uta_tables_82599 - Initialize the Unicast Table Array
 * @hw: pointer to hardware structure
 */
s32
ixgbe_init_uta_tables_82599(struct ixgbe_hw *hw)
{
	int i;
	DEBUGOUT(" Clearing UTA\n");

	for (i = 0; i < 128; i++)
		IXGBE_WRITE_REG(hw, IXGBE_UTA(i), 0);

	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_reinit_fdir_tables_82599 - Reinitialize Flow Director tables.
 * @hw: pointer to hardware structure
 */
s32
ixgbe_reinit_fdir_tables_82599(struct ixgbe_hw *hw)
{
	u32 fdirctrl = IXGBE_READ_REG(hw, IXGBE_FDIRCTRL);
	fdirctrl &= ~IXGBE_FDIRCTRL_INIT_DONE;
	IXGBE_WRITE_REG(hw, IXGBE_FDIRFREE, 0);
	IXGBE_WRITE_FLUSH(hw);
	IXGBE_WRITE_REG(hw, IXGBE_FDIRCTRL, fdirctrl);

	return (IXGBE_SUCCESS);
}

#define	IXGBE_FDIR_INIT_DONE_POLL	10
/*
 * ixgbe_init_fdir_signature_82599 - Initialize Flow Director signature filters
 * @hw: pointer to hardware structure
 * @pballoc: which mode to allocate filters with
 */
s32
ixgbe_init_fdir_signature_82599(struct ixgbe_hw *hw, u32 pballoc)
{
	u32 fdirctrl = 0;
	u32 pbsize;
	int i;

	/*
	 * Before enabling Flow Director, the Rx Packet Buffer size
	 * must be reduced.  The new value is the current size minus
	 * flow director memory usage size.
	 */
	pbsize = (1 << (IXGBE_FDIR_PBALLOC_SIZE_SHIFT + pballoc));
	IXGBE_WRITE_REG(hw, IXGBE_RXPBSIZE(0),
	    IXGBE_READ_REG(hw, IXGBE_RXPBSIZE(0)) - pbsize);

	/*
	 * The defaults in the HW for RX PB 1-7 are not zero and so should be
	 * intialized to zero for non DCB mode otherwise actual total RX PB
	 * would be bigger than programmed and filter space would run into
	 * the PB 0 region.
	 */
	for (i = 1; i < 8; i++)
		IXGBE_WRITE_REG(hw, IXGBE_RXPBSIZE(i), 0);

	/* Send interrupt when 64 filters are left */
	fdirctrl |= 4 << IXGBE_FDIRCTRL_FULL_THRESH_SHIFT;

	/* Set the maximum length per hash bucket to 0xA filters */
	fdirctrl |= 0xA << IXGBE_FDIRCTRL_MAX_LENGTH_SHIFT;

	switch (pballoc) {
	case IXGBE_FDIR_PBALLOC_64K:
		/* 8k - 1 signature filters */
		fdirctrl |= IXGBE_FDIRCTRL_PBALLOC_64K;
		break;
	case IXGBE_FDIR_PBALLOC_128K:
		/* 16k - 1 signature filters */
		fdirctrl |= IXGBE_FDIRCTRL_PBALLOC_128K;
		break;
	case IXGBE_FDIR_PBALLOC_256K:
		/* 32k - 1 signature filters */
		fdirctrl |= IXGBE_FDIRCTRL_PBALLOC_256K;
		break;
	default:
		/* bad value */
		return (IXGBE_ERR_CONFIG);
	};

	/* Move the flexible bytes to use the ethertype - shift 6 words */
	fdirctrl |= (0x6 << IXGBE_FDIRCTRL_FLEX_SHIFT);

	/* Prime the keys for hashing */
	IXGBE_WRITE_REG(hw, IXGBE_FDIRHKEY,
	    IXGBE_HTONL(IXGBE_ATR_BUCKET_HASH_KEY));
	IXGBE_WRITE_REG(hw, IXGBE_FDIRSKEY,
	    IXGBE_HTONL(IXGBE_ATR_SIGNATURE_HASH_KEY));

	/*
	 * Poll init-done after we write the register.  Estimated times:
	 *   10G: PBALLOC = 11b, timing is 60us
	 *    1G: PBALLOC = 11b, timing is 600us
	 *  100M: PBALLOC = 11b, timing is 6ms
	 *
	 *   Multiple these timings by 4 if under full Rx load
	 *
	 * So we'll poll for IXGBE_FDIR_INIT_DONE_POLL times, sleeping for
	 * 1 msec per poll time.  If we're at line rate and drop to 100M, then
	 * this might not finish in our poll time, but we can live with that
	 * for now.
	 */
	IXGBE_WRITE_REG(hw, IXGBE_FDIRCTRL, fdirctrl);
	IXGBE_WRITE_FLUSH(hw);
	for (i = 0; i < IXGBE_FDIR_INIT_DONE_POLL; i++) {
		if (IXGBE_READ_REG(hw, IXGBE_FDIRCTRL) &
		    IXGBE_FDIRCTRL_INIT_DONE)
			break;

		msec_delay(1);
	}
	if (i >= IXGBE_FDIR_INIT_DONE_POLL) {
		DEBUGOUT("Flow Director Signature poll time exceeded!\n");
	}

	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_init_fdir_perfect_82599 - Initialize Flow Director perfect filters
 * @hw: pointer to hardware structure
 * @pballoc: which mode to allocate filters with
 */
s32
ixgbe_init_fdir_perfect_82599(struct ixgbe_hw *hw, u32 pballoc)
{
	u32 fdirctrl = 0;
	u32 pbsize;
	int i;

	/*
	 * Before enabling Flow Director, the Rx Packet Buffer size
	 * must be reduced.  The new value is the current size minus
	 * flow director memory usage size.
	 */

	pbsize = (1 << (IXGBE_FDIR_PBALLOC_SIZE_SHIFT + pballoc));
	IXGBE_WRITE_REG(hw, IXGBE_RXPBSIZE(0),
	    IXGBE_READ_REG(hw, IXGBE_RXPBSIZE(0)) - pbsize);

	/*
	 * The defaults in the HW for RX PB 1-7 are not zero and so should be
	 * intialized to zero for non DCB mode otherwise actual total RX PB
	 * would be bigger than programmed and filter space would run into
	 * the PB 0 region.
	 */
	for (i = 1; i < 8; i++)
		IXGBE_WRITE_REG(hw, IXGBE_RXPBSIZE(i), 0);

	/* Send interrupt when 64 filters are left */
	fdirctrl |= 4 << IXGBE_FDIRCTRL_FULL_THRESH_SHIFT;

	switch (pballoc) {
	case IXGBE_FDIR_PBALLOC_64K:
		/* 2k - 1 perfect filters */
		fdirctrl |= IXGBE_FDIRCTRL_PBALLOC_64K;
		break;
	case IXGBE_FDIR_PBALLOC_128K:
		/* 4k - 1 perfect filters */
		fdirctrl |= IXGBE_FDIRCTRL_PBALLOC_128K;
		break;
	case IXGBE_FDIR_PBALLOC_256K:
		/* 8k - 1 perfect filters */
		fdirctrl |= IXGBE_FDIRCTRL_PBALLOC_256K;
		break;
	default:
		/* bad value */
		return (IXGBE_ERR_CONFIG);
	};

	/* Turn perfect match filtering on */
	fdirctrl |= IXGBE_FDIRCTRL_PERFECT_MATCH;
	fdirctrl |= IXGBE_FDIRCTRL_REPORT_STATUS;

	/* Move the flexible bytes to use the ethertype - shift 6 words */
	fdirctrl |= (0x6 << IXGBE_FDIRCTRL_FLEX_SHIFT);

	/* Prime the keys for hashing */
	IXGBE_WRITE_REG(hw, IXGBE_FDIRHKEY,
	    IXGBE_HTONL(IXGBE_ATR_BUCKET_HASH_KEY));
	IXGBE_WRITE_REG(hw, IXGBE_FDIRSKEY,
	    IXGBE_HTONL(IXGBE_ATR_SIGNATURE_HASH_KEY));

	/*
	 * Poll init-done after we write the register.  Estimated times:
	 *   10G: PBALLOC = 11b, timing is 60us
	 *    1G: PBALLOC = 11b, timing is 600us
	 *  100M: PBALLOC = 11b, timing is 6ms
	 *
	 *  Multiple these timings by 4 if under full Rx load
	 *
	 * So we'll poll for IXGBE_FDIR_INIT_DONE_POLL times, sleeping for
	 * 1 msec per poll time.  If we're at line rate and drop to 100M, then
	 * this might not finish in our poll time, but we can live with that
	 * for now.
	 */

	/* Set the maximum length per hash bucket to 0xA filters */
	fdirctrl |= (0xA << IXGBE_FDIRCTRL_MAX_LENGTH_SHIFT);

	IXGBE_WRITE_REG(hw, IXGBE_FDIRCTRL, fdirctrl);
	IXGBE_WRITE_FLUSH(hw);
	for (i = 0; i < IXGBE_FDIR_INIT_DONE_POLL; i++) {
		if (IXGBE_READ_REG(hw, IXGBE_FDIRCTRL) &
		    IXGBE_FDIRCTRL_INIT_DONE)
			break;

		msec_delay(1);
	}
	if (i >= IXGBE_FDIR_INIT_DONE_POLL) {
		DEBUGOUT("Flow Director Perfect poll time exceeded!\n");
	}

	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_atr_compute_hash_82599 - Compute the hashes for SW ATR
 * @stream: input bitstream to compute the hash on
 * @key: 32-bit hash key
 */
u16
ixgbe_atr_compute_hash_82599(struct ixgbe_atr_input *atr_input, u32 key)
{
	/*
	 * The algorithm is as follows:
	 *    Hash[15:0] = Sum { S[n] x K[n+16] }, n = 0...350
	 *    where Sum {A[n]}, n = 0...n is bitwise XOR of A[0], A[1]...A[n]
	 *    and A[n] x B[n] is bitwise AND between same length strings
	 *
	 *    K[n] is 16 bits, defined as:
	 *	for n modulo 32 >= 15, K[n] = K[n % 32 : (n % 32) - 15]
	 *	for n modulo 32 < 15, K[n] =
	 *		K[(n % 32:0) | (31:31 - (14 - (n % 32)))]
	 *
	 *    S[n] is 16 bits, defined as:
	 *	for n >= 15, S[n] = S[n:n - 15]
	 *	for n < 15, S[n] = S[(n:0) | (350:350 - (14 - n))]
	 *
	 *    To simplify for programming, the algorithm is implemented
	 *    in software this way:
	 *
	 *    Key[31:0], Stream[335:0]
	 *
	 *    tmp_key[11 * 32 - 1:0] = 11{Key[31:0] = key concatenated 11 times
	 *    int_key[350:0] = tmp_key[351:1]
	 *    int_stream[365:0] = Stream[14:0] | Stream[335:0] | Stream[335:321]
	 *
	 *    hash[15:0] = 0;
	 *    for (i = 0; i < 351; i++) {
	 *	if (int_key[i])
	 *	hash ^= int_stream[(i + 15):i];
	 *    }
	 */

	union {
		u32	key[11];
		u8	key_stream[44];
	} tmp_key;

	u8 *stream = (u8 *)atr_input;
	u8 int_key[44];		/* upper-most bit unused */
	u8 hash_str[46];	/* upper-most 2 bits unused */
	u16 hash_result = 0;
	u16 tmp = 0;
	int i, j, k, h;

	(void) memset(&tmp_key, 0, sizeof (tmp_key));
	/* First load the temporary key stream */
	for (i = 0; i < 11; i++)
		tmp_key.key[i] = key;

	/*
	 * Set the interim key for the hashing.  Bit 352 is unused, so we must
	 * shift and compensate when building the key.
	 */
	int_key[0] = tmp_key.key_stream[0] >> 1;
	for (i = 1, j = 0; i < 44; i++) {
		int_key[i] = (tmp_key.key_stream[j] & 0x1) << 7;
		j++;
		int_key[i] |= tmp_key.key_stream[j] >> 1;
	}

	/*
	 * Set the interim bit string for the hashing.  Bits 368 and 367 are
	 * unused, so shift and compensate when building the string.
	 */
	hash_str[0] = (stream[40] & 0x7f) >> 1;
	for (i = 1, j = 40; i < 46; i++) {
		hash_str[i] = (stream[j] & 0x1) << 7;
		j++;
		if (j > 41)
			j = 0;
		hash_str[i] |= stream[j] >> 1;
	}

	/*
	 * Now compute the hash.  i is the index into hash_str, j is into our
	 * key stream, k is counting the number of bits, and h interates within
	 * each byte.
	 */
	for (i = 45, j = 43, k = 0; k < 351 && i >= 2 && j >= 0; i--, j--) {
		for (h = 0; h < 8 && k < 351; h++, k++) {
			if ((int_key[j] >> h) & 0x1) {
				/*
				 * Key bit is set, XOR in the current 16-bit
				 * string.  Example of processing:
				 *	h = 0,
				 *	tmp = (hash_str[i - 2] & 0 << 16) |
				 *		(hash_str[i - 1] & 0xff << 8) |
				 *		(hash_str[i] & 0xff >> 0)
				 *	So tmp = hash_str[15 + k:k], since the
				 *	i + 2 clause rolls off the 16-bit value
				 *	h = 7,
				 *	tmp = (hash_str[i - 2] & 0x7f << 9) |
				 *		(hash_str[i - 1] & 0xff << 1) |
				 *		(hash_str[i] & 0x80 >> 7)
				 */
				tmp = ((hash_str[i] & (0xff << h)) >> h);
				tmp |= ((hash_str[i - 1] & 0xff) << (8 - h));
				tmp |= (hash_str[i - 2] & (0xff >> (8 - h)))
				    << (16 - h);
				hash_result ^= tmp;
			}
		}
	}

	return (hash_result);
}

/*
 * ixgbe_atr_set_vlan_id_82599 - Sets the VLAN id in the ATR input stream
 * @input: input stream to modify
 * @vlan: the VLAN id to load
 */
s32
ixgbe_atr_set_vlan_id_82599(struct ixgbe_atr_input *input, u16 vlan)
{
	input->byte_stream[IXGBE_ATR_VLAN_OFFSET + 1] = vlan >> 8;
	input->byte_stream[IXGBE_ATR_VLAN_OFFSET] = vlan & 0xff;

	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_atr_set_src_ipv4_82599 - Sets the source IPv4 address
 * @input: input stream to modify
 * @src_addr: the IP address to load
 */
s32
ixgbe_atr_set_src_ipv4_82599(struct ixgbe_atr_input *input, u32 src_addr)
{
	input->byte_stream[IXGBE_ATR_SRC_IPV4_OFFSET + 3] = src_addr >> 24;
	input->byte_stream[IXGBE_ATR_SRC_IPV4_OFFSET + 2] =
	    (src_addr >> 16) & 0xff;
	input->byte_stream[IXGBE_ATR_SRC_IPV4_OFFSET + 1] =
	    (src_addr >> 8) & 0xff;
	input->byte_stream[IXGBE_ATR_SRC_IPV4_OFFSET] = src_addr & 0xff;

	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_atr_set_dst_ipv4_82599 - Sets the destination IPv4 address
 * @input: input stream to modify
 * @dst_addr: the IP address to load
 */
s32
ixgbe_atr_set_dst_ipv4_82599(struct ixgbe_atr_input *input, u32 dst_addr)
{
	input->byte_stream[IXGBE_ATR_DST_IPV4_OFFSET + 3] = dst_addr >> 24;
	input->byte_stream[IXGBE_ATR_DST_IPV4_OFFSET + 2] =
	    (dst_addr >> 16) & 0xff;
	input->byte_stream[IXGBE_ATR_DST_IPV4_OFFSET + 1] =
	    (dst_addr >> 8) & 0xff;
	input->byte_stream[IXGBE_ATR_DST_IPV4_OFFSET] = dst_addr & 0xff;

	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_atr_set_src_ipv6_82599 - Sets the source IPv6 address
 * @input: input stream to modify
 * @src_addr_1: the first 4 bytes of the IP address to load
 * @src_addr_2: the second 4 bytes of the IP address to load
 * @src_addr_3: the third 4 bytes of the IP address to load
 * @src_addr_4: the fourth 4 bytes of the IP address to load
 */
s32
ixgbe_atr_set_src_ipv6_82599(struct ixgbe_atr_input *input,
    u32 src_addr_1, u32 src_addr_2, u32 src_addr_3, u32 src_addr_4)
{
	input->byte_stream[IXGBE_ATR_SRC_IPV6_OFFSET] = src_addr_4 & 0xff;
	input->byte_stream[IXGBE_ATR_SRC_IPV6_OFFSET + 1] =
	    (src_addr_4 >> 8) & 0xff;
	input->byte_stream[IXGBE_ATR_SRC_IPV6_OFFSET + 2] =
	    (src_addr_4 >> 16) & 0xff;
	input->byte_stream[IXGBE_ATR_SRC_IPV6_OFFSET + 3] = src_addr_4 >> 24;

	input->byte_stream[IXGBE_ATR_SRC_IPV6_OFFSET + 4] = src_addr_3 & 0xff;
	input->byte_stream[IXGBE_ATR_SRC_IPV6_OFFSET + 5] =
	    (src_addr_3 >> 8) & 0xff;
	input->byte_stream[IXGBE_ATR_SRC_IPV6_OFFSET + 6] =
	    (src_addr_3 >> 16) & 0xff;
	input->byte_stream[IXGBE_ATR_SRC_IPV6_OFFSET + 7] = src_addr_3 >> 24;

	input->byte_stream[IXGBE_ATR_SRC_IPV6_OFFSET + 8] = src_addr_2 & 0xff;
	input->byte_stream[IXGBE_ATR_SRC_IPV6_OFFSET + 9] =
	    (src_addr_2 >> 8) & 0xff;
	input->byte_stream[IXGBE_ATR_SRC_IPV6_OFFSET + 10] =
	    (src_addr_2 >> 16) & 0xff;
	input->byte_stream[IXGBE_ATR_SRC_IPV6_OFFSET + 11] = src_addr_2 >> 24;

	input->byte_stream[IXGBE_ATR_SRC_IPV6_OFFSET + 12] = src_addr_1 & 0xff;
	input->byte_stream[IXGBE_ATR_SRC_IPV6_OFFSET + 13] =
	    (src_addr_1 >> 8) & 0xff;
	input->byte_stream[IXGBE_ATR_SRC_IPV6_OFFSET + 14] =
	    (src_addr_1 >> 16) & 0xff;
	input->byte_stream[IXGBE_ATR_SRC_IPV6_OFFSET + 15] = src_addr_1 >> 24;

	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_atr_set_dst_ipv6_82599 - Sets the destination IPv6 address
 * @input: input stream to modify
 * @dst_addr_1: the first 4 bytes of the IP address to load
 * @dst_addr_2: the second 4 bytes of the IP address to load
 * @dst_addr_3: the third 4 bytes of the IP address to load
 * @dst_addr_4: the fourth 4 bytes of the IP address to load
 */
s32
ixgbe_atr_set_dst_ipv6_82599(struct ixgbe_atr_input *input,
    u32 dst_addr_1, u32 dst_addr_2, u32 dst_addr_3, u32 dst_addr_4)
{
	input->byte_stream[IXGBE_ATR_DST_IPV6_OFFSET] = dst_addr_4 & 0xff;
	input->byte_stream[IXGBE_ATR_DST_IPV6_OFFSET + 1] =
	    (dst_addr_4 >> 8) & 0xff;
	input->byte_stream[IXGBE_ATR_DST_IPV6_OFFSET + 2] =
	    (dst_addr_4 >> 16) & 0xff;
	input->byte_stream[IXGBE_ATR_DST_IPV6_OFFSET + 3] = dst_addr_4 >> 24;

	input->byte_stream[IXGBE_ATR_DST_IPV6_OFFSET + 4] = dst_addr_3 & 0xff;
	input->byte_stream[IXGBE_ATR_DST_IPV6_OFFSET + 5] =
	    (dst_addr_3 >> 8) & 0xff;
	input->byte_stream[IXGBE_ATR_DST_IPV6_OFFSET + 6] =
	    (dst_addr_3 >> 16) & 0xff;
	input->byte_stream[IXGBE_ATR_DST_IPV6_OFFSET + 7] = dst_addr_3 >> 24;

	input->byte_stream[IXGBE_ATR_DST_IPV6_OFFSET + 8] = dst_addr_2 & 0xff;
	input->byte_stream[IXGBE_ATR_DST_IPV6_OFFSET + 9] =
	    (dst_addr_2 >> 8) & 0xff;
	input->byte_stream[IXGBE_ATR_DST_IPV6_OFFSET + 10] =
	    (dst_addr_2 >> 16) & 0xff;
	input->byte_stream[IXGBE_ATR_DST_IPV6_OFFSET + 11] = dst_addr_2 >> 24;

	input->byte_stream[IXGBE_ATR_DST_IPV6_OFFSET + 12] = dst_addr_1 & 0xff;
	input->byte_stream[IXGBE_ATR_DST_IPV6_OFFSET + 13] =
	    (dst_addr_1 >> 8) & 0xff;
	input->byte_stream[IXGBE_ATR_DST_IPV6_OFFSET + 14] =
	    (dst_addr_1 >> 16) & 0xff;
	input->byte_stream[IXGBE_ATR_DST_IPV6_OFFSET + 15] = dst_addr_1 >> 24;

	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_atr_set_src_port_82599 - Sets the source port
 * @input: input stream to modify
 * @src_port: the source port to load
 */
s32
ixgbe_atr_set_src_port_82599(struct ixgbe_atr_input *input, u16 src_port)
{
	input->byte_stream[IXGBE_ATR_SRC_PORT_OFFSET + 1] = src_port >> 8;
	input->byte_stream[IXGBE_ATR_SRC_PORT_OFFSET] = src_port & 0xff;

	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_atr_set_dst_port_82599 - Sets the destination port
 * @input: input stream to modify
 * @dst_port: the destination port to load
 */
s32
ixgbe_atr_set_dst_port_82599(struct ixgbe_atr_input *input, u16 dst_port)
{
	input->byte_stream[IXGBE_ATR_DST_PORT_OFFSET + 1] = dst_port >> 8;
	input->byte_stream[IXGBE_ATR_DST_PORT_OFFSET] = dst_port & 0xff;

	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_atr_set_flex_byte_82599 - Sets the flexible bytes
 * @input: input stream to modify
 * @flex_bytes: the flexible bytes to load
 */
s32
ixgbe_atr_set_flex_byte_82599(struct ixgbe_atr_input *input, u16 flex_byte)
{
	input->byte_stream[IXGBE_ATR_FLEX_BYTE_OFFSET + 1] = flex_byte >> 8;
	input->byte_stream[IXGBE_ATR_FLEX_BYTE_OFFSET] = flex_byte & 0xff;

	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_atr_set_vm_pool_82599 - Sets the Virtual Machine pool
 * @input: input stream to modify
 * @vm_pool: the Virtual Machine pool to load
 */
s32
ixgbe_atr_set_vm_pool_82599(struct ixgbe_atr_input *input, u8 vm_pool)
{
	input->byte_stream[IXGBE_ATR_VM_POOL_OFFSET] = vm_pool;

	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_atr_set_l4type_82599 - Sets the layer 4 packet type
 * @input: input stream to modify
 * @l4type: the layer 4 type value to load
 */
s32
ixgbe_atr_set_l4type_82599(struct ixgbe_atr_input *input, u8 l4type)
{
	input->byte_stream[IXGBE_ATR_L4TYPE_OFFSET] = l4type;

	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_atr_get_vlan_id_82599 - Gets the VLAN id from the ATR input stream
 * @input: input stream to search
 * @vlan: the VLAN id to load
 */
s32
ixgbe_atr_get_vlan_id_82599(struct ixgbe_atr_input *input, u16 *vlan)
{
	*vlan = input->byte_stream[IXGBE_ATR_VLAN_OFFSET];
	*vlan |= input->byte_stream[IXGBE_ATR_VLAN_OFFSET + 1] << 8;

	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_atr_get_src_ipv4_82599 - Gets the source IPv4 address
 * @input: input stream to search
 * @src_addr: the IP address to load
 */
s32
ixgbe_atr_get_src_ipv4_82599(struct ixgbe_atr_input *input, u32 *src_addr)
{
	*src_addr = input->byte_stream[IXGBE_ATR_SRC_IPV4_OFFSET];
	*src_addr |= input->byte_stream[IXGBE_ATR_SRC_IPV4_OFFSET + 1] << 8;
	*src_addr |= input->byte_stream[IXGBE_ATR_SRC_IPV4_OFFSET + 2] << 16;
	*src_addr |= input->byte_stream[IXGBE_ATR_SRC_IPV4_OFFSET + 3] << 24;

	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_atr_get_dst_ipv4_82599 - Gets the destination IPv4 address
 * @input: input stream to search
 * @dst_addr: the IP address to load
 */
s32
ixgbe_atr_get_dst_ipv4_82599(struct ixgbe_atr_input *input, u32 *dst_addr)
{
	*dst_addr = input->byte_stream[IXGBE_ATR_DST_IPV4_OFFSET];
	*dst_addr |= input->byte_stream[IXGBE_ATR_DST_IPV4_OFFSET + 1] << 8;
	*dst_addr |= input->byte_stream[IXGBE_ATR_DST_IPV4_OFFSET + 2] << 16;
	*dst_addr |= input->byte_stream[IXGBE_ATR_DST_IPV4_OFFSET + 3] << 24;

	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_atr_get_src_ipv6_82599 - Gets the source IPv6 address
 * @input: input stream to search
 * @src_addr_1: the first 4 bytes of the IP address to load
 * @src_addr_2: the second 4 bytes of the IP address to load
 * @src_addr_3: the third 4 bytes of the IP address to load
 * @src_addr_4: the fourth 4 bytes of the IP address to load
 */
s32
ixgbe_atr_get_src_ipv6_82599(struct ixgbe_atr_input *input,
    u32 *src_addr_1, u32 *src_addr_2, u32 *src_addr_3, u32 *src_addr_4)
{
	*src_addr_1 = input->byte_stream[IXGBE_ATR_SRC_IPV6_OFFSET + 12];
	*src_addr_1 = input->byte_stream[IXGBE_ATR_SRC_IPV6_OFFSET + 13] << 8;
	*src_addr_1 = input->byte_stream[IXGBE_ATR_SRC_IPV6_OFFSET + 14] << 16;
	*src_addr_1 = input->byte_stream[IXGBE_ATR_SRC_IPV6_OFFSET + 15] << 24;

	*src_addr_2 = input->byte_stream[IXGBE_ATR_SRC_IPV6_OFFSET + 8];
	*src_addr_2 = input->byte_stream[IXGBE_ATR_SRC_IPV6_OFFSET + 9] << 8;
	*src_addr_2 = input->byte_stream[IXGBE_ATR_SRC_IPV6_OFFSET + 10] << 16;
	*src_addr_2 = input->byte_stream[IXGBE_ATR_SRC_IPV6_OFFSET + 11] << 24;

	*src_addr_3 = input->byte_stream[IXGBE_ATR_SRC_IPV6_OFFSET + 4];
	*src_addr_3 = input->byte_stream[IXGBE_ATR_SRC_IPV6_OFFSET + 5] << 8;
	*src_addr_3 = input->byte_stream[IXGBE_ATR_SRC_IPV6_OFFSET + 6] << 16;
	*src_addr_3 = input->byte_stream[IXGBE_ATR_SRC_IPV6_OFFSET + 7] << 24;

	*src_addr_4 = input->byte_stream[IXGBE_ATR_SRC_IPV6_OFFSET];
	*src_addr_4 = input->byte_stream[IXGBE_ATR_SRC_IPV6_OFFSET + 1] << 8;
	*src_addr_4 = input->byte_stream[IXGBE_ATR_SRC_IPV6_OFFSET + 2] << 16;
	*src_addr_4 = input->byte_stream[IXGBE_ATR_SRC_IPV6_OFFSET + 3] << 24;

	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_atr_get_dst_ipv6_82599 - Gets the destination IPv6 address
 * @input: input stream to search
 * @dst_addr_1: the first 4 bytes of the IP address to load
 * @dst_addr_2: the second 4 bytes of the IP address to load
 * @dst_addr_3: the third 4 bytes of the IP address to load
 * @dst_addr_4: the fourth 4 bytes of the IP address to load
 */
s32
ixgbe_atr_get_dst_ipv6_82599(struct ixgbe_atr_input *input,
    u32 *dst_addr_1, u32 *dst_addr_2, u32 *dst_addr_3, u32 *dst_addr_4)
{
	*dst_addr_1 = input->byte_stream[IXGBE_ATR_DST_IPV6_OFFSET + 12];
	*dst_addr_1 = input->byte_stream[IXGBE_ATR_DST_IPV6_OFFSET + 13] << 8;
	*dst_addr_1 = input->byte_stream[IXGBE_ATR_DST_IPV6_OFFSET + 14] << 16;
	*dst_addr_1 = input->byte_stream[IXGBE_ATR_DST_IPV6_OFFSET + 15] << 24;

	*dst_addr_2 = input->byte_stream[IXGBE_ATR_DST_IPV6_OFFSET + 8];
	*dst_addr_2 = input->byte_stream[IXGBE_ATR_DST_IPV6_OFFSET + 9] << 8;
	*dst_addr_2 = input->byte_stream[IXGBE_ATR_DST_IPV6_OFFSET + 10] << 16;
	*dst_addr_2 = input->byte_stream[IXGBE_ATR_DST_IPV6_OFFSET + 11] << 24;

	*dst_addr_3 = input->byte_stream[IXGBE_ATR_DST_IPV6_OFFSET + 4];
	*dst_addr_3 = input->byte_stream[IXGBE_ATR_DST_IPV6_OFFSET + 5] << 8;
	*dst_addr_3 = input->byte_stream[IXGBE_ATR_DST_IPV6_OFFSET + 6] << 16;
	*dst_addr_3 = input->byte_stream[IXGBE_ATR_DST_IPV6_OFFSET + 7] << 24;

	*dst_addr_4 = input->byte_stream[IXGBE_ATR_DST_IPV6_OFFSET];
	*dst_addr_4 = input->byte_stream[IXGBE_ATR_DST_IPV6_OFFSET + 1] << 8;
	*dst_addr_4 = input->byte_stream[IXGBE_ATR_DST_IPV6_OFFSET + 2] << 16;
	*dst_addr_4 = input->byte_stream[IXGBE_ATR_DST_IPV6_OFFSET + 3] << 24;

	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_atr_get_src_port_82599 - Gets the source port
 * @input: input stream to modify
 * @src_port: the source port to load
 *
 * Even though the input is given in big-endian, the FDIRPORT registers
 * expect the ports to be programmed in little-endian.  Hence the need to swap
 * endianness when retrieving the data.  This can be confusing since the
 * internal hash engine expects it to be big-endian.
 */
s32
ixgbe_atr_get_src_port_82599(struct ixgbe_atr_input *input, u16 *src_port)
{
	*src_port = input->byte_stream[IXGBE_ATR_SRC_PORT_OFFSET] << 8;
	*src_port |= input->byte_stream[IXGBE_ATR_SRC_PORT_OFFSET + 1];

	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_atr_get_dst_port_82599 - Gets the destination port
 * @input: input stream to modify
 * @dst_port: the destination port to load
 *
 * Even though the input is given in big-endian, the FDIRPORT registers
 * expect the ports to be programmed in little-endian.  Hence the need to swap
 * endianness when retrieving the data.  This can be confusing since the
 * internal hash engine expects it to be big-endian.
 */
s32
ixgbe_atr_get_dst_port_82599(struct ixgbe_atr_input *input, u16 *dst_port)
{
	*dst_port = input->byte_stream[IXGBE_ATR_DST_PORT_OFFSET] << 8;
	*dst_port |= input->byte_stream[IXGBE_ATR_DST_PORT_OFFSET + 1];

	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_atr_get_flex_byte_82599 - Gets the flexible bytes
 * @input: input stream to modify
 * @flex_bytes: the flexible bytes to load
 */
s32
ixgbe_atr_get_flex_byte_82599(struct ixgbe_atr_input *input, u16 *flex_byte)
{
	*flex_byte = input->byte_stream[IXGBE_ATR_FLEX_BYTE_OFFSET];
	*flex_byte |= input->byte_stream[IXGBE_ATR_FLEX_BYTE_OFFSET + 1] << 8;

	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_atr_get_vm_pool_82599 - Gets the Virtual Machine pool
 * @input: input stream to modify
 * @vm_pool: the Virtual Machine pool to load
 */
s32
ixgbe_atr_get_vm_pool_82599(struct ixgbe_atr_input *input, u8 *vm_pool)
{
	*vm_pool = input->byte_stream[IXGBE_ATR_VM_POOL_OFFSET];

	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_atr_get_l4type_82599 - Gets the layer 4 packet type
 * @input: input stream to modify
 * @l4type: the layer 4 type value to load
 */
s32
ixgbe_atr_get_l4type_82599(struct ixgbe_atr_input *input, u8 *l4type)
{
	*l4type = input->byte_stream[IXGBE_ATR_L4TYPE_OFFSET];

	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_atr_add_signature_filter_82599 - Adds a signature hash filter
 * @hw: pointer to hardware structure
 * @stream: input bitstream
 * @queue: queue index to direct traffic to
 */
s32
ixgbe_fdir_add_signature_filter_82599(struct ixgbe_hw *hw,
    struct ixgbe_atr_input *input, u8 queue)
{
	u64  fdirhashcmd;
	u64  fdircmd;
	u32  fdirhash;
	u16  bucket_hash, sig_hash;
	u8   l4type;

	bucket_hash = ixgbe_atr_compute_hash_82599(input,
	    IXGBE_ATR_BUCKET_HASH_KEY);

	/* bucket_hash is only 15 bits */
	bucket_hash &= IXGBE_ATR_HASH_MASK;

	sig_hash = ixgbe_atr_compute_hash_82599(input,
	    IXGBE_ATR_SIGNATURE_HASH_KEY);

	/* Get the l4type in order to program FDIRCMD properly */
	/* lowest 2 bits are FDIRCMD.L4TYPE, third lowest bit is FDIRCMD.IPV6 */
	(void) ixgbe_atr_get_l4type_82599(input, &l4type);

	/*
	 * The lower 32-bits of fdirhashcmd is for FDIRHASH, the upper 32-bits
	 * is for FDIRCMD.  Then do a 64-bit register write from FDIRHASH.
	 */
	fdirhash = sig_hash << IXGBE_FDIRHASH_SIG_SW_INDEX_SHIFT | bucket_hash;

	fdircmd = (IXGBE_FDIRCMD_CMD_ADD_FLOW | IXGBE_FDIRCMD_FILTER_UPDATE |
	    IXGBE_FDIRCMD_LAST | IXGBE_FDIRCMD_QUEUE_EN);

	switch (l4type & IXGBE_ATR_L4TYPE_MASK) {
	case IXGBE_ATR_L4TYPE_TCP:
		fdircmd |= IXGBE_FDIRCMD_L4TYPE_TCP;
		break;
	case IXGBE_ATR_L4TYPE_UDP:
		fdircmd |= IXGBE_FDIRCMD_L4TYPE_UDP;
		break;
	case IXGBE_ATR_L4TYPE_SCTP:
		fdircmd |= IXGBE_FDIRCMD_L4TYPE_SCTP;
		break;
	default:
		DEBUGOUT(" Error on l4type input\n");
		return (IXGBE_ERR_CONFIG);
	}

	if (l4type & IXGBE_ATR_L4TYPE_IPV6_MASK)
		fdircmd |= IXGBE_FDIRCMD_IPV6;

	fdircmd |= ((u64)queue << IXGBE_FDIRCMD_RX_QUEUE_SHIFT);
	fdirhashcmd = ((fdircmd << 32) | fdirhash);

	DEBUGOUT2("Tx Queue=%x hash=%x\n", queue, fdirhash & 0x7FFF7FFF);
	IXGBE_WRITE_REG64(hw, IXGBE_FDIRHASH, fdirhashcmd);

	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_fdir_add_perfect_filter_82599 - Adds a perfect filter
 * @hw: pointer to hardware structure
 * @input: input bitstream
 * @queue: queue index to direct traffic to
 *
 * Note that the caller to this function must lock before calling, since the
 * hardware writes must be protected from one another.
 */
s32
ixgbe_fdir_add_perfect_filter_82599(struct ixgbe_hw *hw,
    struct ixgbe_atr_input *input, u16 soft_id, u8 queue)
{
	u32 fdircmd = 0;
	u32 fdirhash;
	u32 src_ipv4, dst_ipv4;
	u32 src_ipv6_1, src_ipv6_2, src_ipv6_3, src_ipv6_4;
	u16 src_port, dst_port, vlan_id, flex_bytes;
	u16 bucket_hash;
	u8  l4type;

	/* Get our input values */
	(void) ixgbe_atr_get_l4type_82599(input, &l4type);

	/*
	 * Check l4type formatting, and bail out before we touch the hardware
	 * if there's a configuration issue
	 */
	switch (l4type & IXGBE_ATR_L4TYPE_MASK) {
	case IXGBE_ATR_L4TYPE_TCP:
		fdircmd |= IXGBE_FDIRCMD_L4TYPE_TCP;
		break;
	case IXGBE_ATR_L4TYPE_UDP:
		fdircmd |= IXGBE_FDIRCMD_L4TYPE_UDP;
		break;
	case IXGBE_ATR_L4TYPE_SCTP:
		fdircmd |= IXGBE_FDIRCMD_L4TYPE_SCTP;
		break;
	default:
		DEBUGOUT(" Error on l4type input\n");
		return (IXGBE_ERR_CONFIG);
	}

	bucket_hash = ixgbe_atr_compute_hash_82599(input,
	    IXGBE_ATR_BUCKET_HASH_KEY);

	/* bucket_hash is only 15 bits */
	bucket_hash &= IXGBE_ATR_HASH_MASK;

	(void) ixgbe_atr_get_vlan_id_82599(input, &vlan_id);
	(void) ixgbe_atr_get_src_port_82599(input, &src_port);
	(void) ixgbe_atr_get_dst_port_82599(input, &dst_port);
	(void) ixgbe_atr_get_flex_byte_82599(input, &flex_bytes);

	fdirhash = soft_id << IXGBE_FDIRHASH_SIG_SW_INDEX_SHIFT | bucket_hash;

	/* Now figure out if we're IPv4 or IPv6 */
	if (l4type & IXGBE_ATR_L4TYPE_IPV6_MASK) {
		/* IPv6 */
		(void) ixgbe_atr_get_src_ipv6_82599(input, &src_ipv6_1,
		    &src_ipv6_2, &src_ipv6_3, &src_ipv6_4);

		IXGBE_WRITE_REG(hw, IXGBE_FDIRSIPv6(0), src_ipv6_1);
		IXGBE_WRITE_REG(hw, IXGBE_FDIRSIPv6(1), src_ipv6_2);
		IXGBE_WRITE_REG(hw, IXGBE_FDIRSIPv6(2), src_ipv6_3);
		/* The last 4 bytes is the same register as IPv4 */
		IXGBE_WRITE_REG(hw, IXGBE_FDIRIPSA, src_ipv6_4);

		fdircmd |= IXGBE_FDIRCMD_IPV6;
		fdircmd |= IXGBE_FDIRCMD_IPv6DMATCH;
	} else {
		/* IPv4 */
		(void) ixgbe_atr_get_src_ipv4_82599(input, &src_ipv4);
		IXGBE_WRITE_REG(hw, IXGBE_FDIRIPSA, src_ipv4);

	}

	(void) ixgbe_atr_get_dst_ipv4_82599(input, &dst_ipv4);
	IXGBE_WRITE_REG(hw, IXGBE_FDIRIPDA, dst_ipv4);

	IXGBE_WRITE_REG(hw, IXGBE_FDIRVLAN, (vlan_id |
	    (flex_bytes << IXGBE_FDIRVLAN_FLEX_SHIFT)));
	IXGBE_WRITE_REG(hw, IXGBE_FDIRPORT, (src_port |
	    (dst_port << IXGBE_FDIRPORT_DESTINATION_SHIFT)));

	fdircmd |= IXGBE_FDIRCMD_CMD_ADD_FLOW;
	fdircmd |= IXGBE_FDIRCMD_FILTER_UPDATE;
	fdircmd |= IXGBE_FDIRCMD_LAST;
	fdircmd |= IXGBE_FDIRCMD_QUEUE_EN;
	fdircmd |= queue << IXGBE_FDIRCMD_RX_QUEUE_SHIFT;

	IXGBE_WRITE_REG(hw, IXGBE_FDIRHASH, fdirhash);
	IXGBE_WRITE_REG(hw, IXGBE_FDIRCMD, fdircmd);

	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_read_analog_reg8_82599 - Reads 8 bit Omer analog register
 * @hw: pointer to hardware structure
 * @reg: analog register to read
 * @val: read value
 *
 * Performs read operation to Omer analog register specified.
 */
s32
ixgbe_read_analog_reg8_82599(struct ixgbe_hw *hw, u32 reg, u8 *val)
{
	u32  core_ctl;

	IXGBE_WRITE_REG(hw, IXGBE_CORECTL, IXGBE_CORECTL_WRITE_CMD |
	    (reg << 8));
	IXGBE_WRITE_FLUSH(hw);
	usec_delay(10);
	core_ctl = IXGBE_READ_REG(hw, IXGBE_CORECTL);
	*val = (u8)core_ctl;

	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_write_analog_reg8_82599 - Writes 8 bit Omer analog register
 * @hw: pointer to hardware structure
 * @reg: atlas register to write
 * @val: value to write
 *
 * Performs write operation to Omer analog register specified.
 */
s32
ixgbe_write_analog_reg8_82599(struct ixgbe_hw *hw, u32 reg, u8 val)
{
	u32  core_ctl;

	core_ctl = (reg << 8) | val;
	IXGBE_WRITE_REG(hw, IXGBE_CORECTL, core_ctl);
	IXGBE_WRITE_FLUSH(hw);
	usec_delay(10);

	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_start_hw_rev_1_82599 - Prepare hardware for Tx/Rx
 * @hw: pointer to hardware structure
 *
 * Starts the hardware using the generic start_hw function.
 * Then performs revision-specific operations:
 * Clears the rate limiter registers.
 */
s32
ixgbe_start_hw_rev_1_82599(struct ixgbe_hw *hw)
{
	u32 q_num;
	s32 ret_val = IXGBE_SUCCESS;

	ret_val = ixgbe_start_hw_generic(hw);

	/* Clear the rate limiters */
	for (q_num = 0; q_num < hw->mac.max_tx_queues; q_num++) {
		IXGBE_WRITE_REG(hw, IXGBE_RTTDQSEL, q_num);
		IXGBE_WRITE_REG(hw, IXGBE_RTTBCNRC, 0);
	}
	IXGBE_WRITE_FLUSH(hw);

	return (ret_val);
}

/*
 * ixgbe_identify_phy_82599 - Get physical layer module
 * @hw: pointer to hardware structure
 *
 * Determines the physical layer module found on the current adapter.
 * If PHY already detected, maintains current PHY type in hw struct,
 * otherwise executes the PHY detection routine.
 */
s32
ixgbe_identify_phy_82599(struct ixgbe_hw *hw)
{
	s32 status = IXGBE_ERR_PHY_ADDR_INVALID;

	/* Detect PHY if not unknown - returns success if already detected. */
	status = ixgbe_identify_phy_generic(hw);
	if (status != IXGBE_SUCCESS)
		status = ixgbe_identify_sfp_module_generic(hw);
	/* Set PHY type none if no PHY detected */
	if (hw->phy.type == ixgbe_phy_unknown) {
		hw->phy.type = ixgbe_phy_none;
		status = IXGBE_SUCCESS;
	}

	/* Return error if SFP module has been detected but is not supported */
	if (hw->phy.type == ixgbe_phy_sfp_unsupported)
		status = IXGBE_ERR_SFP_NOT_SUPPORTED;

	return (status);
}

/*
 * ixgbe_get_supported_physical_layer_82599 - Returns physical layer type
 * @hw: pointer to hardware structure
 *
 * Determines physical layer capabilities of the current configuration.
 */
u32
ixgbe_get_supported_physical_layer_82599(struct ixgbe_hw *hw)
{
	u32 physical_layer = IXGBE_PHYSICAL_LAYER_UNKNOWN;
	u32 autoc = IXGBE_READ_REG(hw, IXGBE_AUTOC);
	u32 autoc2 = IXGBE_READ_REG(hw, IXGBE_AUTOC2);
	u32 pma_pmd_10g_serial = autoc2 & IXGBE_AUTOC2_10G_SERIAL_PMA_PMD_MASK;
	u32 pma_pmd_10g_parallel = autoc & IXGBE_AUTOC_10G_PMA_PMD_MASK;
	u32 pma_pmd_1g = autoc & IXGBE_AUTOC_1G_PMA_PMD_MASK;
	u16 ext_ability = 0;
	u8 comp_codes_10g = 0;

	hw->phy.ops.identify(hw);

	if (hw->phy.type == ixgbe_phy_tn ||
	    hw->phy.type == ixgbe_phy_cu_unknown) {
		hw->phy.ops.read_reg(hw, IXGBE_MDIO_PHY_EXT_ABILITY,
		    IXGBE_MDIO_PMA_PMD_DEV_TYPE, &ext_ability);
		if (ext_ability & IXGBE_MDIO_PHY_10GBASET_ABILITY)
			physical_layer |= IXGBE_PHYSICAL_LAYER_10GBASE_T;
		if (ext_ability & IXGBE_MDIO_PHY_1000BASET_ABILITY)
			physical_layer |= IXGBE_PHYSICAL_LAYER_1000BASE_T;
		if (ext_ability & IXGBE_MDIO_PHY_100BASETX_ABILITY)
			physical_layer |= IXGBE_PHYSICAL_LAYER_100BASE_TX;
		goto out;
	}

	switch (autoc & IXGBE_AUTOC_LMS_MASK) {
	case IXGBE_AUTOC_LMS_1G_AN:
	case IXGBE_AUTOC_LMS_1G_LINK_NO_AN:
		if (pma_pmd_1g == IXGBE_AUTOC_1G_KX_BX) {
			physical_layer = IXGBE_PHYSICAL_LAYER_1000BASE_KX |
			    IXGBE_PHYSICAL_LAYER_1000BASE_BX;
			goto out;
		} else
			/* SFI mode so read SFP module */
			goto sfp_check;
	case IXGBE_AUTOC_LMS_10G_LINK_NO_AN:
		if (pma_pmd_10g_parallel == IXGBE_AUTOC_10G_CX4)
			physical_layer = IXGBE_PHYSICAL_LAYER_10GBASE_CX4;
		else if (pma_pmd_10g_parallel == IXGBE_AUTOC_10G_KX4)
			physical_layer = IXGBE_PHYSICAL_LAYER_10GBASE_KX4;
		goto out;
	case IXGBE_AUTOC_LMS_10G_SERIAL:
		if (pma_pmd_10g_serial == IXGBE_AUTOC2_10G_KR) {
			physical_layer = IXGBE_PHYSICAL_LAYER_10GBASE_KR;
			goto out;
		} else if (pma_pmd_10g_serial == IXGBE_AUTOC2_10G_SFI)
			goto sfp_check;
		break;
	case IXGBE_AUTOC_LMS_KX4_KX_KR:
	case IXGBE_AUTOC_LMS_KX4_KX_KR_1G_AN:
		if (autoc & IXGBE_AUTOC_KX_SUPP)
			physical_layer |= IXGBE_PHYSICAL_LAYER_1000BASE_KX;
		if (autoc & IXGBE_AUTOC_KX4_SUPP)
			physical_layer |= IXGBE_PHYSICAL_LAYER_10GBASE_KX4;
		if (autoc & IXGBE_AUTOC_KR_SUPP)
			physical_layer |= IXGBE_PHYSICAL_LAYER_10GBASE_KR;
		goto out;
	default:
		goto out;
	}

sfp_check:
	/*
	 * SFP check must be done last since DA modules are sometimes used to
	 * test KR mode -  we need to id KR mode correctly before SFP module.
	 * Call identify_sfp because the pluggable module may have changed
	 */
	hw->phy.ops.identify_sfp(hw);
	if (hw->phy.sfp_type == ixgbe_sfp_type_not_present)
		goto out;

	switch (hw->phy.type) {
	case ixgbe_phy_tw_tyco:
	case ixgbe_phy_tw_unknown:
		physical_layer = IXGBE_PHYSICAL_LAYER_SFP_PLUS_CU;
		break;
	case ixgbe_phy_sfp_avago:
	case ixgbe_phy_sfp_ftl:
	case ixgbe_phy_sfp_intel:
	case ixgbe_phy_sfp_unknown:
		hw->phy.ops.read_i2c_eeprom(hw,
		    IXGBE_SFF_10GBE_COMP_CODES, &comp_codes_10g);
		if (comp_codes_10g & IXGBE_SFF_10GBASESR_CAPABLE)
			physical_layer = IXGBE_PHYSICAL_LAYER_10GBASE_SR;
		else if (comp_codes_10g & IXGBE_SFF_10GBASELR_CAPABLE)
			physical_layer = IXGBE_PHYSICAL_LAYER_10GBASE_LR;
		break;
	default:
		break;
	}

out:
	return (physical_layer);
}

/*
 * ixgbe_enable_rx_dma_82599 - Enable the Rx DMA unit on 82599
 * @hw: pointer to hardware structure
 * @regval: register value to write to RXCTRL
 *
 * Enables the Rx DMA unit for 82599
 */
s32
ixgbe_enable_rx_dma_82599(struct ixgbe_hw *hw, u32 regval)
{
#define	IXGBE_MAX_SECRX_POLL	30
	int i;
	int secrxreg;

	/*
	 * Workaround for 82599 silicon errata when enabling the Rx datapath.
	 * If traffic is incoming before we enable the Rx unit, it could hang
	 * the Rx DMA unit.  Therefore, make sure the security engine is
	 * completely disabled prior to enabling the Rx unit.
	 */
	secrxreg = IXGBE_READ_REG(hw, IXGBE_SECRXCTRL);
	secrxreg |= IXGBE_SECRXCTRL_RX_DIS;
	IXGBE_WRITE_REG(hw, IXGBE_SECRXCTRL, secrxreg);
	for (i = 0; i < IXGBE_MAX_SECRX_POLL; i++) {
		secrxreg = IXGBE_READ_REG(hw, IXGBE_SECRXSTAT);
		if (secrxreg & IXGBE_SECRXSTAT_SECRX_RDY)
			break;
		else
			/* Use interrupt-safe sleep just in case */
			usec_delay(10);
	}

	/* For informational purposes only */
	if (i >= IXGBE_MAX_SECRX_POLL)
		DEBUGOUT("Rx unit being enabled before security "
		    "path fully disabled.	Continuing with init.\n");

	IXGBE_WRITE_REG(hw, IXGBE_RXCTRL, regval);
	secrxreg = IXGBE_READ_REG(hw, IXGBE_SECRXCTRL);
	secrxreg &= ~IXGBE_SECRXCTRL_RX_DIS;
	IXGBE_WRITE_REG(hw, IXGBE_SECRXCTRL, secrxreg);
	IXGBE_WRITE_FLUSH(hw);

	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_get_device_caps_82599 - Get additional device capabilities
 * @hw: pointer to hardware structure
 * @device_caps: the EEPROM word with the extra device capabilities
 *
 * This function will read the EEPROM location for the device capabilities,
 * and return the word through device_caps.
 */
s32
ixgbe_get_device_caps_82599(struct ixgbe_hw *hw, u16 *device_caps)
{
	hw->eeprom.ops.read(hw, IXGBE_DEVICE_CAPS, device_caps);

	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_get_san_mac_addr_offset_82599 - SAN MAC address offset for 82599
 * @hw: pointer to hardware structure
 * @san_mac_offset: SAN MAC address offset
 *
 * This function will read the EEPROM location for the SAN MAC address
 * pointer, and returns the value at that location.  This is used in both
 * get and set mac_addr routines.
 */
s32
ixgbe_get_san_mac_addr_offset_82599(struct ixgbe_hw *hw, u16 *san_mac_offset)
{
	/*
	 * First read the EEPROM pointer to see if the MAC addresses are
	 * available.
	 */
	hw->eeprom.ops.read(hw, IXGBE_SAN_MAC_ADDR_PTR, san_mac_offset);

	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_get_san_mac_addr_82599 - SAN MAC address retrieval for 82599
 * @hw: pointer to hardware structure
 * @san_mac_addr: SAN MAC address
 *
 * Reads the SAN MAC address from the EEPROM, if it's available.  This is
 * per-port, so set_lan_id() must be called before reading the addresses.
 * set_lan_id() is called by identify_sfp(), but this cannot be relied
 * upon for non-SFP connections, so we must call it here.
 */
s32
ixgbe_get_san_mac_addr_82599(struct ixgbe_hw *hw, u8 *san_mac_addr)
{
	u16 san_mac_data, san_mac_offset;
	u8 i;

	/*
	 * First read the EEPROM pointer to see if the MAC addresses are
	 * available.  If they're not, no point in calling set_lan_id() here.
	 */
	(void) ixgbe_get_san_mac_addr_offset_82599(hw, &san_mac_offset);

	if ((san_mac_offset == 0) || (san_mac_offset == 0xFFFF)) {
		/*
		 * No addresses available in this EEPROM.  It's not an
		 * error though, so just wipe the local address and return.
		 */
		for (i = 0; i < 6; i++)
			san_mac_addr[i] = 0xFF;

		goto san_mac_addr_out;
	}

	/* make sure we know which port we need to program */
	hw->mac.ops.set_lan_id(hw);
	/* apply the port offset to the address offset */
	(hw->bus.func) ? (san_mac_offset += IXGBE_SAN_MAC_ADDR_PORT1_OFFSET) :
	    (san_mac_offset += IXGBE_SAN_MAC_ADDR_PORT0_OFFSET);
	for (i = 0; i < 3; i++) {
		hw->eeprom.ops.read(hw, san_mac_offset, &san_mac_data);
		san_mac_addr[i * 2] = (u8)(san_mac_data);
		san_mac_addr[i * 2 + 1] = (u8)(san_mac_data >> 8);
		san_mac_offset++;
	}

san_mac_addr_out:
	return (IXGBE_SUCCESS);
}

/*
 * ixgbe_set_san_mac_addr_82599 - Write the SAN MAC address to the EEPROM
 * @hw: pointer to hardware structure
 * @san_mac_addr: SAN MAC address
 *
 * Write a SAN MAC address to the EEPROM.
 */
s32
ixgbe_set_san_mac_addr_82599(struct ixgbe_hw *hw, u8 *san_mac_addr)
{
	s32 status = IXGBE_SUCCESS;
	u16 san_mac_data, san_mac_offset;
	u8 i;

	/* Look for SAN mac address pointer.  If not defined, return */
	(void) ixgbe_get_san_mac_addr_offset_82599(hw, &san_mac_offset);

	if ((san_mac_offset == 0) || (san_mac_offset == 0xFFFF)) {
		status = IXGBE_ERR_NO_SAN_ADDR_PTR;
		goto san_mac_addr_out;
	}

	/* Make sure we know which port we need to write */
	hw->mac.ops.set_lan_id(hw);
	/* Apply the port offset to the address offset */
	(hw->bus.func) ? (san_mac_offset += IXGBE_SAN_MAC_ADDR_PORT1_OFFSET) :
	    (san_mac_offset += IXGBE_SAN_MAC_ADDR_PORT0_OFFSET);

	for (i = 0; i < 3; i++) {
		san_mac_data = (u16)((u16)(san_mac_addr[i * 2 + 1]) << 8);
		san_mac_data |= (u16)(san_mac_addr[i * 2]);
		hw->eeprom.ops.write(hw, san_mac_offset, san_mac_data);
		san_mac_offset++;
	}

san_mac_addr_out:
	return (status);
}