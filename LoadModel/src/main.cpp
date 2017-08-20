/*
 * main.cpp
 *
 *  Created on: 07/08/2016
 *      Author: rey
 */
#include "Headers/GLApplication.h"
#include "Headers/GLFWManager.h"

int main(int argc, char ** argv) {
	GLFWManager * ptrWindowManager = new GLFWManager();

	CameraFPS * camera = new CameraFPS(glm::vec3(0.0, 0.0, 3.0));

	ptrWindowManager->inputManager.setCamera(camera);

	GLApplication application;
	application.setWindowManager(ptrWindowManager);
	application.setCamera(camera);
	application.GLMain();

	return EXIT_SUCCESS;
}

