#pragma once

class Clock
{
public:
	Clock() = default;
	float get();
	void tick();
	bool trigger();
	void set(const float interval);

private:
	static inline float s_clock{}; // global clock
	float m_clock{}; // local clock
};