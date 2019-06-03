# gICT Video Conference - Streaming Server

**Supported Operating Systems:** Only Ubuntu Linux was tested.

## I. DEVELOPMENT ENVIRONMENT AND BUILD

### Requirements:

- CMake >= 3.10
- OpenCV >= 4.0.1
- C++ 17 compiler

### Setup for Linux - Ubuntu 18.04

#### Setup

- Install OpenCV

```
https://linuxize.com/post/how-to-install-opencv-on-ubuntu-18-04/
```

- Install portaudio
```
sudo apt install portaudio19-dev
```

#### Compile and Run

- Compile
```
cd <project directory>
mkdir build
cd build
cmake ..
make
```

- Run
```
cd bin
./StreamServer <config-file>
```

- Example config file at [./example-conf.yaml](example-conf.yaml)

### NOTE:

- Modify `src/dev.h` for development settings.
