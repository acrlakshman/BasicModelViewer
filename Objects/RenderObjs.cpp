/*
 * Copyright (c) 2014, Lakshman Anumolu
 * All rights reserved.
 *
 * This file is part of Basic Model Viewer whose distribution is governed by
 * the BSD 2-Clause License contained in the accompanying LICENSE.txt file.
 */

#include "RenderObjs.h"

RenderObjs::RenderObjs()
{
}

RenderObjs::~RenderObjs()
{
}

void RenderObjs::TakeDown()
{
    for (int i = 0; i < render_objs.size(); ++i)
        this->render_objs[i]->TakeDown();
    
    this->render_objs.erase(this->render_objs.begin(), this->render_objs.end());
}

bool RenderObjs::Draw(
        const QMatrix4x4 &projection,
        const QMatrix4x4 &modelview_cam,
        QMatrix4x4 modelview,
        QOpenGLShaderProgram *shader_
        )
{
    modelview.translate(QVector3D(-this->x_base_move, -this->y_base_move, -this->z_base_move));
    for (int i = 0; i < render_objs.size(); ++i)
        if (!this->render_objs[i]->Draw(projection, modelview_cam, modelview, shader_))
            return false;

    return true;
}

void RenderObjs::AddObj(RenderObj *render_obj_)
{
    this->render_objs.push_back(render_obj_);

    this->ComputeGlobalBoundingBox();
    this->x_base_move = (this->global_b_box.x_range.x() + this->global_b_box.x_range.y()) /  2.0;
    this->y_base_move = this->global_b_box.y_range.x();
    //this->y_base_move = (this->global_b_box.y_range.x() + this->global_b_box.y_range.y()) /  2.0;
    this->z_base_move = (this->global_b_box.z_range.x() + this->global_b_box.z_range.y()) /  2.0;
}

void RenderObjs::ComputeGlobalBoundingBox()
{
    QVector<bounding_box> b_boxes;
    for (int i = 0; i < render_objs.size(); ++i) {
        QVector<bounding_box> tmp_b_boxes;
        tmp_b_boxes = render_objs[i]->GetBoundingBoxes();
        for (int j = 0; j < tmp_b_boxes.size(); ++j) {
            b_boxes.push_back(tmp_b_boxes[j]);
        }
        tmp_b_boxes.clear();
    }
    this->global_b_box = BoundingBoxesUnion(b_boxes);
    /* PrintBoundingBox(this->global_b_box); */
    b_boxes.clear();
}
