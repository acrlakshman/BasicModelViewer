/*
 * Copyright (c) 2014, Lakshman Anumolu
 * All rights reserved.
 *
 * This file is part of Basic Model Viewer whose distribution is governed by
 * the BSD-style license contained in the accompanying file license.txt.
 */

#include <QApplication>
#include <QLabel>

#ifndef QT_NO_OPENGL
#include "ViewerWindow.h"
#endif

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Basic Model Viewer");
    app.setApplicationVersion("0.1");

    QGLFormat gl_format;
    gl_format.setVersion(3, 3);
    gl_format.setProfile(QGLFormat::CoreProfile);
    //gl_format.setSampleBuffers(true);

#ifndef QT_NO_OPENGL
    ViewerWindow widget(gl_format);
    widget.show();
    widget.resize(640, 480);
#else
    QLabel note("OpenGL Support required");
    note.show();
#endif
    return app.exec();
}

