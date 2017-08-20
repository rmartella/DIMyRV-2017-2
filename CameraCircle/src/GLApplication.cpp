/*
 * GLApplication.cpp
 *
 *  Created on: 07/08/2016
 *      Author: rey
 */

#include "Headers/GLApplication.h"

GLuint VBO, VAO;
Shader shader;

Texture * texture1;
Texture * texture2;

struct VertexTexture {
	glm::vec3 m_Pos;
	glm::vec2 m_TexCoord;
};

GLApplication::GLApplication() :
		windowManager(nullptr) {
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
			|| !windowManager->initialize(800, 700, "Triangles", false)) {
		this->destroy();
		exit(-1);
	}

	glViewport(0, 0, WindowManager::screenWidth, WindowManager::screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	shader.initialize("Shaders/sistemascoordenados.vs",
			"Shaders/sistemascoordenados.fs");

	// Set up vertex data (and buffer(s)) and attribute pointers

	VertexTexture vertices[36] = { { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(
			0.0f, 0.0f) }, { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f,
			0.0f) }, { glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f) }, {
			glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f) }, { glm::vec3(
			-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 1.0f) }, { glm::vec3(-0.5f,
			-0.5f, -0.5f), glm::vec2(0.0f, 0.0f) }, { glm::vec3(-0.5f, -0.5f,
			0.5f), glm::vec2(0.0f, 0.0f) }, { glm::vec3(0.5f, -0.5f, 0.5f),
			glm::vec2(1.0f, 0.0f) }, { glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(
			1.0f, 1.0f) },
			{ glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f) }, { glm::vec3(
					-0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 1.0f) }, { glm::vec3(
					-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f) }, { glm::vec3(
					-0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f) }, { glm::vec3(
					-0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f) }, { glm::vec3(
					-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f) }, { glm::vec3(
					-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f) }, { glm::vec3(
					-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f) }, { glm::vec3(
					-0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f) }, { glm::vec3(
					0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f) }, { glm::vec3(
					0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f) }, { glm::vec3(
					0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f) }, { glm::vec3(
					0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f) }, { glm::vec3(
					0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f) }, { glm::vec3(
					0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f) }, { glm::vec3(
					-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f) }, { glm::vec3(
					0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 1.0f) }, { glm::vec3(
					0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f) }, { glm::vec3(
					0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f) }, { glm::vec3(
					-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f) }, { glm::vec3(
					-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f) }, { glm::vec3(
					-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 1.0f) }, { glm::vec3(
					0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f) }, { glm::vec3(
					0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f) }, { glm::vec3(
					0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f) }, { glm::vec3(
					-0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 0.0f) }, { glm::vec3(
					-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 1.0f) } };

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]),
			(GLvoid*) 0);
	glEnableVertexAttribArray(0);
	// Texture Mapping attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]),
			(GLvoid*) sizeof(vertices[0].m_Pos));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

	texture1 = new Texture(GL_TEXTURE_2D, "Textures/test.png");
	texture1->load();

	texture2 = new Texture(GL_TEXTURE_2D, "Textures/awesomeface.png");
	texture2->load();

}

void GLApplication::applicationLoop() {
	bool processInput = true;
	// World space positions of our cubes
	glm::vec3 cubePositions[] =
			{ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2.0f, 5.0f, -15.0f),
					glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f,
							-12.3f), glm::vec3(2.4f, -0.4f, -3.5f), glm::vec3(
							-1.7f, 3.0f, -7.5f), glm::vec3(1.3f, -2.0f, -2.5f),
					glm::vec3(1.5f, 2.0f, -2.5f), glm::vec3(1.5f, 0.2f, -1.5f),
					glm::vec3(-1.3f, 1.0f, -1.5f) };
	double lastTime = TimeManager::Instance().GetTime();
	while (processInput) {
		processInput = windowManager->processInput(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		shader.turnOn();

		GLfloat timeValue = TimeManager::Instance().GetTime() - lastTime;

		// Bind Texture
		texture1->bind(GL_TEXTURE0);
		glUniform1i(shader.getUniformLocation("ourTexture1"), 0);
		texture2->bind(GL_TEXTURE1);
		glUniform1i(shader.getUniformLocation("ourTexture2"), 1);

		// Create transformations
		glm::mat4 view;

		GLfloat radius = 10.0f;
		GLfloat camX = sin((GLfloat) timeValue * 1.0f) * radius;
		GLfloat camZ = cos((GLfloat) timeValue * 1.0f) * radius;
		view = glm::lookAt(glm::vec3(camX, 0.0f, camZ),
				glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 projection;
		projection = glm::perspective(45.0f,
				(GLfloat) WindowManager::screenWidth
						/ (GLfloat) WindowManager::screenHeight, 0.1f, 100.0f);
		// Get their uniform location
		shader.getUniformLocation("model");
		GLint modelLoc = shader.getUniformLocation("model");
		GLint viewLoc = shader.getUniformLocation("view");
		GLint projLoc = shader.getUniformLocation("projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		// Note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);
		for (GLuint i = 0; i < 10; i++) {
			// Calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			GLfloat angle = 20.0f * i;
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		shader.turnOff();

		windowManager->swapTheBuffers();
	}
}

void GLApplication::destroy() {
	if (windowManager) {
		delete windowManager;
		windowManager = nullptr;
	}

	shader.destroy();

	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBO);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);

	delete (texture1);
	delete (texture2);
}
