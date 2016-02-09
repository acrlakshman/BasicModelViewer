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
 */

#include "GenericObject.h"

GenericObject::GenericObject()
	: vertex_positions_available(false),
	vertex_normals_available(false),
	vertex_texture_coordinates_available(false),
	vertex_indices_available(false),
	vertex_color_available(false),
	num_vertices(0),
	num_vertex_indices(0),
	show_normals_(false),
	include_ambience_(true),
	include_diffuse_(true),
	include_specular_(true),
	is_shader_for_normals_initialized(false),
	vertex_global_color(QVector3D(0.0, 0.9, 0.0)),
        vertex_positions_global_offset(QVector3D(0.0, 0.0, 0.0))
{
	this->wire_frame_mode(false);
}

GenericObject::~GenericObject()
{
}

bool GenericObject::LocalInitialize()
{
	if (!is_shader_for_normals_initialized) {
		this->shaders_list.InitializeShader("../Shaders/solid_shader.vert", "../Shaders/solid_shader.frag");
		is_shader_for_normals_initialized = true;
	}

	if (!this->vertex_positions_available || !this->vertex_indices_available)
		return false;

	if (!this->vertex_color_available)
		this->InitializeVerticesColorToGlobal();

	if (!this->vertex_normals_available) {
		this->ComputeVertexNormals();
		this->vertex_normals_available = true;
	}

	if (!this->Initialize())
		return false;

	/*for (GLuint i = 0; i < vertices.size(); ++i) {
		qDebug() << "vertices[" << i << "].position = " << vertices[i].position;
		qDebug() << "vertices[" << i << "].normal = " << vertices[i].normal;
		qDebug() << "vertices[" << i << "].texture_coordinate = " << vertices[i].texture_coordinate;
	}*/

	// Build normal vectors
	this->BuildNormalVisualizationGeometry();

	if (!this->PostGLInitialize())
		return false;

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCNT), 0);  //Position

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCNT), (GLvoid *) (sizeof(QVector3D) * 1)); // Color

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCNT), (GLvoid *) (sizeof(QVector3D) * 2)); // Normals

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCNT), (GLvoid *) (sizeof(QVector3D) * 3)); // Tex coordinates

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	/* TODO: This is explicitly done here, instead of keeping this in Object.cpp */
	glGenVertexArrays(1, &this->normal_array_handle);
	glBindVertexArray(this->normal_array_handle);

	glGenBuffers(1, &this->normal_data_handle);
	glBindBuffer(GL_ARRAY_BUFFER, this->normal_data_handle);
	glBufferData(GL_ARRAY_BUFFER, this->normal_vertices.size() * sizeof(VertexAttributesP), &this->normal_vertices[0], GL_DYNAMIC_DRAW);

	glGenBuffers(1, &this->normal_indices_handle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->normal_indices_handle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->normal_indices.size() * sizeof(GLuint), &this->normal_indices[0], GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesP), 0);  //Position

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return !GLReturnedError("GenericObject::LocalInitialize - on exit");
}

