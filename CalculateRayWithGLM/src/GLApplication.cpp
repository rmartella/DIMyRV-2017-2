/*
 * GLApplication.cpp
 *
 *  Created on: 07/08/2016
 *      Author: rey
 */

#include "Headers/GLApplication.h"

Shader shader;
Shader shader1;

GLuint VBO, EBO;

std::vector<GLuint> rays;

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

	shader.initialize("Shaders/lampShader.vs", "Shaders/lampShader.fs");
	shader1.initialize("Shaders/lightingSpecularMap.vs",
			"Shaders/lightingSpecularMap.fs");

}
void GLApplication::applicationLoop() {
	bool processInput = true;

	glm::vec3 lightPos(0.0f, 0.0f, 10.0f);
	Model objModel("objects/nanosuit/nanosuit.obj");

	while (processInput) {
		processInput = windowManager->processInput(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::vec4 viewport = glm::vec4(0.0f, 0.0f, WindowManager::screenWidth,
				WindowManager::screenHeight);

		shader.turnOn();

		// Transformation matrices
		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 projection;
		projection = glm::perspective(45.0f,
				(GLfloat) WindowManager::screenWidth
						/ (GLfloat) WindowManager::screenHeight, 0.1f, 100.0f);

		if (windowManager->inputManager.isGenerateRay()) {
			std::cout << "Generate ray:" << std::endl;

			glm::vec3 o =
					glm::unProject(
							glm::vec3(
									windowManager->inputManager.getLastMousePos().x,
									WindowManager::screenHeight
											- windowManager->inputManager.getLastMousePos().y,
									0.0f), camera->GetViewMatrix(), projection,
							viewport);
			glm::vec3 t =
					glm::unProject(
							glm::vec3(
									windowManager->inputManager.getLastMousePos().x,
									WindowManager::screenHeight
											- windowManager->inputManager.getLastMousePos().y,
									1.0f), camera->GetViewMatrix(), projection,
							viewport);

			windowManager->inputManager.setGenerateRay(false);

			GLuint VAO;

			VertexColor vertex[2] = { { o, glm::vec3(0.0) },
					{ t, glm::vec3(0.0) } };
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex,
			GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex[0]),
					(GLvoid*) 0);
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			rays.push_back(VAO);
		}

		// Get the uniform locations
		GLint modelLoc = shader.getUniformLocation("model");
		GLint viewLoc = shader.getUniformLocation("view");
		GLint projLoc = shader.getUniformLocation("projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Draw the loaded model
		glm::mat4 model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		for (int i = 0; i < rays.size(); i++) {
			glBindVertexArray(rays[i]);
			glDrawArrays(GL_LINES, 0, 2);
		}

		shader.turnOff();

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
		modelLoc = shader1.getUniformLocation("model");
		viewLoc = shader1.getUniformLocation("view");
		projLoc = shader1.getUniformLocation("projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Draw the loaded model
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		objModel.render(&shader1);

		shader1.turnOff();

		windowManager->swapTheBuffers();
	}
}

void GLApplication::destroy() {
	if (windowManager) {
		delete windowManager;
		windowManager = nullptr;
	}

	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDeleteBuffers(1, &EBO);

	glBindVertexArray(0);

	for (int i = 0; i < rays.size(); i++) {
		glDeleteVertexArrays(1, &rays[i]);
	}

}
