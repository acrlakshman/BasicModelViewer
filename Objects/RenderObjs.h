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