bool GenericObject::InitializeVertexPositionsAndIndices(
	QVector<QVector3D> &vertex_positions_,
	QVector<GLuint> &vertex_indices_
	)
{
	this->num_vertices = vertex_positions_.size();
	this->vertices.resize(this->num_vertices);

	float x_min, x_max, y_min, y_max, z_min, z_max;
	x_min = x_max = y_min = y_max = z_min = z_max = 0.0;

	for (GLuint i = 0; i < this->num_vertices; ++i) {
		this->vertices[i].position = vertex_positions_[i] +
                                            vertex_positions_global_offset;

		x_min = std::min(x_min, this->vertices[i].position.x());
		x_max = std::max(x_max, this->vertices[i].position.x());
		y_min = std::min(y_min, this->vertices[i].position.y());
		y_max = std::max(y_max, this->vertices[i].position.y());
		z_min = std::min(z_min, this->vertices[i].position.z());
		z_max = std::max(z_max, this->vertices[i].position.z());
	}
	b_box_.x_range = QVector2D(x_min, x_max);
	b_box_.y_range = QVector2D(y_min, y_max);
	b_box_.z_range = QVector2D(z_min, z_max);
	this->SetBoundingBox(&this->b_box_);

	this->num_vertex_indices = vertex_indices_.size();
	this->vertex_indices.resize(this->num_vertex_indices);

	for (GLuint i = 0; i < this->num_vertex_indices; ++i) {
		this->vertex_indices[i] = vertex_indices_[i];
	}

	this->vertex_positions_available = true;
	this->vertex_indices_available = true;

	/* TODO: replace push_back by preallcoating memory */
	//this->vertex_indices_wire_frame.resize(this->num_vertex_indices * 2, 0);
	for (GLuint i = 0; i < this->num_vertex_indices;) {
		this->vertex_indices_wire_frame.push_back(this->vertex_indices[i]);
		this->vertex_indices_wire_frame.push_back(this->vertex_indices[i+1]);

		this->vertex_indices_wire_frame.push_back(this->vertex_indices[i]);
		this->vertex_indices_wire_frame.push_back(this->vertex_indices[i+2]);

		this->vertex_indices_wire_frame.push_back(this->vertex_indices[i+1]);
		this->vertex_indices_wire_frame.push_back(this->vertex_indices[i+2]);

		i += 3;
	}
	//qDebug() << "this->vertex_indices_wire_frame.size() = " << this->vertex_indices_wire_frame.size();

	return true;
}

bool GenericObject::InitializeVertexNormals(
	QVector<QVector3D> &vertex_normals_
	)
{
	/* Return if either vertex positions are not updated before visiting here
	   or size of vertex_normals_ is not equal to size of vertex positions */
	if (!this->vertex_positions_available || (vertex_normals_.size() != this->num_vertices))
		return false;

	for (GLuint i = 0; i < this->num_vertices; ++i) {
		this->vertices[i].normal = vertex_normals_[i];
	}

	this->vertex_normals_available = true;

	return true;
}

bool GenericObject::InitializeVertexTextureCoordinates(
	QVector<QVector2D> &vertex_texture_coordinates_
	)
{
	/* Return if either vertex positions are not updated before visiting here
	   or size of vertex_texture_coordinates_ is not equal to size of vertex positions */
	if (!this->vertex_positions_available || (vertex_texture_coordinates_.size() != this->num_vertices))
		return false;

	for (GLuint i = 0; i < this->num_vertices; ++i) {
		this->vertices[i].texture_coordinate = vertex_texture_coordinates_[i];
	}

	this->vertex_texture_coordinates_available = true;

	return true;
}

void GenericObject::SetVertexColor(QVector3D &color_)
{
	this->vertex_global_color = color_;
}

bool GenericObject::InitializeVertexColor(
	QVector<QVector3D> &vertex_color_
	)
{
	if (!this->vertex_positions_available || (vertex_color_.size() != this->num_vertices))
		return false;

	for (GLuint i = 0; i < this->num_vertices; ++i) {
		this->vertices[i].color = vertex_color_[i];
	}

	this->vertex_color_available = true;

	return true;
}

