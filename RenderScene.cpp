/*
 * Copyright (c) 2014, Lakshman Anumolu
 * All rights reserved.
 *
 * This file is part of Basic Model Viewer whose distribution is governed by
 * the BSD 2-Clause License contained in the accompanying LICENSE.txt file.
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

    /* Edit default light info */
    Light light_for_teapot;
    light_for_teapot.position = QVector3D(0.0, 0.0, 10.0);
    light_for_teapot.color_ambient = QVector3D(0.75, 0.6, 0.5);
    light_for_teapot.color_diffuse = QVector3D(0.75, 0.6, 0.5);
    light_for_teapot.color_specular = QVector3D(1.0, 1.0, 1.0);

    this->teapot_obj.EditLight(light_for_teapot);

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

    QMatrix4x4 mv_tmp_ = modelview;
    modelview.translate(QVector3D(5.0, 5.0,-2.0));
    if (!teapot_obj.Draw(projection, modelview_cam, modelview, shader_))
        return false;
    modelview = mv_tmp_;

    return true;
}

void RenderScene::TakeDown()
{
    this->shaders_list.TakeDown();
    this->drawing_room.TakeDown();
    this->cubehandle_obj.TakeDown();
    this->teapot_obj.TakeDown();
}
