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
