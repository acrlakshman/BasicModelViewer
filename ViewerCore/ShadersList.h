/*
 * Copyright (c) 2014, Lakshman Anumolu
 * All rights reserved.
 *
 * This file is part of Basic Model Viewer whose distribution is governed by
 * the BSD 2-Clause License contained in the accompanying LICENSE.txt file.
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

    QOpenGLShaderProgram shader_; /* stores shader files provided by user */

    GLuint default_shaders_count;
    GLuint shaders_count;
};

#endif // END OF __SHADERSLIST_H__
