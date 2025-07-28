# Calendar
A TUI calendar program written in C++ **for Windows**. Made for a school project.

# Downloading
To download the source code, it's recommended that you clone this repository.
To do that, you can run `git clone <repository url>` in your desired directory.

An alternative option if you don't have git installed is downloading the source as a zip and extracting it.

# Building
This project is made in C++, and the only nonstandard library used is `conio.h`.
This tutorial will focus on [G++ from MinGW-W64](https://github.com/niXman/mingw-builds-binaries/), which comes with `conio.h`, but other compilers and toolchains may also work.
To compile this project, first [clone](#downloading) this repository, and then run your compiler on the `main.cpp` file.
To compile using g++, run `g++ <inputFilename> -o <outputFilename>`.

Example of cloning and building.
```sh
cd Documents
git clone https://github.com/Obsidianninja11/Calendar.git
cd Calendar/src
g++ main.cpp -o calendar.exe
```

# Running
After compiling, simply run the executable.
