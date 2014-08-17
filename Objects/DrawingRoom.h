/*
 * Copyright (c) 2014, Lakshman Anumolu
 * All rights reserved.
 *
 * This file is part of Basic Model Viewer whose distribution is governed by
 * the BSD-style license contained in the accompanying file license.txt.
 *
 * Description
 *  Wrapper to create a default drawing environment, to visualize objects
 *	inside it.
 */

#ifndef __DRAWINGROOM_H__
#define __DRAWINGROOM_H__

#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>

#include <AuxiliaryMethods/CheckErrors.h>
#include <AuxiliaryMethods/HelperFunctions.h>
#include <Objects/Plane.h>

class DrawingRoom
{
public:
	DrawingRoom();
	~DrawingRoom();

	bool LocalInitialize();
	bool LocalInitialize(
		QVector2D range_min,
		QVector2D range_max,
		GLuint slices_ = 2,
		GLuint stacks_ = 2
		);

	void SetVertexColor(QVector3D &color_);

	bool Draw(
		const QMatrix4x4 &projection,
		const QMatrix4x4 &modelview_cam,
		QMatrix4x4 modelview,
		QOpenGLShaderProgram *shader_
		);

	// Add light
	void AddLight(Light light_);

	// Set object shininess parameter
	void SetShininess(GLfloat shininess_);

	void show_wire_frame(bool swf_);
	void show_normals(bool sn_);

	// Delete all created vectors, associated shaders
	void TakeDown();

private:
	QVector2D xy_min, xy_max;

	Plane plane_object;

	QVector3D default_vertex_color;
	Light default_light;
};

#endif // END OF __DRAWINGROOM_H__
