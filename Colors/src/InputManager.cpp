#include <iostream>

#include "Headers/InputManager.h"
#include "Headers/TimeManager.h"

InputCodes InputManager::toApplicationKey(int key) {
	switch (key) {
	case 256:
		return InputCodes::kEscape;
	case 87:
		return InputCodes::W;
	case 83:
		return InputCodes::S;
	case 65:
		return InputCodes::A;
	case 68:
		return InputCodes::D;
	case 265:
		return InputCodes::Up;
	case 264:
		return InputCodes::Down;
	case 263:
		return InputCodes::Left;
	case 262:
		return InputCodes::Right;
	}
}

State InputManager::toApplicationState(int state) {
	switch (state) {
	case 0:
		return State::RELESED;
	case 1:
		return State::PRESSED;
	}
}

MouseButtonIndex InputManager::toMouseButtonIndex(int button) {
	switch (button) {
	case 0:
		return MouseButtonIndex::LEFT;
	case 1:
		return MouseButtonIndex::RIGHT;
	case 2:
		return MouseButtonIndex::MIDDLE;
	}
}

void InputManager::keyPressed(InputCodes code, float deltaTime, State state) {
	keyState[code] = state;
	switch (code) {
	case w:
	case W:
	case Up:
		camera->ProcessKeyboard(Camera_Movement::C_FORWARD, 3 * deltaTime);
		break;
	case s:
	case S:
	case Down:
		camera->ProcessKeyboard(Camera_Movement::C_BACKWARD, 3 * deltaTime);
		break;
	case a:
	case A:
	case Left:
		camera->ProcessKeyboard(Camera_Movement::C_LEFT, 3 * deltaTime);
		break;
	case d:
	case D:
	case Right:
		camera->ProcessKeyboard(Camera_Movement::C_RIGHT, deltaTime);
		break;
	}
}

void InputManager::mouseMoved(float mouseX, float mouseY) {
	if (mouseButtomState[MouseButtonIndex::LEFT]) {
		float xoffset = mouseX - lastMousePos.x;
		float yoffset = lastMousePos.y - mouseY;
		camera->ProcessMouseMovement(xoffset, yoffset);
	}
	lastMousePos.x = mouseX;
	lastMousePos.y = mouseY;
}

void InputManager::mouseClicked(MouseButtonIndex code, float mouseX,
		float mouseY, State state) {
	switch (code) {
	case RIGHT:
		std::cout << "lastMousePos.x:" << lastMousePos.x << std::endl;
		break;
	case LEFT:
		std::cout << "lastMousePos.y:" << lastMousePos.y << std::endl;
		lastMousePos.x = mouseX;
		lastMousePos.y = mouseY;
		mouseButtomState[code] = state;
		break;
	case MIDDLE:
		std::cout << "lastMousePos.x:" << lastMousePos.x << std::endl;
		std::cout << "lastMousePos.y:" << lastMousePos.y << std::endl;
		break;
	}
}

void InputManager::mouseScroll(float yoffset) {
}
