/*
 * definition.h
 *
 *  Created on: 06/09/2016
 *      Author: rey
 */

#ifndef HEADERS_DEFINITION_H_
#define HEADERS_DEFINITION_H_

#include <glm/glm.hpp>
#include <GL/glew.h>

typedef struct _VertexColor {

	_VertexColor() {
	}

	_VertexColor(glm::vec3 position, glm::vec3 color) {
		this->position = position;
		this->color = color;
	}

	glm::vec3 position;
	glm::vec3 color;
} VertexColor;

typedef struct _VertexColorNormal {

	_VertexColorNormal() {
	}

	_VertexColorNormal(glm::vec3 position, glm::vec3 colorl, glm::vec3 normal) {
		this->position = position;
		this->color = colorl;
		this->normal = normal;
	}

	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 normal;
} VertexColorNormals;

#endif /* HEADERS_DEFINITION_H_ */
