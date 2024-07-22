# Hello World - Blinking Blue LED  

Traditionally `Hello World` applications demonstrate how to build and run
a program. More importantly they validate:
- the development and execution environments
- the steps required to write, modify, build, and run the user's code

Acheiving the same for an embedded system, with no existing code, where user
code must be cross-compiled and uploaded/programmed to the target system, 
is a little more complicated.

The embedded `Hello World` program demonstrates:
- The correct toolchain is being used (compiler and linker)
- The toolchain is configured correctly (e.g. the correct target system architecture selected)
- The program doesn't depend on inaccessible/unavailable dependencies (e.g. standard libraries)
- The program correctly starts up the target system and passes control to user code. For example:
  - installs a reset handler function
  - establishes a stack pointer
- The program is correctly laid out for the target address space (linker configuration)
- A compatible programmer or bootloader is used, and writing a user-generated program to the target system is successful: 
  - the physical connection to the target is sufficient (soldering might be needed)
  - target system "fuses" that may prevent programming are disabled
  - the host system software is correctly installed and configured
  (programming/uploading application, drivers)

There are many points of failure.

Additionally, the seemingly modest goal of outputting "Hello World!" to some terminal is lofty 
in this environment. A target system, with no existing code, cannot be assumed to have working communication
ports (e.g. USB). Adding communication port configuration would be another point of failure, a simpler 
feedback method will suffice.

The embedded `Hello World` program is much harder to acheive, and requires some creativity
when validating if code is running correctly*.

## Blinking Blue LED  

Code: [`c/RedBoardTurbo/blinking_blue_led/main.c`](https://github.com/vafeond/BareMetalCapers/blob/wip/c/RedBoardTurbo/blinking_blue_led/main.c)

A common approach is to toggle the state of an I/O pin and convert the output to a visual
signal using an oscilliscope or wire the pin to an LED. The RedBoard Turbo has several LEDs 
already wired up.

### Design?  
Design is thinking it through "enough" before starting. The right amount of design saves time and 
frustration. Too much time spent in design is wasteful. What's in a design? How much is enough?

A design typically consists of:
- *Requirements*, specifies the use cases and scope. 
- *Interfaces*, roughly the actions and related data used for external, or notable internal, communication. 
- *Static Structure*, the building blocks the system will be decomposed into.
- *Dynamic Behavior*, validation of the *Static Structure* by defining the runtime interactions of 
the specified building blocks. 
- *Alternatives Considered*, document design decisions that were rejected and why. Useful for design
reveiwers or future maintainers to have rationale to an obvious design alternative to be explained, meaning
time is not wasted pursuing that path.

Trying to fill all this out for a nascent project is wasteful. It's typical for some construction 
(e.g. proof of concept, spike) to be completed to properly inform the design. The workflow pattern of 
informal constructions, design, the  formal implementation and validation, may repeat many times in a
project. Even at this nascent stage the project will benefit from some design, specifically the
scope of the project.

At the time of writing the stated purpose in the root directory README is "Bare metal programming on ARM 
Cortex-M0+ using SparkFun RedBoard Turbo". It's very specific, a single microcontroller and 
development board is named. But some natural scope extensions exist:
natural scope expansions exist:
- Other ARM Cortex-M0+ development boards?
- Other microcontrollers closely related to Microchip's SAMD21 (the datasheet itself covers
multiple variants)?
- Other ARM microcontrollers?
- Other non-ARM microcontrollers?

It behooves the developer to try and future-proof the code and support such expansion. The upfront cost 
to do so is small, so not wasteful if not needed. If expansion is needed the saving in time and frustration 
could be significant.

So, the code will use a common design pattern where hardware specifics are constrained to a
Hardware Abstration Layer (HAL). The code and build system will be organized and crafted accordingly.
More detailed aspects of the design can wait. This is enough design to confidently proceed with 
coding.

### Memory Mapped I/O
The RedBoard Turbo graphical datasheet states the Blue LED is wired to SAMD21 pin PA17.

The SAMD21 datasheet provides all the information needed to determine how to configure
PA17 for output and toggle its state. Deciphering a datasheet can be intimidating, it's large,
there's a lot of acronyms and jargon, and there are seeminly endless references.

In this case we're looking for the "I/O Pin Controller" peripheral. SAMD21 calls this peripheral
PORT. The PORT specification specifies memory addresses where the CPU can write values to
configure and control I/O pins. This is memory mapped I/O. Our program can simply needs to 
correctly set these memory addresses to acheive the desired result.

The `blinking_blue_led` application (linked above) crudely determines the addresses of PORT
registers and creates some macros to interact with them. Its easy to use those macros and work
backwards to references in the SAMD21 datasheet, which I leave as an exercise for the user.

The `main` function loops endlessly toggling PA17 periodically causing the blue LED to blink.

### Bootstrap

