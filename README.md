## Requirements

 * Blackmagic Desktop Video Software
 * Blackmagic DeckLink SDK
 * OpenCV
 * Boost C++

## Compilation

- Set `DeckLink_DIR` in CMakeLists.txt. It is recommended to extract Blackmagic DeckLink SDK and create a symblic link, such as "bmdsdk" to the unarchived directory.
- Using CMake, compile the program.

## Running

- run the developed executable file named **blackmagic_test**
- Ensure that you have OpenCV and Boost shared libraries in your PATH (or as appropriate for your system).

## Note

In practice, this project was forked from [motiz88 repo](https://github.com/motiz88/blackmagic-opencv-test) and add a few modifications to compile this on MacOS.
