/*
 * GLApplication.h
 *
 *  Created on: 07/08/2016
 *      Author: rey
 */

#ifndef GLAPPLICATION_H_
#define GLAPPLICATION_H_

#include <stdlib.h>

#include <GL/glew.h>

#include <FreeImage.h>

#include "WindowManager.h"
#include "TimeManager.h"
#include "Shader.h"

class GLApplication {
public:

	GLApplication();
	~GLApplication();

	void GLMain();
	void initialize();
	void applicationLoop();
	void destroy();

	void setWindowManager(WindowManager * windowManager) {
		this->windowManager = windowManager;
	}

	WindowManager * getWindowManager() {
		return windowManager;
	}

protected:
	WindowManager * windowManager;

};

#endif /* GLAPPLICATION_H_ */
