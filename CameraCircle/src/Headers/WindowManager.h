/*
 * WindowManager.h
 *
 *  Created on: 07/08/2016
 *      Author: rey
 */

#ifndef WINDOWMANAGER_H_
#define WINDOWMANAGER_H_

//std includes
#include <string>
#include <iostream>

#include "InputManager.h"

class WindowManager {
public:

	static int screenWidth;
	static int screenHeight;
	static InputManager inputManager;

	virtual ~WindowManager() {
	}

	virtual bool initialize(int width, int height, std::string strTitle,
			bool isFullScreen) = 0;
	virtual bool processInput(bool continueApplication) = 0;
	virtual void swapTheBuffers() = 0;
	virtual void destroy() = 0;

};

#endif /* WINDOWMANAGER_H_ */
