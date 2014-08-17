/*
 * Copyright (c) 2014, Lakshman Anumolu
 * All rights reserved.
 *
 * This file is part of Basic Model Viewer whose distribution is governed by
 * the BSD-style license contained in the accompanying file license.txt.
 *
 * Description
 *	Contains functions to report errors.
 */

#ifndef __CHECKERRORS_H__
#define __CHECKERRORS_H__

#include <QOpenGLFunctions>
#include <QString>

static bool GLReturnedError(QString s)
{
	bool return_error = false;
//#ifdef _DEBUG
	GLenum glerror;

	while ((glerror = glGetError()) != GL_NO_ERROR)
	{
	return_error = true;
	//cerr << s << ": " << gluErrorString(glerror) << endl;
	}
//#endif
	return return_error;
}

#endif // END OF __CHECKERRORS_H__
