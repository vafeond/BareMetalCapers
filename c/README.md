# C Quickstart

- *[optional]* Create the Docker container. The build system has convenience
  commands for Docker image and container management. If the host system runs
  `make` they can be used directly, otherwise suitable Docker commands can be
  found in [`c/build/Makefile_DevContainer.mk`](https://github.com/vafeond/BareMetalCapers/blob/main/c/build/Makefile_DevContainer.mk).
  If using the build system, the following can be run from the repository root 
  directory:
  - `make cbuild`, `make ccreate`, `make cexec` (`sudo` might be needed to run Docker)
  - For more information see [`c/build/README.md`](https://github.com/vafeond/BareMetalCapers/blob/main/c/build/README.md)

- If not using the Docker container, see [`c/Dockerfile`](https://github.com/vafeond/BareMetalCapers/blob/main/c/Dockerfile) 
  for commands to configure your host system

- From the `c` subdirectory, within the Docker container or not:
  - `make ARCH=samd21`, build SAMD21 debug binaries for all applications (see 
  below)
  - Generated `ELF`, `HEX`, and `BIN` files are stored under the application's `bin`
    subdirectory.

**Applications**
- [`blinking_blue_led`](https://github.com/vafeond/BareMetalCapers/tree/main/c/hal/board/RedBoardTurbo/blinking_blue_led), minimal application validating development setup

For more information see `build/README.md`. 
