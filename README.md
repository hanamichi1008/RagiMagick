# RagiMagick


## How to Build
### for MacOS

To build RagiMagick on a MacOS ( or Linux OS ), follow the instructions under the below.  

---

#### 1. install cmake
install with brew.

```
$ brew install cmake
```

#### 2. make `build` directory
move to the directory which has main.cpp .

```
$ pwd
/${PATH_TO_RAGIMAGICK}/RagiMagick

$ ls
main.cpp ragii ..etc

$ mkdir build
$ cd build
```

#### 3. run `cmake`
run `cmake` command to create `Makefile` .

```
$ pwd
/${PATH_TO_RAGINMAGICK}/RagiMagick/build

$ cmake ../
```

#### 4. run make
run `make` command to create a binary file.

```
$ ls
Makfile ...etc

$ make
$ ls
RagiMagick  ...etc
```
