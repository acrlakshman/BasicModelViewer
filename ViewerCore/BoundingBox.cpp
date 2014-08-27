/*
 * Copyright (c) 2014, Lakshman Anumolu
 * All rights reserved.
 *
 * This file is part of Basic Model Viewer whose distribution is governed by
 * the BSD 2-Clause License contained in the accompanying LICENSE.txt file.
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
