/*
 * Copyright (c) 2014, Lakshman Anumolu
 * All rights reserved.
 *
 * This file is part of Basic Model Viewer whose distribution is governed by
 * the BSD-style license contained in the accompanying file license.txt.
 *
 * Description
 *	This class parses wavefront .obj file to the format used in this package
 *	with the help of tiny_obj_loader.
 *
 * Acknowledgements
 *	Thanks to Hammad Mazhar
 *	(https://github.com/hmazhar), whose code written for chrono-opengl is of a
 *	help in completing this class.
 */

#ifndef __PARSEOBJ_H__
#define __PARSEOBJ_H__

#include <vector>
#include <string>

#include <QString>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QDebug>

#include <AuxiliaryMethods/tiny_obj_loader.h>

class ParseObj {
public:
	ParseObj();
	~ParseObj();

	void ParseObjFile(const char* file_name);

	void LoadObjFile(
		const char* file_name,
		QVector<QVector<QVector3D> > &vertex_positions,
		QVector<QVector<QVector3D> > &vertex_normals,
		QVector<QVector<QVector2D> > &vertex_texture_coordinates,
		QVector<QVector<unsigned int> > &vertex_indices,
		QVector<std::string> &names
		);

public:
	QVector<QVector<QVector3D> > vertex_positions;
	QVector<QVector<QVector3D> > vertex_normals;
	QVector<QVector<QVector2D> > vertex_texture_coordinates;
	QVector<QVector<unsigned int> > vertex_indices;
	QVector<std::string> names;
};

#endif // END OF __PARSEOBJ_H__
