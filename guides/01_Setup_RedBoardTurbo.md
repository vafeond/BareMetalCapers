# Setup - RedBoard Turbo  

## Development Environment
It's likely there are many ways to setup a compatible development environment
and there should be no need to repurchase gear you already own; therefore I'm 
listing the environment requirement and how my setup satisfies it, plus 
possible alternatives if I know of any.

* *[Requirement]* ARM MCU Programmer
  - My setup: *Seggar EDU Mini*. Requires Windows to run *Microchip Studio* as 
  Seggar Linux-based software complains about a license. 
  [SparkFun ARM programming guide](https://learn.sparkfun.com/tutorials/arm-programming) 
  uses this setup so good instructions and support are available.
  - Alternatives:
    - Any ARM programmer (SWD) via a 2x5 header will probably work, though
    be sure you know what you're doing before straying from the vendor 
    suggestion.

* *[Requirement]* Sparkfun RedBoard Turbo
  - Alternatives: 
    - Many features will work on any *samd21* development board, but if not 
    using the RedBoard Turbo features of this project specific to it will 
    need modification
    - The Docker environment and build system could be a starting point for 
    any ARM-based project

* *[Requirement]* Run the toolchain 
  - My setup: Debian 12 desktop with `make`
    - I `ssh` into the desktop from a laptop and attach to a `tmux` session 
    - `make` allows use of the project build system
    - I use a custom linker script via build system variable 
    `LINK_COMMAND_EXE_CMDARG` to copy generated executables to a NAS, where a
    Windows laptop can download them for programming
  - Alternatives:
    - Any machine that can run Docker (use the provided Dockerfile)
        - Windows is a good choice if using the *Seggar EDU Mini* programmer,
        though hopefully I can make a non-Windows option available
    - The toolchain [downloads page](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)
      has executables for Windows, Linux, and Mac. Presumably an of 
      these will work as a host.

## Development Flow

### Docker
If your host system runs `make` and Docker the project build system provides
convenience targets for container management. From the language root directory
(e.g. `c`, `rust`) run:
- `make cbuild`, build the Docker image
- `make ccreate`, create and start the Docker container
- `make cstart`, start Docker container if not running
- `make cexec`, enter BASH shell prompt inside the running Docker container  

Note: Some Linux distributions may require `sudo` to run Docker

For more information see the `build` directory under the language root 
directory.

### Building
See language-specific quickstarts.
- [`c`](https://github.com/vafeond/BareMetalCapers/blob/main/c/README.md)
- [`Rust`](https://github.com/vafeond/BareMetalCapers/blob/main/rust/README.md)

### Programming  
See [SparkFun ARM programming guide](https://learn.sparkfun.com/tutorials/arm-programming). 
Some additional notes:
- The application is *Microchip Studio*
- Upon startup *Microchip Studio* prompts to download the XC8 compiler, this is NOT 
required to use the programmer
- A *Michrochip Studio* project is NOT required to use the programmer
- Once *Microchip Studio* is opened the programmer tool can be opened. Click 
the button with an icon of a through hole IC and lightning bolt.
- Troubleshooting:
    - See the SparkFun guide linked above regarding fuses
    - If the programmer can't connect to the board try:
        - Restarting the board
        - Restarting *Microchip Studio*
        - Slightly holding/pressing the 2x5 header inserted into the 
        *RedBoard Turbo* (if this is required consider soldering) 

### Programmer vs. Boot Loader - Why is a programmer required?  
Boot loaders eliminate the need for a programmer. Boot loaders take up a small 
amount of flash memory but are simpler and more convenient to use. Typically 
the convenience is a good trade off.

The *RedBoard Turbo* comes pre-programmed with a boot loader that allows the 
*Arduino IDE* to upload `HEX` format executables created by the *Arduino* 
development environment.

SparkFun provides the [SparkFun BOSSA GUI](https://github.com/sparkfun/SparkFun_BOSSA_GUI) 
as an alternative. It supports uploading `BIN` format executables using the 
pre-programmed bootloader.

So far, I cannot use *SparkFun BOSSA GUI* and the *RedBoard Turbo boot loader* 
to upload this project's more than once. After uploading the 
executable neither the *Arduino IDE* nor the *SparkFun BOSSA GUI* detect the 
*RedBoard Turbo*, and the bootloader must be re-programmed.

It seems the created executables do not configure the device or its peripherals
correctly to support the boot loader, though I beleived that would be the 
responsibility of the bootloader itself, so I'm unsure why this doesn't work. 
In fairness, the *RedBoard Turbo boot loader* is intended to support *Arduino*, 
so if the boot loader is deficient (and I'm not saying it is) then it isn't to 
be blamed for a use case outside its scope.

What's clear is that I'm missing something here. I'm sure the reason for 
boot loader incompatibility will become clear as I progress, but for now, a 
programmer is a necessary.

I recommend getting a programmer anyway to restore the 
[RedTurbo Board bootloader](https://github.com/sparkfun/Arduino_Boards/tree/main/sparkfun/samd/bootloaders/turbo) 
if needed.

