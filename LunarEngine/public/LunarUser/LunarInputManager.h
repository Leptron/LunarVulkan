#pragma once

#include <GLFW/glfw3.h>
#include "../LunarUltralight/UltralightManager.h"

namespace LunarInput {
	int GLFWModsToUltralightMods(int mods);
	int GLFWKeyCodeToUltralightKeyCode(int key);
	void WindowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void WindowCharCallback(GLFWwindow* window, unsigned int codepoint);

	class InputManager {
	public:
		InputManager();
		~InputManager();

		void AttachGUIManager(LunarGUI::UltralightManager* manager);
		void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

		static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	private:
		LunarGUI::UltralightManager* manager;
	};
}