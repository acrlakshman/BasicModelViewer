/*
 * This file is part of Basic Model Viewer (visualization package)
 *
 * Copyright (c) 2015, Lakshman Anumolu
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
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
    app.setApplicationVersion("0.9");

    QGLFormat gl_format;
    gl_format.setVersion(3, 3);
    gl_format.setProfile(QGLFormat::CoreProfile);
    gl_format.setSampleBuffers(true);
    gl_format.setSamples(4);

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
