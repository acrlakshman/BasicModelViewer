Basic Model Viewer
==================

Basic Model Viewer (BMV) is a [Qt5] based visualization tool written in Modern OpenGL and
currently support basic objects and .obj files.

![Alt example figures](http://lakshmananumolu.com/Projects/Applications/BasicModelViewer/cubehandle_teapot_poster)
Rendering two objects using Basic Model Viewer.

Version
-------

0.9.0

Basic Usage
-----------

Please check the quick start guide at [BMV Introduction].

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

Tested on:

    Mac OS X 10.9 [Graphics card: HD Graphics 4000, Radeon HD 5750]
    Mac OS X 10.10 [Graphics card: HD Graphics 4000]
    Mac OS X 10.11 [Graphics card: HD Graphics 4000]
    Mac OS X 10.14 [Graphics card: HD Graphics 4000]

License
-------

This software is distributed under the [LGPLv2.1] license. Please read [LICENSE] for information on the software availability and distribution.

Acknowledgements
----------------

[Hammad Mazhar] both for helping me to understand the basics of computer graphics and for making his camera class open source, the basic functionalities of which are used in this project.

[Syoyo Fujita] for his obj file loader.

[Pradeep Garigipati] for providing sample obj files.

[Andrew Seidl]

[BMV Introduction]:http://lakshmananumolu.com/Projects/Applications/BasicModelViewer/
[Qt5]:http://qt-project.org/qt5
[Hammad Mazhar]:https://github.com/hmazhar
[Syoyo Fujita]:https://github.com/syoyo
[Pradeep Garigipati]:https://github.com/9prady9
[Andrew Seidl]:https://github.com/andrewseidl
[LICENSE]:https://github.com/acrlakshman/BasicModelViewer/blob/master/LICENSE.txt
[LGPLv2.1]:http://www.gnu.org/licenses/lgpl-2.1.html
