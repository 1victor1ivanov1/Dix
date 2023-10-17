#pragma once

#include "Dix/Events/Event.h"

#include <sstream>

namespace Dix
{
	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryWindow)
	};

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(int width, int height)
			: m_Width(width), m_Height(height)
		{}

		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResize: " << m_Width << " " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryWindow)

	private:
		int m_Width, m_Height;
	};
}