[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/X8X6RXV10)

# FeedbackDelay

Does what it says on the tin: A feedback delay plugin for SuperCollider with cubic interpolation and support for both audio and control rate arguments.

## Features

The following plugins are included:
- **FeedbackDelay**: A feedback delay with cubic interpolation and support for both audio and control rate arguments.
- **VAStateVariableFilter**: A state variable filter with cutoff, resonance and 8 different filter types. This is based on an analog topology.

## Development

The delay line code is written using modern C++ templating tricks, to allow easily making variations with different operations.

All dependencies are handled by [CPM](https://github.com/cpm-cmake/CPM.cmake).

## Requirements

- CMake >= 3.12

## Building

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

# Credits

The VA state variable filter code is a rewrite of [Jordan T Harris' juce svf filter](https://github.com/JordanTHarris/VAStateVariableFilter).
