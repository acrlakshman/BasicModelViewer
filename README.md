Basic Model Viewer
==================

Basic Model Viewer (BMV) is a [Qt5] based visualization tool written in ModernGL and
currently support basic objects and .obj files.

![Alt text](http://homepages.cae.wisc.edu/~anumolu/Projects/Applications/BasicModelViewer/cubehandle_teapot_poster)

Version
-------

0.1

Basic Usage
-----------

Please check the demo at [BMV demo page].

Prerequisites
-------------

Install [Qt5] with opengl support. Apart from this, no other dependency on third
party libraries.

Installation
------------

```sh
mkdir build
cd build
qmake ..
make
```

Tested environment(s)
---------------------

Currently tested on:

    Mac OSX 10.9 [Graphics card: HD Graphics 4000, Radeon HD 5750]

License
-------

Please check the accompanying license.txt file.

[BMV demo page]:http://homepages.cae.wisc.edu/~anumolu/Projects/Applications/BasicModelViewer/Demo.html
[Qt5]:http://qt-project.org/qt5