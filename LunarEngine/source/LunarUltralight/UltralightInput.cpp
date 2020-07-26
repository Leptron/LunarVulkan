#include "../../public/LunarUltralight/UltralightManager.h"
#include <thread>

namespace LunarGUI {
	LunarInput::MousePos UltralightManager::_cPos = {};

	bool UltralightManager::updatePos = false;
	bool UltralightManager::updateKey = false;
	bool UltralightManager::secondKey = false;
	bool UltralightManager::updateScroll = false;

	ultralight::KeyEvent UltralightManager::evt = {};
	ultralight::KeyEvent UltralightManager::sEvt = {};
	ultralight::ScrollEvent UltralightManager::scrollEvent = {};

	void UltralightManager::PassMouseInput(LunarInput::MousePos pos) {
		if (updatePos) {
			ultralight::MouseEvent evt;
			if (pos.buttonState == LunarInput::MouseButton::None) {
				evt.type = ultralight::MouseEvent::kType_MouseMoved;
				evt.x = pos.x;
				evt.y = pos.y;
				evt.button = ultralight::MouseEvent::kButton_None;
			} else if (pos.buttonState != LunarInput::MouseButton::None && pos.pressed) {
				evt.type = ultralight::MouseEvent::kType_MouseDown;
				evt.x = pos.x;
				evt.y = pos.y;
				if (pos.buttonState == LunarInput::MouseButton::Left)
					evt.button = ultralight::MouseEvent::kButton_Left;
				else if (pos.buttonState == LunarInput::MouseButton::Right)
					evt.button = ultralight::MouseEvent::kButton_Right;
				else if (pos.buttonState == LunarInput::MouseButton::Middle)
					evt.button = ultralight::MouseEvent::kButton_Middle;
			} else if (pos.buttonState != LunarInput::MouseButton::None && !pos.pressed) {
				evt.type = ultralight::MouseEvent::kType_MouseUp;
				evt.x = pos.x;
				evt.y = pos.y;
				if (pos.buttonState == LunarInput::MouseButton::Left)
					evt.button = ultralight::MouseEvent::kButton_Left;
				else if (pos.buttonState == LunarInput::MouseButton::Right)
					evt.button = ultralight::MouseEvent::kButton_Right;
				else if (pos.buttonState == LunarInput::MouseButton::Middle)
					evt.button = ultralight::MouseEvent::kButton_Middle;
			}

			for (auto& layout : layouts) {
				if (layout.active) {
					for (auto& pane : layout._panes) {
						if ((pos.x >= pane.tX && pos.x <= pane.fX) && (pos.y >= pane.tY && pos.y <= pane.fY)) {
							evt.x -= pane.tX;
							evt.y -= pane.tY;

							pane._view->get()->FireMouseEvent(evt);	
						}
					}
				}
			}
		}
	}

	void UltralightManager::UpdateKeyEvent() {
		if (updateKey) {
			for (auto& layout : layouts) {
				if (layout.active) {
					for (auto& pane : layout._panes) {
						pane._view->get()->FireKeyEvent(evt);
						if (secondKey) {
							pane._view->get()->FireKeyEvent(sEvt);
							secondKey = false;
						}

						updateKey = false;
					}
				}
			}
		}

		if(updateScroll) {
			for(auto& layout : layouts) {
				if(layout.active) {
					for(auto& pane : layout._panes) {
						pane._view->get()->FireScrollEvent(scrollEvent);
					}
				}
			}

			updateScroll = false;
		}
	}

	void UltralightManager::OnChangeCursor(ultralight::View* caller, ultralight::Cursor cursor) {
		switch (cursor) {
		case ultralight::kCursor_Cross:
			glfwSetCursor(window, _cursor_crosshair);
			break;
		case ultralight::kCursor_Hand:
			glfwSetCursor(window, _cursor_hand);
			break;
		case ultralight::kCursor_IBeam:
			glfwSetCursor(window, _cursor_ibeam);
			break;
		case ultralight::kCursor_EastWestResize:
			glfwSetCursor(window, _cursor_hresize);
			break;
		case ultralight::kCursor_NorthSouthResize:
			glfwSetCursor(window, _cursor_vresize);
			break;
		default:
			glfwSetCursor(window, nullptr);
		}
	}
}