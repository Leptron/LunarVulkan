#include "../../public/LunarUser/LunarInputManager.h"

namespace LunarInput {
	InputManager::InputManager() {

	}

	InputManager::~InputManager() {

	}

	//TODO implement Attachment, keycallback, character callback , and mouse input
	void InputManager::AttachGUIManager(LunarGUI::UltralightManager* manager) {
		this->manager = manager;
	}

	void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
	}

	void InputManager::CharacterCallback(GLFWwindow* window, unsigned int codepoint)
	{
	}

	void InputManager::CursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
		MousePos pos = {};
		pos.buttonState = MouseButton::None;
		pos.x = xpos;
		pos.y = ypos;

		LunarGUI::UltralightManager::_cPos = pos;
		LunarGUI::UltralightManager::updatePos = true;
	}

	void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			MousePos pos = {};
			pos.buttonState = MouseButton::Left;
			pos.x = xpos;
			pos.y = ypos;
			pos.pressed = true;

			LunarGUI::UltralightManager::_cPos = pos;
			LunarGUI::UltralightManager::updatePos = true;
		} else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			MousePos pos = {};
			pos.buttonState = MouseButton::Left;
			pos.x = xpos;
			pos.y = ypos;
			pos.pressed = false;

			LunarGUI::UltralightManager::_cPos = pos;
			LunarGUI::UltralightManager::updatePos = true;
		} else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			MousePos pos = {};
			pos.buttonState = MouseButton::Right;
			pos.x = xpos;
			pos.y = ypos;
			pos.pressed = true;

			LunarGUI::UltralightManager::_cPos = pos;
			LunarGUI::UltralightManager::updatePos = true;
		} else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			MousePos pos = {};
			pos.buttonState = MouseButton::Right;
			pos.x = xpos;
			pos.y = ypos;
			pos.pressed = false;

			LunarGUI::UltralightManager::_cPos = pos;
			LunarGUI::UltralightManager::updatePos = true;
		} else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			MousePos pos = {};
			pos.buttonState = MouseButton::Middle;
			pos.x = xpos;
			pos.y = ypos;
			pos.pressed = true;

			LunarGUI::UltralightManager::_cPos = pos;
			LunarGUI::UltralightManager::updatePos = true;
		} else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE) {
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			MousePos pos = {};
			pos.buttonState = MouseButton::Middle;
			pos.x = xpos;
			pos.y = ypos;
			pos.pressed = false;

			LunarGUI::UltralightManager::_cPos = pos;
			LunarGUI::UltralightManager::updatePos = true;
		}
	}
}