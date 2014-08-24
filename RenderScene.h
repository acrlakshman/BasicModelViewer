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
#include <Objects/RenderObjs.h>
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
    RenderObj bunny_obj;

    /* Render objs */
    RenderObjs obj_models;

    /* Render EnSight generated POV */
    //RenderEnSightPOV test1_pov;
};

#endif // END OF __RENDERSCENE_H__