Code: 
- [`c/hal/common/bootstrap.c`](https://github.com/vafeond/BareMetalCapers/blob/wip/c/hal/common/bootstrap.c)
- [`c/hal/samd21/vector_table.c`](https://github.com/vafeond/BareMetalCapers/blob/wip/c/hal/samd21/vector_table.c)

The `main` function represents the entry point for user code. As this system does not run
the C runtime (`crt`) all the code to initialize the system, including meeting `crt` requirements
assumed by user code, is hand written.

`bootstrap.c` performs two important functions:
1. For static and global variables that are initialized, copy the initial values from program into RAM, initializing those variables
2. For static and global variables that aren't initialized, zero their memory in RAM

The bootstrap code is common for supported CPUs, so it is defined once in `common` subdirectory.

The entry point for any code is configured via the ISR Vector Table. For every CPU interrupt an
Interrupt Service Routine can be specified, which is just a C function. When the interrupt occurs 
the configured ISR is called.

`vector_table.c` contains the SAMD21 ISR Vector Table. This application only needs to handle the
reset interrupt so only defines that ISR, though it is best practice to supply a function
for every possible ISR even if it doesn't nothing.

The SAMD21 ISR Vector Table also specifies the top of the stack, which is configured as the
highest RAM address (the stack grows downwards).

### Linker Configuration

Code: [`c/hal/samd21/linker*.ld`](https://github.com/vafeond/BareMetalCapers/tree/wip/c/hal/samd21)

The linker script maps the memory of a CPU, allowing:
- raw addresses of variables and subroutines to be correctly computed
- positions for position-specific code, like the ISR Vector Table, can be specified

For a great explaination of linker script see [mcyoung's article](https://mcyoung.xyz/2021/06/01/linker-script/). 
[Help pages are also available](https://ftp.gnu.org/old-gnu/Manuals/ld-2.9.1/html_chapter/ld_3.html#:~:text=Linker%20Scripts,impact%20on%20the%20linking%20process.).

The samd21 linker scripts support two binary positions:
1. The whole flash memory is available
2. The SparkFun RedBoard Turbo Arduino bootloader occupies the first 8kb of flash

The top-level linker script specifies the available flash and then includes the common
linker script `linker.ld`. This allows the desired configuration to be easily selected
via build scripts or the build system.

Validating a linker script can be tricky. One way is to use `objdump` tool to manually 
reconcile the sections and symbols from all objects against the generated ELF file. 
I use that tool so frequently I added support for it to the build system (see `dump`
section in [`c/build/README.md`](https://github.com/vafeond/BareMetalCapers/blob/wip/c/build/README.md)).

### Compiler Configuration

Code: [`c/build/Makefile_config.mk`](https://github.com/vafeond/BareMetalCapers/blob/wip/c/build/Makefile_Config.mk)
Code: [`c/hal/samd21/Makefile_platform.mk`](https://github.com/vafeond/BareMetalCapers/blob/wip/c/hal/samd21/Makefile_platform.mk)

`Makefile_Config.mk` contains common compiler flags.
`Makefile_platform.mk` contains architecture-specific compiler flags.


### Build System

Code: 
- [`c/build`](https://github.com/vafeond/BareMetalCapers/tree/wip/c/build)
- [`c/build/README.md`](https://github.com/vafeond/BareMetalCapers/blob/wip/c/build/README.md)
- `c/hal/<arch>/Makefile_platform.mk`

Where the C code and linker script have been deliberately minimized, the build system has
not. I have customized from another personal project and is quite flexible and extensible,
but complex compared to starter `make` scripts.

I've no intent to dive into `make` and build systems here, but there is a `README.md` for
anyone wishing to use it. For this application the relevent commands are:
- `make ARCH=samd21`, build the application for SAMD21 CPU, no bootloader
- `make ARCH=samd21 BOOTLOADER=Arduino`, same as above but for Arduino bootloader

These commands can be run from the repository root directory, or from the application
directory [`c/RedBoardTurbo/blinking_blue_led`](https://github.com/vafeond/BareMetalCapers/tree/wip/c/RedBoardTurbo/blinking_blue_led).

Upon success build artifacts are written to `c/RedBoardTurbo/blinking_blue_led/bin/debug/sam21`.

Three versions of the executable are created:
1. `redboard_turbo_blinking_blue_led.elf`
2. `redboard_turbo_blinking_blue_led.hex`, use when programming with Seggar programmer (Microchip Studio)
3. `redboard_turbo_blinking_blue_led.bin`, use when uploading with Sparkfun BOSSA GUI (with Arduino bootloader)

## Wrap Up
Once running the RedBoard Turbo blue LED should be blinking. Alter [`TOGGLE_DELAY_CNT`](https://github.com/vafeond/BareMetalCapers/blob/wip/c/RedBoardTurbo/blinking_blue_led/main.c) 
to change the blinking frequency and ensure the code can be modified and programmed/uploaded 
to the target.

Once the development setup is validated, arguably the largest hurdle to bare metal embedded
projects is overcome.

---

[*] Arduino does a great job of simplifying this and making microcontroller systems accessible by
abstracting the above details away. Those who want to go beyond Arduino's environment and use 
microcontrollers and development boards without Arduino support will benefit from understanding
all the components needed to develop on embedded systems.
