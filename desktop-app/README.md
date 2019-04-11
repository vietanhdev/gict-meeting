# gICT Video Conference - Desktop App

**Supported Operating Systems:** Only Ubuntu Linux was tested.

## I. DEVELOPMENT ENVIRONMENT AND BUILD

### Requirements:

- CMake >= 3.10
- Qt 5
- OpenCV >= 4.0.1
- C++ 17 compiler

### Setup for Linux - Ubuntu 18.04

#### Setup

- Install QT:

```
sudo apt-get install build-essential
sudo apt-get install qtcreator
sudo apt-get install qt5-default
sudo apt-get install qt5-doc
sudo apt-get install qt5-doc-html qtbase5-doc-html
sudo apt-get install qtbase5-examples
sudo /sbin/ldconfig -v
```

- Install OpenCV

```
https://linuxize.com/post/how-to-install-opencv-on-ubuntu-18-04/
```

- Install SDL2 lib
```
sudo apt-get install libsdl2-dev
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
./VideoConf
```

- Choose "Join A Conference" and pick a configure file at `./data/example-conf*`.

## III. REFERENCES / CITE

- In this project, we use code from:
    + Dark Theme for Qt5 by Juergen Skrotzky: [https://github.com/Jorgen-VikingGod/Qt-Frameless-Window-DarkStyle](https://github.com/Jorgen-VikingGod/Qt-Frameless-Window-DarkStyle).
    + Image viewer from [https://github.com/easymodo/qimgv](https://github.com/easymodo/qimgv).

- Images, icons from:
    + https://www.flaticon.com/authors/smashicons
    + https://www.flaticon.com/authors/roundicons
    + https://www.freepik.com/free-vector/bunny-ears-nose-carnival-mask-photo_4015599.htm
    + and other images collected from the internet.
