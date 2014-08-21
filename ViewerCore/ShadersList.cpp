/*
 * Copyright (c) 2014, Lakshman Anumolu
 * All rights reserved.
 *
 * This file is part of Basic Model Viewer whose distribution is governed by
 * the BSD 2-Clause License contained in the accompanying LICENSE.txt file.
 */

#include "ShadersList.h"

ShadersList::ShadersList()
    : default_shaders_count(0),
    shaders_count(0)
{
}

bool ShadersList::InitializeDefaultShaders()
{
    if (!GetShaderFromSourceFile(&this->default_shader0, "../Shaders/phong_shader.vert", "../Shaders/phong_shader.frag"))
        return false;

    if (!GetShaderFromSourceFile(&this->default_shader1, "../Shaders/solid_shader.vert", "../Shaders/solid_shader.frag"))
        return false;

    if (!GetShaderFromSourceFile(&this->default_shader2, "../Shaders/gouraud_shader.vert", "../Shaders/gouraud_shader.frag"))
        return false;

    this->default_shaders.push_back(&this->default_shader0);
    this->default_shaders.push_back(&this->default_shader1);
    this->default_shaders.push_back(&this->default_shader2);

    default_shaders_count = default_shaders.size();

    return true;
}

bool ShadersList::InitializeShader(QString vertex_shader_file_, QString fragment_shader_file_)
{
    if (!GetShaderFromSourceFile(&this->shader_, vertex_shader_file_, fragment_shader_file_)) {
        return false;
    }

    shaders.push_back(&this->shader_);
    shaders_count++;

    return true;
}

bool ShadersList::GetShaderFromSourceFile(
                                            QOpenGLShaderProgram *shader_,
                                            QString vertex_shader_file,
                                            QString fragment_shader_file
                                            )
{
    /* Compile vertex shader */
    if (!shader_->addShaderFromSourceFile(QOpenGLShader::Vertex, vertex_shader_file))
        return false;

    /* Compile fragment shader */
    if (!shader_->addShaderFromSourceFile(QOpenGLShader::Fragment, fragment_shader_file))
        return false;

    /* Link shader pipeline */
    if (!shader_->link())
        return false;

    // Bind shader pipeline for use
    //if (!shader_->bind())
      //  return false;

    return true;
}

void ShadersList::TakeDown()
{
    for (int i = 0; i < this->default_shaders.size(); ++i)
        this->default_shaders[i]->removeAllShaders();
    this->default_shaders.erase(this->default_shaders.begin(), this->default_shaders.end());

    default_shaders_count = 0;

    for (int i = 0; i < this->shaders.size(); ++i)
        this->shaders[i]->removeAllShaders();
    this->shaders.erase(this->shaders.begin(), this->shaders.end());

    shaders_count = 0;
}
