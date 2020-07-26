#include "../../public/LunarUser/LunarInputManager.h"


#include <Ultralight/KeyCodes.h>
#include <cmath>

using namespace ultralight::KeyCodes;

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

	void WindowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		ultralight::KeyEvent evt = {};
		evt.type = action == GLFW_PRESS || action == GLFW_REPEAT ? ultralight::KeyEvent::kType_RawKeyDown : ultralight::KeyEvent::kType_KeyUp;
		evt.virtual_key_code = GLFWKeyCodeToUltralightKeyCode(key);
		evt.native_key_code = scancode;
		ultralight::GetKeyIdentifierFromVirtualKeyCode(evt.virtual_key_code, evt.key_identifier);
		evt.modifiers = GLFWModsToUltralightMods(mods);
        LunarGUI::UltralightManager::evt = evt;
        

        if (evt.type == ultralight::KeyEvent::kType_RawKeyDown && (key == GLFW_KEY_ENTER || key == GLFW_KEY_TAB)) {
            ultralight::KeyEvent sEvt;
            sEvt.type = ultralight::KeyEvent::kType_Char;
            ultralight::String text = key == GLFW_KEY_ENTER ? ultralight::String("\r") : ultralight::String("\t");
            sEvt.text = text;
            sEvt.unmodified_text = text;

            LunarGUI::UltralightManager::sEvt = sEvt;   
            LunarGUI::UltralightManager::secondKey = true;
        }

        LunarGUI::UltralightManager::updateKey = true;
	}

    void WindowCharCallback(GLFWwindow* window, unsigned int codepoint) {
        ultralight::KeyEvent evt;
        evt.type = ultralight::KeyEvent::kType_Char;

        ultralight::String text = ultralight::String32((const char32_t*)&codepoint, 1);

        evt.text = text;
        evt.unmodified_text = text;

        LunarGUI::UltralightManager::evt = evt;
        LunarGUI::UltralightManager::updateKey = true;
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

    double Scale(GLFWwindow* window) {
        float xscale, yscale;
        glfwGetWindowContentScale(window, &xscale, &yscale);

        return (double)xscale;
    }

    int deviceToPixels(int val, GLFWwindow* window) {
        return (int)std::round(val * Scale(window));
    }

    void InputManager::WindowScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        ultralight::ScrollEvent evt;
        evt.type = ultralight::ScrollEvent::kType_ScrollByPixel;
        evt.delta_x = deviceToPixels((int)xoffset * 32, window);
        evt.delta_y = deviceToPixels((int)yoffset * 32, window);

        LunarGUI::UltralightManager::scrollEvent = evt;
        LunarGUI::UltralightManager::updateScroll = true;
    }

	int GLFWModsToUltralightMods(int mods) {
		int result = 0;
		if (mods & GLFW_MOD_ALT)
			result |= ultralight::KeyEvent::kMod_AltKey;
		if (mods & GLFW_MOD_CONTROL)
			result |= ultralight::KeyEvent::kMod_CtrlKey;
		if (mods & GLFW_MOD_SUPER)
			result |= ultralight::KeyEvent::kMod_MetaKey;
		if (mods & GLFW_MOD_SHIFT)
			result |= ultralight::KeyEvent::kMod_ShiftKey;
		return result;
	}

	int GLFWKeyCodeToUltralightKeyCode(int key) {
        switch (key) {
        case GLFW_KEY_SPACE:
            return ultralight::KeyCodes::GK_SPACE;
        case GLFW_KEY_COMMA:
            return ultralight::KeyCodes::GK_OEM_COMMA;
        case GLFW_KEY_MINUS:
            return ultralight::KeyCodes::GK_OEM_MINUS;
        case GLFW_KEY_PERIOD:
            return ultralight::KeyCodes::GK_OEM_PERIOD;
        case GLFW_KEY_SLASH:
            return ultralight::KeyCodes::GK_OEM_2;
        case GLFW_KEY_0:
            return ultralight::KeyCodes::GK_0;
        case GLFW_KEY_1:
            return ultralight::KeyCodes::GK_1;
        case GLFW_KEY_2:
            return ultralight::KeyCodes::GK_2;
        case GLFW_KEY_3:
            return ultralight::KeyCodes::GK_3;
        case GLFW_KEY_4:
            return ultralight::KeyCodes::GK_4;
        case GLFW_KEY_5:
            return ultralight::KeyCodes::GK_5;
        case GLFW_KEY_6:
            return ultralight::KeyCodes::GK_6;
        case GLFW_KEY_7:
            return ultralight::KeyCodes::GK_7;
        case GLFW_KEY_8:
            return ultralight::KeyCodes::GK_8;
        case GLFW_KEY_9:
            return ultralight::KeyCodes::GK_9;
        case GLFW_KEY_SEMICOLON:
            return ultralight::KeyCodes::GK_OEM_1;
        case GLFW_KEY_EQUAL:
            return ultralight::KeyCodes::GK_OEM_PLUS;
        case GLFW_KEY_A:
            return ultralight::KeyCodes::GK_A;
        case GLFW_KEY_B:
            return ultralight::KeyCodes::GK_B;
        case GLFW_KEY_C:
            return ultralight::KeyCodes::GK_C;
        case GLFW_KEY_D:
            return ultralight::KeyCodes::GK_D;
        case GLFW_KEY_E:
            return ultralight::KeyCodes::GK_E;
        case GLFW_KEY_F:
            return ultralight::KeyCodes::GK_F;
        case GLFW_KEY_G:
            return ultralight::KeyCodes::GK_G;
        case GLFW_KEY_H:
            return ultralight::KeyCodes::GK_H;
        case GLFW_KEY_I:
            return ultralight::KeyCodes::GK_I;
        case GLFW_KEY_J:
            return ultralight::KeyCodes::GK_J;
        case GLFW_KEY_K:
            return ultralight::KeyCodes::GK_K;
        case GLFW_KEY_L:
            return ultralight::KeyCodes::GK_L;
        case GLFW_KEY_M:
            return ultralight::KeyCodes::GK_M;
        case GLFW_KEY_N:
            return ultralight::KeyCodes::GK_N;
        case GLFW_KEY_O:
            return ultralight::KeyCodes::GK_O;
        case GLFW_KEY_P:
            return ultralight::KeyCodes::GK_P;
        case GLFW_KEY_Q:
            return ultralight::KeyCodes::GK_Q;
        case GLFW_KEY_R:
            return ultralight::KeyCodes::GK_R;
        case GLFW_KEY_S:
            return ultralight::KeyCodes::GK_S;
        case GLFW_KEY_T:
            return ultralight::KeyCodes::GK_T;
        case GLFW_KEY_U:
            return ultralight::KeyCodes::GK_U;
        case GLFW_KEY_V:
            return ultralight::KeyCodes::GK_V;
        case GLFW_KEY_W:
            return ultralight::KeyCodes::GK_W;
        case GLFW_KEY_X:
            return ultralight::KeyCodes::GK_X;
        case GLFW_KEY_Y:
            return ultralight::KeyCodes::GK_Y;
        case GLFW_KEY_Z:
            return ultralight::KeyCodes::GK_Z;
        case GLFW_KEY_LEFT_BRACKET:
            return ultralight::KeyCodes::GK_OEM_4;
        case GLFW_KEY_BACKSLASH:
            return ultralight::KeyCodes::GK_OEM_5;
        case GLFW_KEY_RIGHT_BRACKET:
            return ultralight::KeyCodes::GK_OEM_6;
        case GLFW_KEY_GRAVE_ACCENT:
            return ultralight::KeyCodes::GK_OEM_3;
        case GLFW_KEY_WORLD_1:
            return ultralight::KeyCodes::GK_UNKNOWN;
        case GLFW_KEY_WORLD_2:
            return ultralight::KeyCodes::GK_UNKNOWN;
        case GLFW_KEY_ESCAPE:
            return ultralight::KeyCodes::GK_ESCAPE;
        case GLFW_KEY_ENTER:
            return ultralight::KeyCodes::GK_RETURN;
        case GLFW_KEY_TAB:
            return ultralight::KeyCodes::GK_TAB;
        case GLFW_KEY_BACKSPACE:
            return ultralight::KeyCodes::GK_BACK;
        case GLFW_KEY_INSERT:
            return ultralight::KeyCodes::GK_INSERT;
        case GLFW_KEY_DELETE:
            return ultralight::KeyCodes::GK_DELETE;
        case GLFW_KEY_RIGHT:
            return ultralight::KeyCodes::GK_RIGHT;
        case GLFW_KEY_LEFT:
            return ultralight::KeyCodes::GK_LEFT;
        case GLFW_KEY_DOWN:
            return ultralight::KeyCodes::GK_DOWN;
        case GLFW_KEY_UP:
            return ultralight::KeyCodes::GK_UP;
        case GLFW_KEY_PAGE_UP:
            return ultralight::KeyCodes::GK_PRIOR;
        case GLFW_KEY_PAGE_DOWN:
            return ultralight::KeyCodes::GK_NEXT;
        case GLFW_KEY_HOME:
            return ultralight::KeyCodes::GK_HOME;
        case GLFW_KEY_END:
            return ultralight::KeyCodes::GK_END;
        case GLFW_KEY_CAPS_LOCK:
            return ultralight::KeyCodes::GK_CAPITAL;
        case GLFW_KEY_SCROLL_LOCK:
            return ultralight::KeyCodes::GK_SCROLL;
        case GLFW_KEY_NUM_LOCK:
            return ultralight::KeyCodes::GK_NUMLOCK;
        case GLFW_KEY_PRINT_SCREEN:
            return ultralight::KeyCodes::GK_SNAPSHOT;
        case GLFW_KEY_PAUSE:
            return ultralight::KeyCodes::GK_PAUSE;
        case GLFW_KEY_F1:
            return ultralight::KeyCodes::GK_F1;
        case GLFW_KEY_F2:
            return ultralight::KeyCodes::GK_F2;
        case GLFW_KEY_F3:
            return ultralight::KeyCodes::GK_F3;
        case GLFW_KEY_F4:
            return ultralight::KeyCodes::GK_F4;
        case GLFW_KEY_F5:
            return ultralight::KeyCodes::GK_F5;
        case GLFW_KEY_F6:
            return ultralight::KeyCodes::GK_F6;
        case GLFW_KEY_F7:
            return ultralight::KeyCodes::GK_F7;
        case GLFW_KEY_F8:
            return ultralight::KeyCodes::GK_F8;
        case GLFW_KEY_F9:
            return ultralight::KeyCodes::GK_F9;
        case GLFW_KEY_F10:
            return ultralight::KeyCodes::GK_F10;
        case GLFW_KEY_F11:
            return ultralight::KeyCodes::GK_F11;
        case GLFW_KEY_F12:
            return ultralight::KeyCodes::GK_F12;
        case GLFW_KEY_F13:
            return ultralight::KeyCodes::GK_F13;
        case GLFW_KEY_F14:
            return ultralight::KeyCodes::GK_F14;
        case GLFW_KEY_F15:
            return ultralight::KeyCodes::GK_F15;
        case GLFW_KEY_F16:
            return ultralight::KeyCodes::GK_F16;
        case GLFW_KEY_F17:
            return ultralight::KeyCodes::GK_F17;
        case GLFW_KEY_F18:
            return ultralight::KeyCodes::GK_F18;
        case GLFW_KEY_F19:
            return ultralight::KeyCodes::GK_F19;
        case GLFW_KEY_F20:
            return ultralight::KeyCodes::GK_F20;
        case GLFW_KEY_F21:
            return ultralight::KeyCodes::GK_F21;
        case GLFW_KEY_F22:
            return ultralight::KeyCodes::GK_F22;
        case GLFW_KEY_F23:
            return ultralight::KeyCodes::GK_F23;
        case GLFW_KEY_F24:
            return ultralight::KeyCodes::GK_F24;
        case GLFW_KEY_F25:
            return ultralight::KeyCodes::GK_UNKNOWN;
        case GLFW_KEY_KP_0:
            return ultralight::KeyCodes::GK_NUMPAD0;
        case GLFW_KEY_KP_1:
            return ultralight::KeyCodes::GK_NUMPAD1;
        case GLFW_KEY_KP_2:
            return ultralight::KeyCodes::GK_NUMPAD2;
        case GLFW_KEY_KP_3:
            return GK_NUMPAD3;
        case GLFW_KEY_KP_4:
            return GK_NUMPAD4;
        case GLFW_KEY_KP_5:
            return GK_NUMPAD5;
        case GLFW_KEY_KP_6:
            return GK_NUMPAD6;
        case GLFW_KEY_KP_7:
            return GK_NUMPAD7;
        case GLFW_KEY_KP_8:
            return GK_NUMPAD8;
        case GLFW_KEY_KP_9:
            return GK_NUMPAD9;
        case GLFW_KEY_KP_DECIMAL:
            return GK_DECIMAL;
        case GLFW_KEY_KP_DIVIDE:
            return GK_DIVIDE;
        case GLFW_KEY_KP_MULTIPLY:
            return GK_MULTIPLY;
        case GLFW_KEY_KP_SUBTRACT:
            return GK_SUBTRACT;
        case GLFW_KEY_KP_ADD:
            return GK_ADD;
        case GLFW_KEY_KP_ENTER:
            return GK_RETURN;
        case GLFW_KEY_KP_EQUAL:
            return GK_OEM_PLUS;
        case GLFW_KEY_LEFT_SHIFT:
            return GK_SHIFT;
        case GLFW_KEY_LEFT_CONTROL:
            return GK_CONTROL;
        case GLFW_KEY_LEFT_ALT:
            return GK_MENU;
        case GLFW_KEY_LEFT_SUPER:
            return GK_LWIN;
        case GLFW_KEY_RIGHT_SHIFT:
            return GK_SHIFT;
        case GLFW_KEY_RIGHT_CONTROL:
            return GK_CONTROL;
        case GLFW_KEY_RIGHT_ALT:
            return GK_MENU;
        case GLFW_KEY_RIGHT_SUPER:
            return GK_RWIN;
        case GLFW_KEY_MENU:
            return GK_UNKNOWN;
        default:
            return GK_UNKNOWN;
        }
	}
}