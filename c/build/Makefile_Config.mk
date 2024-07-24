#  This is free and unencumbered software released into the public domain.
#
#  Anyone is free to copy, modify, publish, use, compile, sell, or
#  distribute this software, either in source code form or as a compiled
#  binary, for any purpose, commercial or non-commercial, and by any
#  means.
#
#  In jurisdictions that recognize copyright laws, the author or authors
#  of this software dedicate any and all copyright interest in the
#  software to the public domain. We make this dedication for the benefit
#  of the public at large and to the detriment of our heirs and
#  successors. We intend this dedication to be an overt act of
#  relinquishment in perpetuity of all present and future rights to this
#  software under copyright law.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
#  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
#  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
#  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
#  OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
#  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
#  OTHER DEALINGS IN THE SOFTWARE.
#
#  For more information, please refer to <https://unlicense.org>

ifneq (true,$(MAKEFILE_CONFIG_INCLUDED))
MAKEFILE_CONFIG_INCLUDED=true

# ensure we're only building for debug or release
ifneq (,$(filter debug run_debug,$(MAKECMDGOALS)))
ifneq (,$(filter release run_release,$(MAKECMDGOALS)))
$(error Can only build for debug or release, not both!)
endif
endif

ifeq (,$(filter release run_release test_release bench,$(MAKECMDGOALS)))
# if not release do debug
CONFIG=debug
else
# otherwise do release
CONFIG=release
endif

# build tool configuration - default assumes below tools in PATH
CC=gcc
LINKER=ld
ARCHIVER=ar
OBJCOPY=objcopy
OBJDUMP=objdump

# general configuration
CFLAGS_COMMON =-W -Wall -Wpedantic
CFLAGS_COMMON += -std=gnu2x
CFLAGS_COMMON += -funsigned-char -funsigned-bitfields
# CFLAGS_COMMON += -fdata-sections -ffunction-sections
CFLAGS_EXTRA=
LFLAGS_COMMON=$(LFLAGS_SANITIZE)
# LFLAGS_COMMON += --gc-sections
LFLAGS_EXTRA=

# default linker command - assume host executable
LINK_COMMAND_EXE = $(LINKER) -o $@ $(OBJECTS_FULL_PATH) $(EXTRA_OBJECTS_FULL_PATH) $(addprefix -l,$(SYSLIBS)) $(LFLAGS)


# resolve architecture
ifeq (,$(ARCH))
# no ARCH provided, go with defaults
else 
ifneq (,$(wildcard $(BUILD_ROOT)/../hal/mcu/$(ARCH)/Makefile_platform.mk))
include $(realpath $(BUILD_ROOT)/../hal/mcu/$(ARCH)/Makefile_platform.mk)
else
$(error Unknown ARCH: $(ARCH))
endif
endif

# configuration-specific flags
ifeq (debug,$(CONFIG))
CFLAGS=$(CFLAGS_COMMON) -g $(CFLAGS_EXTRA)
CPPFLAGS=-DDEBUG $(CPPFLAGS_COMMON) $(CPPFLAGS_EXTRA)
LFLAGS=$(LFLAGS_COMMON) $(LFLAGS_EXTRA)
else
CFLAGS=$(CFLAGS_COMMON) $(CFLAGS_EXTRA)
CPPFLAGS=-DNDEBUG $(CPPFLAGS_COMMON) $(CPPFLAGS_EXTRA)
LFLAGS=$(LFLAGS_COMMON) $(LFLAGS_EXTRA)
endif

BIN_PATH:=bin
BIN_CONFIG_PATH:=$(BIN_PATH)/$(CONFIG)/$(ARCH)
# binary config padding is needed to support extra_sources which has
# a relative path that contains ../, so need to add some depth to
# binary directory structure so the created artifacts (e.g. objects and deps) 
# are properly contained withing the bin/$(CONFIG) directory
# this whole thing is due to how make pattern rules work
# below dervies the needed padding based on the deepest Makefile
MAKEFILE_PATHS_RAW!=find $(BUILD_ROOT).. -name Makefile 
# $(info MAKEFILE_PATHS_RAW=$(MAKEFILE_PATHS_RAW))
MAKEFILE_PATHS_FROM_ROOT:=$(sort $(strip $(subst Makefile,,$(subst /Makefile,,$(subst $(BUILD_ROOT)../,,$(MAKEFILE_PATHS_RAW))))))
# $(info MAKEFILE_PATHS_FROM_ROOT=$(MAKEFILE_PATHS_FROM_ROOT))
MAKEFILE_PATH_LONGEST:=$(lastword $(MAKEFILE_PATHS_FROM_ROOT))
# $(info MAKEFILE_PATH_LONGEST=$(MAKEFILE_PATH_LONGEST))
DERIVED_PADDING=$(subst $(subst ,, ),,$(patsubst %,p/,$(subst /, ,$(MAKEFILE_PATH_LONGEST))))
# $(info DERIVED_PADDING=$(DERIVED_PADDING))
BIN_CONFIG_PAD:=$(DERIVED_PADDING)
# if above starts failing can always hardcode as below, and extend number of p's as needed
# BIN_CONFIG_PAD=p/p/p/
# $(info BIN_CONFIG_PAD=$(BIN_CONFIG_PAD))

endif # MAKEFILE_CONFIG_INCLUDED
