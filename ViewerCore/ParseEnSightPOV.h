/*
 * Copyright (c) 2014, Lakshman Anumolu
 * All rights reserved.
 *
 * This file is part of Basic Model Viewer whose distribution is governed by
 * the BSD 2-Clause License contained in the accompanying LICENSE.txt file.
 *
 * Description
 *	This class parses POV file genereated by EnSight to the format used in this
 *	package.
 *	CAUTION: File is buffered to memory completely before performing operations
 *	ISSUE: Parser gave wrong results when applied on preliminary .pov files
 *	hence need some modifications.
 */

#ifndef __PARSEENSIGHTPOV_H__
#define __PARSEENSIGHTPOV_H__

#include <stdio.h>
#include <string>
#include <math.h>

#include <QString>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QDebug>

class ParseEnSightPOV {
public:
	ParseEnSightPOV();
	~ParseEnSightPOV();

	void TakeDown();

	bool LoadPOV(
			const char *file_name_
			);

	bool ParsePOVString(const std::string &str_, int &status_);

	bool ExtractLastInt(const std::string &str_, int &int_num_, int &start_pos);
	void ExtractAllInts(const std::string &str_);

	bool ExtractLastFloat(const std::string &str_, float &float_num_, int &start_pos);
	void ExtractAllFloats(const std::string &str_, int &status_);

	int num_vertices, num_indices;
	QVector<QVector3D> vertex_positions;
	QVector<QVector3D> vertex_normals;
	QVector<unsigned int> vertex_indices;
};

#endif // END OF __PARSEENSIGHTPOV_H__
