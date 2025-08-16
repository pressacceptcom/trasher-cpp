/**
 * FF92:       Interrupt Request Enable Register (IRQENR)
 *
 *        BIT 7   -
 *        BIT 6   -
 *        BIT 5 = TMR         Interrupt from Timer enabled
 *        BIT 4 = HBORD       Horizontal Border IRQ enabled
 *        BIT 3 = VBORD       Vertical Border IRQ enabled
 *        BIT 2 = EI2         Serial Data IRQ enabled
 *        BIT 1 = EI1         Keyboard IRQ enabled
 *        BIT 0 = EI0         Cartridge IRQ enabled
 *
 * FF93:       Fast Interrupt Request Enable Register (FIRQENR)
 *
 *        BIT 7   -
 *        BIT 6   -
 *        BIT 5 = TMR         Interrupt from Timer enabled
 *        BIT 4 = HBORD       Horizontal Border IRQ enabled
 *        BIT 3 = VBORD       Vertical Border IRQ enabled
 *        BIT 2 = EI2         Serial Data IRQ enabled
 *        BIT 1 = EI1         Keyboard IRQ enabled
 *        BIT 0 = EI0         Cartridge IRQ enabled
 */

#ifndef INTERRUPTREQUESTENABLEREGISTER_H
#define INTERRUPTREQUESTENABLEREGISTER_H

#include <Godot.hpp>
#include <Reference.hpp>

namespace godot
{
	class InterruptRequestEnableRegister : public Reference
	{
		GODOT_CLASS(InterruptRequestEnableRegister, Reference)

	private:
	public:
		enum Bits
		{
			SERIAL_DATA_IRQ_ENABLED = 0x01,
			KEYBOARD_IRQ_ENABLED = 0x02,
			CARTRIDGE_IRQ_ENABLED = 0x04,
			VERTICAL_BORDER_IRQ_ENABLED = 0x08,
			HORIZONTAL_BORDER_IRQ_ENABLED = 0x10,
			TIMER_IRQ_ENABLED = 0x20
		};

		union
		{
			std::uint8_t byte;
			struct
			{
				bool serial_data_irq_enabled : 1;
				bool keyboard_irq_enabled : 1;
				bool cartridge_irq_enabled : 1;
				bool vertical_border_irq_enabled : 1;
				bool horizontal_border_irq_enabled : 1;
				bool timer_irq_enabled : 1;
			} bits;
		};

		// C++ Specific Definitions

		InterruptRequestEnableRegister();

		~InterruptRequestEnableRegister();

		// Godot Specific Definitions

		static void _register_methods();

		void _init(); // our initializer called by Godot

		// Class Definitions

		void set_register(std::uint8_t new_value);
		std::uint8_t get_register();

		void set_bit(int bit, bool new_value);
		bool get_bit(int bit);

		void set_serial_data_irq_enabled(bool new_value);
		bool get_serial_data_irq_enabled();

		void set_keyboard_irq_enabled(bool new_value);
		bool get_keyboard_irq_enabled();

		void set_cartridge_irq_enabled(bool new_value);
		bool get_cartridge_irq_enabled();

		void set_vertical_border_irq_enabled(bool new_value);
		bool get_vertical_border_irq_enabled();

		void set_horizontal_border_irq_enabled(bool new_value);
		bool get_horizontal_border_irq_enabled();

		void set_timer_irq_enabled(bool new_value);
		bool get_timer_irq_enabled();
	};
}

#endif