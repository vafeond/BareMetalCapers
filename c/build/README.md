# Build System
Build system infrastructure is stored in `<repo root>/build`

**Examples**:
- `make` debug binaries for host platform
- `make BOARD=rbt` creates debug embedded application for *RedBoard Turbo*
- `make BOOTLOADER=Arduino` creates debug application allowing room for Arduino 
bootloader
- `make BOARD=rbt BOOTLOADER=Arduino` combination of above

## Targets  
Below are the make targets supported for each supported binary type. The 
commands can be issued after navigating to the directory containing the binary 
Makefile. 

### Static Library  
- `make` and `make debug`: Build the debug version of the library
- `make release`: Build the release version of the library
- `make clean`: Removes all build artifacts

### Application  
- `make`, `make run` and `make run_debug`: Build a debug version of the test 
application and run it where possible (not cross-compiled)
- `make run_release`: Build a release version of the test application and run it
- `make debug`: Build a debug version of the application
- `make release`: Build a release version of the application
- `make clean`: Removes all build artifacts

### Object Dump
- `make dump OBJ=<path to object>`: Convenience target to show object symbol 
information.

## Input Variables  
- `BOARD=<board>`, builds for specified board, setting the appropriate `ARCH` and
cross-compiling if neccesary
    - Default: no board is set, typically this builds for host platform
    - `rbt` or `RedBoardTurbo`, build embedded applications for *Sparkfun RedBoardTurbo*
- `ARCH=<platform>`, builds for specified architecture, cross-compiling if 
neccesary
    - Default: use default toolchain, typically this builds for host platform
    - `samd21`, build embedded applications for SAMD21 MCUs
    - Note: Favor using `BOARD` instead of `ARCH`
- `BOOTLOADER=<bootloader name>`, build embedded target allowing for the 
selected bootloader
    - Default: no bootloader accounted for
    - `Arduino`, account for Arduino bootloader
- `LINK_COMMAND_EXE_CMDARG`, command called after platform-specific linker
    - Default: <nothing>
    - I use to copy created hex binary to NAS for easy download and program on 
    Windows machine, similar to  
    `LINK_COMMAND_EXE_CMDARG="cp \$@.hex <NAS path>"`  (Note escaping of `$`)  

## Adding a binary  
- Follow an existing example for the type of binary you want to add, cloning and
owning the directory structure and Makefiles
- Top-level Makefiles have a specific structure:
  - Set variables required to describe you binary (see below)
  - Copy code to detect the `BUILD_ROOT` (path to the build system Makefiles 
  from you Makefile location)
  - Include the appropriate next-level Makefile:
    - `include $(BUILD_ROOT)Makefile_Application.mk`, if creating an application
    - `include $(BUILD_ROOT)Makefile_LibraryStatic.mk`, if creating a static 
    library

## Variables  
The following variables can be used when configuring a top-level build file for
a library or executable.

* `TARGET`: The name of the target to be built.
  * For executables this is the full name of the created executable
    * If not set when using `Makefile_Application.mk` `TARGET` is set to `app`
  * For libraries this is the library "name spec" and the appropriate prefix and
  suffix are added
    * For example, the static library `TARGET=hal` becomes `libhal.a`

* `SOURCE_PATHS`: Space-separated list of relative paths where source files to 
be included into the binary are found

* `INCLUDE_PATHS`: Space-separated list of relative paths where header files 
needed to build the binary are found

* `EXTRA_SOURCE_PATHS`: Paths relative to `extra_sources` for additional source
files to include

* `EXTRA_SOURCE_INCLUDE_PATHS`: Paths relative to `extra_sources` for additional
source header files

* `CFLAGS_EXTRA`: Arguments passed to the compiler

* `CPPFLAGS_EXTRA`: Preprocessor flags specific to the binary that must be 
passed to the compiler

* `LFLAGS_EXTRA`: Arguments passed to the linker

## Docker Container Management  
Convenience commands for Docker image and container management. 
From `<repo root>/c` directory (`sudo` may be needed for some Linux 
distributions):
- `make cbuild`, build the Docker image
- `make cbuildnocache`, build the Docker image from scratch (use if weird error
rebuilding existing image)
- `make ccreate`, create and start the Docker container
- `make cstart`, start Docker container if not running
- `make cexec`, enter BASH shell prompt inside the running Docker container 
- `make cclean`, delete Docker container and image

See `<root>/c/build/Makefile_DevContainer.mk`

