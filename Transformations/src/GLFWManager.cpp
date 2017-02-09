/*
 * GLFWManager.cpp
 *
 *  Created on: 07/08/2016
 *      Author: rey
 */
#include <GL/glew.h>
#include "Headers/GLFWManager.h"

int WindowManager::screenWidth = 800;
int WindowManager::screenHeight = 700;
InputManager WindowManager::inputManager;
double GLFWManager::deltaTime;

GLFWManager::GLFWManager() :
		window(nullptr) {
}

GLFWManager::~GLFWManager() {
	destroy();
}

bool GLFWManager::initialize(int width, int height, std::string strTitle,
		bool bFullScreen) {
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return false;
	}

	this->screenWidth = width;
	this->screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
				glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
				nullptr);

	if (window == nullptr) {
		std::cerr
				<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
				<< std::endl;
		destroy();
		return false;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		return false;
	}

	return true;
}

bool GLFWManager::processInput(bool continueApplication = true) {
	if (glfwWindowShouldClose(window) != 0 || inputManager.isExitApp()) {
		return false;
	}
	deltaTime = 1 / TimeManager::Instance().CalculateFrameRate(false);
	//std::cout << "Frame rate:" << deltaTime << std::endl;
	glfwPollEvents();
	return continueApplication;
}

void GLFWManager::swapTheBuffers() {
	glfwSwapBuffers(window);
}

void GLFWManager::destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void GLFWManager::reshapeCallback(GLFWwindow* Window, int widthRes,
		int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void GLFWManager::keyCallback(GLFWwindow* window, int key, int scancode,
		int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		inputManager.keyPressed(InputCodes::kEscape, deltaTime);
}

void GLFWManager::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	inputManager.mouseMoved(xpos, ypos);
}

void GLFWManager::mouseButtonCallback(GLFWwindow* window, int button, int state,
		int mod) {
	if (button == GLFW_MOUSE_BUTTON_RIGHT && state == GLFW_PRESS)
		inputManager.mouseClicked(MouseCodes::button_right);
	else if (button == GLFW_MOUSE_BUTTON_LEFT && state == GLFW_PRESS)
		inputManager.mouseClicked(MouseCodes::button_left);
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE && state == GLFW_PRESS)
		inputManager.mouseClicked(MouseCodes::button_middle);
}
