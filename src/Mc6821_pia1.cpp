#include "Mc6821_pia1.hpp"

using namespace godot;

Mc6821_PIA1::Mc6821_PIA1()
{
	data_register_a.byte = 0;
	control_register_a.byte = 0;
	data_direction_register_a.byte = 0;
	data_register_b.byte = 0;
	control_register_b.byte = 0;
	data_direction_register_b.byte = 0;
}

Mc6821_PIA1::~Mc6821_PIA1()
{
}

void Mc6821_PIA1::_register_methods()
{
	// Properties
	register_property<Mc6821_PIA1, std::uint8_t>("data_register_a", &Mc6821_PIA1::set_data_register_a, &Mc6821_PIA1::get_data_register_a, 0);
	register_property<Mc6821_PIA1, bool>("keyboard_row_1", &Mc6821_PIA1::set_keyboard_row_1, &Mc6821_PIA1::get_keyboard_row_1, false);
	register_property<Mc6821_PIA1, bool>("keyboard_row_2", &Mc6821_PIA1::set_keyboard_row_2, &Mc6821_PIA1::get_keyboard_row_2, false);
	register_property<Mc6821_PIA1, bool>("keyboard_row_3", &Mc6821_PIA1::set_keyboard_row_3, &Mc6821_PIA1::get_keyboard_row_3, false);
	register_property<Mc6821_PIA1, bool>("keyboard_row_4", &Mc6821_PIA1::set_keyboard_row_4, &Mc6821_PIA1::get_keyboard_row_4, false);
	register_property<Mc6821_PIA1, bool>("keyboard_row_5", &Mc6821_PIA1::set_keyboard_row_5, &Mc6821_PIA1::get_keyboard_row_5, false);
	register_property<Mc6821_PIA1, bool>("keyboard_row_6", &Mc6821_PIA1::set_keyboard_row_6, &Mc6821_PIA1::get_keyboard_row_6, false);
	register_property<Mc6821_PIA1, bool>("keyboard_row_7", &Mc6821_PIA1::set_keyboard_row_7, &Mc6821_PIA1::get_keyboard_row_7, false);
	register_property<Mc6821_PIA1, bool>("joystick_comparison_input", &Mc6821_PIA1::set_joystick_comparison_input, &Mc6821_PIA1::get_joystick_comparison_input, false);
	register_property<Mc6821_PIA1, std::uint8_t>("control_register_a", &Mc6821_PIA1::set_control_register_a, &Mc6821_PIA1::get_control_register_a, 0);
	register_property<Mc6821_PIA1, bool>("horizontal_sync_interrupt_enable", &Mc6821_PIA1::set_horizontal_sync_interrupt_enable, &Mc6821_PIA1::get_horizontal_sync_interrupt_enable, false);
	register_property<Mc6821_PIA1, bool>("horizontal_sync_interrupt_polarity", &Mc6821_PIA1::set_horizontal_sync_interrupt_polarity, &Mc6821_PIA1::get_horizontal_sync_interrupt_polarity, false);
	register_property<Mc6821_PIA1, bool>("data_direction_a", &Mc6821_PIA1::set_data_direction_a, &Mc6821_PIA1::get_data_direction_a, false);
	register_property<Mc6821_PIA1, bool>("mux_select_0", &Mc6821_PIA1::set_mux_select_0, &Mc6821_PIA1::get_mux_select_0, false);
	register_property<Mc6821_PIA1, bool>("horizontal_sync_interrupt_flag", &Mc6821_PIA1::set_horizontal_sync_interrupt_flag, &Mc6821_PIA1::get_horizontal_sync_interrupt_flag, false);
	register_property<Mc6821_PIA1, std::uint8_t>("data_direction_register_a", &Mc6821_PIA1::set_data_direction_register_a, &Mc6821_PIA1::get_data_direction_register_a, 0);
	register_property<Mc6821_PIA1, bool>("ddra_0", &Mc6821_PIA1::set_ddra_0, &Mc6821_PIA1::get_ddra_0, false);
	register_property<Mc6821_PIA1, bool>("ddra_1", &Mc6821_PIA1::set_ddra_1, &Mc6821_PIA1::get_ddra_1, false);
	register_property<Mc6821_PIA1, bool>("ddra_2", &Mc6821_PIA1::set_ddra_2, &Mc6821_PIA1::get_ddra_2, false);
	register_property<Mc6821_PIA1, bool>("ddra_3", &Mc6821_PIA1::set_ddra_3, &Mc6821_PIA1::get_ddra_3, false);
	register_property<Mc6821_PIA1, bool>("ddra_4", &Mc6821_PIA1::set_ddra_4, &Mc6821_PIA1::get_ddra_4, false);
	register_property<Mc6821_PIA1, bool>("ddra_5", &Mc6821_PIA1::set_ddra_5, &Mc6821_PIA1::get_ddra_5, false);
	register_property<Mc6821_PIA1, bool>("ddra_6", &Mc6821_PIA1::set_ddra_6, &Mc6821_PIA1::get_ddra_6, false);
	register_property<Mc6821_PIA1, bool>("ddra_7", &Mc6821_PIA1::set_ddra_7, &Mc6821_PIA1::get_ddra_7, false);
	register_property<Mc6821_PIA1, std::uint8_t>("data_register_b", &Mc6821_PIA1::set_data_register_b, &Mc6821_PIA1::get_data_register_b, 0);
	register_property<Mc6821_PIA1, bool>("keyboard_column_1", &Mc6821_PIA1::set_keyboard_column_1, &Mc6821_PIA1::get_keyboard_column_1, false);
	register_property<Mc6821_PIA1, bool>("keyboard_column_2", &Mc6821_PIA1::set_keyboard_column_2, &Mc6821_PIA1::get_keyboard_column_2, false);
	register_property<Mc6821_PIA1, bool>("keyboard_column_3", &Mc6821_PIA1::set_keyboard_column_3, &Mc6821_PIA1::get_keyboard_column_3, false);
	register_property<Mc6821_PIA1, bool>("keyboard_column_4", &Mc6821_PIA1::set_keyboard_column_4, &Mc6821_PIA1::get_keyboard_column_4, false);
	register_property<Mc6821_PIA1, bool>("keyboard_column_5", &Mc6821_PIA1::set_keyboard_column_5, &Mc6821_PIA1::get_keyboard_column_5, false);
	register_property<Mc6821_PIA1, bool>("keyboard_column_6", &Mc6821_PIA1::set_keyboard_column_6, &Mc6821_PIA1::get_keyboard_column_6, false);
	register_property<Mc6821_PIA1, bool>("keyboard_column_7", &Mc6821_PIA1::set_keyboard_column_7, &Mc6821_PIA1::get_keyboard_column_7, false);
	register_property<Mc6821_PIA1, bool>("keyboard_column_8", &Mc6821_PIA1::set_keyboard_column_8, &Mc6821_PIA1::get_keyboard_column_8, false);
	register_property<Mc6821_PIA1, bool>("ram_size_output", &Mc6821_PIA1::set_ram_size_output, &Mc6821_PIA1::get_ram_size_output, false);
	register_property<Mc6821_PIA1, std::uint8_t>("control_register_b", &Mc6821_PIA1::set_control_register_b, &Mc6821_PIA1::get_control_register_b, 0);
	register_property<Mc6821_PIA1, bool>("vertical_sync_interrupt_enable", &Mc6821_PIA1::set_vertical_sync_interrupt_enable, &Mc6821_PIA1::get_vertical_sync_interrupt_enable, false);
	register_property<Mc6821_PIA1, bool>("vertical_sync_interrupt_polarity", &Mc6821_PIA1::set_vertical_sync_interrupt_polarity, &Mc6821_PIA1::get_vertical_sync_interrupt_polarity, false);
	register_property<Mc6821_PIA1, bool>("data_direction_b", &Mc6821_PIA1::set_data_direction_a, &Mc6821_PIA1::get_data_direction_b, false);
	register_property<Mc6821_PIA1, bool>("mux_select_1", &Mc6821_PIA1::set_mux_select_1, &Mc6821_PIA1::get_mux_select_1, false);
	register_property<Mc6821_PIA1, bool>("vertical_sync_interrupt_flag", &Mc6821_PIA1::set_vertical_sync_interrupt_flag, &Mc6821_PIA1::get_vertical_sync_interrupt_flag, false);
	register_property<Mc6821_PIA1, std::uint8_t>("data_direction_register_b", &Mc6821_PIA1::set_data_direction_register_b, &Mc6821_PIA1::get_data_direction_register_b, 0);
	register_property<Mc6821_PIA1, bool>("ddrb_0", &Mc6821_PIA1::set_ddrb_0, &Mc6821_PIA1::get_ddrb_0, false);
	register_property<Mc6821_PIA1, bool>("ddrb_1", &Mc6821_PIA1::set_ddrb_1, &Mc6821_PIA1::get_ddrb_1, false);
	register_property<Mc6821_PIA1, bool>("ddrb_2", &Mc6821_PIA1::set_ddrb_2, &Mc6821_PIA1::get_ddrb_2, false);
	register_property<Mc6821_PIA1, bool>("ddrb_3", &Mc6821_PIA1::set_ddrb_3, &Mc6821_PIA1::get_ddrb_3, false);
	register_property<Mc6821_PIA1, bool>("ddrb_4", &Mc6821_PIA1::set_ddrb_4, &Mc6821_PIA1::get_ddrb_4, false);
	register_property<Mc6821_PIA1, bool>("ddrb_5", &Mc6821_PIA1::set_ddrb_5, &Mc6821_PIA1::get_ddrb_5, false);
	register_property<Mc6821_PIA1, bool>("ddrb_6", &Mc6821_PIA1::set_ddrb_6, &Mc6821_PIA1::get_ddrb_6, false);
	register_property<Mc6821_PIA1, bool>("ddrb_7", &Mc6821_PIA1::set_ddrb_7, &Mc6821_PIA1::get_ddrb_7, false);

	// Signals
	register_signal<Mc6821_PIA1>("data_register_a_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<Mc6821_PIA1>("keyboard_row_1_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("keyboard_row_2_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("keyboard_row_3_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("keyboard_row_4_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("keyboard_row_5_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("keyboard_row_6_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("keyboard_row_7_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("joystick_comparison_input", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("control_register_a_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<Mc6821_PIA1>("horizontal_sync_interrupt_enable_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("horizontal_sync_interrupt_polarity_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("data_direction_a_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("mux_select_0_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("horizontal_sync_interrupt_flag", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("data_direction_register_a_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<Mc6821_PIA1>("ddra_0_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("ddra_1_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("ddra_2_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("ddra_3_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("ddra_4_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("ddra_5_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("ddra_6_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("ddra_7_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("data_register_b_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<Mc6821_PIA1>("keyboard_column_1_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("keyboard_column_2_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("keyboard_column_3_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("keyboard_column_4_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("keyboard_column_5_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("keyboard_column_6_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("keyboard_column_7_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("keyboard_column_8_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("ram_size_output_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("control_register_b_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<Mc6821_PIA1>("vertical_sync_interrupt_enable_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("vertical_sync_interrupt_polarity_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("data_direction_b_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("mux_select_1_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("vertical_sync_interrupt_flag_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("data_direction_register_b_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<Mc6821_PIA1>("ddrb_0_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("ddrb_1_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("ddrb_2_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("ddrb_3_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("ddrb_4_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("ddrb_5_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("ddrb_6_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA1>("ddrb_7_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
}

void Mc6821_PIA1::_init()
{
}

/**
 * -----------------------------------
 * | Address | PIA Registers         |
 * -----------------------------------
 * |  FF00   | DRA and DDRA of PIA 1 |
 * |  FF01   | CRA of PIA 1          |
 * |  FF02   | DRB and DDRB of PIA 1 |
 * |  FF03   | CRB of PIA 1          |
 * |  FF20   | DRA and DDRA of PIA 2 |
 * |  FF21   | CRA of PIA 2          |
 * |  FF22   | DRB and DDRB of PIA 2 |
 * |  FF23   | CRB of PIA 2          |
 * -----------------------------------
 *
 * Table 9-7 Primary Dedicated Addreses of the PIA Registers.
 */
std::uint8_t Mc6821_PIA1::read(std::uint8_t address)
{
	address &= 0xFF;

	switch (address)
	{
	case 0x00:
		// DRA and DDRA of PIA 1
		if (control_register_a.bits.accessing_data_register)
		{
			/* Upon reading the data register, its content will be in an MPU register and bits 6
			 * and 7 in the control register are cleared.
			 */
			set_control_register_a(control_register_a.byte & 0x3F);

			/* If a bit in the DDRA is clear, the corresponding bit in the DRA is configured for
			 * inputting. If a bit in the DDRA is set, the corresponding bit in the DRA is
			 * configured for outputting
			 */
			return data_register_a.byte & (~data_direction_register_a.byte);
		}
		else
		{
			return data_direction_register_a.byte;
		}

	case 0x01:
		// CRA of PIA 1
		return control_register_a.byte;

	case 0x02:
		// DRB and DDRB of PIA 1
		if (control_register_b.bits.accessing_data_register)
		{
			/* Upon reading the data register, its content will be in an MPU register and bits 6
			 * and 7 in the control register are cleared.
			 */
			set_control_register_b(control_register_b.byte & 0x3F);

			/* If a bit in the DDRA is clear, the corresponding bit in the DRA is configured for
			 * inputting. If a bit in the DDRA is set, the corresponding bit in the DRA is
			 * configured for outputting
			 */
			return data_register_b.byte & (~data_direction_register_b.byte);
		}
		else
		{
			return data_direction_register_a.byte;
		}

	case 0x03:
		// CRB of PIA 1
		return control_register_b.byte;
	}

	return 0;
}

void Mc6821_PIA1::write(std::uint8_t address, std::uint8_t data)
{
	address &= 0xFF;

	switch (address)
	{
	case 0x00:
		// DRA AND DDRA of PIA 1
		if (control_register_a.bits.accessing_data_register)
		{
			set_data_register_a(data);
		}
		else
		{
			set_data_direction_register_a(data);
		}
		break;

	case 0x01:
		// CRA of PIA 1
		set_control_register_a(data & 0x3F);
		break;

	case 0x02:
		// DRB and DDRB of PIA 1
		if (control_register_b.bits.accessing_data_register)
		{
			set_data_register_b(data);
		}
		else
		{
			set_data_direction_register_b(data);
		}
		break;

	case 0x03:
		// CRB of PIA 1
		set_control_register_b(data & 0x3F);
		break;
	}
}

void Mc6821_PIA1::horizontal_sync_transition(bool sync_value)
{
	if (sync_value)
	{
		// low-to-high
		if (control_register_a.bits.horizontal_sync_interrupt_polarity)
		{
			set_horizontal_sync_interrupt_flag(true);
			if (control_register_a.bits.horizontal_sync_interrupt_enable)
			{
				mpu_pins_state->set_irq(true);
				mpu_pins_state->delay_irq = true;
			}
		}
	}
	else
	{
		// high-to-low
		if (!control_register_a.bits.horizontal_sync_interrupt_polarity)
		{
			set_horizontal_sync_interrupt_flag(true);
			if (control_register_a.bits.horizontal_sync_interrupt_enable)
			{
				mpu_pins_state->set_irq(true);
				mpu_pins_state->delay_irq = true;
			}
		}
	}
}

void Mc6821_PIA1::vertical_sync_transition(bool sync_value)
{
	/*     Bit 1, when clear, causes the CA1 signal to set an active state in the PIA when the
	 * signal received by CA1 from another device changes from a high (logical 1) to a low
	 * (logical 0).
	 *     Bit 1, when set, causes the CA1 signal to set an active state in the PIA when the
	 * signal received by CA1 from another device changes from a low (logical 0) to a high
	 * (logical 1).
	 */

	/*     Bit 0, when clear, lets the CA1 active state set bit 7 (CA1 flag) of the CRA. Bit 7 set
	 * indicate that CA1 was activated by the required transition.
	 *     Bit 0, when set, lets the CA1 active state set bit 7 of the CRA and send an interrupt to
	 * the MPU. Bit 0 of the CRA can enable or disable the sending of interrupt generated by CA1.
	 */

	if (sync_value)
	{
		// low-to-high
		if (control_register_b.bits.vertical_sync_interrupt_polarity)
		{
			set_vertical_sync_interrupt_flag(true);
			if (control_register_b.bits.vertical_sync_interrupt_enable)
			{
				mpu_pins_state->set_irq(true);
				mpu_pins_state->delay_irq = true;
			}
		}
	}
	else
	{
		// high-to=low
		if (!control_register_b.bits.vertical_sync_interrupt_polarity)
		{
			set_vertical_sync_interrupt_flag(true);
			if (control_register_b.bits.vertical_sync_interrupt_enable)
			{
				mpu_pins_state->set_irq(true);
				mpu_pins_state->delay_irq = true;
			}
		}
	}
}

void Mc6821_PIA1::set_data_register_a(std::uint8_t new_value)
{
	if (data_register_a.byte != new_value)
	{
		if (!debug)
		{
			data_register_a.byte = new_value;
		}
		else
		{
			bool old_keyboard_row_1 = data_register_a.bits.keyboard_row_1;
			bool old_keyboard_row_2 = data_register_a.bits.keyboard_row_2;
			bool old_keyboard_row_3 = data_register_a.bits.keyboard_row_3;
			bool old_keyboard_row_4 = data_register_a.bits.keyboard_row_4;
			bool old_keyboard_row_5 = data_register_a.bits.keyboard_row_5;
			bool old_keyboard_row_6 = data_register_a.bits.keyboard_row_6;
			bool old_keyboard_row_7 = data_register_a.bits.keyboard_row_7;
			bool old_joystick_comparison_input = data_register_a.bits.joystick_comparison_input;

			data_register_a.byte = new_value;

			if (old_keyboard_row_1 != data_register_a.bits.keyboard_row_1)
			{
				emit_signal("keyboard_row_1_toggled", data_register_a.bits.keyboard_row_1);
			}

			if (old_keyboard_row_2 != data_register_a.bits.keyboard_row_2)
			{
				emit_signal("keyboard_row_2_toggled", data_register_a.bits.keyboard_row_2);
			}

			if (old_keyboard_row_3 != data_register_a.bits.keyboard_row_3)
			{
				emit_signal("keyboard_row_3_toggled", data_register_a.bits.keyboard_row_3);
			}

			if (old_keyboard_row_4 != data_register_a.bits.keyboard_row_4)
			{
				emit_signal("keyboard_row_4_toggled", data_register_a.bits.keyboard_row_4);
			}

			if (old_keyboard_row_5 != data_register_a.bits.keyboard_row_5)
			{
				emit_signal("keyboard_row_5_toggled", data_register_a.bits.keyboard_row_5);
			}

			if (old_keyboard_row_6 != data_register_a.bits.keyboard_row_6)
			{
				emit_signal("keyboard_row_6_toggled", data_register_a.bits.keyboard_row_6);
			}

			if (old_keyboard_row_7 != data_register_a.bits.keyboard_row_7)
			{
				emit_signal("keyboard_row_7_toggled", data_register_a.bits.keyboard_row_7);
			}

			if (old_joystick_comparison_input != data_register_a.bits.joystick_comparison_input)
			{
				emit_signal("joystick_comparison_input_toggled", data_register_a.bits.joystick_comparison_input);
			}

			emit_signal("data_register_a_set", new_value);
		}
	}
}

std::uint8_t Mc6821_PIA1::get_data_register_a()
{
	return data_register_a.byte;
}

void Mc6821_PIA1::set_keyboard_row_1(bool new_value)
{
	if (data_register_a.bits.keyboard_row_1 != new_value)
	{
		data_register_a.bits.keyboard_row_1 = new_value;
		if (debug)
		{
			emit_signal("keyboard_row_1_toggled", new_value);
			emit_signal("data_register_a_set", data_register_a.byte);
		}
	}
}

bool Mc6821_PIA1::get_keyboard_row_1()
{
	return data_register_a.bits.keyboard_row_1;
}

void Mc6821_PIA1::set_keyboard_row_2(bool new_value)
{
	if (data_register_a.bits.keyboard_row_2 != new_value)
	{
		data_register_a.bits.keyboard_row_2 = new_value;
		if (debug)
		{
			emit_signal("keyboard_row_2_toggled", new_value);
			emit_signal("data_register_a_set", data_register_a.byte);
		}
	}
}

bool Mc6821_PIA1::get_keyboard_row_2()
{
	return data_register_a.bits.keyboard_row_2;
}

void Mc6821_PIA1::set_keyboard_row_3(bool new_value)
{
	if (data_register_a.bits.keyboard_row_3 != new_value)
	{
		data_register_a.bits.keyboard_row_3 = new_value;
		if (debug)
		{
			emit_signal("keyboard_row_3_toggled", new_value);
			emit_signal("data_register_a_set", data_register_a.byte);
		}
	}
}

bool Mc6821_PIA1::get_keyboard_row_3()
{
	return data_register_a.bits.keyboard_row_3;
}

void Mc6821_PIA1::set_keyboard_row_4(bool new_value)
{
	if (data_register_a.bits.keyboard_row_4 != new_value)
	{
		data_register_a.bits.keyboard_row_4 = new_value;
		if (debug)
		{
			emit_signal("keyboard_row_4_toggled", new_value);
			emit_signal("data_register_a_set", data_register_a.byte);
		}
	}
}

bool Mc6821_PIA1::get_keyboard_row_4()
{
	return data_register_a.bits.keyboard_row_4;
}

void Mc6821_PIA1::set_keyboard_row_5(bool new_value)
{
	if (data_register_a.bits.keyboard_row_5 != new_value)
	{
		data_register_a.bits.keyboard_row_5 = new_value;
		if (debug)
		{
			emit_signal("keyboard_row_5_toggled", new_value);
			emit_signal("data_register_a_set", data_register_a.byte);
		}
	}
}

bool Mc6821_PIA1::get_keyboard_row_5()
{
	return data_register_a.bits.keyboard_row_5;
}

void Mc6821_PIA1::set_keyboard_row_6(bool new_value)
{
	if (data_register_a.bits.keyboard_row_6 != new_value)
	{
		data_register_a.bits.keyboard_row_6 = new_value;
		if (debug)
		{
			emit_signal("keyboard_row_6_toggled", new_value);
			emit_signal("data_register_a_set", data_register_a.byte);
		}
	}
}

bool Mc6821_PIA1::get_keyboard_row_6()
{
	return data_register_a.bits.keyboard_row_6;
}

void Mc6821_PIA1::set_keyboard_row_7(bool new_value)
{
	if (data_register_a.bits.keyboard_row_7 != new_value)
	{
		data_register_a.bits.keyboard_row_7 = new_value;
		if (debug)
		{
			emit_signal("keyboard_row_7_toggled", new_value);
			emit_signal("data_register_a_set", data_register_a.byte);
		}
	}
}

bool Mc6821_PIA1::get_keyboard_row_7()
{
	return data_register_a.bits.keyboard_row_7;
}

void Mc6821_PIA1::set_joystick_comparison_input(bool new_value)
{
	if (data_register_a.bits.joystick_comparison_input != new_value)
	{
		data_register_a.bits.joystick_comparison_input = new_value;
		if (debug)
		{
			emit_signal("joystick_comparison_input_toggled", new_value);
			emit_signal("data_register_a_set", data_register_a.byte);
		}
	}
}

bool Mc6821_PIA1::get_joystick_comparison_input()
{
	return data_register_a.bits.joystick_comparison_input;
}

void Mc6821_PIA1::set_control_register_a(std::uint8_t new_value)
{
	if (control_register_a.byte != new_value)
	{
		if (!debug)
		{
			control_register_a.byte = new_value;
		}
		else
		{
			bool old_horizontal_sync_interrupt_enable = control_register_a.bits.horizontal_sync_interrupt_enable;
			bool old_horizontal_sync_interrupt_polarity = control_register_a.bits.horizontal_sync_interrupt_polarity;
			bool old_accessing_data_register = control_register_a.bits.accessing_data_register;
			bool old_mux_select_0 = control_register_a.bits.mux_select_0;
			bool old_horizontal_sync_interrupt_flag = control_register_a.bits.horizontal_sync_interrupt_flag;

			control_register_a.byte = new_value;

			if (old_horizontal_sync_interrupt_enable != control_register_a.bits.horizontal_sync_interrupt_enable)
			{
				emit_signal("horizontal_sync_interrupt_enable_toggled", control_register_a.bits.horizontal_sync_interrupt_enable);
			}

			if (old_horizontal_sync_interrupt_polarity != control_register_a.bits.horizontal_sync_interrupt_polarity)
			{
				emit_signal("horizontal_sync_interrupt_polarity_toggled", control_register_a.bits.horizontal_sync_interrupt_polarity);
			}

			if (old_accessing_data_register != control_register_a.bits.accessing_data_register)
			{
				emit_signal("data_direction_a_toggled", control_register_a.bits.accessing_data_register);
			}

			if (old_mux_select_0 != control_register_a.bits.mux_select_0)
			{
				emit_signal("mux_select_0_toggled", control_register_a.bits.mux_select_0);
			}

			if (old_horizontal_sync_interrupt_flag != control_register_a.bits.horizontal_sync_interrupt_flag)
			{
				emit_signal("horizontal_sync_interrupt_flag", control_register_a.bits.horizontal_sync_interrupt_flag);
			}

			emit_signal("control_register_a_set", new_value);
		}
	}
}

std::uint8_t Mc6821_PIA1::get_control_register_a()
{
	return control_register_a.byte;
}

void Mc6821_PIA1::set_horizontal_sync_interrupt_enable(bool new_value)
{
	if (control_register_a.bits.horizontal_sync_interrupt_enable != new_value)
	{
		control_register_a.bits.horizontal_sync_interrupt_enable = new_value;
		if (debug)
		{
			emit_signal("horizontal_sync_interrupt_enable_toggled", new_value);
			emit_signal("control_register_a_set", control_register_a.byte);
		}
	}
}

bool Mc6821_PIA1::get_horizontal_sync_interrupt_enable()
{
	return control_register_a.bits.horizontal_sync_interrupt_enable;
}

void Mc6821_PIA1::set_horizontal_sync_interrupt_polarity(bool new_value)
{
	if (control_register_a.bits.horizontal_sync_interrupt_polarity != new_value)
	{
		control_register_a.bits.horizontal_sync_interrupt_polarity = new_value;
		if (debug)
		{
			emit_signal("horizontal_sync_interrupt_polarity_toggled", new_value);
			emit_signal("control_register_a_set", control_register_a.byte);
		}
	}
}

bool Mc6821_PIA1::get_horizontal_sync_interrupt_polarity()
{
	return control_register_a.bits.horizontal_sync_interrupt_polarity;
}

void Mc6821_PIA1::set_data_direction_a(bool new_value)
{
	if (control_register_a.bits.accessing_data_register != new_value)
	{
		control_register_a.bits.accessing_data_register = new_value;
		if (debug)
		{
			emit_signal("data_direction_a_toggled", new_value);
			emit_signal("control_register_a_set", control_register_a.byte);
		}
	}
}

bool Mc6821_PIA1::get_data_direction_a()
{
	return control_register_a.bits.accessing_data_register;
}

void Mc6821_PIA1::set_mux_select_0(bool new_value)
{
	if (control_register_a.bits.mux_select_0 != new_value)
	{
		control_register_a.bits.mux_select_0 = new_value;
		if (debug)
		{
			emit_signal("mux_select_0_toggled", new_value);
			emit_signal("control_register_a_set", control_register_a.byte);
		}
	}
}

bool Mc6821_PIA1::get_mux_select_0()
{
	return control_register_a.bits.mux_select_0;
}

void Mc6821_PIA1::set_horizontal_sync_interrupt_flag(bool new_value)
{
	if (control_register_a.bits.horizontal_sync_interrupt_flag != new_value)
	{
		control_register_a.bits.horizontal_sync_interrupt_flag = new_value;
		if (debug)
		{
			emit_signal("horizontal_sync_interrupt_flag_toggled", new_value);
			emit_signal("control_register_a_set", control_register_a.byte);
		}
	}
}

bool Mc6821_PIA1::get_horizontal_sync_interrupt_flag()
{
	return control_register_a.bits.horizontal_sync_interrupt_flag;
}

void Mc6821_PIA1::set_data_direction_register_a(std::uint8_t new_value)
{
	if (data_direction_register_a.byte != new_value)
	{
		if (!debug)
		{
			data_direction_register_a.byte = new_value;
		}
		else
		{
			bool old_ddra_0 = data_direction_register_a.bits.ddra_0;
			bool old_ddra_1 = data_direction_register_a.bits.ddra_1;
			bool old_ddra_2 = data_direction_register_a.bits.ddra_2;
			bool old_ddra_3 = data_direction_register_a.bits.ddra_3;
			bool old_ddra_4 = data_direction_register_a.bits.ddra_4;
			bool old_ddra_5 = data_direction_register_a.bits.ddra_5;
			bool old_ddra_6 = data_direction_register_a.bits.ddra_6;
			bool old_ddra_7 = data_direction_register_a.bits.ddra_7;

			data_direction_register_a.byte = new_value;

			if (old_ddra_0 != data_direction_register_a.bits.ddra_0)
			{
				emit_signal("ddra_0_toggled", data_direction_register_a.bits.ddra_0);
			}

			if (old_ddra_1 != data_direction_register_a.bits.ddra_1)
			{
				emit_signal("ddra_1_toggled", data_direction_register_a.bits.ddra_1);
			}

			if (old_ddra_2 != data_direction_register_a.bits.ddra_2)
			{
				emit_signal("ddra_2_toggled", data_direction_register_a.bits.ddra_2);
			}

			if (old_ddra_3 != data_direction_register_a.bits.ddra_3)
			{
				emit_signal("ddra_3_toggled", data_direction_register_a.bits.ddra_3);
			}

			if (old_ddra_4 != data_direction_register_a.bits.ddra_4)
			{
				emit_signal("ddra_4_toggled", data_direction_register_a.bits.ddra_4);
			}

			if (old_ddra_5 != data_direction_register_a.bits.ddra_5)
			{
				emit_signal("ddra_5_toggled", data_direction_register_a.bits.ddra_5);
			}

			if (old_ddra_6 != data_direction_register_a.bits.ddra_6)
			{
				emit_signal("ddra_6_toggled", data_direction_register_a.bits.ddra_6);
			}

			if (old_ddra_7 != data_direction_register_a.bits.ddra_7)
			{
				emit_signal("ddra_7_toggled", data_direction_register_a.bits.ddra_7);
			}

			emit_signal("data_direction_register_a_set", new_value);
		}
	}
}

std::uint8_t Mc6821_PIA1::get_data_direction_register_a()
{
	return data_direction_register_a.byte;
}

void Mc6821_PIA1::set_ddra_0(bool new_value)
{
	if (data_direction_register_a.bits.ddra_0 != new_value)
	{
		data_direction_register_a.bits.ddra_0 = new_value;
		if (debug)
		{
			emit_signal("ddra_0_toggled", new_value);
			emit_signal("data_direction_register_a_set", data_direction_register_a.byte);
		}
	}
}

bool Mc6821_PIA1::get_ddra_0()
{
	return data_direction_register_a.bits.ddra_0;
}

void Mc6821_PIA1::set_ddra_1(bool new_value)
{
	if (data_direction_register_a.bits.ddra_1 != new_value)
	{
		data_direction_register_a.bits.ddra_1 = new_value;
		if (debug)
		{
			emit_signal("ddra_1_toggled", new_value);
			emit_signal("data_direction_register_a_set", data_direction_register_a.byte);
		}
	}
}

bool Mc6821_PIA1::get_ddra_1()
{
	return data_direction_register_a.bits.ddra_1;
}

void Mc6821_PIA1::set_ddra_2(bool new_value)
{
	if (data_direction_register_a.bits.ddra_2 != new_value)
	{
		data_direction_register_a.bits.ddra_2 = new_value;
		if (debug)
		{
			emit_signal("ddra_2_toggled", new_value);
			emit_signal("data_direction_register_a_set", data_direction_register_a.byte);
		}
	}
}

bool Mc6821_PIA1::get_ddra_2()
{
	return data_direction_register_a.bits.ddra_2;
}

void Mc6821_PIA1::set_ddra_3(bool new_value)
{
	if (data_direction_register_a.bits.ddra_3 != new_value)
	{
		data_direction_register_a.bits.ddra_3 = new_value;
		if (debug)
		{
			emit_signal("ddra_3_toggled", new_value);
			emit_signal("data_direction_register_a_set", data_direction_register_a.byte);
		}
	}
}

bool Mc6821_PIA1::get_ddra_3()
{
	return data_direction_register_a.bits.ddra_3;
}

void Mc6821_PIA1::set_ddra_4(bool new_value)
{
	if (data_direction_register_a.bits.ddra_4 != new_value)
	{
		data_direction_register_a.bits.ddra_4 = new_value;
		if (debug)
		{
			emit_signal("ddra_4_toggled", new_value);
			emit_signal("data_direction_register_a_set", data_direction_register_a.byte);
		}
	}
}

bool Mc6821_PIA1::get_ddra_4()
{
	return data_direction_register_a.bits.ddra_4;
}

void Mc6821_PIA1::set_ddra_5(bool new_value)
{
	if (data_direction_register_a.bits.ddra_5 != new_value)
	{
		data_direction_register_a.bits.ddra_5 = new_value;
		if (debug)
		{
			emit_signal("ddra_5_toggled", new_value);
			emit_signal("data_direction_register_a_set", data_direction_register_a.byte);
		}
	}
}

bool Mc6821_PIA1::get_ddra_5()
{
	return data_direction_register_a.bits.ddra_5;
}

void Mc6821_PIA1::set_ddra_6(bool new_value)
{
	if (data_direction_register_a.bits.ddra_6 != new_value)
	{
		data_direction_register_a.bits.ddra_6 = new_value;
		if (debug)
		{
			emit_signal("ddra_6_toggled", new_value);
			emit_signal("data_direction_register_a_set", data_direction_register_a.byte);
		}
	}
}

bool Mc6821_PIA1::get_ddra_6()
{
	return data_direction_register_a.bits.ddra_6;
}

void Mc6821_PIA1::set_ddra_7(bool new_value)
{
	if (data_direction_register_a.bits.ddra_7 != new_value)
	{
		data_direction_register_a.bits.ddra_7 = new_value;
		if (debug)
		{
			emit_signal("ddra_7_toggled", new_value);
			emit_signal("data_direction_register_a_set", data_direction_register_a.byte);
		}
	}
}

bool Mc6821_PIA1::get_ddra_7()
{
	return data_direction_register_a.bits.ddra_7;
}

void Mc6821_PIA1::set_data_register_b(std::uint8_t new_value)
{
	if (data_register_b.byte != new_value)
	{
		if (!debug)
		{
			data_register_b.byte = new_value;
		}
		else
		{
			bool old_keyboard_column_1 = data_register_b.bits.keyboard_column_1;
			bool old_keyboard_column_2 = data_register_b.bits.keyboard_column_2;
			bool old_keyboard_column_3 = data_register_b.bits.keyboard_column_3;
			bool old_keyboard_column_4 = data_register_b.bits.keyboard_column_4;
			bool old_keyboard_column_5 = data_register_b.bits.keyboard_column_5;
			bool old_keyboard_column_6 = data_register_b.bits.keyboard_column_6;
			bool old_keyboard_column_7 = data_register_b.bits.keyboard_column_7;
			bool old_keyboard_column_8 = data_register_b.bits.keyboard_column_8;
			bool old_ram_size_output = data_register_b.output.ram_size_output;

			data_register_b.byte = new_value;

			if (old_keyboard_column_1 != data_register_b.bits.keyboard_column_1)
			{
				emit_signal("keyboard_column_1_toggled", data_register_b.bits.keyboard_column_1);
			}

			if (old_keyboard_column_2 != data_register_b.bits.keyboard_column_2)
			{
				emit_signal("keyboard_column_2_toggled", data_register_b.bits.keyboard_column_1);
			}

			if (old_keyboard_column_3 != data_register_b.bits.keyboard_column_3)
			{
				emit_signal("keyboard_column_3_toggled", data_register_b.bits.keyboard_column_1);
			}

			if (old_keyboard_column_4 != data_register_b.bits.keyboard_column_4)
			{
				emit_signal("keyboard_column_4_toggled", data_register_b.bits.keyboard_column_1);
			}

			if (old_keyboard_column_5 != data_register_b.bits.keyboard_column_5)
			{
				emit_signal("keyboard_column_5_toggled", data_register_b.bits.keyboard_column_1);
			}

			if (old_keyboard_column_6 != data_register_b.bits.keyboard_column_6)
			{
				emit_signal("keyboard_column_6_toggled", data_register_b.bits.keyboard_column_1);
			}

			if (old_keyboard_column_7 != data_register_b.bits.keyboard_column_7)
			{
				emit_signal("keyboard_column_7_toggled", data_register_b.bits.keyboard_column_1);
			}

			if (old_keyboard_column_8 != data_register_b.bits.keyboard_column_8)
			{
				emit_signal("keyboard_column_8_toggled", data_register_b.bits.keyboard_column_1);
			}

			if (old_ram_size_output != data_register_b.output.ram_size_output)
			{
				emit_signal("ram_size_output_toggled", data_register_b.output.ram_size_output);
			}

			emit_signal("data_register_b_set", new_value);
		}
	}
}

std::uint8_t Mc6821_PIA1::get_data_register_b()
{
	return data_register_b.byte;
}

void Mc6821_PIA1::set_keyboard_column_1(bool new_value)
{
	if (data_register_b.bits.keyboard_column_1 != new_value)
	{
		data_register_b.bits.keyboard_column_1 = new_value;
		if (debug)
		{
			emit_signal("keyboard_column_1_toggled", new_value);
			emit_signal("data_register_b_set", data_register_b.byte);
		}
	}
}

bool Mc6821_PIA1::get_keyboard_column_1()
{
	return data_register_b.bits.keyboard_column_1;
}

void Mc6821_PIA1::set_keyboard_column_2(bool new_value)
{
	if (data_register_b.bits.keyboard_column_2 != new_value)
	{
		data_register_b.bits.keyboard_column_2 = new_value;
		if (debug)
		{
			emit_signal("keyboard_column_2_toggled", new_value);
			emit_signal("data_register_b_set", data_register_b.byte);
		}
	}
}

bool Mc6821_PIA1::get_keyboard_column_2()
{
	return data_register_b.bits.keyboard_column_2;
}

void Mc6821_PIA1::set_keyboard_column_3(bool new_value)
{
	if (data_register_b.bits.keyboard_column_3 != new_value)
	{
		data_register_b.bits.keyboard_column_3 = new_value;
		if (debug)
		{
			emit_signal("keyboard_column_3_toggled", new_value);
			emit_signal("data_register_b_set", data_register_b.byte);
		}
	}
}

bool Mc6821_PIA1::get_keyboard_column_3()
{
	return data_register_b.bits.keyboard_column_3;
}

void Mc6821_PIA1::set_keyboard_column_4(bool new_value)
{
	if (data_register_b.bits.keyboard_column_4 != new_value)
	{
		data_register_b.bits.keyboard_column_4 = new_value;
		if (debug)
		{
			emit_signal("keyboard_column_4_toggled", new_value);
			emit_signal("data_register_b_set", data_register_b.byte);
		}
	}
}

bool Mc6821_PIA1::get_keyboard_column_4()
{
	return data_register_b.bits.keyboard_column_4;
}

void Mc6821_PIA1::set_keyboard_column_5(bool new_value)
{
	if (data_register_b.bits.keyboard_column_5 != new_value)
	{
		data_register_b.bits.keyboard_column_5 = new_value;
		if (debug)
		{
			emit_signal("keyboard_column_5_toggled", new_value);
			emit_signal("data_register_b_set", data_register_b.byte);
		}
	}
}

bool Mc6821_PIA1::get_keyboard_column_5()
{
	return data_register_b.bits.keyboard_column_5;
}

void Mc6821_PIA1::set_keyboard_column_6(bool new_value)
{
	if (data_register_b.bits.keyboard_column_6 != new_value)
	{
		data_register_b.bits.keyboard_column_6 = new_value;
		if (debug)
		{
			emit_signal("keyboard_column_6_toggled", new_value);
			emit_signal("data_register_b_set", data_register_b.byte);
		}
	}
}

bool Mc6821_PIA1::get_keyboard_column_6()
{
	return data_register_b.bits.keyboard_column_6;
}

void Mc6821_PIA1::set_keyboard_column_7(bool new_value)
{
	if (data_register_b.bits.keyboard_column_7 != new_value)
	{
		data_register_b.bits.keyboard_column_7 = new_value;
		if (debug)
		{
			emit_signal("ram_size_output_toggled", new_value);
			emit_signal("keyboard_column_7_toggled", new_value);
			emit_signal("data_register_b_set", data_register_b.byte);
		}
	}
}

bool Mc6821_PIA1::get_keyboard_column_7()
{
	return data_register_b.bits.keyboard_column_7;
}

void Mc6821_PIA1::set_keyboard_column_8(bool new_value)
{
	if (data_register_b.bits.keyboard_column_8 != new_value)
	{
		data_register_b.bits.keyboard_column_8 = new_value;
		if (debug)
		{
			emit_signal("keyboard_column_8_toggled", new_value);
			emit_signal("data_register_b_set", data_register_b.byte);
		}
	}
}

bool Mc6821_PIA1::get_keyboard_column_8()
{
	return data_register_b.bits.keyboard_column_8;
}

void Mc6821_PIA1::set_ram_size_output(bool new_value)
{
	if (data_register_b.output.ram_size_output != new_value)
	{
		data_register_b.output.ram_size_output = new_value;
		if (debug)
		{
			emit_signal("ram_size_output_toggled", new_value);
			emit_signal("data_register_b_set", data_register_b.byte);
		}
	}
}

bool Mc6821_PIA1::get_ram_size_output()
{
	return data_register_b.output.ram_size_output;
}

void Mc6821_PIA1::set_control_register_b(std::uint8_t new_value)
{
	if (control_register_b.byte != new_value)
	{
		if (!debug)
		{
			control_register_b.byte = new_value;
		}
		else
		{
			bool old_vertical_sync_interrupt_enable = control_register_b.bits.vertical_sync_interrupt_enable;
			bool old_vertical_sync_interrupt_polarity = control_register_b.bits.vertical_sync_interrupt_polarity;
			bool old_accessing_data_register = control_register_b.bits.accessing_data_register;
			bool old_mux_select_1 = control_register_b.bits.mux_select_1;
			bool old_vertical_sync_interrupt_flag = control_register_b.bits.vertical_sync_interrupt_flag;

			control_register_b.byte = new_value;

			if (old_vertical_sync_interrupt_enable != control_register_b.bits.vertical_sync_interrupt_enable)
			{
				emit_signal("vertical_sync_interrupt_enable_toggled", control_register_b.bits.vertical_sync_interrupt_enable);
			}

			if (old_vertical_sync_interrupt_polarity != control_register_b.bits.vertical_sync_interrupt_polarity)
			{
				emit_signal("vertical_sync_interrupt_polarity_toggled", control_register_b.bits.vertical_sync_interrupt_polarity);
			}

			if (old_accessing_data_register != control_register_b.bits.accessing_data_register)
			{
				emit_signal("data_direction_b_toggled", control_register_b.bits.accessing_data_register);
			}

			if (old_mux_select_1 != control_register_b.bits.mux_select_1)
			{
				emit_signal("mux_select_1_toggled", control_register_b.bits.mux_select_1);
			}

			if (old_vertical_sync_interrupt_flag != control_register_b.bits.vertical_sync_interrupt_flag)
			{
				emit_signal("vertical_sync_interrupt_flag_toggled", control_register_b.bits.vertical_sync_interrupt_flag);
			}

			emit_signal("control_register_b_set", new_value);
		}
	}
}

std::uint8_t Mc6821_PIA1::get_control_register_b()
{
	return control_register_b.byte;
}

void Mc6821_PIA1::set_vertical_sync_interrupt_enable(bool new_value)
{
	if (control_register_b.bits.vertical_sync_interrupt_enable != new_value)
	{
		control_register_b.bits.vertical_sync_interrupt_enable = new_value;
		if (debug)
		{
			emit_signal("vertical_sync_interrupt_enable_toggled", new_value);
			emit_signal("control_register_b_set", control_register_b.byte);
		}
	}
}

bool Mc6821_PIA1::get_vertical_sync_interrupt_enable()
{
	return control_register_b.bits.vertical_sync_interrupt_enable;
}

void Mc6821_PIA1::set_vertical_sync_interrupt_polarity(bool new_value)
{
	if (control_register_b.bits.vertical_sync_interrupt_polarity != new_value)
	{
		control_register_b.bits.vertical_sync_interrupt_polarity = new_value;
		if (debug)
		{
			emit_signal("vertical_sync_interrupt_polarity_toggled", new_value);
			emit_signal("control_register_b_set", control_register_b.byte);
		}
	}
}

bool Mc6821_PIA1::get_vertical_sync_interrupt_polarity()
{
	return control_register_b.bits.vertical_sync_interrupt_polarity;
}

void Mc6821_PIA1::set_data_direction_b(bool new_value)
{
	if (control_register_b.bits.accessing_data_register != new_value)
	{
		control_register_b.bits.accessing_data_register = new_value;
		if (debug)
		{
			emit_signal("data_direction_b_toggled", new_value);
			emit_signal("control_register_b_set", control_register_b.byte);
		}
	}
}

bool Mc6821_PIA1::get_data_direction_b()
{
	return control_register_b.bits.accessing_data_register;
}

void Mc6821_PIA1::set_mux_select_1(bool new_value)
{
	if (control_register_b.bits.mux_select_1 != new_value)
	{
		control_register_b.bits.mux_select_1 = new_value;
		if (debug)
		{
			emit_signal("mux_select_1_toggled", new_value);
			emit_signal("control_register_b_set", control_register_b.byte);
		}
	}
}

bool Mc6821_PIA1::get_mux_select_1()
{
	return control_register_b.bits.mux_select_1;
}

void Mc6821_PIA1::set_vertical_sync_interrupt_flag(bool new_value)
{
	if (control_register_b.bits.vertical_sync_interrupt_flag != new_value)
	{
		control_register_b.bits.vertical_sync_interrupt_flag = new_value;
		if (debug)
		{
			emit_signal("vertical_sync_interrupt_flag_toggled", new_value);
			emit_signal("control_register_b_set", control_register_b.byte);
		}
	}
}

bool Mc6821_PIA1::get_vertical_sync_interrupt_flag()
{
	return control_register_b.bits.vertical_sync_interrupt_flag;
}

void Mc6821_PIA1::set_data_direction_register_b(std::uint8_t new_value)
{
	if (data_direction_register_b.byte != new_value)
	{
		if (!debug)
		{
			data_direction_register_b.byte = new_value;
		}
		else
		{
			bool old_ddrb_0 = data_direction_register_b.bits.ddrb_0;
			bool old_ddrb_1 = data_direction_register_b.bits.ddrb_1;
			bool old_ddrb_2 = data_direction_register_b.bits.ddrb_2;
			bool old_ddrb_3 = data_direction_register_b.bits.ddrb_3;
			bool old_ddrb_4 = data_direction_register_b.bits.ddrb_4;
			bool old_ddrb_5 = data_direction_register_b.bits.ddrb_5;
			bool old_ddrb_6 = data_direction_register_b.bits.ddrb_6;
			bool old_ddrb_7 = data_direction_register_b.bits.ddrb_7;

			data_direction_register_b.byte = new_value;

			if (old_ddrb_0 != data_direction_register_b.bits.ddrb_0)
			{
				emit_signal("ddrb_0_toggled", data_direction_register_b.bits.ddrb_0);
			}

			if (old_ddrb_1 != data_direction_register_b.bits.ddrb_1)
			{
				emit_signal("ddrb_1_toggled", data_direction_register_b.bits.ddrb_1);
			}

			if (old_ddrb_2 != data_direction_register_b.bits.ddrb_2)
			{
				emit_signal("ddrb_2_toggled", data_direction_register_b.bits.ddrb_2);
			}

			if (old_ddrb_3 != data_direction_register_b.bits.ddrb_3)
			{
				emit_signal("ddrb_3_toggled", data_direction_register_b.bits.ddrb_3);
			}

			if (old_ddrb_4 != data_direction_register_b.bits.ddrb_4)
			{
				emit_signal("ddrb_4_toggled", data_direction_register_b.bits.ddrb_4);
			}

			if (old_ddrb_5 != data_direction_register_b.bits.ddrb_5)
			{
				emit_signal("ddrb_5_toggled", data_direction_register_b.bits.ddrb_5);
			}

			if (old_ddrb_6 != data_direction_register_b.bits.ddrb_6)
			{
				emit_signal("ddrb_6_toggled", data_direction_register_b.bits.ddrb_6);
			}

			if (old_ddrb_7 != data_direction_register_b.bits.ddrb_7)
			{
				emit_signal("ddrb_7_toggled", data_direction_register_b.bits.ddrb_7);
			}

			emit_signal("data_direction_register_b_set", new_value);
		}
	}
}

std::uint8_t Mc6821_PIA1::get_data_direction_register_b()
{
	return data_direction_register_b.byte;
}

void Mc6821_PIA1::set_ddrb_0(bool new_value)
{
	if (data_direction_register_b.bits.ddrb_0 != new_value)
	{
		data_direction_register_b.bits.ddrb_0 = new_value;
		if (debug)
		{
			emit_signal("ddrb_0_toggled", new_value);
			emit_signal("data_direction_register_b_set", data_direction_register_b.byte);
		}
	}
}

bool Mc6821_PIA1::get_ddrb_0()
{
	return data_direction_register_b.bits.ddrb_0;
}

void Mc6821_PIA1::set_ddrb_1(bool new_value)
{
	if (data_direction_register_b.bits.ddrb_1 != new_value)
	{
		data_direction_register_b.bits.ddrb_1 = new_value;
		if (debug)
		{
			emit_signal("ddrb_1_toggled", new_value);
			emit_signal("data_direction_register_b_set", data_direction_register_b.byte);
		}
	}
}

bool Mc6821_PIA1::get_ddrb_1()
{
	return data_direction_register_b.bits.ddrb_1;
}

void Mc6821_PIA1::set_ddrb_2(bool new_value)
{
	if (data_direction_register_b.bits.ddrb_2 != new_value)
	{
		data_direction_register_b.bits.ddrb_2 = new_value;
		if (debug)
		{
			emit_signal("ddrb_2_toggled", new_value);
			emit_signal("data_direction_register_b_set", data_direction_register_b.byte);
		}
	}
}

bool Mc6821_PIA1::get_ddrb_2()
{
	return data_direction_register_b.bits.ddrb_2;
}

void Mc6821_PIA1::set_ddrb_3(bool new_value)
{
	if (data_direction_register_b.bits.ddrb_3 != new_value)
	{
		data_direction_register_b.bits.ddrb_3 = new_value;
		if (debug)
		{
			emit_signal("ddrb_3_toggled", new_value);
			emit_signal("data_direction_register_b_set", data_direction_register_b.byte);
		}
	}
}

bool Mc6821_PIA1::get_ddrb_3()
{
	return data_direction_register_b.bits.ddrb_3;
}

void Mc6821_PIA1::set_ddrb_4(bool new_value)
{
	if (data_direction_register_b.bits.ddrb_4 != new_value)
	{
		data_direction_register_b.bits.ddrb_4 = new_value;
		if (debug)
		{
			emit_signal("ddrb_4_toggled", new_value);
			emit_signal("data_direction_register_b_set", data_direction_register_b.byte);
		}
	}
}

bool Mc6821_PIA1::get_ddrb_4()
{
	return data_direction_register_b.bits.ddrb_4;
}

void Mc6821_PIA1::set_ddrb_5(bool new_value)
{
	if (data_direction_register_b.bits.ddrb_5 != new_value)
	{
		data_direction_register_b.bits.ddrb_5 = new_value;
		if (debug)
		{
			emit_signal("ddrb_5_toggled", new_value);
			emit_signal("data_direction_register_b_set", data_direction_register_b.byte);
		}
	}
}

bool Mc6821_PIA1::get_ddrb_5()
{
	return data_direction_register_b.bits.ddrb_5;
}

void Mc6821_PIA1::set_ddrb_6(bool new_value)
{
	if (data_direction_register_b.bits.ddrb_6 != new_value)
	{
		data_direction_register_b.bits.ddrb_6 = new_value;
		if (debug)
		{
			emit_signal("ddrb_6_toggled", new_value);
			emit_signal("data_direction_register_b_set", data_direction_register_b.byte);
		}
	}
}

bool Mc6821_PIA1::get_ddrb_6()
{
	return data_direction_register_b.bits.ddrb_6;
}

void Mc6821_PIA1::set_ddrb_7(bool new_value)
{
	if (data_direction_register_b.bits.ddrb_7 != new_value)
	{
		data_direction_register_b.bits.ddrb_7 = new_value;
		if (debug)
		{
			emit_signal("ddrb_7_toggled", new_value);
			emit_signal("data_direction_register_b_set", data_direction_register_b.byte);
		}
	}
}

bool Mc6821_PIA1::get_ddrb_7()
{
	return data_direction_register_b.bits.ddrb_7;
}
