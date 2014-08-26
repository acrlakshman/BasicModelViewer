/*
 * Copyright (c) 2014, Lakshman Anumolu
 * All rights reserved.
 *
 * This file is part of Basic Model Viewer whose distribution is governed by
 * the BSD-style license contained in the accompanying file license.txt.
 *
 * Description
 *	Helper functions and some data structures used in this package are defined
 *	here.
 */

#ifndef __HELPERFUNCTIONS_H__
#define __HELPERFUNCTIONS_H__

#include <stdio.h>

#include <QGenericMatrix>
#include <QMatrix4x4>
#include <QVector3D>
#include <QVector4D>

/* Data structures */

struct Light {
	QVector3D position = QVector3D(0.0, 0.0, 10.0);
	QVector3D color_ambient = QVector3D(0.5, 0.5, 0.5);
	QVector3D color_diffuse = QVector3D(0.5, 0.5, 0.5);
	QVector3D color_specular = QVector3D(1.0, 1.0, 1.0);
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
