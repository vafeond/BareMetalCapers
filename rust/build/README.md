# Build System
A GNU Make wrapper to simplify Docker container management.

From `<repo root>/rust` directory (`sudo` may be needed for some Linuc distributions):
- `make cbuild`, build the Docker image
- `make cbuildnocache`, build the Docker image from scratch (use if weird error rebuilding existing image)
- `make ccreate`, create and start the Docker container
- `make cstart`, start Docker container if not running
- `make cexec`, enter BASH shell prompt inside the running Docker container 
- `make cclean`, delete Docker container and image

See `<root>/rust/build/Makefile_DevContainer.mk`

