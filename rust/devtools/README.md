# IDE Container
Example of development container extension to support a text-based IDE, being LunarVim.

1. Create the IDE Docker container: 
  - From the project root (`sudo` may be needed to use Docker):
  - Build the development Docker image, `make cbuild`  
  - Build the IDE Docker image, `make ibuild`
  - Create the IDE Docker container, `make icreate`

2. Install and configure LunarVim
  - Enter the IDE Docker container, `make iexec`
  - Install LunarVim, `LV_BRANCH='release-1.3/neovim-0.9' bash <(curl -s https://raw.githubusercontent.com/LunarVim/LunarVim/release-1.3/neovim-0.9/utils/installer/install.sh)`
    - Check for latest install command
    - Install Rust tools when/if prompted
  - Symbolic link LunarVim application, `ln -s /root/.local/bin/lvim /usr/bin/lvim`
  - Run LunarVim, `lvim README.md` (assumes README.md exists)
  - Open LunarVim config `<leader> ; c`
  - Replace contents with your LunarVim config

