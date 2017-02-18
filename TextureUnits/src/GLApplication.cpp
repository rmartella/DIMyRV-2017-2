/*
 * GLApplication.cpp
 *
 *  Created on: 07/08/2016
 *      Author: rey
 */

#include "Headers/GLApplication.h"

GLuint VBO, VAO, EBO;
Shader shader;

Texture * texture1;
Texture * texture2;

struct VertexColorTexture {
	glm::vec3 m_Pos;
	glm::vec3 m_Color;
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

	shader.initialize("Shaders/texture.vs", "Shaders/texture.fs");

	// Set up vertex data (and buffer(s)) and attribute pointers
	VertexColorTexture vertices[4] = { { glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(
			1.0f, 0.0f, 0.0f), glm::vec2 { 1.0f, 1.0f } }, // 0
			{ glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
					glm::vec2 { 1.0f, 0.0f } }, // 1
			{ glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),
					glm::vec2 { 0.0f, 0.0f } }, // 2
			{ glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f),
					glm::vec2 { 0.0f, 1.0f } }, // 3
			};
	GLuint indices[] = {  // Note that we start from 0!
			0, 1, 3, // First Triangle
					1, 2, 3  // Second Triangle
			};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
	GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]),
			(GLvoid*) 0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]),
			(GLvoid*) sizeof(vertices[0].m_Pos));
	glEnableVertexAttribArray(1);
	// Texture Mapping attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]),
			(GLvoid*) (sizeof(vertices[0].m_Color) + sizeof(vertices[0].m_Pos)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

	texture1 = new Texture(GL_TEXTURE_2D, "Textures/awesomeface.png");
	texture1->load();

	texture2 = new Texture(GL_TEXTURE_2D, "Textures/vertex_array_objects.png");
	texture2->load();

}

void GLApplication::applicationLoop() {
	bool processInput = true;
	while (processInput) {
		processInput = windowManager->processInput(true);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		shader.turnOn();

		// Bind Texture
		texture1->bind(GL_TEXTURE0);
		glUniform1i(shader.getUniformLocation("ourTexture1"), 0);
		texture2->bind(GL_TEXTURE1);
		glUniform1i(shader.getUniformLocation("ourTexture2"), 1);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

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
