#pragma once

#include "Dix/Events/Event.h"

#include <sstream>

namespace Dix
{
	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(f32 xoffset, f32 yoffset)
			: m_OffsetX(xoffset), m_OffsetY(yoffset)
		{}

		f32 GetOffsetX() const { return m_OffsetX; }
		f32 GetOffsetY() const { return m_OffsetY; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolled: " << m_OffsetX << " " << m_OffsetY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton)

	private:
		f32 m_OffsetX, m_OffsetY;
	};

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(f32 xpos, f32 ypos)
			: m_PositionX(xpos), m_PositionY(ypos)
		{}

		f32 GetPositionX() const { return m_PositionX; }
		f32 GetPositionY() const { return m_PositionY; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMoved: " << m_PositionX << " " << m_PositionY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)

	private:
		f32 m_PositionX, m_PositionY;
	};
}