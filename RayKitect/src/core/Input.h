#pragma once

#include "KeyCodes.h"
#include "MouseCodes.h"

#include <glm/glm.hpp>

class Input {
public:
	static bool IsKeyPressed(KeyCode key);
	static bool IsMouseButtonPressed(MouseCode button);
	static glm::vec2 GetMousePosition();
	static float GetMouseX();
	static float GetMouseY();

	static void SetCursorPosition(const glm::vec2& pos);
	static void SetCursorVisibility(bool show);
};
