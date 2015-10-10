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
