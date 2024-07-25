# Rust Quickstart

- *[optional]* Create the Docker container. The build system has convenience
  commands for Docker image and container management. If the host system runs
  `make` they can be used directly, otherwise suitable Docker commands can be
  found in [`rust/build/Makefile_DevContainer.mk`](https://github.com/vafeond/BareMetalCapers/blob/main/rust/build/Makefile_DevContainer.mk).
  If using the build system, the following can be run from the repository root 
  directory:
  - `make cbuild`, `make ccreate`, `make cexec` (`sudo` might be needed to run Docker)
  - For more information see [`rust/build/README.md`](https://github.com/vafeond/BareMetalCapers/blob/main/rust/build/README.md)

- If not using the Docker container, see [`rust/Dockerfile`](https://github.com/vafeond/BareMetalCapers/blob/main/rust/Dockerfile) 
  for commands to configure your host system

- From the `rust` subdirectory, within the Docker container or not:
  - `cargo build`, build all binaries for all applications (see below)
  - `cargo clean`, delete all build artifacts
  - **Tip:** Cargo can be easily customized. Cargo searches `PATH` locations for
  executables starting with "`cargo-`" and adds the remainder of the executable
  name as a command. An executable can be a BASH script, so it's easy to add a
  custom build script.
    - `cargo bbuild --bin <binary name>` runs command in `cargo-bbuild` script, that:
      - Builds all binaries, including `ELF` files for SAMD21 applications
      - Generates the `HEX` format binary from the specified `ELF` binary  

**Applications**
- [`blinking_blue_led`](https://github.com/vafeond/BareMetalCapers/blob/main/rust/hal/board/redboardturbo/src/bin/blinking_blue_led.rs), minimal application validating development setup
- [`my_first_comms`](https://github.com/vafeond/BareMetalCapers/blob/main/rust/hal/board/redboardturbo/src/bin/my_first_comms.rs), communication with host via USB

For more information see `build/README.md`. 
