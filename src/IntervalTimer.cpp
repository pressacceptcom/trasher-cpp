#include "IntervalTimer.hpp"

using namespace godot;

IntervalTimer::IntervalTimer()
{
}

IntervalTimer::~IntervalTimer()
{
}

void IntervalTimer::_register_methods()
{
	// Methods
	register_method("decrement_timer", &IntervalTimer::decrement_timer);
	register_method("decrease_timer", &IntervalTimer::decrease_timer);

	// Properties
	register_property<IntervalTimer, std::int16_t>("interval", &IntervalTimer::set_interval, &IntervalTimer::get_interval, 0);
	register_property<IntervalTimer, std::int16_t>("timer", &IntervalTimer::set_timer, &IntervalTimer::get_timer, 0);
	register_property<IntervalTimer, std::uint8_t>("most_significant_nibble", &IntervalTimer::set_most_significant_nibble, &IntervalTimer::get_most_significant_nibble, 0);
	register_property<IntervalTimer, std::uint8_t>("least_significant_byte", &IntervalTimer::set_least_significant_byte, &IntervalTimer::get_least_significant_byte, 0);

	// Signals
	register_signal<IntervalTimer>("hit_zero");
}

void IntervalTimer::_init()
{
}

void IntervalTimer::set_interval(std::int16_t new_value)
{
	interval = new_value & TIMER_MASK;
	timer = new_value & TIMER_MASK;
	enabled = interval != 0;
}

std::int16_t IntervalTimer::get_interval()
{
	return interval;
}

void IntervalTimer::set_timer(std::int16_t new_value)
{
}

std::int16_t IntervalTimer::get_timer()
{
	return timer;
}

void IntervalTimer::set_most_significant_nibble(std::uint8_t new_value)
{
	interval = ((new_value & 0xFF) << 8) | (interval & 0xFF);
	enabled = interval != 0;
	timer = interval;
}

std::uint8_t IntervalTimer::get_most_significant_nibble()
{
	return (interval >> 8) & 0x0F;
}

void IntervalTimer::set_least_significant_byte(std::uint8_t new_value)
{
	interval = (interval & 0xF00) | (new_value & 0xFF);
	enabled = interval != 0;
	timer = interval;
}

std::uint8_t IntervalTimer::get_least_significant_byte()
{
	return interval & 0xFF;
}

void IntervalTimer::decrement_timer()
{
	if (!enabled)
	{
		return;
	}

	timer--;

	if (timer <= 0)
	{
		emit_signal("hit_zero");
	}

	timer = interval;
}

void IntervalTimer::decrease_timer(std::int16_t amount)
{
	if (!enabled)
	{
		return;
	}

	timer -= amount;

	if (timer <= 0)
	{
		emit_signal("hit_zero");
	}

	timer = interval - (timer < 0 ? timer : 0);
}
