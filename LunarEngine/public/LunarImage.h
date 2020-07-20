#pragma once
#include "LunarUltralight/UltralightData.h"

#include <string>

struct Image {
	void* pixels;
	int width, height;
	int size;
};

void ReadImage(Image imageData, LunarGUI::Pane& _pane, VkDevice device);