#pragma once


class Deltatime {
public:
	Deltatime(float dt = 0.0) 
		:m_Time(dt)
	{
	}

	operator float() const { return m_Time; }

	float GetSeconds() const { return m_Time; }
	float GetMilliseconds() const { return m_Time * 1e3; }

private:
	float m_Time;
};

