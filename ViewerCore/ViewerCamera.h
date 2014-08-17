/*
 * Copyright (c) 2014, Lakshman Anumolu
 * All rights reserved.
 *
 * This file is part of Basic Model Viewer whose distribution is governed by
 * the BSD-style license contained in the accompanying file license.txt.
 *
 * Description
 *  Class to control camera and its motion.
 *
 * Acknowledgements
 *	This class was originally written by Hammad Mazhar
 *	(https://github.com/hmazhar). Additional functionalities are added here
 *	along with new functions that are not available in Qt5, (e.g. UnProjectf).
 */

#ifndef __VIEWERCAMERA_H__
#define __VIEWERCAMERA_H__

#include <QQuaternion>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>

enum CameraType {
	ONSPHERE, FREE
};
enum CameraDirection {
	UP, DOWN, LEFT, RIGHT, FORWARD, BACK
};

class ViewerCamera {
public:
	ViewerCamera();
	~ViewerCamera();

	void Reset();

	//This function updates the camera
	//Depending on the current camera mode, the projection and viewport matricies are computed
	//Then the position and location of the camera is updated
	void Update();

	//Given a specific moving direction, the camera will be moved in the appropriate direction
	//For a spherical camera this will be around the look_at point
	//For a free camera a delta will be computed for the direction of movement.
	void Move(CameraDirection dir);
	//Change the pitch (up, down) for the free camera
	void ChangePitch(float degrees);
	//Change heading (left, right) for the free camera
	void ChangeHeading(float degrees);

	//Change the heading and pitch of the camera based on the 2d movement of the mouse
	void Move2D(int x, int y);

	//Setting Functions
	//Changes the camera mode, only three valid modes, Ortho, Free, and Spherical
	void SetMode(CameraType cam_mode);
	//Set the position of the camera
	void SetPosition(QVector3D pos);
	//Set's the look at point for the camera
	void SetLookAt(QVector3D pos);

	// Set camera up vector
	void SetUpVector(QVector3D up_vec_);
	
	//Changes the Field of View (FOV) for the camera
	void SetFOV(double fov);
	//Change the viewport location and size
	void SetViewport(int loc_x, int loc_y, int width, int height);
	//Change the clipping distance for the camera
	void SetClipping(double near_clip_distance, double far_clip_distance);

	void SetDistance(double cam_dist);
	void SetPos(int button, int state, int x, int y);

	//Getting Functions
	CameraType GetMode();
	void GetViewport(int &loc_x, int &loc_y, int &width, int &height);
	void GetMatrices(QMatrix4x4 &P, QMatrix4x4 &V, QMatrix4x4 &M);
	QVector3D GetCameraPosition();
	QVector3D GetCameraLookAt();
	QVector3D GetCameraUpVector();

	// Implementation of unproject function
	/* source: http://www.opengl.org/wiki/GluProject_and_gluUnProject_code */
	/* This function is NOT TESTED */
	int UnProjectf(QVector3D pos_on_window, QVector4D &viewport, QVector3D &unproj_pos);

	CameraType camera_mode;

	int viewport_x;
	int viewport_y;

	int window_width;
	int window_height;

	double aspect;
	double field_of_view;
	double near_clip;
	double far_clip;

	float camera_scale;
	float camera_heading;
	float camera_pitch;
	float camera_mouse_scale;

	float max_pitch_rate;
	float max_heading_rate;
	bool move_camera;

	float angle_step;

	QVector3D camera_position;
	QVector3D camera_position_delta;
	QVector3D camera_look_at;
	QVector3D camera_direction;

	QVector3D camera_up;
	QQuaternion rotation_quaternion;
	QVector3D mouse_position;

	QMatrix4x4 projection;
	QMatrix4x4 view;
	QMatrix4x4 model;
	QMatrix4x4 MVP;
	QMatrix4x4 modelview_mat;
};

#endif // END OF __VIEWERCAMERA_H__
