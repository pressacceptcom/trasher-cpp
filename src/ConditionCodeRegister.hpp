/**
 * CONDITION CODE REGISTER
 *
 * See pg 4-301 "Programming Model"
 *
 *    7    Condition Code Register    0
 *    ---------------------------------
 *    | E | F | H | I | N | Z | V | C |
 *    ---------------------------------
 *      |   |   |   |   |   |   |   |
 * Entire   |   |   |   |   |   |   Carry
 *          |   |   |   |   |   |
 *  FIRQ Mask   |   |   |   |   Overflow
 *              |   |   |   |
 *     Half Carry   |   |   Zero
 *                  |   |
 *           IRQ Mask   Negative
 */

#ifndef CONDITIONCODEREGISTER_H
#define CONDITIONCODEREGISTER_H

#include <cstdint>

#include <Godot.hpp>
#include <Reference.hpp>

namespace godot
{
	class ConditionCodeRegister : public Reference
	{
		GODOT_CLASS(ConditionCodeRegister, Reference)

	private:
	public:
		enum ConditionCode
		{
			C_CARRY = 0x01,
			V_OVERFLOW = 0x02,
			Z_ZERO = 0x04,
			N_NEGATIVE = 0x08,
			I_IRQ_MASK = 0x10,
			H_HALF_CARRY = 0x20,
			F_FIRQ_MASK = 0x40,
			E_ENTIRE = 0x80
		};

		bool debug = false;

		union
		{
			std::uint8_t slot;
			struct
			{
				bool carry : 1;
				bool overflow : 1;
				bool zero : 1;
				bool negative : 1;
				bool irq_mask : 1;
				bool half_carry : 1;
				bool firq_mask : 1;
				bool entire : 1;
			} bits;
		};

		// C++ Specific Definitions

		ConditionCodeRegister();

		~ConditionCodeRegister();

		// Godot Specific Definitions

		static void _register_methods();

		void _init(); // our initializer called by Godot

		// Class Specific Definitions

		void set_debug(bool new_value);
		bool get_debug();

		void set_register(std::uint8_t new_value);
		std::uint8_t get_register();

		void or_register(std::uint8_t or_value);
		void and_register(std::uint8_t and_value);

		void set_bit(int bit, bool new_value);
		bool get_bit(int bit);

		void set_carry(bool new_value);
		bool get_carry();

		void set_overflow(bool new_value);
		bool get_overflow();

		void set_zero(bool new_value);
		bool get_zero();

		void set_negative(bool new_value);
		bool get_negative();

		void set_irq_mask(bool new_value);
		bool get_irq_mask();

		void set_half_carry(bool new_value);
		bool get_half_carry();

		void set_firq_mask(bool new_value);
		bool get_firq_mask();

		void set_entire(bool new_value);
		bool get_entire();
	};
}

#endif