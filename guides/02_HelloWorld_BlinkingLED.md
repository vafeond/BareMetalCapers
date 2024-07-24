# Hello World - Blinking Blue LED  

"Hello World!" applications demonstrate how to build and run
a program, validating:
- The development and execution environments
- The steps required to write, modify, build, and run user code

Acheiving the same for an embedded system, with no existing code, where user
code must be cross-compiled and uploaded/programmed to the target system, 
is a little more complicated.

The embedded "Hello World!" program demonstrates:
- The correct toolchain is being used (compiler and linker)
- The toolchain is configured correctly (e.g. the correct target system architecture selected)
- The program doesn't depend on inaccessible/unavailable software (e.g. standard libraries)
- The program correctly starts up the target system and passes control to user code. For example:
  - Installs a reset handler function
  - Establishes a stack pointer
- The program is correctly laid out for the target address space (linker configuration)
- A compatible programmer or boot loader is used, and programming/uploading a user-generated binary to the target system is successful: 
  - The physical connection to the target is sufficient (soldering might be needed)
  - Target system "fuses" that may prevent programming are disabled
  - The host system software is correctly installed and configured (e.g. programming/uploading application, drivers)

There are many points of failure.

Additionally, the seemingly modest goal of outputting "Hello World!" to some terminal is lofty 
in this environment. A target system, with no existing code, cannot be assumed to have working communication
ports (e.g. USB). Adding communication port configuration would be another point of failure and 
uneccessary when a simpler feedback method will suffice.

## Blinking Blue LED  
A common approach is to toggle an I/O pin and convert the output to a visual
signal using an oscilliscope or wire the pin to an LED. The *RedBoard Turbo* has several LEDs 
already wired up, including a blue LED connected to I/O pin `PA17` (see the *RedBoard Turbo*
graphical datasheet).

## Design?  
Design is thinking it through enough before starting. The right amount of design saves time and 
frustration. Too much design is wasteful. What's in a design? How much is enough?

A design typically consists of:
- *Requirements*, specifies the use cases and scope. 
- *Interfaces*, describes the actions and related data used for external and notable internal communication. 
- *Static Structure*, the building blocks the system will be decomposed into.
- *Dynamic Behavior*, validation of the *Static Structure* by defining the runtime interactions of 
the specified building blocks (show how they are composed to meet *Requirements*) 
- *Alternatives Considered*, document design decisions that were rejected and why. It is useful for reveiwers 
and future maintainers to have the rationale for why alternatives were rejected, preventing wasted effort
pursuing that path again.

Defining all this out for a nascent project is wasteful. It's typical for some construction 
(e.g. proof of concept, spike) to be completed to properly inform design. The workflow pattern of 
informal construction, design, formal implementation, and validation, may repeat many times in a
project. Even at this stage this project can benefit from some design, specifically the
scope of the project.

At the time of writing the stated purpose in the root README is "Bare metal programming on ARM 
Cortex-M0+ using SparkFun RedBoard Turbo". It's very specific. A single microcontroller and 
development board is named. Although some natural scope expansion exists:
- Other ARM Cortex-M0+ development boards?
- Other microcontrollers closely related to Microchip's SAMD21 (the datasheet itself covers
multiple variants)?
- Other ARM microcontrollers?
- Other non-ARM microcontrollers?

It behooves the developer to try and future-proof the code and support such expansion. The upfront cost 
to do so is small so it is not wasteful if not needed. But, if scope expansion does happen after a lot
of code is written the saving in time and frustration could be significant.

So, the project will use a common design pattern where hardware specifics are constrained to a
Hardware Abstration Layer (HAL). The code and build system will be organized accordingly.
Specifically:
- Use a HAL-specific directory for each MCU, organized under `hal/mcu`
  - Code common to all MCUs residing under `hal/common`
- Use a HAL-specific directory for each development board, organized under `hal/board`

More detailed aspects of the design can wait. This is enough design to confidently proceed with 
coding a "Hello World!" application.

## Memory-mapped I/O
The SAMD21 datasheet provides all the information needed to determine how to configure
`PA17` for output and toggle its state. Deciphering a datasheet can be intimidating. It's large,
there's a lot of acronyms and jargon, and there are seeminly endless references.

SAMD21's I/O pin control peripheral is called PORT. It specifies memory addresses where the CPU 
can write values to configure and control I/O pins. This is an example of memory-mapped I/O, though 
memory-mapped I/O is not constrained to I/O pins. This "Hello World!" application needs to use 
the I/O pin memory-mapped registers to configure `PA17` for output and toggle its state 
periodically, making the *RedBoard Turbo* blue LED blink.

