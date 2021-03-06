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
 *	Helper functions and some data structures used in this package are defined
 *	here.
 */

#ifndef __HELPERFUNCTIONS_H__
#define __HELPERFUNCTIONS_H__

#include <stdio.h>

#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>
#include <QGenericMatrix>

/* Data structures */

struct Light {
	QVector3D position = QVector3D(0.0, 0.0, 10.0);
	QVector3D color_ambient = QVector3D(0.5, 0.5, 0.5);
	QVector3D color_diffuse = QVector3D(0.5, 0.5, 0.5);
	QVector3D color_specular = QVector3D(1.0, 1.0, 1.0);
};

struct bounding_box {
	QVector2D x_range = QVector2D(0.0, 0.0);
	QVector2D y_range = QVector2D(0.0, 0.0);
	QVector2D z_range = QVector2D(0.0, 0.0);
};

/* Functions */

static void Mat4ToMat3(const QMatrix4x4 &Mat4, QMatrix3x3 &Mat3)
{
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			Mat3(i, j) = Mat4(i, j);
}

static void Mat3ToMat4(const QMatrix3x3 &Mat3, QMatrix4x4 &Mat4)
{
	Mat4.setToIdentity();
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			Mat4(i, j) = Mat3(i, j);
}

/* Equivalent to glm::inverse(glm::transpose(glm::mat3(mv))) */
static bool Inverse_Transpose_Mat4ToMat3(const QMatrix4x4 &Mat4, QMatrix3x3 &Mat3)
{
	QMatrix4x4 Mat4_tmp = QMatrix4x4();

	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			Mat4_tmp(i, j) = Mat4(i, j);

	Mat4_tmp = Mat4_tmp.transposed();
	bool invertible_ = true;
	Mat4_tmp = Mat4_tmp.inverted(&invertible_);

	Mat4ToMat3(Mat4_tmp, Mat3);
	return invertible_;
}

/* (QMatrix3x3 * QVector3D) */
static QVector3D Multiply_Mat3_Vec3(const QMatrix3x3 &Mat3, const QVector3D &Vec3)
{
	QMatrix4x4 Mat4_tmp = QMatrix4x4();
	Mat3ToMat4(Mat3, Mat4_tmp);

	QVector4D Vec4_tmp = QVector4D(Vec3, 1.0f);

	Vec4_tmp = Mat4_tmp * Vec4_tmp;

	return QVector3D(Vec4_tmp);
}

/* Union of two bounding boxes */
static bounding_box BoundingBoxesUnion2(const bounding_box &b_box1, const bounding_box &b_box2)
{
	bounding_box u_b_box;
	u_b_box.x_range.setX(std::min(b_box1.x_range.x(), b_box2.x_range.x()));
	u_b_box.x_range.setY(std::max(b_box1.x_range.y(), b_box2.x_range.y()));
	u_b_box.y_range.setX(std::min(b_box1.y_range.x(), b_box2.y_range.x()));
	u_b_box.y_range.setY(std::max(b_box1.y_range.y(), b_box2.y_range.y()));
	u_b_box.z_range.setX(std::min(b_box1.z_range.x(), b_box2.z_range.x()));
	u_b_box.z_range.setY(std::max(b_box1.z_range.y(), b_box2.z_range.y()));

	return u_b_box;
}

/* Union of a vector of bounding boxes */
static bounding_box BoundingBoxesUnion(const QVector<bounding_box> &b_boxes)
{
	bounding_box u_b_box;
	for (int i = 0; i < b_boxes.size(); ++i) {
		u_b_box = BoundingBoxesUnion2(u_b_box, b_boxes[i]);
	}
	return u_b_box;
}

/* Print bounding box */
static void PrintBoundingBox(const bounding_box &b_box)
{
	qDebug() << "x_range = [" << b_box.x_range.x() << "," << b_box.x_range.y() << "]";
	qDebug() << "y_range = [" << b_box.y_range.x() << "," << b_box.y_range.y() << "]";
	qDebug() << "z_range = [" << b_box.z_range.x() << "," << b_box.z_range.y() << "]";
}

/* Save camera details to a file */
static void SaveCameraDetails(
	QVector3D &camera_position_,
	QVector3D &camera_lookat_,
	QVector3D &camera_up_,
	float &fov_
	)
{
	FILE *file_p;
	file_p = fopen("../scene_details.txt", "w");
	if (file_p != NULL){
		fputs("Camera details (Right handed system)\n", file_p);
		fprintf(file_p, "Camera position: (%f, %f, %f)\n", camera_position_.x(),
								camera_position_.y(),
								camera_position_.z());
		fprintf(file_p, "Camera look at: (%f, %f, %f)\n", camera_lookat_.x(),
								camera_lookat_.y(),
								camera_lookat_.z());
		fprintf(file_p, "Camera up vector: (%f, %f, %f)\n", camera_up_.x(),
								camera_up_.y(),
								camera_up_.z());
		fprintf(file_p, "Camera field of view: %f\n", fov_);
		fputs("\nCamera details (Left handed system)\n", file_p);
		fprintf(file_p, "Camera position: (%f, %f, %f)\n", camera_position_.x(),
								camera_position_.y(),
								-camera_position_.z());
		fprintf(file_p, "Camera look at: (%f, %f, %f)\n", camera_lookat_.x(),
								camera_lookat_.y(),
								-camera_lookat_.z());
		fprintf(file_p, "Camera up vector: (%f, %f, %f)\n", camera_up_.x(),
								camera_up_.y(),
								-camera_up_.z());
		fprintf(file_p, "Camera field of view: %f\n", fov_);
	} else {
		qDebug() << "Error!";
	}
	fclose(file_p);
}

/* Save light details to a file */
static void SaveLightDetails(
	Light light_
	)
{
	FILE *file_p;
	file_p = fopen("../scene_details.txt", "a");
	if (file_p != NULL){
		fputs("\nLight details (Right handed system)\n", file_p);
		fprintf(file_p, "Light position: (%f, %f, %f)\n", light_.position.x(),
								light_.position.y(),
								light_.position.z());
		fputs("\nLight details (Left handed system)\n", file_p);
		fprintf(file_p, "Light position: (%f, %f, %f)\n", light_.position.x(),
								light_.position.y(),
								-light_.position.z());
	} else {
		qDebug() << "Error!";
	}
	fclose(file_p);
}

#endif // END OF __HELPERFUNCTIONS_H__