bool GenericObject::Draw(
	const QMatrix4x4 &projection,
	const QMatrix4x4 &modelview_cam,
	QMatrix4x4 modelview,
	QOpenGLShaderProgram *shader_
	)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	QMatrix3x3 nm = QMatrix3x3();
	bool invertible_ = Inverse_Transpose_Mat4ToMat3(modelview, nm);

	shader_->bind();
	shader_->setUniformValue("projection_matrix", projection);
	shader_->setUniformValue("modelview_matrix", modelview);
	shader_->setUniformValue("mvp_matrix", projection * modelview);
	shader_->setUniformValue("normal_matrix", nm);
	shader_->setUniformValue("modelview_cam", modelview_cam);
	shader_->setUniformValue("texture_t", 0);

	const char num_lights_char[] = "num_lights";
	shader_->setUniformValue(num_lights_char, this->num_lights);

	for (GLuint i = 0; i < this->num_lights; ++i) {
		QString light_pos_char("light[");
		light_pos_char.append(QString::number(i));
		light_pos_char.append("].position");

		shader_->setUniformValue(light_pos_char.toUtf8().constData(), this->light_details[i].position);

		QString light_col_amb_char("light[");
		light_col_amb_char.append(QString::number(i));
		light_col_amb_char.append("].color_ambient");

		shader_->setUniformValue(light_col_amb_char.toUtf8().constData(), this->light_details[i].color_ambient);

		QString light_col_diff_char("light[");
		light_col_diff_char.append(QString::number(i));
		light_col_diff_char.append("].color_diffuse");

		shader_->setUniformValue(light_col_diff_char.toUtf8().constData(), this->light_details[i].color_diffuse);

		QString light_col_spec_char("light[");
		light_col_spec_char.append(QString::number(i));
		light_col_spec_char.append("].color_specular");

		shader_->setUniformValue(light_col_spec_char.toUtf8().constData(), this->light_details[i].color_specular);
	}

	shader_->setUniformValue("shininess", shininess);

	shader_->setUniformValue("include_ambience", include_ambience_);
	shader_->setUniformValue("include_diffuse", include_diffuse_);
	shader_->setUniformValue("include_specular", include_specular_);

	glBindVertexArray(this->vertex_array_handle);

	glBindBuffer(GL_ARRAY_BUFFER, this->vertex_data_handle);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(VertexAttributesPCNT), &this->vertices[0], GL_STATIC_DRAW);

	if (!is_wire_frame) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vertex_indices_handle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->vertex_indices.size() * sizeof(GLuint), &this->vertex_indices[0], GL_STATIC_DRAW);

		glDrawElements(GL_TRIANGLES, this->vertex_indices.size(), GL_UNSIGNED_INT, (void*) 0);
	} else {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_indices_wire_frame_handle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertex_indices_wire_frame.size() * sizeof(GLuint), &vertex_indices_wire_frame[0], GL_STATIC_DRAW);

		glDrawElements(GL_LINES, this->vertex_indices_wire_frame.size(), GL_UNSIGNED_INT, (void*) 0);
	}

	GLReturnedError("GenericObject::Draw - after draw");
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	shader_->release();

	if (this->show_normals_)
	{
		shaders_list.shaders[0]->bind();
		//shaders_list.shaders[0]->setUniformValue("projection_matrix", projection);
		//shaders_list.shaders[0]->setUniformValue("modelview_matrix", modelview);
		shaders_list.shaders[0]->setUniformValue("mvp_matrix", projection * modelview);
		//shaders_list.shaders[0]->setUniformValue("normal_matrix", nm);
		shaders_list.shaders[0]->setUniformValue("texture_t", 0);

		glBindVertexArray(this->normal_array_handle);

		glBindBuffer(GL_ARRAY_BUFFER, this->normal_data_handle);
		glBufferData(GL_ARRAY_BUFFER, this->normal_vertices.size() * sizeof(VertexAttributesP), &this->normal_vertices[0], GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->normal_indices_handle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->normal_indices.size() * sizeof(GLuint), &this->normal_indices[0], GL_DYNAMIC_DRAW);

		glDrawElements(GL_LINES , this->normal_indices.size(), GL_UNSIGNED_INT , (void*) 0);
		glBindVertexArray(0);

		shaders_list.shaders[0]->release();
	}

	return !GLReturnedError("GenericObject::Draw - on exit");
}

void GenericObject::ComputeTriangleNormals()
{
	/* This is not needed for a GenericObject */
}

