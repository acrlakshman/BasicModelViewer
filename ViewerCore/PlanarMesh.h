/*
 * Copyright (c) 2014, Lakshman Anumolu
 * All rights reserved.
 *
 * This file is part of Basic Model Viewer whose distribution is governed by
 * the FreeBSD License contained in the accompanying LICENSE.txt file.
 *
 * Description
 *	This class creates a 2D mesh which will be used to create objects by
 *	stiching the points in this mesh.
 */

#ifndef __PLANERMESH_H__
#define __PLANERMESH_H__

#include <QVector>
#include <QString>
#include <QOpenGLFunctions>

class PlanarMesh
{
public:
	/*
	PlanarMesh (
		GLuint slices_,
		GLuint stacks_
		);
	*/

	// Constructor
	PlanarMesh();

	// Destructor
	~PlanarMesh();

	// Initializes mesh i.e. fills in vertex_indices
	void InitializePlanarMesh(
		GLuint slices_,
		GLuint stacks_,
		QVector<GLuint> &vertex_indices,
		QVector<GLuint> &vertex_indices_wire_frame,
		QString direction = "CounterClockWise"
		);

	// Get the 0-based cell index for which
	// the 1-based vertex index (w,h) is the base
	// vertex.
	// 1 <= w <= W-1
	// 1 <= h <= H-1
	GLuint GetCellIndex (
		GLuint w,
		GLuint h
		);

	// Converts 2d (w,h) index to 1d equivalent
	GLuint GetIndex (
		GLuint w,
		GLuint h
		);

	// Initialize vertex indices for planar mesh
	void InitializeVertexIndices (
		QVector<GLuint> &vertex_indices,
		QVector<GLuint> &vertex_indices_wire_frame,
		QString direction
		);

	GLuint slices; // horizontal divisions (W)
	GLuint stacks; // vertical divisions (H)

};

#endif // END OF __PLANERMESH_H__
