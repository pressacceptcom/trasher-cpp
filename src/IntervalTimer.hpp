/**
 * FF94:       Timer Most Significant Nibble
 * FF95:       Timer Least Significant Byte
 *
 *        TIMER: This is a 12-bit interval timer. When a value is loaded into the MSB, the count is
 *               begun. The input clock is either 14 MHz or horizontal sync, as selected by TINS
 *               (bit 5 of FF91). As the count falls through zero, an interrupt is generated (if
 *               enabled), and the count is automatically reloaded.
 */

#ifndef INTERVALTIMER_H
#define INTERVALTIMER_H

#include <Godot.hpp>
#include <Reference.hpp>

namespace godot
{
	class IntervalTimer : public Reference
	{
		GODOT_CLASS(IntervalTimer, Reference)

	private:
	public:
		static const std::uint16_t TIMER_MASK = 0x0FFF;

		bool enabled = false;

		std::int16_t interval = 0;

		std::int16_t timer = 0;

		// C++ Specific Definitions

		IntervalTimer();

		~IntervalTimer();

		// Godot Specific Definitions

		static void _register_methods();

		void _init(); // our initializer called by Godot

		// Class Definitions

		void set_interval(std::int16_t new_value);
		std::int16_t get_interval();

		void set_timer(std::int16_t new_value);
		std::int16_t get_timer();

		void set_most_significant_nibble(std::uint8_t new_value);
		std::uint8_t get_most_significant_nibble();

		void set_least_significant_byte(std::uint8_t new_value);
		std::uint8_t get_least_significant_byte();

		void decrement_timer();
		void decrease_timer(std::int16_t amount);
	};
}

#endif