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

#include "ViewerWindow.h"

#include <QMouseEvent>
#include <QVector3D>
#include <QVBoxLayout>
#include <QPushButton>

#include <math.h>

ViewerWindow::ViewerWindow(const QGLFormat &format, QWidget *parent) :
    QGLWidget(format, parent),
    angularSpeed(0)
{
    cam_type = ONSPHERE;
    cam_position_initial = QVector3D(1.599270, 1.355425, 1.289346); //QVector3D(36.402737, 41.488392, 30.879433); //QVector3D(20.0, 0.0, 60.0);
    cam_lookat_initial = QVector3D(0.0, 0.0, 0.0);
    cam_up_initial = QVector3D(-0.524259, 0.731961, -0.435155); //QVector3D(-0.493296, 0.754725, -0.432489); //QVector3D(0.0, 1.0, 0.0);
    this->camera_position = this->cam_position_initial;
    this->camera_lookat = this->cam_lookat_initial;
    this->camera_up = this->cam_up_initial;
    fov_init = 14.000000; //45.0;
    fov = fov_init;
    fov_min = 10.0;
    fov_max = 120.0;
    near_clip_plane = 0.01;
    far_clip_plane = 500.0;
    xRot = 0;
    yRot = 0;
    zRot = 0;

    time_ = clock();
    angle_ = 0;
}

ViewerWindow::~ViewerWindow()
{
    deleteTexture(texture_t);
}

void ViewerWindow::timerEvent(QTimerEvent *)
{
    this->time_ = clock() - this->time_;
    this->angle_ += -500.0*(float)this->time_/CLOCKS_PER_SEC;
    this->time_ = clock();
    //qDebug() << "angle_ = " << this->angle_;
    updateGL();
}

void ViewerWindow::initializeGL()
{
    //qDebug() << "Widget OpenGL: " << format().majorVersion() << "." << format().minorVersion();

    initializeOpenGLFunctions();
    qglClearColor(Qt::black);
    initializeShaders();
    initializeTextures("cube", "png");

    // Camera initialization
    this->camera.SetMode(cam_type);
    this->camera.SetPosition(cam_position_initial);
    this->camera.SetLookAt(cam_lookat_initial);
    this->camera.SetUpVector(cam_up_initial);
    this->camera.SetFOV(fov);
    this->camera.SetViewport(0, 0, 640, 480);
    this->camera.SetClipping(near_clip_plane, far_clip_plane);

    glEnable(GL_DEPTH_TEST); /* Enable depth buffer */
    glEnable(GL_CULL_FACE); /* Enable back face culling */

    this->render_scene.InitializeScene();

    // Start timer
    timer.start(12, this);
}

void ViewerWindow::resizeGL(int w, int h)
{
    // Set OpenGL viewport to cover whole widget
    glViewport(0, 0, w, h);

    this->camera.SetViewport(0, 0, w, h);

    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);
    aspect_ratio = aspect;
}

void ViewerWindow::paintGL()
{
    /* Clear color and depth buffer */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Update camera */
    this->camera.Update();

    // Get camera details
    this->camera_position = this->camera.GetCameraPosition();
    this->camera_lookat = this->camera.GetCameraLookAt();
    this->camera_up = this->camera.GetCameraUpVector();

    /* Get model, view, projection matrices */
    QMatrix4x4 model, view;
    this->projection.setToIdentity();
    this->camera.GetMatrices(projection, view, model);
    QMatrix4x4 modelview_cam = view * model;

    /* Following rotations are implemented for current right click action.
     * TODO: Move camera to different base sphere on right click.
     */
    modelview_cam.rotate(xRot, QVector3D(1.0, 0.0, 0.0)); // Rotation with right click
    modelview_cam.rotate(yRot, QVector3D(0.0, 1.0, 0.0)); // Rotation with right click

    QMatrix4x4 modelview = modelview_cam;

    modelview.rotate(this->angle_, QVector3D(0.0, 1.0, 0.0));
    if (!this->render_scene.Draw(projection, modelview_cam, modelview, shaders_list.default_shaders[0]))
        close();

    QVector2D size = QVector2D(640, 480);
}

void ViewerWindow::closeEvent(QCloseEvent *e)
{
    this->render_scene.TakeDown();
    e->accept();
}

void ViewerWindow::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mousePressPosition = QVector2D(e->localPos());
}

void ViewerWindow::mouseReleaseEvent(QMouseEvent *e)
{
    // Mouse release position - mouse press position
    QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    qreal acc = diff.length() / 100.0;

    // Calculate new rotation axis as weighted sum
    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

    // Increase angular speed
    angularSpeed += acc;
}

void ViewerWindow::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->x() - lastPos.x();
    int dy = e->y() - lastPos.y();

    if (e->buttons() & Qt::LeftButton) {
        if (dx > 0) {
            this->camera.Move(LEFT);
        }
        if (dx < 0) {
            this->camera.Move(RIGHT);
        }
        if (dy > 0) {
            this->camera.Move(FORWARD);
        }
        if (dy < 0) {
            this->camera.Move(BACK);
        }
    } else if (e->buttons() & Qt::MiddleButton) {
        /*
         * TODO: Implement some action for this event.
         */
        /*this->camera.SetMode(FREE);
        if (dx > 0) {
            this->camera.Move(LEFT);
        }
        if (dx < 0) {
            this->camera.Move(RIGHT);
        }
        if (dy > 0) {
            this->camera.Move(UP);
        }
        if (dy < 0) {
            this->camera.Move(DOWN);
        }
        this->camera.Update();
        this->camera.SetMode(cam_type);*/
    } else if (e->buttons() & Qt::RightButton) {
        /*
         * TODO: Move camera to different base sphere on right click.
         */
        if (dx > 0)
            setYRotation(1);
        if (dx < 0)
            setYRotation(-1);
        if (dy > 0)
            setXRotation(1);
        if (dy < 0)
            setXRotation(-1);
    }
    lastPos = e->pos();
    updateGL();
    e->accept();
}

void ViewerWindow::wheelEvent(QWheelEvent *e)
{
    fov -= e->delta() / 120;
    if (fov > fov_max)
        fov = fov_max;
    else if (fov < fov_min)
        fov = fov_min;

    this->camera.SetFOV(fov);
    updateGL();
    e->accept();
}

void ViewerWindow::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Escape:
        close();
        break;

    case Qt::Key_C:
        SaveCameraDetails(this->camera_position, this->camera_lookat, this->camera_up, this->fov);
        Light light_;
        SaveLightDetails(light_);
        break;
    }
}

void ViewerWindow::initializeShaders()
{
    if (!this->shaders_list.InitializeDefaultShaders())
        close();

}

void ViewerWindow::initializeTextures(QString img_name, QString img_extension)
{
    QString img_path = ":/";
    img_path.append(img_name);
    img_path.append(".");
    img_path.append(img_extension);

    // Load cube.png image
    glEnable(GL_TEXTURE_2D);
    texture_t = bindTexture(QImage(img_path));

    // Set nearest filtering mode for texture minification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Set bilinear filtering mode for texture magnification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void ViewerWindow::setXRotation(int angle)
{
    //qNormalizeAngle(angle);
    xRot += angle;
    /*if (angle != xRot)
        xRot += angle;*/
}

void ViewerWindow::setYRotation(int angle)
{
    //qNormalizeAngle(angle);
    yRot += angle;
    /*if (angle != yRot)
        yRot += angle;*/
}

void ViewerWindow::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot)
        zRot += angle;
}
