/**
 * MPU/CPU Pins
 *
 * PIN ASSIGNMENT
 *
 *       ---\__/---
 *  VSS [| 1   40 |] HALT
 *       |        |
 *  NMI [| 2   39 |] TSC
 *       |        |
 *  IRQ [| 3   38 |] LIC
 *       |        |
 * FIRQ [| 4   37 |] RESET
 *       |        |
 *   BS [| 5   36 |] AVMA
 *       |        |
 *   BA [| 6   35 |] Q
 *       |        |
 *  VCC [| 7   34 |] E
 *       |        |
 *   A0 [| 8   33 |] BUSY
 *       |        |
 *   A1 [| 9   32 |] R/W
 *       |        |
 *   A2 [| 10  31 |] D0
 *       |        |
 *   A3 [| 11  30 |] D1
 *       |        |
 *   A4 [| 12  29 |] D2
 *       |        |
 *   A5 [| 13  28 |] D3
 *       |        |
 *   A6 [| 14  27 |] D4
 *       |        |
 *   A7 [| 15  26 |] D5
 *       |        |
 *   A8 [| 16  25 |] D6
 *       |        |
 *   A9 [| 17  24 |] D7
 *       |        |
 *  A10 [| 18  23 |] A15
 *       |        |
 *  A11 [| 19  22 |] A14
 *       |        |
 *  A12 [| 20  21 |] A13
 *       ----------
 */

#ifndef PINS_H
#define PINS_H

#include <Godot.hpp>
#include <Reference.hpp>

namespace godot
{
	class PinsState : public Reference
	{
		GODOT_CLASS(PinsState, Reference)

	private:
	public:
		enum Pins
		{
			// VSS,
			NMI,
			IRQ,
			FIRQ,
			BS,
			BA,
			// VCC,
			// A0,
			// A1,
			// A2,
			// A3,
			// A4,
			// A5,
			// A6,
			// A7,
			// A8,
			// A9,
			// A10,
			// A11,
			// A12,
			// A13,
			// A14,
			// A15,
			// D7,
			// D6,
			// D5,
			// D4,
			// D3,
			// D2,
			// D1,
			// D0,
			// RW,
			// BUSY,
			// E,
			// Q,
			// AVMA,
			RESET,
			// LIC,
			// TSC,
			HALT
		};

		enum MPUState
		{
			NORMAL,
			INTERRUPT_RESET_ACKNOWLEDGE,
			SYNC_ACKNOWLEDGE,
			HALT_ACKNOLWEDGE
		};

		bool debug = false;

		bool nmi = false;
		bool irq = false;
		bool firq = false;
		bool bs = false;
		bool ba = false;
		bool reset = false;
		bool halt = false;

		bool delay_irq = false;

		// C++ Specific Definitions

		PinsState();

		~PinsState();

		// Godot Specific Definitions

		static void _register_methods();

		void _init(); // our initializer called by Godot

		// Class Specific Definitions

		void set_pin(int pin, bool new_value);
		bool get_pin(int pint);

		void set_nmi(bool new_value);
		bool get_nmi();

		void set_irq(bool new_value);
		bool get_irq();

		void set_firq(bool new_value);
		bool get_firq();

		void set_bs(bool new_value);
		bool get_bs();

		void set_ba(bool new_value);
		bool get_ba();

		void set_reset(bool new_value);
		bool get_reset();

		void set_halt(bool new_value);
		bool get_halt();

		void set_state(int ignored);
		int get_state();
	};
}

#endif