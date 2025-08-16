/**
 * # FF91:       Initialization Register 1 (INIT1)
 *
 *        BIT 7   -
 *        BIT 6   -
 *        BIT 5 = TINS        Timer Input Select: 1 = 70 nsec / 0 = 63 µsec
 *        BIT 4   -
 *        BIT 3   -
 *        BIT 2   -
 *        BIT 1   -
 *        BIT 0 = TR          MMU Task Register Select
 */

#ifndef INITIALIZATIONREGISTER1_H
#define INITIALIZATIONREGISTER1_H

#include <cstdint>

#include <Godot.hpp>
#include <Reference.hpp>

namespace godot
{
	class InitializationRegister1 : public Reference
	{
		GODOT_CLASS(InitializationRegister1, Reference)

	private:
	public:
		enum Bits
		{
			MMU_TASK_REGISTER_SELECT = 0x01,
			TIMER_INPUT_SELECT = 0x20
		};

		union
		{
			std::uint8_t byte;
			struct
			{
				bool mmu_task_register_select : 1;
				std::uint8_t ignored_0 : 4;
				bool timer_input_select : 1;
			} bits;
		};

		// C++ Specific Definitions

		InitializationRegister1();

		~InitializationRegister1();

		// Godot Specific Definitions

		static void _register_methods();

		void _init(); // our initializer called by Godot

		// Class Definitions

		void set_register(std::uint8_t new_value);
		std::uint8_t get_register();

		void set_bit(int bit, bool new_value);
		bool get_bit(int bit);

		void set_mmu_task_register_select(bool new_value);
		bool get_mmu_task_register_select();

		void set_timer_input_select(bool new_value);
		bool get_timer_input_select();
	};
}

#endif