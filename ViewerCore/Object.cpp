/*
 * Copyright (c) 2014, Lakshman Anumolu
 * All rights reserved.
 *
 * This file is part of Basic Model Viewer whose distribution is governed by
 * the BSD-style license contained in the accompanying file license.txt.
 */

#include "Object.h"

Object::Object()
	: max_lights(32),
	num_lights(0),
	shininess(100)
{
}

Object::~Object()
{
	assert(this->vertex_array_handle == GLuint(-1));
	assert(this->vertex_data_handle == GLuint(-1));
	assert(this->normal_array_handle == GLuint(-1));
	assert(this->normal_data_handle == GLuint(-1));
}

bool Object::Initialize()
{
	this->InternalInitialize();
	return true;
}

bool Object::PostGLInitialize()
{
	
	glGenVertexArrays(1, &this->vertex_array_handle);
	glBindVertexArray(this->vertex_array_handle);

	glGenBuffers(1, &this->vertex_data_handle);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertex_data_handle);
	//glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(VertexData), &this->vertices[0], GL_DYNAMIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(VertexAttributesPCNT), &this->vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &this->vertex_indices_handle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vertex_indices_handle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->vertex_indices.size() * sizeof(GLuint), &this->vertex_indices[0], GL_STATIC_DRAW);

	glGenBuffers(1, & this->vertex_indices_wire_frame_handle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vertex_indices_wire_frame_handle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->vertex_indices_wire_frame.size() * sizeof(GLuint), &this->vertex_indices_wire_frame[0], GL_STATIC_DRAW);

	/* TODO: Currently normal handles are explicitly initialized in each object, bring them back here */
	/*
	glGenVertexArrays(1, &this->normal_array_handle);
	glBindVertexArray(this->normal_array_handle);

	glGenBuffers(1, &this->normal_data_handle);
	glBindBuffer(GL_ARRAY_BUFFER, this->normal_data_handle);
	glBufferData(GL_ARRAY_BUFFER, this->normal_vertices.size() * sizeof(VertexAttributesP), &this->normal_vertices[0], GL_DYNAMIC_DRAW);

	glGenBuffers(1, &this->normal_indices_handle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->normal_indices_handle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->normal_indices.size() * sizeof(GLuint), &this->normal_indices[0], GL_DYNAMIC_DRAW);
	*/

	return !GLReturnedError("test_object::PostGLInitialize - on exit");
}

void Object::AppendLight(Light light_details_)
{
	if (this->num_lights < this->max_lights) {
		this->light_details.push_back(light_details_);
		this->num_lights++;
	}
}

void Object::TakeDown()
{
	if (this->vertex_array_handle != GLuint(-1))
		glDeleteVertexArrays(1, &this->vertex_array_handle);
	
	if (this->vertex_data_handle != GLuint(-1))
		glDeleteBuffers(1, &this->vertex_data_handle);
	
	if (this->vertex_indices_handle != GLuint(-1))
		glDeleteBuffers(1, &this->vertex_indices_handle);
	
	if (this->vertex_indices_wire_frame_handle != GLuint(-1))
		glDeleteBuffers(1, &this->vertex_indices_wire_frame_handle);

	vertices.clear();
	vertex_indices.clear();
	vertex_indices_wire_frame.clear();

	/* TODO: Enable normals too */
	/*
	if (this->normal_array_handle != GLuint(-1))
		glDeleteVertexArrays(1, &this->normal_array_handle);

	if (this->normal_data_handle != GLuint(-1))
		glDeleteBuffers(1, &this->normal_data_handle);

	if (this->normal_indices_handle != GLuint(-1))
		glDeleteBuffers(1, &this->normal_indices_handle);
	*/

	normal_triangles.clear();
	normal_vertices.clear();
	normal_indices.clear();

	light_details.clear();
	
	this->InternalInitialize();
}

void Object::InternalInitialize()
{
	this->vertex_array_handle = GLuint(-1);
	this->vertex_data_handle = GLuint(-1);
	this->vertex_indices_handle = GLuint(-1);
	this->vertex_indices_wire_frame_handle = GLuint(-1);

	/* TODO: Enable normals too */
	this->normal_array_handle = this->normal_data_handle = this->normal_indices_handle = GLuint(-1);
	
}