void GenericObject::ComputeVertexNormals()
{
	QVector<GLuint> normal_counter_(this->num_vertices, 0);

	for (GLuint i = 0; i < this->num_vertex_indices; ++i) {
		//qDebug() << i;
		GLuint vertex_1 = this->vertex_indices[i];
		GLuint vertex_2 = this->vertex_indices[++i];
		GLuint vertex_3 = this->vertex_indices[++i];

		if (QVector3D::crossProduct(this->vertices[vertex_2].position - this->vertices[vertex_1].position,
									this->vertices[vertex_3].position - this->vertices[vertex_1].position) ==
									QVector3D(0.0, 0.0, 0.0)) {
			this->vertices[vertex_1].normal += QVector3D(0.0, 0.0, 0.0);
			this->vertices[vertex_2].normal += QVector3D(0.0, 0.0, 0.0);
			this->vertices[vertex_3].normal += QVector3D(0.0, 0.0, 0.0);
		} else {
			QVector3D tmp_ = QVector3D::crossProduct(this->vertices[vertex_2].position - this->vertices[vertex_1].position,
													this->vertices[vertex_3].position - this->vertices[vertex_1].position);
			this->vertices[vertex_1].normal += tmp_.normalized();
			this->vertices[vertex_2].normal += tmp_.normalized();
			this->vertices[vertex_3].normal += tmp_.normalized();

			normal_counter_[vertex_1]++;
			normal_counter_[vertex_2]++;
			normal_counter_[vertex_3]++;
		}
	}

	//qDebug() << "normal_counter_.size() = " << normal_counter_.size();
	//qDebug() << "this->num_vertices = " << this->num_vertices;
	//qDebug() << "this->vertices.size() = " << this->vertices.size();

	for (GLuint i = 0; i < this->num_vertices; ++i) {
		if (normal_counter_[i] > 0) {
			this->vertices[i].normal /= normal_counter_[i];
		}
	}
}

QVector<GLuint> GenericObject::AdjacentTriangleIndices(GLuint w, GLuint h)
{
	return QVector<GLuint>(0);
}

void GenericObject::BuildNormalVisualizationGeometry()
{
	this->normal_vertices.clear();
	this->normal_indices.clear();
	const float normal_scalar = 0.99f;
	for (GLuint j = 0; j < (GLuint)this->vertices.size(); ++j)
	{
		this->normal_vertices.push_back(VertexAttributesP(this->vertices[j].position));
		this->normal_vertices.push_back(VertexAttributesP(this->vertices[j].position + this->vertices[j].normal * normal_scalar));
		this->normal_indices.push_back(this->normal_vertices.size() - 2);
		this->normal_indices.push_back(this->normal_vertices.size() - 1);
	}
}

void GenericObject::InitializeVerticesColorToGlobal()
{
	for (GLuint i = 0; i < this->num_vertices; ++i) {
		this->vertices[i].color = this->vertex_global_color;
	}
}

void GenericObject::AddLight(Light &light_)
{
	this->AppendLight(light_);
}

GLuint GenericObject::GetNumberOfLights()
{
	return this->num_lights;
}

void GenericObject::EditLight(Light &light_details_, GLuint idx_)
{
	if (!this->num_lights) {
		this->AddLight(light_details_);
	} else {
		if (idx_ > this->num_lights-1)
			this->AddLight(light_details_);
		else if (idx_ < 0)
			this->light_details.replace(0, light_details_);
		else
			this->light_details.replace(idx_, light_details_);
	}
}

void GenericObject::SetShininess(GLfloat &shininess_)
{
	this->shininess = shininess_;
}

void GenericObject::SetVertexPositionsGlobalOffset(const QVector3D
                                                   position_offset)
{
  this->vertex_positions_global_offset = position_offset;
}

void GenericObject::show_wire_frame(bool swf_)
{
	this->wire_frame_mode(swf_);
}

void GenericObject::show_normals(bool sn_)
{
	this->show_normals_ = sn_;
}

void GenericObject::include_ambience(bool ia_)
{
	this->include_ambience_ = ia_;
}

void GenericObject::include_diffuse(bool id_)
{
	this->include_diffuse_ = id_;
}

void GenericObject::include_specular(bool is_)
{
	this->include_specular_ = is_;
}

// Delete all created vectors, associated shaders
void GenericObject::TakeDown()
{
	if (this->normal_array_handle != GLuint(-1))
		glDeleteVertexArrays(1, &this->normal_array_handle);

	if (this->normal_data_handle != GLuint(-1))
		glDeleteBuffers(1, &this->normal_data_handle);

	if (this->normal_indices_handle != GLuint(-1))
		glDeleteBuffers(1, &this->normal_indices_handle);

	this->normal_vertices.clear();
	this->normal_indices.clear();

	super::TakeDown();
	shaders_list.TakeDown();
}
