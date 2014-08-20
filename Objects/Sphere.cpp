/*
 * Copyright (c) 2014, Lakshman Anumolu
 * All rights reserved.
 *
 * This file is part of Basic Model Viewer whose distribution is governed by
 * the BSD 2-Clause License contained in the accompanying LICENSE.txt file.
 */

#include "Sphere.h"

Sphere::Sphere()
	: slices(0),
	  stacks(0)
{
	this->wire_frame_mode(false);
}

Sphere::~Sphere()
{
}

bool Sphere::LocalInitialize(GLuint slices_, GLuint stacks_)
{
	shaders_list.InitializeDefaultShaders();
	this->slices = slices_;
	this->stacks = stacks_;
	this->radius = 0.5;

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
	float PI = 3.1415;

	float x_o = 0.0; // center.x();
	float y_o = 0.0; // center.y();
	float z_o = 0.0; // center.z();
	float r_s = this->radius;

	vertices.resize( W * H );
	GLuint k = -1;
	for (GLuint j = 0; j < H; ++j) {
		for (GLuint i = 0; i < W; ++i) {
			float h_j = float(j) * (2*r_s) / float( H-1 );
			float r_j = float( (r_s) * qSin( qAcos( 1. - h_j/r_s ) ) );
			if (h_j == r_s) r_j = 0.f;

			float x_p = x_o + r_j * qCos( 2*PI*i/(W-1) );
			float y_p = y_o + ( j * (2*r_s) / (H-1) ) - (r_s);
			float z_p = z_o + r_j * qSin( 2*PI*i/(W-1) );

			vertices[++k].position = QVector3D( x_p, y_p, z_p );
			//qDebug() << "vertices[" << k << "].position = " << vertices[k].position;
			QVector3D color_ = QVector3D(0.0f, 1.0f, 0.0f);
			
			vertices[k].color = color_;

			vertices[k].texture_coordinate = QVector2D( float(i) / (W - 1), 1.-float(j) / (H-1) );
		}
		vertices[k].position = vertices[k-(W-1)].position;
		//qDebug() << "vertices[" << k << "].position = " << vertices[k].position;
	}

	// Compute triangle normals
	this->ComputeTriangleNormals();

	// Compute normals at vertices
	this->ComputeVertexNormals();

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

	return true;
}

bool Sphere::Draw(const QMatrix4x4 &projection, QMatrix4x4 modelview, QOpenGLShaderProgram *shader_)
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
	shader_->setUniformValue("texture_t", 0);

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

	if (true)
	{
		shaders_list.default_shaders[0]->bind();
		//shaders_list.default_shaders[2]->setUniformValue("projection_matrix", projection);
		//shaders_list.default_shaders[2]->setUniformValue("modelview_matrix", modelview);
		shaders_list.default_shaders[0]->setUniformValue("mvp_matrix", projection * modelview);
		//shaders_list.default_shaders[2]->setUniformValue("normal_matrix", nm);
		shaders_list.default_shaders[0]->setUniformValue("texture_t", 0);

		glBindVertexArray(this->normal_array_handle);

		glBindBuffer(GL_ARRAY_BUFFER, this->normal_data_handle);
		glBufferData(GL_ARRAY_BUFFER, this->normal_vertices.size() * sizeof(VertexAttributesP), &this->normal_vertices[0], GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->normal_indices_handle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->normal_indices.size() * sizeof(GLuint), &this->normal_indices[0], GL_DYNAMIC_DRAW);

		glDrawElements(GL_LINES , this->normal_indices.size(), GL_UNSIGNED_INT , (void*) 0);
		glBindVertexArray(0);

		shaders_list.default_shaders[0]->release();
	}

	return true;
}

