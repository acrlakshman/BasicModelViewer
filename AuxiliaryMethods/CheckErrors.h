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
