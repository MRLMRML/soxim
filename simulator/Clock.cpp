#include "Clock.h"

float Clock::get()
{
	return s_clock;
}

void Clock::tick()
{
	s_clock++;
}

bool Clock::trigger()
{
	return (s_clock >= m_clock) ? true : false;
}

void Clock::set(const float interval)
{
	m_clock += interval;
}