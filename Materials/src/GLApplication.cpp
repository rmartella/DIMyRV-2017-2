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
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	VertexColorNormals vertex[24] =
			{
					// Face front
					{ glm::vec3(1, 1, 1), glm::vec3(1.0, 0.0, 0.0), glm::vec3(0,
							0, 1) }, { glm::vec3(-1, 1, 1), glm::vec3(0.0, 1.0,
							0.0), glm::vec3(0, 0, 1) }, { glm::vec3(-1, -1, 1),
							glm::vec3(0.0, 0.0, 1.0), glm::vec3(0, 0, 1) }, {
							glm::vec3(1, -1, 1), glm::vec3(1.0, 1.0, 0.0),
							glm::vec3(0, 0, 1) },
					// Face right
					{ glm::vec3(1, 1, -1), glm::vec3(0.0, 0.0, 1.0), glm::vec3(
							1, 0, 0) }, { glm::vec3(1, 1, 1), glm::vec3(1.0,
							0.0, 1.0), glm::vec3(1, 0, 0) }, { glm::vec3(1, -1,
							1), glm::vec3(0.0, 1.0, 0.0), glm::vec3(1, 0, 0) },
					{ glm::vec3(1, -1, -1), glm::vec3(0.0, 1.0, 1.0), glm::vec3(
							1, 0, 0) },
					// Face back
					{ glm::vec3(-1, 1, -1), glm::vec3(0.0, 1.0, 1.0), glm::vec3(
							0, 0, -1) }, { glm::vec3(1, 1, -1), glm::vec3(1.0,
							0.0, 1.0), glm::vec3(0, 0, -1) },
					{ glm::vec3(1, -1, -1), glm::vec3(1.0, 0.0, 1.0), glm::vec3(
							0, 0, -1) }, { glm::vec3(-1, -1, -1), glm::vec3(1.0,
							0.0, 1.0), glm::vec3(0, 0, -1) },
					//Face Left
					{ glm::vec3(-1, 1, 1), glm::vec3(1.0, 0.0, 0.0), glm::vec3(
							-1, 0, 0) }, { glm::vec3(-1, 1, -1), glm::vec3(1.0,
							1.0, 0.0), glm::vec3(-1, 0, 0) }, { glm::vec3(-1,
							-1, -1), glm::vec3(0.0, 0.0, 1.0), glm::vec3(-1, 0,
							0) }, { glm::vec3(-1, -1, 1), glm::vec3(0.0, 1.0,
							0.0), glm::vec3(-1, 0, 0) },
					// Face up
					{ glm::vec3(1, 1, 1), glm::vec3(1.0, 0.0, 1.0), glm::vec3(0,
							1, 0) }, { glm::vec3(1, 1, -1), glm::vec3(0.0, 0.0,
							1.0), glm::vec3(0, 1, 0) }, { glm::vec3(-1, 1, -1),
							glm::vec3(1.0, 1.0, .0), glm::vec3(0, 1, 0) }, {
							glm::vec3(-1, 1, 1), glm::vec3(0.0, 1.0, 1.0),
							glm::vec3(0, 1, 0) },
					// Face down
					{ glm::vec3(1, -1, 1), glm::vec3(1.0, 0.0, 1.0), glm::vec3(
							0, -1, 0) }, { glm::vec3(-1, -1, 1), glm::vec3(1.0,
							1.0, 1.0), glm::vec3(0, -1, 0) }, { glm::vec3(-1,
							-1, -1), glm::vec3(0.0, 1.0, 1.0), glm::vec3(0, -1,
							0) }, { glm::vec3(1, -1, -1), glm::vec3(1.0, 1.0,
							0.0), glm::vec3(0, -1, 0) } };

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
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex[0]),
			(GLvoid*) (sizeof(vertex[0].position) + sizeof(vertex[0].color)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO

	lightingShader.initialize("Shaders/materialLighting.vs",
			"Shaders/materialLighting.fs");
	lampShader.initialize("Shaders/lampShader.vs", "Shaders/lampShader.fs");

}
void GLApplication::applicationLoop() {
	bool processInput = true;

	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

	while (processInput) {
		processInput = windowManager->processInput(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model;
		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 projection;
		projection = glm::perspective(45.0f,
				(GLfloat) WindowManager::screenWidth
						/ (GLfloat) WindowManager::screenHeight, 0.1f, 100.0f);

		lightingShader.turnOn();

		GLint objectColorLoc = lightingShader.getUniformLocation("objectColor");
		GLint lightPosLoc = lightingShader.getUniformLocation("lightPos");
		GLint viewPosLoc = lightingShader.getUniformLocation("viewPos");
		glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(viewPosLoc, camera->Position.x, camera->Position.y,
				camera->Position.z);

		GLint matAmbientLoc = lightingShader.getUniformLocation(
				"material.ambient");
		GLint matDiffuseLoc = lightingShader.getUniformLocation(
				"material.diffuse");
		GLint matSpecularLoc = lightingShader.getUniformLocation(
				"material.specular");
		GLint matShineLoc = lightingShader.getUniformLocation(
				"material.shininess");
		glUniform3f(matAmbientLoc, 1.0f, 0.5f, 0.31f);
		glUniform3f(matDiffuseLoc, 1.0f, 0.5f, 0.31f);
		glUniform3f(matSpecularLoc, 0.5f, 0.5f, 0.5f);
		glUniform1f(matShineLoc, 32.0f);

		GLint modelLoc = lightingShader.getUniformLocation("model");
		GLint viewLoc = lightingShader.getUniformLocation("view");
		GLint projLoc = lightingShader.getUniformLocation("projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

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

		model = glm::scale(glm::mat4(), glm::vec3(0.5, 0.5, 0.5));
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2));
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

	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDeleteBuffers(1, &EBO);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);
}
