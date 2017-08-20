/*
 * GLApplication.cpp
 *
 *  Created on: 07/08/2016
 *      Author: rey
 */

#include "Headers/GLApplication.h"
#include "Headers/collision.h"

Sphere sphere(1.0, 25, 25, MODEL_MODE::VERTEX_LIGHT_TEXTURE);
Texture textureDifuse(GL_TEXTURE_2D, "Textures/container2.png");
Texture textureSpecular(GL_TEXTURE_2D, "Textures/container2_specular.png");

std::vector<GLuint> rays;
GLuint VBO, EBO;

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

	shader.initialize("Shaders/lightingSpecularMap.vs",
			"Shaders/lightingSpecularMap.fs");
	shader2.initialize("Shaders/lampShader.vs", "Shaders/lampShader.fs");
	sphere.init();
	sphere.load();

	textureDifuse.load();
	textureSpecular.load();

	objModel1.loadModel("objects/nanosuit/nanosuit.obj");
	objModel2.loadModel("objects/cyborg/cyborg.obj");

}
void GLApplication::applicationLoop() {
	bool processInput = true;

	glm::vec3 lightPos(0.0f, 0.0f, 10.0f);

	SBB sbb1 = getSBB(objModel1.getMeshes());
	SBB sbb2 = getSBB(objModel2.getMeshes());

	while (processInput) {
		processInput = windowManager->processInput(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::vec4 viewport = glm::vec4(0.0f, 0.0f, WindowManager::screenWidth,
				WindowManager::screenHeight);

		shader.turnOn();

		GLint viewPosLoc = shader.getUniformLocation("viewPos");
		glUniform3f(viewPosLoc, camera->Position.x, camera->Position.y,
				camera->Position.z);

		// Set material properties
		GLint matDiffuseLoc = shader.getUniformLocation("material.diffuse");
		GLint matSpecularLoc = shader.getUniformLocation("material.specular");
		GLint matShineLoc = shader.getUniformLocation("material.shininess");
		glUniform1i(matDiffuseLoc, 0);
		glUniform1i(matSpecularLoc, 1);
		glUniform1f(matShineLoc, 32.0f);

		// Set lights properties
		GLint lightAmbientLoc = shader.getUniformLocation("light.ambient");
		GLint lightDiffuseLoc = shader.getUniformLocation("light.diffuse");
		GLint lightSpecularLoc = shader.getUniformLocation("light.specular");
		GLint lightPosLoc = shader.getUniformLocation("light.position");
		glUniform3f(lightAmbientLoc, 0.2f, 0.2f, 0.2f);
		glUniform3f(lightDiffuseLoc, 0.5f, 0.5f, 0.5f); // Let's darken the light a bit to fit the scene
		glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);

		// Transformation matrices
		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 projection;
		projection = glm::perspective(45.0f,
				(GLfloat) WindowManager::screenWidth
						/ (GLfloat) WindowManager::screenHeight, 0.1f, 100.0f);
		// Get the uniform locations
		GLint modelLoc = shader.getUniformLocation("model");
		GLint viewLoc = shader.getUniformLocation("view");
		GLint projLoc = shader.getUniformLocation("projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Draw the loaded model
		glm::mat4 model1;
		model1 = glm::translate(model1, glm::vec3(0.0f, -4.0f, -1.0f));
		model1 = glm::scale(model1, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1));
		objModel1.render(&shader);

		glm::mat4 model2;
		model2 = glm::translate(model2, glm::vec3(3.0f, -4.0f, -1.0f));
		model2 = glm::scale(model2, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
		objModel2.render(&shader);

		shader.turnOff();

		shader2.turnOn();
		modelLoc = shader2.getUniformLocation("model");
		viewLoc = shader2.getUniformLocation("view");
		projLoc = shader2.getUniformLocation("projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		/*model1 = glm::mat4();
		 model1 = glm::translate(model1, glm::vec3(0.0f, -4.0f, 0.0f));
		 model1 = glm::scale(model1, glm::vec3(0.2f, 0.2f, 0.2f));*/
		model1 = glm::translate(model1,
				glm::vec3(sbb1.center.x, sbb1.center.y, sbb1.center.z));
		model1 = glm::scale(model1,
				glm::vec3(sbb1.ratio, sbb1.ratio, sbb1.ratio));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1));
		sphere.render();

		/*model2 = glm::mat4();
		 model2 = glm::translate(model2, glm::vec3(3.0f, -4.0f, 0.0f));
		 model2 = glm::scale(model2, glm::vec3(0.8f, 0.8f, 0.8f));*/
		model2 = glm::translate(model2,
				glm::vec3(sbb2.center.x, sbb2.center.y, sbb2.center.z));
		model2 = glm::scale(model2,
				glm::vec3(sbb2.ratio, sbb2.ratio, sbb2.ratio));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
		sphere.render();

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

			std::cout << "compute intersection" << std::endl;

			glm::vec3 c1 = glm::vec3(model1 * glm::vec4(0, 0, 0, 1));
			glm::vec3 c2 = glm::vec3(model2 * glm::vec4(0, 0, 0, 1));
			glm::vec3 dir = glm::normalize(t - o);
			float t1;

			glm::vec3 vDirToSphere = c1 - o;
			float fLineLength = glm::distance(o, t);
			t1 = glm::dot(vDirToSphere, dir);

			/*glm::vec3 vClosestPoint;

			 if (t1 <= 0.0f)
			 vClosestPoint = o;
			 // Si la proyección escalar del origen es mayor a distancia del origen
			 // al destino, el punto mas cercano es el destino.
			 else if (t1 >= fLineLength)
			 vClosestPoint = t;
			 // En caso contrario de calcula el punto sobre la linea usando t.
			 else
			 vClosestPoint = o + dir * (t1);

			 // Se pureba si el punto mas cercao esta contenido en el radio de la esfera.
			 if (glm::distance(c1, vClosestPoint) <= sbb1.ratio * 0.2f)
			 std::cout << "Picking" << std::endl;*/

			if (raySphereIntersect(o, t, dir, c1, sbb1.ratio * 0.2f, t1))
				std::cout << "Picking nanosuit" << std::endl;
			if (raySphereIntersect(o, t, dir, c2, sbb2.ratio * 0.8f, t1))
				std::cout << "Picking cyborg" << std::endl;
			windowManager->inputManager.setGenerateRay(false);
		}

		// For test collision sphere vs sphere
		SBB s1, s2;
		s1.center = glm::vec3(model1 * glm::vec4(0, 0, 0, 1));
		s1.ratio = sbb1.ratio * 0.2f;
		s2.center = glm::vec3(model2 * glm::vec4(0, 0, 0, 1));
		s2.ratio = sbb2.ratio * 0.8f;
		if (testSphereSphereIntersection(s1, s2))
			std::cout << "Model collision:" << std::endl;
		//testSphereSphereIntersection(s1, s2);

		// Get the uniform locations
		modelLoc = shader2.getUniformLocation("model");
		viewLoc = shader2.getUniformLocation("view");
		projLoc = shader2.getUniformLocation("projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Draw the loaded model
		glm::mat4 modelL = glm::mat4();
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelL));

		for (int i = 0; i < rays.size(); i++) {
			glBindVertexArray(rays[i]);
			glDrawArrays(GL_LINES, 0, 2);
		}

		shader2.turnOff();

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

	objModel1.destroy();
	objModel2.destroy();

}
