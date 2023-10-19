#pragma once

#include "Dix/Core/KeyCodes.h"
#include "Dix/Core/MouseCodes.h"

#include <glm/glm.hpp>

namespace Dix
{
	class Input
	{
	public:
		Input() = delete;
		~Input() = delete;

		static bool IsKeyPressed(const KeyCode key);

		static bool IsMouseButtonPressed(const MouseCode button);

		static glm::vec2 GetMousePosition();

		static f32 GetMouseX() { return GetMousePosition().x; }
		static f32 GetMouseY() { return GetMousePosition().y; }
	};
}