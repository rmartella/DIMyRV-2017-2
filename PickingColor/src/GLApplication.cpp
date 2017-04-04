/*
 * GLApplication.cpp
 *
 *  Created on: 07/08/2016
 *      Author: rey
 */

#include "Headers/GLApplication.h"

GLApplication::GLApplication() :
		windowManager(nullptr), camera(nullptr) {
}

GLApplication::~GLApplication() {
	destroy();
}

void GLApplication::GLMain() {
	initialize();
	applicationLoop();
}

void GLApplication::initialize() {
	if (!windowManager
			|| !windowManager->initialize(800, 700, "Window GLFW", false)) {
		this->destroy();
		exit(-1);
	}

	glViewport(0, 0, WindowManager::screenWidth, WindowManager::screenHeight);
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	shader1.initialize("Shaders/lightingSpecularMap.vs",
			"Shaders/lightingSpecularMap.fs");

	shaderSelection.initialize("Shaders/selectionShader.vs",
			"Shaders/selectionShader.fs");

	objModel.loadModel("objects/nanosuit/nanosuit.obj");
	objModel2.loadModel("objects/cyborg/cyborg.obj");

}
void GLApplication::applicationLoop() {
	bool processInput = true;

	glm::vec3 lightPos(0.0f, 0.0f, 10.0f);

	while (processInput) {
		processInput = windowManager->processInput(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::vec4 viewport = glm::vec4(0.0f, 0.0f, WindowManager::screenWidth,
				WindowManager::screenHeight);

		// Transformation matrices
		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 projection;
		projection = glm::perspective(45.0f,
				(GLfloat) WindowManager::screenWidth
						/ (GLfloat) WindowManager::screenHeight, 0.1f, 100.0f);

		shader1.turnOn();

		GLint viewPosLoc = shader1.getUniformLocation("viewPos");
		glUniform3f(viewPosLoc, camera->Position.x, camera->Position.y,
				camera->Position.z);

		// Set material properties
		GLint matDiffuseLoc = shader1.getUniformLocation("material.diffuse");
		GLint matSpecularLoc = shader1.getUniformLocation("material.specular");
		GLint matShineLoc = shader1.getUniformLocation("material.shininess");
		glUniform1i(matDiffuseLoc, 0);
		glUniform1i(matSpecularLoc, 1);
		glUniform1f(matShineLoc, 32.0f);

		// Set lights properties
		GLint lightAmbientLoc = shader1.getUniformLocation("light.ambient");
		GLint lightDiffuseLoc = shader1.getUniformLocation("light.diffuse");
		GLint lightSpecularLoc = shader1.getUniformLocation("light.specular");
		GLint lightPosLoc = shader1.getUniformLocation("light.position");
		glUniform3f(lightAmbientLoc, 0.2f, 0.2f, 0.2f);
		glUniform3f(lightDiffuseLoc, 0.5f, 0.5f, 0.5f); // Let's darken the light a bit to fit the scene
		glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);

		// Get the uniform locations
		GLint modelLoc = shader1.getUniformLocation("model");
		GLint viewLoc = shader1.getUniformLocation("view");
		GLint projLoc = shader1.getUniformLocation("projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Draw the loaded model
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(-2.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		objModel.render(&shader1);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(2.0f, -1.75f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		objModel2.render(&shader1);

		shader1.turnOff();

		windowManager->swapTheBuffers();

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderSelection.turnOn();

		// Get the uniform locations
		modelLoc = shaderSelection.getUniformLocation("model");
		viewLoc = shaderSelection.getUniformLocation("view");
		projLoc = shaderSelection.getUniformLocation("projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Draw the loaded model
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-2.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		GLint codeLoc = shaderSelection.getUniformLocation("code");
		glUniform1i(codeLoc, 1);

		objModel.render(&shaderSelection);

		codeLoc = shaderSelection.getUniformLocation("code");
		glUniform1i(codeLoc, 2);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(2.0f, -1.75f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		objModel2.render(&shaderSelection);

		shaderSelection.turnOff();

		if (windowManager->inputManager.isReadPixelBuffer()) {
			std::cout << "Read pixel buffer:" << std::endl;
			unsigned char res[4];
			glReadPixels(windowManager->inputManager.getLastMousePos().x,
					WindowManager::screenHeight
							- windowManager->inputManager.getLastMousePos().y,
					1, 1, GL_RGB, GL_UNSIGNED_BYTE, res);
			switch (res[0]) {
			case 0:
				printf("Nothing Picked \n");
				break;
			case 1:
				printf("Picked nanosuit\n");
				break;
			case 2:
				printf("Picked cyborg\n");
				break;
			default:
				printf("Res: %d\n", res[0]);
			}
			windowManager->inputManager.setReadPixelBuffer(false);
		}

		//windowManager->swapTheBuffers();

		glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	}
}

void GLApplication::destroy() {
	if (windowManager) {
		delete windowManager;
		windowManager = nullptr;
	}

	objModel.destroy();
	objModel2.destroy();

}
