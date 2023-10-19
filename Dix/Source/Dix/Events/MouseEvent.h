#pragma once

#include "Dix/Events/Event.h"

#include <sstream>

namespace Dix
{
	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(f32 xoffset, f32 yoffset)
			: m_XOffset(xoffset), m_YOffset(yoffset)
		{}

		f32 GetXOffset() const { return m_XOffset; }
		f32 GetYOffset() const { return m_YOffset; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolled: " << m_XOffset << " " << m_YOffset;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton)

	private:
		f32 m_XOffset, m_YOffset;
	};
}