Basic Model Viewer
==================

Basic Model Viewer (BMV) is a [Qt5] based visualization tool written in ModernGL and
currently support basic objects and .obj files.

![Alt example figures](http://homepages.cae.wisc.edu/~anumolu/Projects/Applications/BasicModelViewer/cubehandle_teapot_poster)
Rendering two objects using Basic Model Viewer.

Version
-------

0.9.0

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

    Mac OS X 10.9 [Graphics card: HD Graphics 4000, Radeon HD 5750]

License
-------

Please check the accompanying license.txt file.

Acknowledgements
----------------

[Hammad Mazhar] both for his help in understanding the basics of computer graphics and providing his powerful camera class, whose basic functionalities are used in this project.

[Syoyo Fujita] for his obj file loader

[Pradeep Garigipati] for providing sample obj files

[Andrew Seidl]

[BMV demo page]:http://homepages.cae.wisc.edu/~anumolu/Projects/Applications/BasicModelViewer/Demo.html
[Qt5]:http://qt-project.org/qt5
[Hammad Mazhar]:https://github.com/hmazhar
[Syoyo Fujita]:https://github.com/syoyo
[Pradeep Garigipati]:https://github.com/9prady9
[Andrew Seidl]:https://github.com/andrewseidl
