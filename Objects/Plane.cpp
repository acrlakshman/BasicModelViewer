/*
 * Copyright (c) 2014, Lakshman Anumolu
 * All rights reserved.
 *
 * This file is part of Basic Model Viewer whose distribution is governed by
 * the BSD-style license contained in the accompanying file license.txt.
 */

#include "Plane.h"

Plane::Plane()
	: slices(0),
	stacks(0),
	show_normals_(false),
	include_ambience_(true),
	include_diffuse_(true),
	include_specular_(true),
	is_shader_for_normals_initialized(false),
	vertex_global_color(QVector3D(0.0, 0.2, 0.0))
{
	this->wire_frame_mode(false);
}

Plane::~Plane()
{
}

bool Plane::LocalInitialize(
	QVector2D &range_min,
	QVector2D &range_max,
	GLuint slices_,
	GLuint stacks_
	)
{
	if (!is_shader_for_normals_initialized) {
		shaders_list.InitializeShader("../Shaders/solid_shader.vert", "../Shaders/solid_shader.frag");
		is_shader_for_normals_initialized = true;
	}

	this->x_min = range_min.x();
	this->y_min = range_min.y();
	this->x_max = range_max.x();
	this->y_max = range_max.y();

	this->slices = slices_;
	this->stacks = stacks_;

	if (!this->Initialize())
		return false;

	this->InitializePlanarMesh(
			this->slices,
			this->stacks,
			this->vertex_indices,
			this->vertex_indices_wire_frame,
			QString("CounterClockWise")
			);

	GLuint W = this->slices;
	GLuint H = this->stacks;
	
	vertices.resize( W * H );
	GLuint k = -1;
	float x_p = x_min, y_p = y_min;
	float dx = (x_max - x_min) / (W-1);
	float dy = (y_max - y_min) / (H-1);
	for (GLuint j = 0; j < H; ++j) {
		x_p = x_min;
		for (GLuint i = 0; i < W; ++i) {
			vertices[++k].position = QVector3D(x_p, y_p, 0);
			vertices[k].color = this->vertex_global_color;
			vertices[k].texture_coordinate = QVector2D(float(i) / (W-1), float(j) / (H-1));
			x_p += dx;
		}
		y_p += dy;
	}

	// Compute triangle normals
	this->ComputeTriangleNormals();

	// Compute normals at vertices
	this->ComputeVertexNormals();

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

	// ---
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

	return !GLReturnedError("plane::LocalInitialize - on exit");
}

void Plane::SetVertexColor(QVector3D &color_)
{
	this->vertex_global_color = color_;
}

bool Plane::Draw(
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

	GLReturnedError("Geometry::Draw - after draw");
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

    return !GLReturnedError("plane::Draw - on exit");
}

void Plane::ComputeTriangleNormals()
{
	// Resize the vector
	GLuint W = this->slices;
	GLuint H = this->stacks;

	GLuint Num_Triangles = (W-1) * (H-1) * 2;

	normal_triangles.clear();
	normal_triangles.resize( Num_Triangles );

	GLuint i = -1;
	for (GLuint h = 1; h < H; ++h) {
		for (GLuint w = 1; w < W; ++w) {
			// Upper triangle
			GLuint vertex_1 = this->GetIndex( w, h );
			GLuint vertex_2 = this->GetIndex( w+1, h+1 );
			GLuint vertex_3 = this->GetIndex( w, h+1 );

			if (QVector3D::crossProduct(this->vertices[vertex_2].position - this->vertices[vertex_1].position,
										this->vertices[vertex_3].position - this->vertices[vertex_1].position) ==
										QVector3D(0.0f, 0.0f, 0.0f)) {
				this->normal_triangles[++i].position = QVector3D(0.0f, 0.0f, 0.0f);
			} else {
				QVector3D tmp_ = QVector3D::crossProduct(this->vertices[vertex_2].position - this->vertices[vertex_1].position,
														 this->vertices[vertex_3].position - this->vertices[vertex_1].position);
				this->normal_triangles[++i].position = tmp_.normalized();
			}

			// Lower triangle
			vertex_2 = this->GetIndex( w+1, h );
			vertex_3 = this->GetIndex( w+1, h+1 );

			if (QVector3D::crossProduct(this->vertices[vertex_2].position - this->vertices[vertex_1].position,
										this->vertices[vertex_3].position - this->vertices[vertex_1].position) ==
										QVector3D(0.0f, 0.0f, 0.0f)) {
				this->normal_triangles[++i].position = QVector3D(0.0f, 0.0f, 0.0f);
			} else {
				QVector3D tmp_ = QVector3D::crossProduct(this->vertices[vertex_2].position - this->vertices[vertex_1].position,
														 this->vertices[vertex_3].position - this->vertices[vertex_1].position);
				this->normal_triangles[++i].position = tmp_.normalized();
			}

		}
	}

}

