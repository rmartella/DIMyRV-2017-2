/*
 * GLFWManager.h
 *
 *  Created on: 07/08/2016
 *      Author: rey
 */

#ifndef GLFWMANAGER_H_
#define GLFWMANAGER_H_

#include <GLFW/glfw3.h>

#include "WindowManager.h"
#include "TimeManager.h"

class GLFWManager: public WindowManager {
public:
	GLFWManager();
	virtual ~GLFWManager();

	virtual bool initialize(int width, int height, std::string strTitle,
			bool isFullScreen);
	virtual void swapTheBuffers();
	virtual bool processInput(bool continueApplication);
	virtual void destroy();

	static void reshapeCallback(GLFWwindow* Window, int widthRes,
			int heightRes);
protected:
	GLFWwindow * window;
};

#endif /* GLFWMANAGER_H_ */
