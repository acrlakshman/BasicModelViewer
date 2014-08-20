/*
 * Copyright (c) 2014, Lakshman Anumolu
 * All rights reserved.
 *
 * This file is part of Basic Model Viewer whose distribution is governed by
 * the BSD 2-Clause License contained in the accompanying LICENSE.txt file.
 */

#include "PlanarMesh.h"

/*
PlanarMesh::PlanarMesh (
	GLuint slices_,
	GLuint stacks_
	)
{
	this->slices = slices_;
	this->stacks = stacks_;
}
*/
PlanarMesh::PlanarMesh ()
{
}

PlanarMesh::~PlanarMesh()
{
}

// Member functions
void PlanarMesh::InitializePlanarMesh(
	GLuint slices_,
	GLuint stacks_,
	QVector<GLuint> &vertex_indices,
	QVector<GLuint> &vertex_indices_wire_frame,
	QString direction
	)
{
	this->slices = slices_;
	this->stacks = stacks_;
	this->InitializeVertexIndices(vertex_indices, vertex_indices_wire_frame, direction);
}

GLuint PlanarMesh::GetCellIndex (
	GLuint w,
	GLuint h
	)
{
	return ( (this->slices - 1)*(h - 1) + (w - 1) );
}

GLuint PlanarMesh::GetIndex (
	GLuint w,
	GLuint h
	)
{
	return ( h * this->slices - this->slices + (w - 1) );
}

void PlanarMesh::InitializeVertexIndices (
	QVector<GLuint> &vertex_indices,
	QVector<GLuint> &vertex_indices_wire_frame,
	QString direction
	)
{
	// Resize the vector
	GLuint W = this->slices;
	GLuint H = this->stacks;

	GLuint Num_Triangles = (W-1) * (H-1) * 2;
	GLuint VI_Size = GLuint(3) * Num_Triangles; // vertex_indices size

	//vertex_indices.clear();
	vertex_indices.resize(VI_Size);
	//std::cout << "VI_Size: " << VI_Size << std::endl;

	if (!direction.compare(QString("CounterClockWise"), Qt::CaseInsensitive)) {
		GLuint i = -1;
		for (GLuint h = 1; h < H; ++h)
			for (GLuint w = 1; w < W; ++w) {
				// Upper triangle
				vertex_indices[++i] = this->GetIndex( w, h );
				vertex_indices[++i] = this->GetIndex( w+1, h+1 );
				vertex_indices[++i] = this->GetIndex( w, h+1 );

				// Lower triangle
				vertex_indices[++i] = this->GetIndex( w, h );
				vertex_indices[++i] = this->GetIndex( w+1, h );
				vertex_indices[++i] = this->GetIndex( w+1, h+1 );
			}
	} else if (!direction.compare(QString("ClockWise"), Qt::CaseInsensitive)) {
		GLuint i = -1;
		for (GLuint h = 1; h < H; ++h)
			for (GLuint w = 1; w < W; ++w) {
				// Upper triangle
				vertex_indices[++i] = GetIndex( w, h );
				vertex_indices[++i] = GetIndex( w, h+1 );
				vertex_indices[++i] = GetIndex( w+1, h+1 );

				// Lower triangle
				vertex_indices[++i] = GetIndex( w, h );
				vertex_indices[++i] = GetIndex( w+1, h+1 );
				vertex_indices[++i] = GetIndex( w+1, h );
				
			}
	}

	for (GLuint h = 1; h < H; ++h) {
		for (GLuint w = 1; w < W; ++w) {
			if (w == 1 && h == 1) {
				GLuint idx1_ = this->GetIndex(w, h);
				GLuint idx2_ = this->GetIndex(w+1, h);
				GLuint idx3_ = this->GetIndex(w, h+1);
				GLuint idx4_ = this->GetIndex(w+1, h+1);

				vertex_indices_wire_frame.push_back(idx1_);
				vertex_indices_wire_frame.push_back(idx2_);

				vertex_indices_wire_frame.push_back(idx1_);
				vertex_indices_wire_frame.push_back(idx3_);

				vertex_indices_wire_frame.push_back(idx1_);
				vertex_indices_wire_frame.push_back(idx4_);

				vertex_indices_wire_frame.push_back(idx3_);
				vertex_indices_wire_frame.push_back(idx4_);

				vertex_indices_wire_frame.push_back(idx2_);
				vertex_indices_wire_frame.push_back(idx4_);
			} else if (h == 1 && w > 1) {
				GLuint idx1_ = this->GetIndex(w, h);
				GLuint idx2_ = this->GetIndex(w+1, h);
				GLuint idx3_ = this->GetIndex(w, h+1);
				GLuint idx4_ = this->GetIndex(w+1, h+1);

				vertex_indices_wire_frame.push_back(idx1_);
				vertex_indices_wire_frame.push_back(idx2_);

				vertex_indices_wire_frame.push_back(idx1_);
				vertex_indices_wire_frame.push_back(idx4_);

				vertex_indices_wire_frame.push_back(idx2_);
				vertex_indices_wire_frame.push_back(idx4_);

				vertex_indices_wire_frame.push_back(idx3_);
				vertex_indices_wire_frame.push_back(idx4_);
			} else if (w == 1 && h > 1) {
				GLuint idx1_ = this->GetIndex(w, h);
				GLuint idx2_ = this->GetIndex(w+1, h);
				GLuint idx3_ = this->GetIndex(w, h+1);
				GLuint idx4_ = this->GetIndex(w+1, h+1);

				vertex_indices_wire_frame.push_back(idx1_);
				vertex_indices_wire_frame.push_back(idx3_);

				vertex_indices_wire_frame.push_back(idx1_);
				vertex_indices_wire_frame.push_back(idx4_);

				vertex_indices_wire_frame.push_back(idx2_);
				vertex_indices_wire_frame.push_back(idx4_);

				vertex_indices_wire_frame.push_back(idx3_);
				vertex_indices_wire_frame.push_back(idx4_);
			} else if (w > 1 && h > 1) {
				GLuint idx1_ = this->GetIndex(w, h);
				GLuint idx2_ = this->GetIndex(w+1, h);
				GLuint idx3_ = this->GetIndex(w, h+1);
				GLuint idx4_ = this->GetIndex(w+1, h+1);

				vertex_indices_wire_frame.push_back(idx1_);
				vertex_indices_wire_frame.push_back(idx4_);

				vertex_indices_wire_frame.push_back(idx2_);
				vertex_indices_wire_frame.push_back(idx4_);

				vertex_indices_wire_frame.push_back(idx3_);
				vertex_indices_wire_frame.push_back(idx4_);
			}
		}
	}

}