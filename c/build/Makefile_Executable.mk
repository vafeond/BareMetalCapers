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

ifeq (,$(BUILD_ROOT))
$(error Must be included in Makefile heirarchy where an ancestor Makefile has detected the build system root relative path and sets it to BUILD_ROOT)
endif

include $(BUILD_ROOT)Makefile_Config.mk
include $(BUILD_ROOT)Makefile_GatherSources.mk
include $(BUILD_ROOT)Makefile_DepsHead.mk

ifeq (,$(TARGET))
$(error Invoking Makefile must set TARGET)
endif
TARGET_FULL_PATH:=$(BIN_CONFIG_PATH)/$(TARGET)

$(TARGET_FULL_PATH): hal $(OBJECTS_FULL_PATH) $(EXTRA_OBJECTS_FULL_PATH)
	$(info )
	$(info [ $(EXECUTABLE_VARIANT) Executable ] =============================================)
	$(info TARGET    : $(TARGET))
	$(info CONFIG    : $(CONFIG))
	$(info ARCH      : $(ARCH))
	$(info LINKER    : $(LINKER))
	$(info SOURCES   : $(SOURCES))
	$(info SYSLIBS   : $(SYSLIBS))
	$(info BOOTLOADER: $(BOOTLOADER))
	$(info )
	$(LINK_COMMAND_EXE) 
	$(LINK_COMMAND_EXE_CMDARG)

$(BIN_CONFIG_PATH)/obj/$(BIN_CONFIG_PAD)%.o : %.$(EXT)
	mkdir -p $(@D)
	mkdir -p $(dir $(DEPDIR)/$*.d)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(DEPFLAGS) $(addprefix -I,$(INCLUDE_PATHS)) $(addprefix -I,$(EXTRA_SOURCE_INCLUDE_PATHS_RESOLVED)) -c -o $@ $<

hal:
	make -C $(BUILD_ROOT)../hal/mcu/$(ARCH) $(CONFIG)

# must be last
include $(BUILD_ROOT)Makefile_DepsTail.mk
