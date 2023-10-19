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
		WindowResizeEvent(i32 width, i32 height)
			: m_Width(width), m_Height(height)
		{}

		i32 GetWidth() const { return m_Width; }
		i32 GetHeight() const { return m_Height; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResize: " << m_Width << " " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryWindow)

	private:
		i32 m_Width, m_Height;
	};
}