/*
 * Copyright (c) 2014, Lakshman Anumolu
 * All rights reserved.
 *
 * This file is part of Basic Model Viewer whose distribution is governed by
 * the BSD-style license contained in the accompanying file license.txt.
 *
 * Description
 *  Class to create a 2D plane with normal pointing along positive Z-direction.
 */

#ifndef __PLANE_H__
#define __PLANE_H__

#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>
#include <QtMath>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include <AuxiliaryMethods/CheckErrors.h>
#include <AuxiliaryMethods/HelperFunctions.h>
#include <ViewerCore/Object.h>
#include <ViewerCore/PlanarMesh.h>
#include <ViewerCore/ShadersList.h>

// TODO: Redo this class by making GenericObject as basic class
class Plane : protected Object, public PlanarMesh
{
public:
	Plane();
	~Plane();

	bool LocalInitialize(
		QVector2D &range_min,
		QVector2D &range_max,
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
	void AddLight(Light &light_);

	// Set object shininess parameter
	void SetShininess(GLfloat &shininess_);

	void show_wire_frame(bool swf_);
	void show_normals(bool sn_);

	void include_ambience(bool ia_);
	void include_diffuse(bool id_);
	void include_specular(bool is_);

	// Delete all created vectors, associated shaders
	void TakeDown();

private:
	float x_min, x_max, y_min, y_max;
	GLuint slices, stacks;

	QVector3D vertex_global_color;

	bool show_normals_;
	
	bool include_ambience_;
	bool include_diffuse_;
	bool include_specular_;

	// Compute and store triangle normals
	void ComputeTriangleNormals();

	// Compute vertex normals as average of neighboring triangles normal vectors.
	void ComputeVertexNormals();

	// Returns all adjacent triangle indices so that for any vertex location
	// we can know triangle indices that should be used for normal calculation purposes.
	QVector<GLuint> AdjacentTriangleIndices(GLuint w, GLuint h);

	// Build vertices that act as end points to normal vectors.
	void BuildNormalVisualizationGeometry();

	ShadersList shaders_list;
	bool is_shader_for_normals_initialized;

	typedef Object super;
};

#endif // END OF __PLANE_H__