ARMs PORT peripheral provides I/O pin convenience registers simplifying the application software.
Specifically, they only require setting of a single bit (bit 17 for `PA17`) to set, clear, or 
toggle the corresponding pin state. This eliminates the need for application software to create 
bitmasks for setting and clearing operations, and reading or storing of the current pin state
in order to set the alternate state (toggle).

The `blinking_blue_led` applications (linked below for each language) crudely determine the 
addresses of PORT memory-mapped registers and interacts with them. No attempt is made to craft 
idiomatic code. To understand where the application's magic numbers come from work backwards from 
the source file's code and comments to locate the appropriate sections in the SAMD21 datasheet.

See:
- [`c/hal/board/RedBoardTurbo/blinking_blue_led/main.c`](https://github.com/vafeond/BareMetalCapers/blob/main/c/hal/board/RedBoardTurbo/blinking_blue_led/main.c)
- [`rust/hal/board/redboardturbo/src/bin/blinking_blue_led.rs`](https://github.com/vafeond/BareMetalCapers/blob/main/rust/hal/board/redboardturbo/src/bin/blinking_blue_led.rs)

## Bootstrap
The application code linked above contains the `main` function for each language, which is the 
entry point for user code. Before user code runs the system must be set up, which means
meeting configuration and initialization requirements of the MCU and `c` runtime.

### MCU Initialization
Typically an MCU requires a special address location to be initialized to the address of the
user-defined initialization code. For SAMD21 MCUs this entry point is part of an Interrupt Service 
Routine (ISR) Vector Table which holds the addresses of user-defined code for all SAMD21 
interrupts. 

The user-defined code can be `c` or `rust` functions with special annotations in the source 
code and positioning via the linker script (covered below).

The "Hello World!" application only needs to respond to the reset interrupt, so only needs to
initialize the reset vector in the ISR Vector Table.

Additionally, the main stack pointer needs to be initialized, which is configured as the
highest RAM address (the stack grows downwards), which is also part of the ISR Vector Table.

SAMD21 ISR Vector Table (abridged):
- `0x00000000`: Main stack pointer (set to address in SRAM)
- `0x00000004`: Reset handler pointer (set to address of `c` or `rust` function to run on system power on or reset)

The code linked below contains the reset handler routines. How these, and the main stack
pointer, are mapped to the ISR Vector Table addresses is covered in *Linker Configuration* 
below.

- [`c/hal/mcu/samd21/vector_table.c`](https://github.com/vafeond/BareMetalCapers/blob/main/c/hal/mcu/samd21/vector_table.c)
- [`rust/hal/mcu/samd21/src/lib.c`](https://github.com/vafeond/BareMetalCapers/blob/main/rust/hal/mcu/samd21/src/lib.rs)

### c Initialization
Usually the `crt` (`c` `r`un`t`ime) performs the initialization required of `c`, but this
system does not include a `crt`, meaning this initialization must be hand-written.

The boostrap module (`c`), or crate (`Rust`), performs the following to meet `c`'s initialization requirement:
1. For static and global variables that are initialized, copy the initial values from Flash into RAM, initializing those variables
2. For static and global variables that aren't initialized, zero their memory in RAM

See  
- [`c/hal/mcu/common/bootstrap.c`](https://github.com/vafeond/BareMetalCapers/blob/main/c/hal/mcu/common/bootstrap.c)
- [`rust/hal/mcu/common/src/lib.rs`](https://github.com/vafeond/BareMetalCapers/blob/main/rust/hal/mcu/common/src/lib.rs)

The initialization routines need Flash and RAM addresses to perform these copy and clear
operations. These are provided by the MCU-specific linker scripts. If the linker script
for every MCU supported by the project provides the same outputs, then the same copy and 
clear code can be used for all supported MCUs; therefore the copy and clear bootstrap 
functions are defined once (for each language) in the `hal/common` subdirectory.

## Linker Configuration
The linker script maps the memory of an MCU, allowing:
- Raw addresses of variables and functions to be correctly computed
- Memory addresses of position-specific code, such as the ISR Vector Table, to be specified

For a great explaination of linker script see [mcyoung's article](https://mcyoung.xyz/2021/06/01/linker-script/). 
[Help pages are also available](https://ftp.gnu.org/old-gnu/Manuals/ld-2.9.1/html_chapter/ld_3.html#:~:text=Linker%20Scripts,impact%20on%20the%20linking%20process.).

The SAMD21 linker scripts provided support two application mappings:
1. The application can use all of the flash memory
2. The *SparkFun RedBoard Turbo boot loader* occupies the first 8kb of flash, meaning the application
is positioned after the boot loader

The top-level linker script specifies the available flash and then includes the common
linker script `linker.ld`. This allows the desired configuration to be easily selected
via build scripts or the build system.

See  
- [`c/hal/mcu/samd21/linker*.ld`](https://github.com/vafeond/BareMetalCapers/tree/main/c/hal/mcu/samd21)
- [`rust/hal/mcu/samd21/linker*.ld`](https://github.com/vafeond/BareMetalCapers/tree/main/rust/hal/mcu/samd21)

The linker script handles the main stack pointer entirely, computing its address and initializing
the ISR Vector Table.

Validating a linker script can be tricky. One way is to use `objdump` tool to manually 
reconcile the sections and symbols from all objects against the generated `ELF` file. 
- `c`: I use that tool frequently enough to warrant adding support in the build system (see `dump`
section in [`c/build/README.md`](https://github.com/vafeond/BareMetalCapers/blob/main/c/build/README.md)).
- `Rust`'s Cargo has an extension to invoke `objdump`. 
  - The project's [Docker](https://github.com/vafeond/BareMetalCapers/blob/main/rust/Dockerfile) 
  container includes all neccessary dependencies. An example invocation is `cargo objdump --bin blinking_blue_led -- -h -t`. 
  - The project's custom Cargo extension [`cargo-bbuild`](https://github.com/vafeond/BareMetalCapers/blob/main/rust/cargo-bbuild) 
  contains a commented call to `objdump` which can be uncommented if convenient (see 
  [`rust/README.md`](https://github.com/vafeond/BareMetalCapers/blob/main/rust/README.md) 
  for more information).

## c Build System

See 
- [`c/build`](https://github.com/vafeond/BareMetalCapers/tree/main/c/build)
- [`c/build/README.md`](https://github.com/vafeond/BareMetalCapers/blob/main/c/build/README.md)
- `c/hal/<arch>/Makefile_platform.mk`

Where the application code and linker scripts have been deliberately minimized, the build 
system has not. It's customized from another personal project and is quite flexible 
and extensible, but complex compared to starter `make` scripts.

I've no intent to dive into `make` and build systems here, but there is a [`README.md`](https://github.com/vafeond/BareMetalCapers/blob/main/c/build/README.md) 
for anyone wishing to use it. For this application the relevent commands are:
- `make ARCH=samd21`, build the application for SAMD21 CPU, no boot loader
- `make ARCH=samd21 BOOTLOADER=Arduino`, same as above but for Arduino boot loader

These commands can be run from the repository root directory, or from the application
directory [`c/RedBoardTurbo/blinking_blue_led`](https://github.com/vafeond/BareMetalCapers/tree/wip/c/RedBoardTurbo/blinking_blue_led).

Upon success build artifacts are written to `c/hasl/board/RedBoardTurbo/blinking_blue_led/bin/debug/sam21`.

Three versions of the executable are created:
1. `c_blinking_blue_led.elf`
2. `c_blinking_blue_led.hex`, use when programming with Seggar programmer (Microchip Studio)
3. `c_blinking_blue_led.bin`, use when uploading with Sparkfun BOSSA GUI (with Arduino boot loader)

### Compiler Configuration

`Makefile_Config.mk` contains common compiler flags.  
`c/hal/mcu/<mcu name>/Makefile_platform.mk` contains architecture-specific compiler flags that overwrite common compiler flags.

See
- [`c/build/Makefile_config.mk`](https://github.com/vafeond/BareMetalCapers/blob/main/c/build/Makefile_Config.mk)
- [`c/hal/samd21/Makefile_platform.mk`](https://github.com/vafeond/BareMetalCapers/blob/main/c/hal/mcu/samd21/Makefile_platform.mk)


## Wrap Up
Once the application is running the *RedBoard Turbo* blue LED should be blinking. 
Alter `TOGGLE_DELAY_CNT` in the `main` function to change the frequency ensuring the code can be 
modified and programmed/uploaded to the target.

Once the development setup is validated, arguably the largest hurdle to bare metal embedded
projects is overcome.

