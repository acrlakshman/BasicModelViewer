/*
 * This class was borrowed from my course project, which was originally written
 * by Perry Kivolowitz. This is modified here to work with Qt.
 */
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
