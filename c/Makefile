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

# top-level Makefile, supporting:
# - environment-level targets
# - building and testing all sub-projects
# - project-level targets

# boilerplate for build support
BUILD_ROOT:=$(dir $(wildcard $(addsuffix /build/Makefile_Config.mk, . .. ../.. ../../..)))
ifeq (,$(BUILD_ROOT))
$(error Build system BUILD_ROOT could not be detected! Where is Makefile_Config.mk? )
endif
include $(BUILD_ROOT)Makefile_Config.mk

$(info MAKEFILE_PATHS_FROM_ROOT=$(MAKEFILE_PATHS_FROM_ROOT))

.PHONY: debug release clean dump help


#
# build all but those excluded (opt-out)
#
# BUILD_EXCLUDES is a comma-separated list of relative Makefile paths
# that should not be invoked by the build targets
BUILD_EXCLUDES=
BUILD_DEBUG_EXCLUDES=
debug:
	$(info Building all...)
	echo -n $(subst $(subst ,, ),:,$(strip $(filter-out $(BUILD_EXCLUDES) $(BUILD_DEBUG_EXCLUDES),$(MAKEFILE_PATHS_FROM_ROOT)))) | xargs --delimiter=: -IARG make -C ARG debug

release:
	$(info Building all...)
	echo -n $(subst $(subst ,, ),:,$(strip $(filter-out $(BUILD_EXCLUDES),$(MAKEFILE_PATHS_FROM_ROOT)))) | xargs --delimiter=: -IARG make -C ARG release

#
# dump object symbol information
# set OBJ to path of object to dump
# e.g. make dump OBJ=<path to object>
dump:
	$(OBJDUMP) -h $(OBJ)
	$(OBJDUMP) -t $(OBJ)

#
# clean all
#
clean:
	$(info Cleaning all...)
	echo -n $(subst $(subst ,, ),:,$(strip $(MAKEFILE_PATHS_FROM_ROOT))) | xargs --delimiter=: -IARG make -C ARG clean

#
# display README
#
help:
	less $(BUILD_ROOT)/README.md


# configure and include development container targets
STACK_PATH=$(CURDIR)
IMAGE_NAME=baremetalcapers-c
CONTAINER_NAME=baremetalcapers-c-container
include build/Makefile_DevContainer.mk