void Sphere::ComputeTriangleNormals()
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

			/*if (QVector3D::crossProduct(this->vertices[vertex_1].position - this->vertices[vertex_2].position,
										this->vertices[vertex_1].position - this->vertices[vertex_3].position) ==
										QVector3D(0.0f, 0.0f, 0.0f)) {
				this->normal_triangles[++i] = QVector3D(0.0f, 0.0f, 0.0f);
			} else {
				QVector3D tmp_ = QVector3D::crossProduct(this->vertices[vertex_1].position - this->vertices[vertex_2].position,
														 this->vertices[vertex_1].position - this->vertices[vertex_3].position);
				this->normal_triangles[++i] = tmp_.normalized();
			}

			// Lower triangle
			vertex_2 = this->GetIndex( w+1, h );
			vertex_3 = this->GetIndex( w+1, h+1 );

			if (QVector3D::crossProduct(this->vertices[vertex_1].position - this->vertices[vertex_2].position,
										this->vertices[vertex_1].position - this->vertices[vertex_3].position) ==
										QVector3D(0.0f, 0.0f, 0.0f)) {
				this->normal_triangles[++i] = QVector3D(0.0f, 0.0f, 0.0f);
			} else {
				QVector3D tmp_ = QVector3D::crossProduct(this->vertices[vertex_1].position - this->vertices[vertex_2].position,
														 this->vertices[vertex_1].position - this->vertices[vertex_3].position);
				this->normal_triangles[++i] = tmp_.normalized();
			}*/

			//-----

			if (QVector3D::crossProduct(this->vertices[vertex_2].position - this->vertices[vertex_1].position,
										this->vertices[vertex_3].position - this->vertices[vertex_1].position) ==
										QVector3D(0.0f, 0.0f, 0.0f)) {
				this->normal_triangles[++i] = QVector3D(0.0f, 0.0f, 0.0f);
			} else {
				QVector3D tmp_ = QVector3D::crossProduct(this->vertices[vertex_2].position - this->vertices[vertex_1].position,
														 this->vertices[vertex_3].position - this->vertices[vertex_1].position);
				this->normal_triangles[++i] = tmp_.normalized();
			}

			// Lower triangle
			vertex_2 = this->GetIndex( w+1, h );
			vertex_3 = this->GetIndex( w+1, h+1 );

			if (QVector3D::crossProduct(this->vertices[vertex_2].position - this->vertices[vertex_1].position,
										this->vertices[vertex_3].position - this->vertices[vertex_1].position) ==
										QVector3D(0.0f, 0.0f, 0.0f)) {
				this->normal_triangles[++i] = QVector3D(0.0f, 0.0f, 0.0f);
			} else {
				QVector3D tmp_ = QVector3D::crossProduct(this->vertices[vertex_2].position - this->vertices[vertex_1].position,
														 this->vertices[vertex_3].position - this->vertices[vertex_1].position);
				this->normal_triangles[++i] = tmp_.normalized();
			}

		}
	}

}

void Sphere::ComputeVertexNormals()
{

	QVector<GLuint> Adjacent_Indices;
	GLuint index;
	GLuint W = this->slices;
	GLuint H = this->stacks;

	for (GLuint h = 1; h <= H; ++h) {
		for (GLuint w = 1; w <= W; ++w) {
			index = this->GetIndex( w, h );
			Adjacent_Indices.clear();
			Adjacent_Indices = AdjacentTriangleIndices( w, h );
			//cout << "w: " << w << "; h: " << h << "; Adjacent_Indices.size(): " << Adjacent_Indices.size() << endl;
			for (GLuint i = 0; i < (GLuint)Adjacent_Indices.size(); ++i)
				//cout << "Adjacent[" << i << "]: " << Adjacent_Indices[i] << endl;
			this->vertices[index].normal = QVector3D(0.0f, 0.0f, 0.0f);
			for (GLuint i = 0; i < (GLuint)Adjacent_Indices.size(); ++i) {
				this->vertices[index].normal = QVector3D( this->normal_triangles[Adjacent_Indices[i]].position.x() + this->vertices[index].normal.x(),
														  this->normal_triangles[Adjacent_Indices[i]].position.y() + this->vertices[index].normal.y(),
														  this->normal_triangles[Adjacent_Indices[i]].position.z() + this->vertices[index].normal.z()
														  );
			}
			GLuint div = Adjacent_Indices.size();

			if ( h == 2 || h == H-1 ) {
				div = 5;
			}
			//cout << "div: " << div << endl;
			this->vertices[index].normal.setX(this->vertices[index].normal.x() / div);
			this->vertices[index].normal.setY(this->vertices[index].normal.y() / div);
			this->vertices[index].normal.setZ(this->vertices[index].normal.z() / div);

			//this->vertices[index].normal = this->vertices[index].position;
			this->vertices[index].normal.normalize();
		}
	}

}

