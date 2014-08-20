/*
 * Copyright (c) 2014, Lakshman Anumolu
 * All rights reserved.
 *
 * This file is part of Basic Model Viewer whose distribution is governed by
 * the BSD 2-Clause License contained in the accompanying LICENSE.txt file.
 */

#include "DrawingRoom.h"

DrawingRoom::DrawingRoom()
{
	default_vertex_color = QVector3D(0.6, 0.6, 0.6);
	default_light.position = QVector3D(0.0, 0.0, -10.0);
	default_light.color_ambient = QVector3D(0.3, 0.3, 0.3);
	default_light.color_diffuse = QVector3D(0.3, 0.3, 0.3);
	default_light.color_specular = QVector3D(1.0, 1.0, 1.0);
}

DrawingRoom::~DrawingRoom()
{
}

bool DrawingRoom::LocalInitialize()
{
	QVector2D xy_min_ = QVector2D(-70,-70);
    QVector2D xy_max_ = QVector2D(70,70);
	return this->LocalInitialize(xy_min_, xy_max_);
}

bool DrawingRoom::LocalInitialize(
	QVector2D range_min,
	QVector2D range_max,
	GLuint slices_,
	GLuint stacks_
	)
{
    this->xy_min = range_min;
    this->xy_max = range_max;

    plane_object.SetVertexColor(this->default_vertex_color);
    this->SetShininess(100.0);
    plane_object.AddLight(default_light);
    plane_object.include_ambience(true);
    plane_object.include_diffuse(true);
    plane_object.include_specular(false);

	plane_object.LocalInitialize(range_min, range_max, slices_, stacks_);

	return !GLReturnedError("DrawingRoom::LocalInitialize - on exit");
}

void DrawingRoom::SetVertexColor(QVector3D &color_)
{
	plane_object.SetVertexColor(color_);
}

bool DrawingRoom::Draw(
	const QMatrix4x4 &projection,
	const QMatrix4x4 &modelview_cam,
	QMatrix4x4 modelview,
	QOpenGLShaderProgram *shader_
	)
{

	QMatrix4x4 mv_tmp = modelview_cam;

    GLfloat offset_ = (qAbs(this->xy_min.x()) + qAbs(this->xy_max.x())) / 2.0;

    mv_tmp.translate(QVector3D(0, 0, -offset_));
    plane_object.Draw(projection, modelview_cam, mv_tmp, shader_);
    mv_tmp = modelview_cam;

    mv_tmp.translate(QVector3D(0, -offset_, 0));
    mv_tmp.rotate(-90., QVector3D(1.0, 0.0, 0.0));
    plane_object.Draw(projection, modelview_cam, mv_tmp, shader_);
    mv_tmp = modelview_cam;

    mv_tmp.translate(QVector3D(-offset_, 0, 0));
    mv_tmp.rotate(90, QVector3D(0.0, 1.0, 0.0));
    plane_object.Draw(projection, modelview_cam, mv_tmp, shader_);
    mv_tmp = modelview_cam;

    mv_tmp.translate(QVector3D(offset_, 0, 0));
    mv_tmp.rotate(-90, QVector3D(0.0, 1.0, 0.0));
    plane_object.Draw(projection, modelview_cam, mv_tmp, shader_);
    mv_tmp = modelview_cam;

    mv_tmp.translate(QVector3D(0, offset_, 0));
    mv_tmp.rotate(90, QVector3D(1.0, 0.0, 0.0));
    plane_object.Draw(projection, modelview_cam, mv_tmp, shader_);
    mv_tmp = modelview_cam;

    return !GLReturnedError("DrawingRoom::Draw - on exit");
}

void DrawingRoom::AddLight(Light light_)
{
	plane_object.AddLight(light_);
}

void DrawingRoom::SetShininess(GLfloat shininess_)
{
	plane_object.SetShininess(shininess_);
}

void DrawingRoom::show_wire_frame(bool swf_)
{
	plane_object.show_wire_frame(swf_);
}

void DrawingRoom::show_normals(bool sn_)
{
	plane_object.show_normals(sn_);
}

void DrawingRoom::TakeDown()
{
	plane_object.TakeDown();
}
