/*
 * Copyright (c) 2014, Lakshman Anumolu
 * All rights reserved.
 *
 * This file is part of Basic Model Viewer whose distribution is governed by
 * the BSD 2-Clause License contained in the accompanying LICENSE.txt file.
 *
 * Description
 *  Class acts as a workshop to prepare a scene to render.
 */

#ifndef __RENDERSCENE_H__
#define __RENDERSCENE_H__

/*
 * Use this code for user project.
 * TODO: Complete GUI implementation, to avoid user modifying this code for every task.
 */

#include <QOpenGLShaderProgram>

#include <AuxiliaryMethods/HelperFunctions.h>
#include <ViewerCore/ShadersList.h>
#include <Objects/DrawingRoom.h>
#include <Objects/RenderObj.h>
#include <Objects/RenderEnSightPOV.h>

class ShadersList;

class RenderScene
{
public:
    RenderScene();
    ~RenderScene();

    bool InitializeScene();
    bool InitializeShaders();

    bool Draw(
        const QMatrix4x4 &projection,
        const QMatrix4x4 &modelview_cam,
        QMatrix4x4 modelview,
        QOpenGLShaderProgram *shader_
        );

    void TakeDown();

private:
    ShadersList shaders_list;

    // Object to show drawing room (don't take it literally)
    DrawingRoom drawing_room;

    // Render obj file
    RenderObj cubehandle_obj;
    RenderObj teapot_obj;

    /* Render EnSight generated POV */
    //RenderEnSightPOV test1_pov;
};

#endif // END OF __RENDERSCENE_H__
