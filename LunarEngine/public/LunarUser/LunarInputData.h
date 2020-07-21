#pragma once

#include <Ultralight/Ultralight.h>

namespace LunarInput {
	enum MouseButton {
		Left, 
		Right,
		Middle,
		None
	};


	struct MousePos {
		int x, y;
		MouseButton buttonState;
		bool pressed;
	};
}