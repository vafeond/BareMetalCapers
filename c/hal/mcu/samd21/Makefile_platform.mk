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

# platform-specific build options
# should be included from Makefile_Config.mk
# do not include or invoke directly

ifeq (,$(BUILD_ROOT))
$(error Must be included in Makefile heirarchy where an ancestor Makefile has detected the build system root relative path and sets it to BUILD_ROOT)
endif

ifeq (,$(MAKEFILE_CONFIG_INCLUDED))
$(error Don't include directly. Should be included from Makefile_Config.mk)
endif

#==============================================================================
# override toolchain
#
ifeq (,$(ARM_TOOLCHAIN_PATH))
# default toolchain path is that expected in the Docker container
ARM_TOOLCHAIN_PATH=/home/toolchains/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin
endif
CC=$(ARM_TOOLCHAIN_PATH)/arm-none-eabi-gcc
LINKER=$(ARM_TOOLCHAIN_PATH)/arm-none-eabi-ld
ARCHIVER=$(ARM_TOOLCHAIN_PATH)/arm-none-eabi-ar
OBJCOPY=$(ARM_TOOLCHAIN_PATH)/arm-none-eabi-objcopy
OBJDUMP=$(ARM_TOOLCHAIN_PATH)/arm-none-eabi-objdump

# compute the path to this platform's source files
REAL_PATH = $(realpath $(BUILD_ROOT)../hal/mcu/$(ARCH))

#==============================================================================
# extend compiler flags
#
CFLAGS_EXTRA += -mcpu=cortex-m0plus 
CFLAGS_EXTRA += -mthumb -c 
CFLAGS_EXTRA += -nostdlib 
CFLAGS_EXTRA += --param max-inline-insns-single=500
CFLAGS_EXTRA += -I$(REAL_PATH)/include
ifeq (release,$(CONFIG))
# no-tree-loop-distribute-patters prevents compiler from replacing
# hand-written code with standard calls, like memcpy and memset
CFLAGS_EXTRA += -Os -fno-tree-loop-distribute-patterns
endif

#==============================================================================
# extend linker flags 
#

# linker script
# nested linker script added via INCLUDE statement, must add its path via -L
# -L path must appear before -T for it to have an impact
LFLAGS_EXTRA += -L $(REAL_PATH)
ifeq (,$(BOOTLOADER))
LFLAGS_EXTRA += -T $(REAL_PATH)/linker_bootloader_none.ld
else
ifeq (Arduino,$(BOOTLOADER))
LFLAGS_EXTRA += -T $(REAL_PATH)/linker_bootloader_arduino.ld 
else
$(error Unknown BOOTLOADER: $(BOOTLOADER))
endif
endif

# link in this architecture's HAL binary
# use "--whole-archive" so important symbols, like ISR vector table aren't 
# optimized away
LFLAGS_HAL += -L $(REAL_PATH)/$(BIN_CONFIG_PATH) --whole-archive -l hal

#==============================================================================
# override linker command for executable
#

# create the ELF 
LINK_COMMAND_EXE = $(LINKER) -o $@.elf $(OBJECTS_FULL_PATH) $(EXTRA_OBJECTS_FULL_PATH) $(addprefix -l,$(SYSLIBS)) $(LFLAGS) $(LFLAGS_HAL);
# generate bin and hex formats as needed by different programmers
LINK_COMMAND_EXE += $(OBJCOPY) -O binary $@.elf $@.bin;
LINK_COMMAND_EXE += $(OBJCOPY) -O ihex $@.elf $@.hex; 

