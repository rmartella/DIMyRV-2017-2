/*
 * Sphere.cpp
 *
 *  Created on: 06/09/2016
 *      Author: rey
 */

#include "Headers/Sphere.h"

Sphere::Sphere(float ratio, int slices, int stacks) :
		ratio(ratio), slices(slices), stacks(stacks), VAO(0), VBO(0), EBO(0) {
}

Sphere::~Sphere() {

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDeleteBuffers(1, &EBO);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);

}

void Sphere::init() {
	vertex.resize(((slices + 1) * (stacks + 1)));
	index.resize((slices * stacks + slices) * 6);
	for (int i = 0; i <= stacks; ++i) {
		float V = i / (float) stacks;
		float phi = V * M_PI;

		for (int j = 0; j <= slices; ++j) {
			float U = j / (float) slices;
			float theta = U * M_PI * 2.0;

			float X = ratio * cos(theta) * sin(phi);
			float Y = ratio * cos(phi);
			float Z = ratio * sin(theta) * sin(phi);

			vertex[i * (slices + 1) + j].position = glm::vec3(X, Y, Z);
			vertex[i * (slices + 1) + j].color = glm::vec3(0.0f, 1.0f, 0.0f);
		}
	}

	for (int i = 0; i < slices * stacks + slices; ++i) {
		index[i * 6] = i;
		index[i * 6 + 1] = i + slices + 1;
		index[i * 6 + 2] = i + slices;
		index[i * 6 + 3] = i + slices + 1;
		index[i * 6 + 4] = i;
		index[i * 6 + 5] = i + 1;
	}
}

void Sphere::load() {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(glm::vec3) * 2,
			vertex.data(),
			GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLuint),
			index.data(),
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

void Sphere::render() {

	glBindVertexArray(VAO);
	glDrawElements( GL_LINES, index.size(), GL_UNSIGNED_INT,
			(GLvoid*) (sizeof(GLuint) * 0));
	glBindVertexArray(0);

}

