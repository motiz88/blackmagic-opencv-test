## This fork
 * Fix compiling in ubuntu
 * Set input mode for Decklink SDI 4K

## Requirements

 * Blackmagic DeckLink SDK
 * OpenCV 3.
 * Boost C++

## Compilation

Use CMake. Make sure to set `DeckLink_DIR`, `OpenCV_DIR` and `Boost_DIR` correctly.

Note that in practice I've only compiled this on Windows with MSVC12 (2013) since forking it from [Jesús Torres' original Linux-only repo](https://github.com/ull-isaatc/blackmagic-test/tree/0a6ba03baeefa90bab00b355a59a566b4eb7b407). I'm fairly confident it can be built on Linux and Mac with minimal changes. If you do so, I'd love to know about it and accept any PRs that may be needed.

## Running

Ensure that you have the OpenCV and Boost shared libraries in your PATH (or as appropriate for your system).
