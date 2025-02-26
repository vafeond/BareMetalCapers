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

ifeq (,$(MAKEFILE_CONFIG_INCLUDED))
$(error Must be included after Makefile_Config.mk)
endif

ifeq (,$(SOURCE_PATHS))
SOURCE_PATHS=. $(SOURCE_PATH_HAL)
endif

EXT=c

# gather sources
SOURCES:=$(wildcard $(addsuffix /*.$(EXT), $(SOURCE_PATHS)))
# $(info SOURCES=$(SOURCES))
OBJECTS:=$(patsubst %.$(EXT),%.o,$(SOURCES))
# $(info OBJECTS=$(OBJECTS))
OBJECTS_FULL_PATH:=$(addprefix $(BIN_CONFIG_PATH)/obj/$(BIN_CONFIG_PAD),$(OBJECTS))
# $(info OBJECTS_FULL_PATH=$(OBJECTS_FULL_PATH))

# gather extra sources
# - extra has special handling just to clean up build/../ that gets introduced to the object path
EXTRA_SOURCES=$(subst build/../,,$(wildcard $(addsuffix /*.$(EXT), $(addprefix $(BUILD_ROOT)../extra_sources/,$(EXTRA_SOURCE_PATHS)))))
# $(info EXTRA_SOURCES=$(EXTRA_SOURCES))
EXTRA_OBJECTS:=$(patsubst %.$(EXT),%.o,$(EXTRA_SOURCES))
# $(info EXTRA_OBJECTS=$(EXTRA_OBJECTS))
EXTRA_OBJECTS_FULL_PATH:=$(addprefix $(BIN_CONFIG_PATH)/obj/$(BIN_CONFIG_PAD),$(EXTRA_OBJECTS))
# $(info EXTRA_OBJECTS_FULL_PATH=$(EXTRA_OBJECTS_FULL_PATH))
EXTRA_SOURCE_INCLUDE_PATHS_RESOLVED=$(addprefix $(BUILD_ROOT)../extra_sources/,$(EXTRA_SOURCE_INCLUDE_PATHS))
# $(info EXTRA_SOURCE_INCLUDE_PATHS_RESOLVED=$(EXTRA_SOURCE_INCLUDE_PATHS_RESOLVED))

