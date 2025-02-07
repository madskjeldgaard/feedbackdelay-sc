# FeedbackDelay

Author: Mads Kjeldgaard

A feedback delay plugin for SuperCollider

### Requirements

- CMake >= 3.12

### Building

The project uses CPM to download the supercollider code and necessary dependencies. 

All you have to do is build:

Clone the project:

    git clone https://github.com/madskjeldgaard/feedbackdelay-sc
    cd feedbackdelay-sc
    mkdir build
    cd build

Then, use CMake to configure and build it:

    cmake .. -DCMAKE_BUILD_TYPE=Release
    cmake --build . --config Release
    cmake --build . --config Release --target install

You may want to manually specify the install location in the first step to point it at your
SuperCollider extensions directory: add the option `-DCMAKE_INSTALL_PREFIX=/path/to/extensions`.

It's expected that the SuperCollider repo is cloned at `../supercollider` relative to this repo. If
it's not: add the option `-DSC_PATH=/path/to/sc/source`.

### Developing

The project includes vscode-style tasks that should be picked up by vscode and neovim etc.
