#pragma once

#include "Dix/Events/Event.h"

namespace Dix
{
	class KeyEvent : public Event
	{
	public:

	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
	};
}