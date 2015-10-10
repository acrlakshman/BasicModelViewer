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
 */

#include "VertexAttributes.h"

VertexAttributesPCNT::VertexAttributesPCNT()
{
	this->position = QVector3D();
	this->color = QVector3D();
	this->normal = QVector3D();
	this->texture_coordinate = QVector2D();
}

VertexAttributesPCNT::VertexAttributesPCNT(const QVector3D &p, const QVector3D &c, const QVector3D &n, const QVector2D &t)
{
	this->position = p;
	this->color = c;
	this->normal = n;
	this->texture_coordinate = t;
}

VertexAttributesPCNT::VertexAttributesPCNT(const VertexAttributesPCNT &other)
{
	this->position = other.position;
	this->color = other.color;
	this->normal = other.normal;
	this->texture_coordinate = other.texture_coordinate;
}

VertexAttributesP::VertexAttributesP(const QVector3D &p)
{
	this->position = p;
}

VertexAttributesP::VertexAttributesP(const VertexAttributesP &other)
{
	this->position = other.position;
}

VertexAttributesP::VertexAttributesP()
{
	this->position = QVector3D();
}

VertexAttributesPN::VertexAttributesPN(const QVector3D &p, const QVector3D &n)
{
	this->position = p;
	this->normal = n;
}

VertexAttributesPN::VertexAttributesPN(const VertexAttributesPN &other)
{
	this->position = other.position;
	this->normal = other.normal;
}

VertexAttributesPN::VertexAttributesPN()
{
	this->position = QVector3D();
	this->normal = QVector3D();
}


VertexAttributesPCN::VertexAttributesPCN()
{
	this->position = QVector3D();
	this->color = QVector3D();
	this->normal = QVector3D();
}

VertexAttributesPCN::VertexAttributesPCN(const QVector3D &p, const QVector3D &c, const QVector3D &n)
{
	this->position = p;
	this->color = c;
	this->normal = n;
}

VertexAttributesPCN::VertexAttributesPCN(const VertexAttributesPCN &other)
{
	this->position = other.position;
	this->color = other.color;
	this->normal = other.normal;
}
