Basic Model Viewer
==================

Basic Model Viewer (BMV) is a [Qt5] based visualization tool written in Modern OpenGL and
currently support basic objects and .obj files.

_This tool was originally written as part of learning experience from Computer Graphics course. Project is not actively developed, however I intend to test on mac as long as I use this tool as an auxiliary utility to quickly obtain camera and object locations for photo realistic rendering._

![Stanford bunny](https://lakshmananumolu.com/Projects/Applications/BasicModelViewer/bunny.gif)

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
    Mac OS X 10.14 [Qt 5.13.0]

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
