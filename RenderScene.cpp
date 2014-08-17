/*
 * Copyright (c) 2014, Lakshman Anumolu
 * All rights reserved.
 *
 * This file is part of Basic Model Viewer whose distribution is governed by
 * the BSD-style license contained in the accompanying file license.txt.
 */

#include "RenderScene.h"

RenderScene::RenderScene()
{
}

RenderScene::~RenderScene()
{
}

bool RenderScene::InitializeScene()
{
    if (!this->InitializeShaders())
        return false;

    // Initialize default objects
    if (!this->drawing_room.LocalInitialize())
        return false;

    if (!this->cubehandle_obj.LocalInitialize("../Resources/cubehandle0fix.obj"))
        return false;
    this->cubehandle_obj.show_wire_frame(false);
    this->cubehandle_obj.show_normals(false);

    if (!this->teapot_obj.LocalInitialize("../Resources/teapot.obj"))
        return false;

    return true;
}

bool RenderScene::InitializeShaders()
{
    return shaders_list.InitializeDefaultShaders();
}

bool RenderScene::Draw(
    const QMatrix4x4 &projection,
    const QMatrix4x4 &modelview_cam,
    QMatrix4x4 modelview,
    QOpenGLShaderProgram *shader_
    )
{
    if (!drawing_room.Draw(projection, modelview_cam, modelview, shader_))
        return false;

    if (!cubehandle_obj.Draw(projection, modelview_cam, modelview, shader_))
        return false;

    modelview.translate(QVector3D(5.0, 5.0,-2.0));
    if (!teapot_obj.Draw(projection, modelview_cam, modelview, shader_))
        return false;

    return true;
}

void RenderScene::TakeDown()
{
    shaders_list.TakeDown();
    drawing_room.TakeDown();
    cubehandle_obj.TakeDown();
    teapot_obj.TakeDown();
}
