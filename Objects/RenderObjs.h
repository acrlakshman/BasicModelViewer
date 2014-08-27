/*
 * Copyright (c) 2014, Lakshman Anumolu
 * All rights reserved.
 *
 * This file is part of Basic Model Viewer whose distribution is governed by
 * the BSD 2-Clause License contained in the accompanying LICENSE.txt file.
 *
 * Description
 *	Wrapper class to render all multiple in obj files.
 */

#ifndef __RENDEROBJS_H__
#define __RENDEROBJS_H__

#include <QVector>
#include <QVector3D>

#include <Objects/RenderObj.h>
#include <AuxiliaryMethods/HelperFunctions.h>

class RenderObjs
{
public:
	RenderObjs();
	~RenderObjs();

	void TakeDown();

	bool Draw(
		const QMatrix4x4 &projection,
		const QMatrix4x4 &modelview_cam,
		QMatrix4x4 modelview,
		QOpenGLShaderProgram *shader_
		);

	void AddObj(RenderObj *render_obj_);
	void ComputeGlobalBoundingBox();

private:
	QVector<RenderObj *> render_objs;

	/* translate obj to origin */
	float x_base_move, y_base_move, z_base_move;

	/* bounding box */
	bounding_box global_b_box;
};

#endif // END OF __RENDEROBJS_H__
