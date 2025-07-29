# Calendar
A TUI calendar program written in C++ **for Windows**. Made for a school project.

# Downloading
To download the source code, it's recommended that you clone this repository.
To do that, you can run `git clone <repository url>` in your desired directory, such as Documents.

An alternative option if you don't have git installed is downloading the source as a zip and extracting it.

# Dependencies
This project requires 1 nonstandard library, `conio.h`, a Windows specific header file that is necessary to detect key presses.
This can be obtained from a variety of toolchains, but the [MinGW-W64-builds toolchain](https://github.com/niXman/mingw-builds-binaries/), is recommended.

# Building
The [MinGW-W64-builds](https://github.com/niXman/mingw-builds-binaries/) toolchain comes with G++, the recommended compiler for this project.
To compile this project, first [clone](#downloading) this repository, and then run your compiler on the `main.cpp` file.
To compile using g++, run `g++ <inputFilename> -o <outputFilename>`.

Full example of cloning and building into the Documents folder:
```sh
cd Documents
git clone https://github.com/Obsidianninja11/Calendar.git
cd Calendar/src
g++ main.cpp -o main.exe
```

# Running
After compiling, simply run the executable by double clicking, or from your terminal.
