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

#include "BoundingBox.h"

BoundingBox::BoundingBox()
{
}

BoundingBox::~BoundingBox()
{
}

void BoundingBox::SetBoundingBox(bounding_box *b_box_)
{
	this->b_box = *b_box_;
}

bounding_box BoundingBox::GetBoundingBox()
{
	return this->b_box;
}

void BoundingBox::PrintBoundingBox()
{
	qDebug() << "x_range = [" << this->b_box.x_range.x() << "," << this->b_box.x_range.y() << "]";
	qDebug() << "y_range = [" << this->b_box.y_range.x() << "," << this->b_box.y_range.y() << "]";
	qDebug() << "z_range = [" << this->b_box.z_range.x() << "," << this->b_box.z_range.y() << "]";
}
