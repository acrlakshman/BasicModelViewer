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
 *	Wrapper class to render all shapes in obj file.
 */

#ifndef __RENDEROBJ_H__
#define __RENDEROBJ_H__

#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>

#include <AuxiliaryMethods/CheckErrors.h>
#include <AuxiliaryMethods/HelperFunctions.h>
#include <ViewerCore/ParseObj.h>
#include <ViewerCore/GenericObject.h>

class RenderObj
{
public:
	RenderObj();
	~RenderObj();

	bool LocalInitialize(QString obj_file_name);

	void SetVertexColor(QVector3D &color_);

	bool Draw(
		const QMatrix4x4 &projection,
		const QMatrix4x4 &modelview_cam,
		QMatrix4x4 modelview,
		QOpenGLShaderProgram *shader_
		);

	// Add light
	void AddLight(Light light_);

	// Return number of lights on this object
	GLuint GetNumberOfLights();

	// Edit light info
	void EditLight(Light light_, GLuint idx_ = 0);

	// Set object shininess parameter
	void SetShininess(GLfloat shininess_);

        // Set position offset
        void SetVertexPositionsGlobalOffset(const QVector3D position_offset);

	void show_wire_frame(bool swf_);
	void show_normals(bool sn_);

	/* Return bounding boxes for all shapes */
	QVector<bounding_box> GetBoundingBoxes();

	// Delete all created vectors, associated shaders
	void TakeDown();

private:
	GLuint number_of_shapes;

	QVector3D default_vertex_color;
	Light default_light;
        QVector3D positions_global_offset;

	ParseObj parse_obj;
	GenericObject *generic_object;
};

#endif // END OF __RENDEROBJ_H__
