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

	GLApplication application;
	application.setWindowManager(ptrWindowManager);
	application.GLMain();

	return EXIT_SUCCESS;
}

