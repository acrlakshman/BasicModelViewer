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

#include "RenderEnSightPOV.h"

RenderEnSightPOV::RenderEnSightPOV()
    : number_of_shapes(1)
{
	default_vertex_color = QVector3D(0.6, 0.6, 0.6);
	default_light.position = QVector3D(0.0, 0.0, -10.0);
	default_light.color_ambient = QVector3D(0.3, 0.3, 0.3);
	default_light.color_diffuse = QVector3D(0.3, 0.3, 0.3);
	default_light.color_specular = QVector3D(1.0, 1.0, 1.0);
}

RenderEnSightPOV::~RenderEnSightPOV()
{
}

bool RenderEnSightPOV::LocalInitialize(QString file_name_)
{
    QVector3D light0_pos = QVector3D(0, 10, 10);
    QVector3D color_ambient = QVector3D(0.5, 0.5, 0.7);
    QVector3D color_diffuse = QVector3D(0.5, 0.5, 0.7);
    QVector3D color_specular = QVector3D(1.0, 1.0, 1.0);
    Light light0;
    light0.position = light0_pos;
    light0.color_ambient = color_ambient;
    light0.color_diffuse = color_diffuse;
    light0.color_specular = color_specular;

    parse_ensight_pov.LoadPOV(file_name_.toUtf8().constData());

    generic_object = new GenericObject[this->number_of_shapes];

    for (GLuint i = 0; i < this->number_of_shapes; ++i) {
        generic_object[i].InitializeVertexPositionsAndIndices(parse_ensight_pov.vertex_positions,
                                                        parse_ensight_pov.vertex_indices);

        if (parse_ensight_pov.vertex_normals.size() > 0)
            generic_object[i].InitializeVertexNormals(parse_ensight_pov.vertex_normals);

        /*if (parse_obj.vertex_texture_coordinates[i].size() > 0)
            generic_object[i].InitializeVertexTextureCoordinates(parse_obj.vertex_texture_coordinates[i]);
        */

        QVector3D generic_object_color = QVector3D(.0, 0.2, 0.7);
        generic_object[i].SetVertexColor(generic_object_color);
        GLfloat shininess_g = 100;
        generic_object[i].SetShininess(shininess_g);
        light0.position = light0_pos;
        generic_object[i].AddLight(light0);

        generic_object[i].LocalInitialize();
    }

	return !GLReturnedError("RenderObj::LocalInitialize - on exit");
}

void RenderEnSightPOV::SetVertexColor(QVector3D &color_)
{
    for (GLuint i = 0; i < this->number_of_shapes; ++i)
	   generic_object[i].SetVertexColor(color_);
}

bool RenderEnSightPOV::Draw(
	const QMatrix4x4 &projection,
	const QMatrix4x4 &modelview_cam,
	QMatrix4x4 modelview,
	QOpenGLShaderProgram *shader_
	)
{

    for (GLuint i = 0; i < this->number_of_shapes; ++i) {
        generic_object[i].Draw(projection, modelview_cam, modelview, shader_);
    }

    return !GLReturnedError("RenderObj::Draw - on exit");
}

void RenderEnSightPOV::AddLight(Light light_)
{
    for (GLuint i = 0; i < this->number_of_shapes; ++i)
	   generic_object[i].AddLight(light_);
}

GLuint RenderEnSightPOV::GetNumberOfLights()
{
    if (this->number_of_shapes > 0)
        return generic_object[0].GetNumberOfLights();
    else
        return 0;
}

void RenderEnSightPOV::EditLight(Light light_, GLuint idx_)
{
    for (GLuint i = 0; i < this->number_of_shapes; ++i)
        generic_object[i].EditLight(light_, idx_);
}

void RenderEnSightPOV::SetShininess(GLfloat shininess_)
{
    for (GLuint i = 0; i < this->number_of_shapes; ++i)
	   generic_object[i].SetShininess(shininess_);
}

void RenderEnSightPOV::show_wire_frame(bool swf_)
{
    for (GLuint i = 0; i < this->number_of_shapes; ++i)
	   generic_object[i].show_wire_frame(swf_);
}

void RenderEnSightPOV::show_normals(bool sn_)
{
    for (GLuint i = 0; i < this->number_of_shapes; ++i)
	   generic_object[i].show_normals(sn_);
}

void RenderEnSightPOV::TakeDown()
{
    for (GLuint i = 0; i < this->number_of_shapes; ++i)
	   generic_object[i].TakeDown();

    delete[] generic_object;
    parse_ensight_pov.TakeDown();
}
