#pragma once
#include "rktpch.h"

#define BIT(x) 1 << x



enum class EventType {
	None = 0,
	WindowClose, WindowResize,
	KeyPressed, KeyReleased, KeyTyped,
	MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

enum EventCategory
{
	None = 0,
	EventCategoryApplication = BIT(0),
	EventCategoryInput = BIT(1),
	EventCategoryKeyboard = BIT(2),
	EventCategoryMouse = BIT(3),
	EventCategoryMouseButton = BIT(4)
};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
							virtual EventType GetEventType() const override { return GetStaticType(); }

#define EVENT_CLASS_CATEGORY(category) virtual int GetEventCategory() const override { return category; }

class Event {
public:
	Event() = default;
	virtual ~Event() = default;

	bool Handled = false;

	virtual EventType GetEventType() const = 0;
	virtual int GetEventCategory() const = 0;
	virtual std::string ToString() const = 0;

	bool IsInCategory(EventCategory c) {
		return GetEventCategory() & c;
	}
};

class EventDispatcher
{
public:
	EventDispatcher(Event& event)
		: m_Event(event)
	{
	}

	// F will be deduced by the compiler
	template<typename T, typename F>
	bool Dispatch(const F& func)
	{
		if (m_Event.GetEventType() == T::GetStaticType())
		{
			m_Event.Handled |= func(static_cast<T&>(m_Event));
			return true;
		}
		return false;
	}
private:
	Event& m_Event;
};

inline std::ostream& operator<<(std::ostream& os, const Event& e)
{
	return os << e.ToString();
}
