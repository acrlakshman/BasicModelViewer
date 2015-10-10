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
 *
 * Description
 *  Class to store all shaders and use them whenever required.
 *  TODO: Rework this class and make it more useful and general.
 */

#ifndef __SHADERSLIST_H__
#define __SHADERSLIST_H__

#include <QOpenGLShaderProgram>
#include <QVector>
#include <QString>

class ShadersList {
public:
    ShadersList();

    /* Load all shader files you want to use in the project */
    bool InitializeDefaultShaders();

    /* Load requested shader files */
    bool InitializeShader(QString vertex_shader_file_, QString fragment_shader_file_);

    /* Load shader from source file */
    bool GetShaderFromSourceFile(
            QOpenGLShaderProgram *shader_,
            QString vertex_shader_file,
            QString fragment_shader_file
            );

    /* Delete all shaders */
    void TakeDown();

    QVector<QOpenGLShaderProgram *> default_shaders;
    QVector<QOpenGLShaderProgram *> shaders;

private:
    /* Different shader objects */
    QOpenGLShaderProgram default_shader0;
    QOpenGLShaderProgram default_shader1;
    QOpenGLShaderProgram default_shader2;

    QOpenGLShaderProgram shader_; /* stores shader files provided by user */

    GLuint default_shaders_count;
    GLuint shaders_count;
};

#endif // END OF __SHADERSLIST_H__