QVector<GLuint> Sphere::AdjacentTriangleIndices(GLuint w, GLuint h)
{
	QVector<GLuint> Adjacent_Indices;
	GLuint index;

	GLuint W = this->slices;
	GLuint H = this->stacks;
	if (w > 1 && h > 1 && w < W && h < H) {
		//cout << "a" << endl;
		/*
		Adjacent_Indices.clear();

		index = this->GetCellIndex( w, h );
		Adjacent_Indices.push_back(2*index);
		Adjacent_Indices.push_back(2*index + 1);

		index = this->GetCellIndex( w, h-1 );
		Adjacent_Indices.push_back(2*index);

		index = this->GetCellIndex( w-1, h-1 );
		Adjacent_Indices.push_back(2*index);
		Adjacent_Indices.push_back(2*index + 1);

		index = this->GetCellIndex( w-1, h );
		Adjacent_Indices.push_back(2*index + 1);
		*/

		Adjacent_Indices.resize( 6 );

		index = this->GetCellIndex( w, h );
		Adjacent_Indices[0] = 2*index;
		Adjacent_Indices[1] = 2*index + 1;

		index = this->GetCellIndex( w, h-1 );
		Adjacent_Indices[2] = 2*index;

		index = this->GetCellIndex( w-1, h-1 );
		Adjacent_Indices[3] = 2*index;
		Adjacent_Indices[4] = 2*index + 1;

		index = this->GetCellIndex( w-1, h );
		Adjacent_Indices[5] = 2*index + 1;

	} else if ( (w == 1 && ( h > 1 && h < H )) ||
				(w == W && ( h > 1 && h < H )) ) {
		//cout << "b" << endl;
		/*
		Adjacent_Indices.clear();

		index = this->GetCellIndex( 1, h );
		Adjacent_Indices.push_back(2*index);
		Adjacent_Indices.push_back(2*index + 1);

		index = this->GetCellIndex( 1, h-1 );
		Adjacent_Indices.push_back(2*index);

		index = this->GetCellIndex( W-1, h );
		Adjacent_Indices.push_back(2*index + 1);

		index = this->GetCellIndex( W-1, h-1 );
		Adjacent_Indices.push_back(2*index);
		Adjacent_Indices.push_back(2*index + 1);
		*/

		Adjacent_Indices.resize( 6 );

		index = this->GetCellIndex( 1, h );
		Adjacent_Indices[0] = 2*index;
		Adjacent_Indices[1] = 2*index + 1;

		index = this->GetCellIndex( 1, h-1 );
		Adjacent_Indices[2] = 2*index;

		index = this->GetCellIndex( W-1, h );
		Adjacent_Indices[3] = 2*index + 1;

		index = this->GetCellIndex( W-1, h-1 );
		Adjacent_Indices[4] = 2*index;
		Adjacent_Indices[5] = 2*index + 1;

	} else if ( h == 1 || h == H ) {

		/*
		Adjacent_Indices.clear();

		if (h == 1) {
			for (GLuint i = 1; i < W; ++i) {
				index = this->GetCellIndex( i, h );
				Adjacent_Indices.push_back(2*index);
			}
		} else {
			for (GLuint i = 1; i < W; ++i) {
				index = this->GetCellIndex( i, h-1 );
				Adjacent_Indices.push_back(2*index + 1);
			}
		}
		*/

		Adjacent_Indices.resize( W-1 );

		if (h == 1) {
			for (GLuint i = 1; i < W; ++i) {
				index = this->GetCellIndex( i, h );
				Adjacent_Indices[i-1] = 2*index;
			}
		} else {
			for (GLuint i = 1; i < W; ++i) {
				index = this->GetCellIndex( i, h-1 );
				Adjacent_Indices[i-1] = 2*index + 1;
			}
		}

	}

	return Adjacent_Indices;
}

void Sphere::BuildNormalVisualizationGeometry()
{
	this->normal_vertices.clear();
	this->normal_indices.clear();
	const float normal_scalar = -this->radius / 10.f;
	for (GLuint j = 0; j < (GLuint)this->vertices.size(); ++j)
	{
		this->normal_vertices.push_back(VertexAttributesP(this->vertices[j].position));
		this->normal_vertices.push_back(VertexAttributesP(this->vertices[j].position + this->vertices[j].normal * normal_scalar));
		this->normal_indices.push_back(this->normal_vertices.size() - 2);
		this->normal_indices.push_back(this->normal_vertices.size() - 1);
	}
}

void Sphere::show_wire_frame(bool swf_)
{
	this->wire_frame_mode(swf_);
}

// Delete all created vectors, associated shaders
void Sphere::TakeDown()
{
	super::TakeDown();
}