void Plane::ComputeVertexNormals()
{

	QVector<GLuint> Adjacent_Indices;
	GLuint index;
	GLuint W = this->slices;
	GLuint H = this->stacks;

	for (GLuint h = 1; h <= H; ++h) {
		for (GLuint w = 1; w <= W; ++w) {
			index = this->GetIndex( w, h );
			Adjacent_Indices.clear();
			Adjacent_Indices = this->AdjacentTriangleIndices( w, h );

			for (GLuint i = 0; i < (GLuint)Adjacent_Indices.size(); ++i)
				this->vertices[index].normal = QVector3D(0, 0, 0);

			for (GLuint i = 0; i < (GLuint)Adjacent_Indices.size(); ++i) {
				GLuint idx_ = Adjacent_Indices[i];

				float x_val_ = this->vertices[index].normal.x();
				x_val_ += this->normal_triangles[idx_].position.x();

				this->vertices[index].normal.setX(x_val_);

				float y_val_ = this->vertices[index].normal.y();
				y_val_ += this->normal_triangles[idx_].position.y();

				this->vertices[index].normal.setY(y_val_);

				float z_val_ = this->vertices[index].normal.z();
				z_val_ += this->normal_triangles[idx_].position.z();

				this->vertices[index].normal.setZ(z_val_);

				/*this->vertices[index].normal = QVector3D( this->normal_triangles[Adjacent_Indices[i]].position.x() + this->vertices[index].normal.x(),
														  this->normal_triangles[Adjacent_Indices[i]].position.y() + this->vertices[index].normal.y(),
														  this->normal_triangles[Adjacent_Indices[i]].position.z() + this->vertices[index].normal.z()
														  );*/
			}
			GLuint div = Adjacent_Indices.size();

			this->vertices[index].normal.setX(this->vertices[index].normal.x() / div);
			this->vertices[index].normal.setY(this->vertices[index].normal.y() / div);
			this->vertices[index].normal.setZ(this->vertices[index].normal.z() / div);

			this->vertices[index].normal.normalize();
		}
	}

}

