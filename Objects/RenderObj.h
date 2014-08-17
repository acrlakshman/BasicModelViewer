/*
 * Copyright (c) 2014, Lakshman Anumolu
 * All rights reserved.
 *
 * This file is part of Basic Model Viewer whose distribution is governed by
 * the BSD-style license contained in the accompanying file license.txt.
 *
 * Description
 *	Wrapper class to render all shapes in obj file.
 */

#ifndef __RENDEROBJ_H__
#define __RENDEROBJ_H__

#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>

#include <AuxiliaryMethods/CheckErrors.h>
#include <AuxiliaryMethods/HelperFunctions.h>
#include <ViewerCore/ParseObj.h>
#include <ViewerCore/GenericObject.h>

class RenderObj
{
public:
	RenderObj();
	~RenderObj();

	bool LocalInitialize(QString obj_file_name);

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
	GLuint number_of_shapes;

	QVector3D default_vertex_color;
	Light default_light;

	ParseObj parse_obj;
	GenericObject *generic_object;
};

#endif // END OF __RENDEROBJ_H__
