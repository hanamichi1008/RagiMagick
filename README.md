  - [How to Build](#how-to-build)
    - [for MacOS](#for-macos)
      - [1. install tools](#1-install-tools)
      - [2. make `build` directory](#2-make-build-directory)
      - [3. run `setup.sh`](#3-run-setupsh)
      - [4. run `cmake`](#4-run-cmake)
      - [5. run make](#5-run-make)
    - [for Windows (Git Bash)](#for-windows-git-bash)
      - [1. install tools](#1-install-tools-1)
      - [2. make `build` directory](#2-make-build-directory-1)
      - [3. run `setup.sh`](#3-run-setupsh-1)
      - [4. run `cmake`](#4-run-cmake-1)
      - [5. run make](#5-run-make-1)

# RagiMagick

## How to Build

### for MacOS

To build RagiMagick on a MacOS, follow the instructions under the below.  

#### 1. install tools
install with brew.

```
$ brew install cmake
$ brew install nasm
```

#### 2. make `build` directory
move to the directory which has CMakeLists.txt .

```
$ pwd
/${PATH_TO_RAGIMAGICK}

$ ls
CMakeLists.txt README.md ..etc

$ mkdir build
$ cd build
```

#### 3. run `setup.sh`
move to `external_lib` directory
run `./setup.sh`

```
$ pwd
/${PATH_TO_RAGIMAGICK}/external_lib

$ chmod +x setup.sh
$ ./setup.sh
```

#### 4. run `cmake`
run `cmake` command to create `Makefile` .

```
$ pwd
/${PATH_TO_RAGIMAGICK}/build

$ cmake ../
```

#### 5. run make
run `make` or `cmake --build . -j <jobs>` command to create a binary file.

```
$ ls
Makfile ...etc

$ make
$ ls
RagiMagick  ...etc
```

---

### for Windows (Git Bash)

To build RagiMagick on a Windows, follow the instructions under the below.  

#### 1. install tools
`CMake`

ttps://cmake.org/download/
```
Binary distributions
    Windows win64-x64 Installer
```

`NASM`

ttps://www.nasm.us/pub/nasm/releasebuilds/2.14.02/win64/
```
*-installer-x64.exe
```

`LLVM`

ttp://releases.llvm.org/download.html#8.0.0
```
Pre-Built Binaries
    Windows (64b-it)
```

#### 2. make `build` directory
move to the directory which has CMakeLists.txt .

```
$ pwd
/${PATH_TO_RAGIMAGICK}

$ ls
CMakeLists.txt README.md ..etc

$ mkdir build
$ cd build
```

#### 3. run `setup.sh`
move to `external_lib` directory
run `./setup.sh`

```
$ pwd
/${PATH_TO_RAGIMAGICK}/external_lib

$ chmod +x setup.sh
$ ./setup.sh
```

#### 4. run `cmake`
run `cmake` command to create `Visual Studio Solution and Project` files.

```
$ pwd
/${PATH_TO_RAGINMAGICK}/build

$ cmake ../ -T llvm
```

#### 5. run make
run `cmake --build . -j <jobs>` command to create a binary file.

```
$ ls
RagiMagick.sln ...etc

$ cmake --build . -j 4
$ ls
RagiMagick  ...etc
```
