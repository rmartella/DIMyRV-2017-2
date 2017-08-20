/*
 * GLApplication.cpp
 *
 *  Created on: 07/08/2016
 *      Author: rey
 */

#include "Headers/GLApplication.h"

Sphere sphere(2.0, 25, 25, MODEL_MODE::VERTEX_LIGHT_TEXTURE);
Texture textureDifuse(GL_TEXTURE_2D, "Textures/container2.png");
Texture textureSpecular(GL_TEXTURE_2D, "Textures/container2_specular.png");

#define NUM_BUFFERS 3
#define NUM_SOURCES 3
#define NUM_ENVIRONMENTS 1

ALfloat listenerPos[] = { 0.0, 0.0, 4.0 };
ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
ALfloat listenerOri[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 0.0 };

ALfloat source0Pos[] = { -2.0, 0.0, 0.0 };
ALfloat source0Vel[] = { 0.0, 0.0, 0.0 };

ALfloat source1Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source1Vel[] = { 0.0, 0.0, 0.0 };

ALfloat source2Pos[] = { 0.0, 0.0, -4.0 };
ALfloat source2Vel[] = { 0.0, 0.0, 0.0 };

ALuint buffer[NUM_BUFFERS];
ALuint source[NUM_SOURCES];
ALuint environment[NUM_ENVIRONMENTS];

ALsizei size, freq;
ALenum format;
ALvoid *data;
int ch;
ALboolean loop;

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
	sphere.init();
	sphere.load();

	textureDifuse.load();
	textureSpecular.load();

	alutInit(0, NULL);

	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_VELOCITY, listenerVel);
	alListenerfv(AL_ORIENTATION, listenerOri);

	alGetError(); // clear any error messages

	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating buffers !!\n");
		exit(1);
	} else {
		printf("init() - No errors yet.");
	}

	// Generate buffers, or else no sound will happen!
	alGenBuffers(NUM_BUFFERS, buffer);

	buffer[0] = alutCreateBufferFromFile("sounds/lawyer1.wav");
	//buffer[0] = alutCreateBufferHelloWorld();

	alGetError(); /* clear error */
	alGenSources(NUM_SOURCES, source);

	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating sources !!\n");
		exit(2);
	} else {
		printf("init - no errors after alGenSources\n");
	}

	alSourcef(source[0], AL_PITCH, 1.0f);
	alSourcef(source[0], AL_GAIN, 1.0f);
	alSourcefv(source[0], AL_POSITION, source0Pos);
	alSourcefv(source[0], AL_VELOCITY, source0Vel);
	alSourcei(source[0], AL_BUFFER, buffer[0]);
	alSourcei(source[0], AL_LOOPING, AL_TRUE);
	alSourcef(source[0], AL_MAX_DISTANCE, 1200);

	objModel.loadModel("objects/nanosuit/nanosuit.obj");

	/*alSourcef(source[1], AL_PITCH, 1.0f);
	 alSourcef(source[1], AL_GAIN, 1.0f);
	 alSourcefv(source[1], AL_POSITION, source1Pos);
	 alSourcefv(source[1], AL_VELOCITY, source1Vel);
	 alSourcei(source[1], AL_BUFFER, buffer[1]);
	 alSourcei(source[1], AL_LOOPING, AL_TRUE);

	 alSourcef(source[2], AL_PITCH, 1.0f);
	 alSourcef(source[2], AL_GAIN, 1.0f);
	 alSourcefv(source[2], AL_POSITION, source2Pos);
	 alSourcefv(source[2], AL_VELOCITY, source2Vel);
	 alSourcei(source[2], AL_BUFFER, buffer[2]);
	 alSourcei(source[2], AL_LOOPING, AL_TRUE);*/

}
void GLApplication::applicationLoop() {
	bool processInput = true;

	glm::vec3 lightPos(0.0f, 0.0f, 10.0f);

	while (processInput) {
		processInput = windowManager->processInput(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		objModel.render(&shader);
		shader.turnOff();

		windowManager->swapTheBuffers();

		source0Pos[0] = model[3].x;
		source0Pos[1] = model[3].y;
		source0Pos[2] = model[3].z;
		alSourcefv(source[0], AL_POSITION, source0Pos);

		listenerPos[0] = camera->Position.x;
		listenerPos[1] = camera->Position.y;
		listenerPos[2] = camera->Position.z;
		alListenerfv(AL_POSITION, listenerPos);

		listenerOri[0] = camera->Front.x;
		listenerOri[1] = camera->Front.y;
		listenerOri[2] = camera->Front.z;
		listenerOri[3] = camera->Up.x;
		listenerOri[4] = camera->Up.y;
		listenerOri[5] = camera->Up.z;
		alListenerfv(AL_ORIENTATION, listenerOri);

		if (windowManager->inputManager.getKeyState()[InputCodes::u]){
			alSourcePlay(source[0]);
		}
	}
}

void GLApplication::destroy() {
	if (windowManager) {
		delete windowManager;
		windowManager = nullptr;
	}

	alutExit();

	objModel.destroy();

}
