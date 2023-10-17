#pragma once

#include "Dix/Core/Base.h"

namespace Dix
{
	enum class EventType
	{
		WindowClose, WindowResize,
		KeyPressed, KeyReleased, KeyRepeat,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
	};

	enum EventCategory
	{
		None						= 0,
		EventCategoryWindow			= BITS(0),
		EventCategoryInput			= BITS(1),
		EventCategoryKeyboard		= BITS(2),
		EventCategoryMouse			= BITS(3),
		EventCategoryMouseButton	= BITS(4),
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; } \
												virtual EventType GetType() const override { return GetStaticType(); } \
												virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual EventCategory GetCategory() const override { return category; }

	class Event
	{
	public:
		virtual ~Event() = default;

		virtual EventType GetType()	const			= 0;
		virtual const char* GetName() const			= 0;	
		virtual EventCategory GetCategory() const	= 0;
		virtual std::string ToString() const { return GetName(); }

		bool IsInCategory(EventCategory category)
		{
			return GetCategory() & category;
		}

		bool Handled = false;
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{}

		template <typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (m_Event.GetType() == T::GetStaticType())
			{
				m_Event.Handled |= func(static_cast<T&>(m_Event));
				return true;
			}

			return false;
		}

	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, Event& event)
	{
		return os << event.ToString();
	}
}