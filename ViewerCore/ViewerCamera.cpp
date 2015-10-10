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
 * Acknowledgements
 *	This class was originally written by Hammad Mazhar
 *	(https://github.com/hmazhar). Additional functionalities are added here
 *	along with new functions that are not available in Qt5, (e.g. UnProjectf).
 */

#include "ViewerCamera.h"

ViewerCamera::ViewerCamera()
{
	camera_mode = FREE;

	field_of_view = 45;
	near_clip = 0.1;
	far_clip = 5;
	rotation_quaternion = QQuaternion();
	camera_position = QVector3D(0.5, 0.5, 1);
	camera_position_delta = QVector3D(0, 0, 0);
	camera_look_at = QVector3D(0, 0, 0);
	camera_direction = QVector3D(0, 1, 0);
	camera_up = QVector3D(0, 1, 0);
	camera_scale = .03f;
	camera_pitch = 0;
	camera_heading = 0;
	camera_mouse_scale = .02f;
	max_pitch_rate = 5;
	max_heading_rate = 5;
	move_camera = false;
	viewport_x = 0;
	viewport_y = 0;
	angle_step = 1.0f;
}

ViewerCamera::~ViewerCamera()
{
}

void ViewerCamera::Reset()
{
	camera_up = QVector3D(0, 1, 0);
}

void ViewerCamera::Update()
{
	camera_direction = (camera_look_at - camera_position).normalized();
	//need to set the matrix state. this is only important because lighting doesn't work if this isn't done
	//glViewport(viewport_x, viewport_y, window_width, window_height);

	if (camera_mode == FREE) {
		projection.setToIdentity();
		projection.perspective(field_of_view, aspect, near_clip, far_clip);
		//detmine axis for pitch rotation
		QVector3D axis = QVector3D::crossProduct(camera_direction, camera_up);
		//compute quaternion for pitch based on the camera pitch angle
		QQuaternion pitch_quat = QQuaternion::fromAxisAndAngle(axis, camera_pitch);
		//determine heading quaternion from the camera up vector and the heading angle
		QQuaternion heading_quat = QQuaternion::fromAxisAndAngle(camera_up, camera_heading);
		//add the two quaternions
		QQuaternion temp = pitch_quat + heading_quat;
		temp.normalize();
		//update the direction from the quaternion
		camera_direction = temp.rotatedVector(camera_direction);
		//add the camera delta
		camera_position += camera_position_delta;
		//set the look at to be infront of the camera
		camera_look_at = camera_position + camera_direction * 1.0f;
		//damping for smooth camera
		camera_heading *= .5;
		camera_pitch *= .5;
		camera_position_delta = camera_position_delta * .5f;
	} else if (camera_mode == ONSPHERE) {
		projection.setToIdentity();
		projection.perspective(field_of_view, aspect, near_clip, far_clip);
	}
	//compute the MVP
	view = QMatrix4x4();
	view.lookAt(camera_position, camera_look_at, camera_up);
	model = QMatrix4x4();
	modelview_mat = view * model;
	MVP = projection * modelview_mat;
}

//Setting Functions
void ViewerCamera::SetMode(CameraType cam_mode)
{
	camera_mode = cam_mode;
	camera_up = QVector3D(0, 1, 0);
	rotation_quaternion = QQuaternion();
}

void ViewerCamera::SetPosition(QVector3D pos)
{
	camera_position = pos;
}

void ViewerCamera::SetLookAt(QVector3D pos)
{
	camera_look_at = pos;
}

void ViewerCamera::SetUpVector(QVector3D up_vec_)
{
	this->camera_up = up_vec_;
}

void ViewerCamera::SetFOV(double fov)
{
	field_of_view = fov;
}

void ViewerCamera::SetViewport(
	int loc_x,
	int loc_y,
	int width,
	int height
	)
{
	viewport_x = loc_x;
	viewport_y = loc_y;
	window_width = width;
	window_height = height;
	//need to use doubles division here, it will not work otherwise and it is possible to get a zero aspect ratio with integer rounding
	aspect = double(width) / double(height);
}

void ViewerCamera::SetClipping(
	double near_clip_distance,
	double far_clip_distance
	)
{
	near_clip = near_clip_distance;
	far_clip = far_clip_distance;
}

void ViewerCamera::Move(CameraDirection dir)
{
	if (camera_mode == FREE) {
		switch (dir) {
		case UP:
			camera_position_delta += camera_up * camera_scale;
			break;
		case DOWN:
			camera_position_delta -= camera_up * camera_scale;
			break;
		case LEFT:
			camera_position_delta -= QVector3D::crossProduct(camera_direction, camera_up) * camera_scale;
			break;
		case RIGHT:
			camera_position_delta += QVector3D::crossProduct(camera_direction, camera_up) * camera_scale;
			break;
		case FORWARD:
			camera_position_delta += camera_direction * camera_scale;
			break;
		case BACK:
			camera_position_delta -= camera_direction * camera_scale;
			break;
		}
	} else if (camera_mode == ONSPHERE) {
		float angle = 0;
		QQuaternion rot_quat;
		QVector3D rot_vector;

		if(dir == BACK || dir == RIGHT) {
			angle = this->angle_step;
		} else if(dir == FORWARD || dir == LEFT) {
			angle = -this->angle_step;
		}
		if(dir == FORWARD || dir == BACK) {
			QVector3D cross_vector = camera_up;
			rot_vector = QVector3D::crossProduct(camera_direction, cross_vector);
		} else if (dir == LEFT || dir == RIGHT) {
			rot_vector = QVector3D(0, 1, 0);
		}

		rot_quat = QQuaternion::fromAxisAndAngle(rot_vector, angle);
		rotation_quaternion = rot_quat * rotation_quaternion;
		camera_position = rot_quat.rotatedVector(camera_position);
		camera_up = rot_quat.rotatedVector(camera_up);
	}
}

void ViewerCamera::ChangePitch(float degrees)
{
	//Check bounds with the max pitch rate so that we aren't moving too fast
	if (degrees < -max_pitch_rate) {
		degrees = -max_pitch_rate;
	} else if (degrees > max_pitch_rate) {
		degrees = max_pitch_rate;
	}
	camera_pitch += degrees * camera_mouse_scale;

	//Check bounds for the camera pitch
	if (camera_pitch > 360.0f) {
		camera_pitch -= 360.0f;
	} else if (camera_pitch < -360.0f) {
		camera_pitch += 360.0f;
	}
}

void ViewerCamera::ChangeHeading(float degrees)
{
	//Check bounds with the max heading rate so that we aren't moving too fast
	if (degrees < -max_heading_rate) {
		degrees = -max_heading_rate;
	} else if (degrees > max_heading_rate) {
		degrees = max_heading_rate;
	}
	//This controls how the heading is changed if the camera is pointed straight up or down
	//The heading delta direction changes
	if ((camera_pitch > 90 && camera_pitch < 270) || (camera_pitch < -90 && camera_pitch > -270)) {
		camera_heading -= degrees * camera_mouse_scale;
	} else {
		camera_heading += degrees * camera_mouse_scale;
	}
	//Check bounds for the camera heading
	if (camera_heading > 360.0f) {
		camera_heading -= 360.0f;
	} else if (camera_heading < -360.0f) {
		camera_heading += 360.0f;
	}
}

void ViewerCamera::Move2D(
	int x,
	int y
	)
{
	//compute the mouse delta from the previous mouse position
	QVector3D mouse_delta = mouse_position - QVector3D(x, y, 0);
	//if the camera is moving, meaning that the mouse was clicked and dragged, change the pitch and heading
	if (move_camera) {
		ChangeHeading(.08f * mouse_delta.x());
		ChangePitch(.08f * mouse_delta.y());
	}
	mouse_position = QVector3D(x, y, 0);
}

void ViewerCamera::SetPos(
	int button,
	int state,
	int x,
	int y
	)
{
	/* TODO: Make this compatible with qt version */
	/*if (button == 3 && state == GLFW_PRESS) {
		camera_position_delta += camera_up * .05f;
	} else if (button == 4 && state == GLFW_PRESS) {
		camera_position_delta -= camera_up * .05f;
	} else if (button == GLFW_MOUSE_BUTTON_LEFT && state == GLFW_PRESS) {
		move_camera = true;
	} else if (button == GLFW_MOUSE_BUTTON_LEFT && state == GLFW_RELEASE) {
		move_camera = false;
	}*/
	mouse_position = QVector3D(x, y, 0);
}

CameraType ViewerCamera::GetMode() {
	return camera_mode;
}

void ViewerCamera::GetViewport(
	int &loc_x,
	int &loc_y,
	int &width,
	int &height
	)
{
	loc_x = viewport_x;
	loc_y = viewport_y;
	width = window_width;
	height = window_height;
}

void ViewerCamera::GetMatrices(
	QMatrix4x4 &P,
	QMatrix4x4 &V,
	QMatrix4x4 &M
	)
{
   P = projection;
   V = view;
   M = model;
}

QVector3D ViewerCamera::GetCameraPosition()
{
	return this->camera_position;
}

QVector3D ViewerCamera::GetCameraLookAt()
{
	return this->camera_look_at;
}

QVector3D ViewerCamera::GetCameraUpVector()
{
	return this->camera_up;
}

int ViewerCamera::UnProjectf(QVector3D pos_on_window, QVector4D &viewport, QVector3D &unproj_pos)
{
	//QMatrix4x4 &modelview_ = modelview_mat;
	QMatrix4x4 &mvp_ = MVP;

	/* Inverse of MVP */
	bool invertible = 0;
	QMatrix4x4 inv_MVP = mvp_.inverted(&invertible);

	/* Transformation of normalized coordinates between -1 and 1 */
	float norm_0 = 2.0f * (pos_on_window.x() - viewport.x()) / viewport.z() - 1.0f;
	float norm_1 = 2.0f * (pos_on_window.y() - viewport.y()) / viewport.w() - 1.0f;
	float norm_2 = 2.0f * pos_on_window.z() - 1.0f;
	QVector4D normalized_coords(norm_0, norm_1, norm_2, 1.0f);

	/* Object coordinates */
	QVector4D obj_coords = inv_MVP * normalized_coords;
	if (obj_coords.w() == 0.0)
		return 0;

	unproj_pos.setX(obj_coords.x() * 1.0f / obj_coords.w());
	unproj_pos.setY(obj_coords.y() * 1.0f / obj_coords.w());
	unproj_pos.setZ(obj_coords.z() * 1.0f / obj_coords.w());
	return 1;
}
