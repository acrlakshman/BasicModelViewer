/*
 * Copyright (c) 2014, Lakshman Anumolu
 * All rights reserved.
 *
 * This file is part of Basic Model Viewer whose distribution is governed by
 * the BSD 2-Clause License contained in the accompanying LICENSE.txt file.
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
