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
 *  Base class for any shape.
 */

#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <assert.h>

#include <QVector>
#include <QOpenGLFunctions>

#include <AuxiliaryMethods/CheckErrors.h>
#include <AuxiliaryMethods/HelperFunctions.h>
#include <ViewerCore/VertexAttributes.h>

class Object
{
public:
	Object();
	~Object();

	/* Initialize the handles to -1, so that all handles start with
	 bad values, which helps during the destructor call */
	virtual bool Initialize();

	/* Generate vertex arrays, vertex buffers and bind data */
	virtual bool PostGLInitialize();

	/* Add light details */
	void AppendLight(Light light_details_);

	inline void wire_frame_mode(bool swf) {
		this->is_wire_frame = swf;
	}

	// Delete all created vectors
	virtual void TakeDown();

protected:
	GLuint vertex_array_handle;
	GLuint vertex_data_handle;
	GLuint vertex_indices_handle;
	GLuint vertex_indices_wire_frame_handle;

	//QVector<VertexData> vertices;
	QVector<VertexAttributesPCNT> vertices;
	QVector<GLuint> vertex_indices;
	QVector<GLuint> vertex_indices_wire_frame;

	bool is_wire_frame;

	QVector<VertexAttributesP> normal_triangles;

	GLuint normal_array_handle;
	GLuint normal_data_handle;
	GLuint normal_indices_handle;

	QVector<VertexAttributesP> normal_vertices;
	QVector<GLuint> normal_indices;

	// Light details
	GLuint max_lights;
	GLuint num_lights;
	GLfloat shininess;
	QVector<Light> light_details;

private:
	void InternalInitialize();
};

#endif // END OF __OBJECT_H__
