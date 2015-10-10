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
 *  Base class for bounding box
 */

#ifndef __BOUNDINGBOX_H__
#define __BOUNDINGBOX_H__

#include <QVector2D>

#include <AuxiliaryMethods/HelperFunctions.h>

class BoundingBox
{
public:
	BoundingBox();
	~BoundingBox();

	void SetBoundingBox(bounding_box *b_box_);
	bounding_box GetBoundingBox();

	void PrintBoundingBox();

private:
	bounding_box b_box;
};

#endif // END OF __BOUNDINGBOX_H__
