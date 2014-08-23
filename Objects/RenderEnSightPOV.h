/*
 * Copyright (c) 2014, Lakshman Anumolu
 * All rights reserved.
 *
 * This file is part of Basic Model Viewer whose distribution is governed by
 * the BSD 2-Clause License contained in the accompanying LICENSE.txt file.
 *
 * Description
 *	Wrapper class to render EnSight generated POV file.
 */

#ifndef __RENDERENSIGHTPOV_H__
#define __RENDERENSIGHTPOV_H__

#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>

#include <AuxiliaryMethods/CheckErrors.h>
#include <AuxiliaryMethods/HelperFunctions.h>
#include <ViewerCore/ParseEnSightPOV.h>
#include <ViewerCore/GenericObject.h>

class RenderEnSightPOV
{
public:
	RenderEnSightPOV();
	~RenderEnSightPOV();

	bool LocalInitialize(QString file_name_);

	void SetVertexColor(QVector3D &color_);

	bool Draw(
		const QMatrix4x4 &projection,
		const QMatrix4x4 &modelview_cam,
		QMatrix4x4 modelview,
		QOpenGLShaderProgram *shader_
		);

	// Add light
	void AddLight(Light light_);

	// Return number of lights on this object
	GLuint GetNumberOfLights();
	
	// Edit light info
	void EditLight(Light light_, GLuint idx_ = 0);

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

	ParseEnSightPOV parse_ensight_pov;
	GenericObject *generic_object;
};

#endif // END OF __RENDERENSIGHTPOV_H__
