# Hello World - My First Comms

TBD
- Why do serial comms next?
  - serial comms for debugging and validation


## USB  
TBD
- why USB is a good choice
- termninal application (maybe bsest in section below)


## Design?  
TBD
- MCU memory mapped I/O modelling
- Board-specifics?
  - RedBoard Turbo USB connector restricts use of those pins, so certain SAMD21
  multiplexing configurations are invalid, so this can be set via board configuration
    - [c]: Board-specific Makefile
    - [Rust]: Crate feature?


## My First Comms Application
TBD
- SAMD21 config:
  - Needed periherals
  - Demystify some ARM specifics (main clock vs. generic clock generator)

UPDATE:
SAMD21 ISR Vector Table (abridged):
- `0x00000000`: Main stack pointer (set to address in SRAM)
- `0x00000004`: Reset handler pointer (set to address of `c` or `rust` function to run on system power on or reset)

UPDATE:
See:
- [`c/hal/board/RedBoardTurbo/blinking_blue_led/main.c`](https://github.com/vafeond/BareMetalCapers/blob/main/c/hal/board/RedBoardTurbo/blinking_blue_led/main.c)
- [`rust/hal/board/redboardturbo/src/bin/blinking_blue_led.rs`](https://github.com/vafeond/BareMetalCapers/blob/main/rust/hal/board/redboardturbo/src/bin/blinking_blue_led.rs)


## Wrap Up
TBD

