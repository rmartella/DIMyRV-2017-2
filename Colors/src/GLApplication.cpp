/*
 * GLApplication.cpp
 *
 *  Created on: 07/08/2016
 *      Author: rey
 */

#include "Headers/GLApplication.h"

Shader lightingShader;
Shader lampShader;

GLuint VAO, VBO, EBO;

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
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	lightingShader.initialize("Shaders/lightColorShader.vs",
			"Shaders/lightColorShader.fs");
	lampShader.initialize("Shaders/lampShader.vs", "Shaders/lampShader.fs");

	VertexColor vertex[24] = {
	// Face front
			{ glm::vec3(1, 1, 1), glm::vec3(1.0, 0.0, 0.0) }, { glm::vec3(-1, 1,
					1), glm::vec3(0.0, 1.0, 0.0) }, { glm::vec3(-1, -1, 1),
					glm::vec3(0.0, 0.0, 1.0) }, { glm::vec3(1, -1, 1),
					glm::vec3(1.0, 1.0, 0.0) },
			// Face right
			{ glm::vec3(1, 1, -1), glm::vec3(0.0, 0.0, 1.0) }, { glm::vec3(1, 1,
					1), glm::vec3(1.0, 0.0, 1.0) }, { glm::vec3(1, -1, 1),
					glm::vec3(0.0, 1.0, 0.0) }, { glm::vec3(1, -1, -1),
					glm::vec3(0.0, 1.0, 1.0) },
			// Face back
			{ glm::vec3(-1, 1, -1), glm::vec3(0.0, 1.0, 1.0) }, { glm::vec3(1,
					1, -1), glm::vec3(1.0, 0.0, 1.0) }, { glm::vec3(1, -1, -1),
					glm::vec3(1.0, 0.0, 1.0) }, { glm::vec3(-1, -1, -1),
					glm::vec3(1.0, 0.0, 1.0) },
			//Face Left
			{ glm::vec3(-1, 1, 1), glm::vec3(1.0, 0.0, 0.0) }, { glm::vec3(-1,
					1, -1), glm::vec3(1.0, 1.0, 0.0) }, { glm::vec3(-1, -1, -1),
					glm::vec3(0.0, 0.0, 1.0) }, { glm::vec3(-1, -1, 1),
					glm::vec3(0.0, 1.0, 0.0) },
			// Face up
			{ glm::vec3(1, 1, 1), glm::vec3(1.0, 0.0, 1.0) }, { glm::vec3(1, 1,
					-1), glm::vec3(0.0, 0.0, 1.0) }, { glm::vec3(-1, 1, -1),
					glm::vec3(1.0, 1.0, 0.0) }, { glm::vec3(-1, 1, 1),
					glm::vec3(0.0, 1.0, 1.0) },
			// Face down
			{ glm::vec3(1, -1, 1), glm::vec3(1.0, 0.0, 1.0) }, { glm::vec3(-1,
					-1, 1), glm::vec3(1.0, 1.0, 1.0) }, { glm::vec3(-1, -1, -1),
					glm::vec3(0.0, 1.0, 1.0) }, { glm::vec3(1, -1, -1),
					glm::vec3(1.0, 1.0, 0.0) } };

	GLuint index[36] = { 0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4, 8, 9, 10, 10, 11,
			8, 12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22,
			23, 20 };

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index,
	GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex[0]),
			(GLvoid*) 0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex[0]),
			(GLvoid*) sizeof(vertex[0].position));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO

}
void GLApplication::applicationLoop() {
	bool processInput = true;

	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

	while (processInput) {
		processInput = windowManager->processInput(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model;
		model = glm::scale(model, glm::vec3(0.3, 0.3, 0.3));
		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 projection;
		projection = glm::perspective(45.0f,
				(GLfloat) WindowManager::screenWidth
						/ (GLfloat) WindowManager::screenHeight, 0.1f, 100.0f);

		lightingShader.turnOn();

		GLint modelLoc = lightingShader.getUniformLocation("model");
		GLint viewLoc = lightingShader.getUniformLocation("view");
		GLint projLoc = lightingShader.getUniformLocation("projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		GLint objectColorLoc = lightingShader.getUniformLocation("objectColor");
		GLint lightColorLoc = lightingShader.getUniformLocation("lightColor");
		glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
		glUniform3f(lightColorLoc, 1.0f, 0.5f, 1.0f);

		glBindVertexArray(VAO);
		glDrawElements( GL_TRIANGLES, 36, GL_UNSIGNED_INT,
				(GLvoid*) (sizeof(GLuint) * 0));
		glBindVertexArray(0);

		lightingShader.turnOff();

		lampShader.turnOn();

		// Create transformations

		modelLoc = lampShader.getUniformLocation("model");
		viewLoc = lampShader.getUniformLocation("view");
		projLoc = lampShader.getUniformLocation("projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::translate(glm::mat4(), lightPos);
		model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);
		glDrawElements( GL_TRIANGLES, 36, GL_UNSIGNED_INT,
				(GLvoid*) (sizeof(GLuint) * 0));
		glBindVertexArray(0);

		lampShader.turnOff();

		windowManager->swapTheBuffers();
	}
}

void GLApplication::destroy() {
	if (windowManager) {
		delete windowManager;
		windowManager = nullptr;
	}
}
