#
# CDDL HEADER START
#
# The contents of this file are subject to the terms of the
# Common Development and Distribution License (the "License").
# You may not use this file except in compliance with the License.
#
# You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
# or http://www.opensolaris.org/os/licensing.
# See the License for the specific language governing permissions
# and limitations under the License.
#
# When distributing Covered Code, include this CDDL HEADER in each
# file and include the License file at usr/src/OPENSOLARIS.LICENSE.
# If applicable, add the following below this CDDL HEADER, with the
# fields enclosed by brackets "[]" replaced with your own identifying
# information: Portions Copyright [yyyy] [name of copyright owner]
#
# CDDL HEADER END
#

#
# Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#

#
#	This makefile drives the production of the sun4u UltraSPARC-IIe
#	driver module.
#
#	sun4u implementation architecture dependent
#

#
#	Path to the base of the uts directory tree (usually /usr/src/uts).
#
UTSBASE	= ../..

#
#	Define the module and object file sets.
#
MODULE		= SUNW,UltraSPARC-IIe
OBJECTS		= $(HUMMINGBIRD_OBJS:%=$(OBJS_DIR)/%)
LINTS		= $(HUMMINGBIRD_OBJS:%.o=$(LINTS_DIR)/%.ln)
ROOTMODULE	= $(ROOT_PSM_CPU_DIR)/$(MODULE)
ROOTSOFTLINKS	= $(SOFTLINKS:%=$(ROOT_PSM_CPU_DIR)/%)

CPU_DIR		= .
HERE		= ../hummingbird

#
#	Include common rules.
#
include $(UTSBASE)/sun4u/Makefile.sun4u

#
#	Override defaults
#
CLEANFILES	+= $(CPULIB) $(SYM_MOD)

#
#	Define targets
#
ALL_TARGET	= $(SYM_MOD)
LINT_TARGET	= $(MODULE).lint
INSTALL_TARGET	= def $(BINARY) $(ROOTMODULE) $(ROOTSOFTLINKS)

#
# lint pass one enforcement
#
CFLAGS += $(CCVERBOSE)

#
# cpu-module-specific flags
#
CPPFLAGS += -DCPU_MODULE -DHUMMINGBIRD
AS_CPPFLAGS += -DCPU_MODULE -DHUMMINGBIRD

#
#	Default build targets.
#
.KEEP_STATE:

def:		$(DEF_DEPS)

all:		$(ALL_DEPS)

clean:		$(CLEAN_DEPS)

clobber:	$(CLOBBER_DEPS)

lint:		$(LINT_DEPS)

modlintlib:	$(MODLINTLIB_DEPS)

clean.lint:	$(CLEAN_LINT_DEPS)

install:	$(INSTALL_DEPS)

$(CPULIB):	$(BINARY)
	$(BUILD.SO) $(BINARY)

$(SYM_MOD):	$(UNIX_O) $(CPULIB)
	@echo "resolving symbols against unix.o"
	@(cd $(UNIX_DIR); pwd; \
	    CPU_DIR=$(HERE) SYM_MOD=$(HERE)/$(SYM_MOD) $(MAKE) symcheck)

$(ROOTSOFTLINKS): $(ROOTMODULE)
	$(RM) $@; $(SYMLINK) $(MODULE) $@

#	Include common targets.
#
include $(UTSBASE)/sun4u/Makefile.targ

#
# For now, disable these lint checks; maintainers should endeavor
# to investigate and remove these for maximum lint coverage.
# Please do not carry these forward to new Makefiles.
#
LINTTAGS	+= -erroff=E_PTRDIFF_OVERFLOW
LINTTAGS	+= -erroff=E_ASSIGN_NARROW_CONV
LINTTAGS	+= -erroff=E_BAD_FORMAT_STR2

CERRWARN	+= -_gcc=-Wno-uninitialized