QVector<GLuint> Plane::AdjacentTriangleIndices(GLuint w, GLuint h)
{
	QVector<GLuint> Adjacent_Indices;
	GLuint index;

	GLuint W = this->slices;
	GLuint H = this->stacks;
	if (w > 1 && h > 1 && w < W && h < H) {

		Adjacent_Indices.resize( 6 );

		index = this->GetCellIndex( w, h );
		Adjacent_Indices[0] = 2*index; // Upper triangle
		Adjacent_Indices[1] = 2*index + 1; // Lower triangle

		index = this->GetCellIndex( w, h-1 );
		Adjacent_Indices[2] = 2*index;

		index = this->GetCellIndex( w-1, h-1 );
		Adjacent_Indices[3] = 2*index;
		Adjacent_Indices[4] = 2*index + 1;

		index = this->GetCellIndex( w-1, h );
		Adjacent_Indices[5] = 2*index + 1;

	} else if (w == 1 && h == 1) {

		Adjacent_Indices.resize(2);

		index = this->GetCellIndex(w, h);
		Adjacent_Indices[0] = 2*index;
		Adjacent_Indices[1] = 2*index + 1;

	} else if (w == W && h == 1) {

		Adjacent_Indices.resize(1);

		index = this->GetCellIndex(w-1, h);
		Adjacent_Indices[0] = 2*index + 1;

	} else if (w == 1 && h == H) {

		Adjacent_Indices.resize(1);

		index = this->GetCellIndex(w, h-1);
		Adjacent_Indices[0] = 2*index;

	} else if (w == W && h == H) {

		Adjacent_Indices.resize(2);

		index = this->GetCellIndex(w-1, h-1);
		Adjacent_Indices[0] = 2*index;
		Adjacent_Indices[1] = 2*index + 1;

	} else if ((w > 1 && w < W) && h == 1) {

		Adjacent_Indices.resize(3);

		index = this->GetCellIndex(w, h);
		Adjacent_Indices[0] = 2*index;
		Adjacent_Indices[1] = 2*index + 1;

		index = this->GetCellIndex(w-1, h);
		Adjacent_Indices[2] = 2*index + 1;

	} else if (w == 1 && (h > 1 && h < H)) {

		Adjacent_Indices.resize(3);

		index = this->GetCellIndex(w, h);
		Adjacent_Indices[0] = 2*index;
		Adjacent_Indices[1] = 2*index + 1;

		index = this->GetCellIndex(w, h-1);
		Adjacent_Indices[2] = 2*index;

	} else if (w == W && (h > 1 && h < H)) {

		Adjacent_Indices.resize(3);

		index = this->GetCellIndex(w-1, h-1);
		Adjacent_Indices[0] = 2*index;
		Adjacent_Indices[1] = 2*index + 1;

		index = this->GetCellIndex(w-1, h);
		Adjacent_Indices[2] = 2*index + 1;

	} else if ((w > 1 && w < W) && h == H) {

		Adjacent_Indices.resize(3);

		index = this->GetCellIndex(w-1, h-1);
		Adjacent_Indices[0] = 2*index;
		Adjacent_Indices[1] = 2*index + 1;

		index = this->GetCellIndex(w, h-1);
		Adjacent_Indices[2] = 2*index;

	}

	return Adjacent_Indices;
}

void Plane::BuildNormalVisualizationGeometry()
{
	this->normal_vertices.clear();
	this->normal_indices.clear();
	const float normal_scalar = 1.f;
	for (GLuint j = 0; j < (GLuint)this->vertices.size(); ++j)
	{
		this->normal_vertices.push_back(VertexAttributesP(this->vertices[j].position));
		this->normal_vertices.push_back(VertexAttributesP(this->vertices[j].position + this->vertices[j].normal * normal_scalar));
		this->normal_indices.push_back(this->normal_vertices.size() - 2);
		this->normal_indices.push_back(this->normal_vertices.size() - 1);
	}
}

void Plane::AddLight(Light &light_)
{
	this->AppendLight(light_);
}

GLuint Plane::GetNumberOfLights()
{
	return this->num_lights;
}

void Plane::EditLight(Light &light_details_, GLuint idx_)
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

void Plane::SetShininess(GLfloat &shininess_)
{
	this->shininess = shininess_;
}

void Plane::show_wire_frame(bool swf_)
{
	this->wire_frame_mode(swf_);
}

void Plane::show_normals(bool sn_)
{
	this->show_normals_ = sn_;
}

void Plane::include_ambience(bool ia_)
{
	this->include_ambience_ = ia_;
}

void Plane::include_diffuse(bool id_)
{
	this->include_diffuse_ = id_;
}

void Plane::include_specular(bool is_)
{
	this->include_specular_ = is_;
}

// Delete all created vectors, associated shaders
void Plane::TakeDown()
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
