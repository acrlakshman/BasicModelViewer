/*
 * This class was borrowed from my course project, which was originally written
 * by Perry Kivolowitz. This is modified here to work with Qt.
 */
/*
 * Copyright (c) 2014, Lakshman Anumolu
 * All rights reserved.
 *
 * This file is part of Basic Model Viewer whose distribution is governed by
 * the BSD 2-Clause License contained in the accompanying LICENSE.txt file.
 *
 * Description
 *  Class to store vertex attributes.
 *	TODO: Try template magic on this class.
 */

#ifndef __VERTEXATTRIBUTES_H__
#define __VERTEXATTRIBUTES_H__

#include <QVector3D>
#include <QVector2D>

/*
	P = position
	C = color
	N = normal
	T = texture coordinates
*/

class VertexAttributesPCNT
{
public:
	VertexAttributesPCNT();
	VertexAttributesPCNT(const QVector3D &p, const QVector3D &c, const QVector3D &n, const QVector2D &t);
	VertexAttributesPCNT(const VertexAttributesPCNT &other);
	QVector3D position;
	QVector3D color;
	QVector3D normal;
	QVector2D texture_coordinate;
};

class VertexAttributesPCN
{
public:
	VertexAttributesPCN();
	VertexAttributesPCN(const QVector3D &p, const QVector3D &c, const QVector3D &n);
	VertexAttributesPCN(const VertexAttributesPCN &other);
	QVector3D position;
	QVector3D color;
	QVector3D normal;
};

class VertexAttributesPN
{
public:
	VertexAttributesPN();
	VertexAttributesPN(const QVector3D &p, const QVector3D &n);
	VertexAttributesPN(const VertexAttributesPN &other);
	QVector3D position;
	QVector3D normal;
};

class VertexAttributesP
{
public:
	VertexAttributesP();
	VertexAttributesP(const QVector3D &p);
	VertexAttributesP(const VertexAttributesP &other);
	QVector3D position;
};

#endif // END OF __VERTEXATTRIBUTES_H__
