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
 *	Class to create sphere by stiching a PlanarMesh.
 *	TODO: This class needs a major rework and is currently not recommended
 *	to use.
 */

#ifndef __SPHERE_H__
#define __SPHERE_H__

#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>
#include <QtMath>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include <ViewerCore/Object.h>
#include <ViewerCore/PlanarMesh.h>
#include <AuxiliaryMethods/CheckErrors.h>
#include <AuxiliaryMethods/HelperFunctions.h>

#include <ViewerCore/ShadersList.h>

class Sphere : protected Object, public PlanarMesh
{
public:
	Sphere();
	~Sphere();

	bool LocalInitialize(GLuint slices_ = 10, GLuint stacks_ = 10);
	bool Draw(const QMatrix4x4 &projection, QMatrix4x4 modelview, QOpenGLShaderProgram *shader_);

	void show_wire_frame(bool swf_);

	// Delete all created vectors, associated shaders
	void TakeDown();

private:
	ShadersList shaders_list;
	GLuint slices, stacks;
	float radius;

	// Compute and store triangle normals
	void ComputeTriangleNormals();

	// Compute vertex normals as average of neighboring triangles normal vectors.
	void ComputeVertexNormals();

	// Returns all adjacent triangle indices so that for any vertex location
	// we can know triangle indices that should be used for normal calculation purposes.
	QVector<GLuint> AdjacentTriangleIndices(GLuint w, GLuint h);

	// Build vertices that act as end points to normal vectors.
	void BuildNormalVisualizationGeometry();

	typedef Object super;
};

#endif // END OF __SPHERE_H__
