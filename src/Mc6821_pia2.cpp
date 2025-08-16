#include "Mc6821_pia2.hpp"

using namespace godot;

Mc6821_PIA2::Mc6821_PIA2()
{
	data_register_a.byte = 0;
	control_register_a.byte = 0;
	data_direction_register_a.byte = 0;
	data_register_b.byte = 0;
	control_register_b.byte = 0;
	data_direction_register_b.byte = 0;
}

Mc6821_PIA2::~Mc6821_PIA2()
{
}

void Mc6821_PIA2::_register_methods()
{
	// Properties
	register_property<Mc6821_PIA2, std::uint8_t>("data_register_a_set", &Mc6821_PIA2::set_data_register_a, &Mc6821_PIA2::get_data_register_a, 0);
	register_property<Mc6821_PIA2, bool>("cassette_data_input", &Mc6821_PIA2::set_cassette_data_input, &Mc6821_PIA2::get_cassette_data_input, false);
	register_property<Mc6821_PIA2, bool>("rs_232c_data_output", &Mc6821_PIA2::set_rs_232c_data_output, &Mc6821_PIA2::get_rs_232c_data_output, false);
	register_property<Mc6821_PIA2, bool>("dac_0", &Mc6821_PIA2::set_dac_0, &Mc6821_PIA2::get_dac_0, false);
	register_property<Mc6821_PIA2, bool>("dac_1", &Mc6821_PIA2::set_dac_1, &Mc6821_PIA2::get_dac_1, false);
	register_property<Mc6821_PIA2, bool>("dac_2", &Mc6821_PIA2::set_dac_2, &Mc6821_PIA2::get_dac_2, false);
	register_property<Mc6821_PIA2, bool>("dac_3", &Mc6821_PIA2::set_dac_3, &Mc6821_PIA2::get_dac_3, false);
	register_property<Mc6821_PIA2, bool>("dac_4", &Mc6821_PIA2::set_dac_4, &Mc6821_PIA2::get_dac_4, false);
	register_property<Mc6821_PIA2, bool>("dac_5", &Mc6821_PIA2::set_dac_5, &Mc6821_PIA2::get_dac_5, false);
	register_property<Mc6821_PIA2, std::uint8_t>("dac", &Mc6821_PIA2::set_dac, &Mc6821_PIA2::get_dac, 0);
	register_property<Mc6821_PIA2, std::uint8_t>("control_register_a", &Mc6821_PIA2::set_control_register_a, &Mc6821_PIA2::get_control_register_a, 0);
	register_property<Mc6821_PIA2, bool>("carrier_detect_interrupt_enable", &Mc6821_PIA2::set_carrier_detect_interrupt_enable, &Mc6821_PIA2::get_carrier_detect_interrupt_enable, false);
	register_property<Mc6821_PIA2, bool>("carrier_detect_interrupt_polarity", &Mc6821_PIA2::set_carrier_detect_interrupt_polarity, &Mc6821_PIA2::get_carrier_detect_interrupt_polarity, false);
	register_property<Mc6821_PIA2, bool>("data_direction_a", &Mc6821_PIA2::set_data_direction_a, &Mc6821_PIA2::get_data_direction_a, false);
	register_property<Mc6821_PIA2, bool>("cassette_motor_control", &Mc6821_PIA2::set_cassette_motor_control, &Mc6821_PIA2::get_cassette_motor_control, false);
	register_property<Mc6821_PIA2, bool>("carrier_detect_interrupt_flag", &Mc6821_PIA2::set_carrier_detect_interrupt_flag, &Mc6821_PIA2::get_carrier_detect_interrupt_flag, false);
	register_property<Mc6821_PIA2, std::uint8_t>("data_direction_register_a", &Mc6821_PIA2::set_data_direction_register_a, &Mc6821_PIA2::get_data_direction_register_a, 0);
	register_property<Mc6821_PIA2, bool>("ddra_0", &Mc6821_PIA2::set_ddra_0, &Mc6821_PIA2::get_ddra_0, false);
	register_property<Mc6821_PIA2, bool>("ddra_1", &Mc6821_PIA2::set_ddra_1, &Mc6821_PIA2::get_ddra_1, false);
	register_property<Mc6821_PIA2, bool>("ddra_2", &Mc6821_PIA2::set_ddra_2, &Mc6821_PIA2::get_ddra_2, false);
	register_property<Mc6821_PIA2, bool>("ddra_3", &Mc6821_PIA2::set_ddra_3, &Mc6821_PIA2::get_ddra_3, false);
	register_property<Mc6821_PIA2, bool>("ddra_4", &Mc6821_PIA2::set_ddra_4, &Mc6821_PIA2::get_ddra_4, false);
	register_property<Mc6821_PIA2, bool>("ddra_5", &Mc6821_PIA2::set_ddra_5, &Mc6821_PIA2::get_ddra_5, false);
	register_property<Mc6821_PIA2, bool>("ddra_6", &Mc6821_PIA2::set_ddra_6, &Mc6821_PIA2::get_ddra_6, false);
	register_property<Mc6821_PIA2, bool>("ddra_7", &Mc6821_PIA2::set_ddra_7, &Mc6821_PIA2::get_ddra_7, false);
	register_property<Mc6821_PIA2, std::uint8_t>("data_register_b", &Mc6821_PIA2::set_data_register_b, &Mc6821_PIA2::get_data_register_b, 0);
	register_property<Mc6821_PIA2, bool>("rs_232c_data_input", &Mc6821_PIA2::set_rs_232c_data_input, &Mc6821_PIA2::get_rs_232c_data_input, false);
	register_property<Mc6821_PIA2, bool>("single_bit_sound_output", &Mc6821_PIA2::set_single_bit_sound_output, &Mc6821_PIA2::get_single_bit_sound_output, false);
	register_property<Mc6821_PIA2, bool>("ram_size_input", &Mc6821_PIA2::set_ram_size_input, &Mc6821_PIA2::get_ram_size_input, false);
	register_property<Mc6821_PIA2, bool>("color_set_select", &Mc6821_PIA2::set_color_set_select, &Mc6821_PIA2::get_color_set_select, false);
	register_property<Mc6821_PIA2, bool>("vdg_control_output_gm0", &Mc6821_PIA2::set_vdg_control_output_gm0, &Mc6821_PIA2::get_vdg_control_output_gm0, false);
	register_property<Mc6821_PIA2, bool>("vdg_control_output_gm1", &Mc6821_PIA2::set_vdg_control_output_gm1, &Mc6821_PIA2::get_vdg_control_output_gm1, false);
	register_property<Mc6821_PIA2, bool>("vdg_control_output_gm2", &Mc6821_PIA2::set_vdg_control_output_gm2, &Mc6821_PIA2::get_vdg_control_output_gm2, false);
	register_property<Mc6821_PIA2, bool>("vdg_control_output_ag", &Mc6821_PIA2::set_vdg_control_output_ag, &Mc6821_PIA2::get_vdg_control_output_ag, false);
	register_property<Mc6821_PIA2, bool>("rgb_monitor_sensing_input", &Mc6821_PIA2::set_rgb_monitor_sensing_input, &Mc6821_PIA2::get_rgb_monitor_sensing_input, false);
	register_property<Mc6821_PIA2, std::uint8_t>("control_register_b", &Mc6821_PIA2::set_control_register_b, &Mc6821_PIA2::get_control_register_b, 0);
	register_property<Mc6821_PIA2, bool>("cartridge_interrupt_enable", &Mc6821_PIA2::set_cartridge_interrupt_enable, &Mc6821_PIA2::get_cartridge_interrupt_enable, false);
	register_property<Mc6821_PIA2, bool>("cartridge_interrupt_polarity", &Mc6821_PIA2::set_cartridge_interrupt_polarity, &Mc6821_PIA2::get_cartridge_interrupt_polarity, false);
	register_property<Mc6821_PIA2, bool>("data_direction_b", &Mc6821_PIA2::set_data_direction_b, &Mc6821_PIA2::get_data_direction_b, false);
	register_property<Mc6821_PIA2, bool>("sound_enable", &Mc6821_PIA2::set_sound_enable, &Mc6821_PIA2::get_sound_enable, false);
	register_property<Mc6821_PIA2, bool>("cartridge_interrupt_flag", &Mc6821_PIA2::set_cartridge_interrupt_flag, &Mc6821_PIA2::get_cartridge_interrupt_flag, false);
	register_property<Mc6821_PIA2, std::uint8_t>("data_direction_register_b", &Mc6821_PIA2::set_data_direction_register_b, &Mc6821_PIA2::get_data_direction_register_b, 0);
	register_property<Mc6821_PIA2, bool>("ddrb_0", &Mc6821_PIA2::set_ddrb_0, &Mc6821_PIA2::get_ddrb_0, false);
	register_property<Mc6821_PIA2, bool>("ddrb_1", &Mc6821_PIA2::set_ddrb_1, &Mc6821_PIA2::get_ddrb_1, false);
	register_property<Mc6821_PIA2, bool>("ddrb_2", &Mc6821_PIA2::set_ddrb_2, &Mc6821_PIA2::get_ddrb_2, false);
	register_property<Mc6821_PIA2, bool>("ddrb_3", &Mc6821_PIA2::set_ddrb_3, &Mc6821_PIA2::get_ddrb_3, false);
	register_property<Mc6821_PIA2, bool>("ddrb_4", &Mc6821_PIA2::set_ddrb_4, &Mc6821_PIA2::get_ddrb_4, false);
	register_property<Mc6821_PIA2, bool>("ddrb_5", &Mc6821_PIA2::set_ddrb_5, &Mc6821_PIA2::get_ddrb_5, false);
	register_property<Mc6821_PIA2, bool>("ddrb_6", &Mc6821_PIA2::set_ddrb_6, &Mc6821_PIA2::get_ddrb_6, false);
	register_property<Mc6821_PIA2, bool>("ddrb_7", &Mc6821_PIA2::set_ddrb_7, &Mc6821_PIA2::get_ddrb_7, false);

	// Signals
	register_signal<Mc6821_PIA2>("data_register_a_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<Mc6821_PIA2>("cassette_data_input_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("rs_232c_data_output_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("dac_0_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("dac_1_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("dac_2_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("dac_3_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("dac_4_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("dac_5_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("dac_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<Mc6821_PIA2>("control_register_a_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<Mc6821_PIA2>("carrier_detect_interrupt_enable_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("carrier_detect_interrupt_polarity_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("data_direction_a_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("cassette_motor_control_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("carrier_detect_interrupt_flag_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("data_direction_register_a_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<Mc6821_PIA2>("ddra_0_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("ddra_1_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("ddra_2_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("ddra_3_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("ddra_4_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("ddra_5_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("ddra_6_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("ddra_7_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("data_register_b_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<Mc6821_PIA2>("rs_232c_data_input_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("single_bit_sound_output_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("ram_size_input_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("color_set_select_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("vdg_control_output_gm0_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("vdg_control_output_gm1_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("vdg_control_output_gm2_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("vdg_control_output_ag_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("rgb_monitor_sensing_input_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("control_register_b_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<Mc6821_PIA2>("cartridge_interrupt_enable_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("cartridge_interrupt_polarity_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("data_direction_b_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("sound_enable_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("cartridge_interrupt_flag_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("data_direction_register_b_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<Mc6821_PIA2>("ddrb_0_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("ddrb_1_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("ddrb_2_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("ddrb_3_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("ddrb_4_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("ddrb_5_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("ddrb_6_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<Mc6821_PIA2>("ddrb_7_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
}

void Mc6821_PIA2::_init()
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
std::uint8_t Mc6821_PIA2::read(std::uint8_t address)
{
	address &= 0xFF;

	switch (address)
	{
	case 0x20:
		// DRA and DDRA of PIA 2
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

	case 0x21:
		// CRA of PIA 2
		return control_register_a.byte;

	case 0x22:
		// DRB and DDRB of PIA 2
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
			return data_direction_register_b.byte;
		}

	case 0x23:
		// CRB of PIA 2
		return control_register_b.byte;
	}

	return 0;
}

void Mc6821_PIA2::write(std::uint8_t address, std::uint8_t data)
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

			vdg_control_register->set_register(data_register_b.byte & data_direction_register_b.byte);
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

void Mc6821_PIA2::carrier_detect_transition(bool sync_value)
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
		if (control_register_a.bits.carrier_detect_interrupt_polarity)
		{
			set_carrier_detect_interrupt_flag(true);
			if (control_register_a.bits.carrier_detect_interrupt_enable)
			{
				/* FIRQ Interrupts - PIA 2 is connected to the FIRQ interrupt signal going to the
				 * MPU. The READY signal form the serial I/O port is connected to CA1. (ED: AKA,
				 * Carrier Detected or CD). A device connected to the serial I/O port, when it goes
				 * from a non-ready state to a ready state, causes the READY signal to go from a
				 * high to a low. If PIA 2 is configured to sense a high-to-low transition at CA1,
				 * an FIRQ interrupt will be generated when the device bcomes ready.
				 */
				mpu_pins_state->set_firq(true);
			}
		}
	}
	else
	{
		// high-to=low
		if (!control_register_a.bits.carrier_detect_interrupt_polarity)
		{
			set_carrier_detect_interrupt_flag(true);
			if (control_register_a.bits.carrier_detect_interrupt_enable)
			{
				/* FIRQ Interrupts - PIA 2 is connected to the FIRQ interrupt signal going to the
				 * MPU. The READY signal form the serial I/O port is connected to CA1. (ED: AKA,
				 * Carrier Detected or CD). A device connected to the serial I/O port, when it goes
				 * from a non-ready state to a ready state, causes the READY signal to go from a
				 * high to a low. If PIA 2 is configured to sense a high-to-low transition at CA1,
				 * an FIRQ interrupt will be generated when the device bcomes ready.
				 */
				mpu_pins_state->set_firq(true);
			}
		}
	}
}

void Mc6821_PIA2::cartridge_transition(bool sync_value)
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
		if (control_register_b.bits.cartridge_interrupt_polarity)
		{
			set_cartridge_interrupt_flag(true);
			if (control_register_b.bits.cartridge_interrupt_enable)
			{
				/* FIRQ Interrupts - PIA 2 is connected to the FIRQ interrupt signal going to the
				 * MPU. The READY signal form the serial I/O port is connected to CA1. (ED: AKA,
				 * Carrier Detected or CD). A device connected to the serial I/O port, when it goes
				 * from a non-ready state to a ready state, causes the READY signal to go from a
				 * high to a low. If PIA 2 is configured to sense a high-to-low transition at CA1,
				 * an FIRQ interrupt will be generated when the device bcomes ready.
				 */
				mpu_pins_state->set_firq(true);
			}
		}
	}
	else
	{
		// high-to=low
		if (!control_register_b.bits.cartridge_interrupt_polarity)
		{
			set_cartridge_interrupt_flag(true);
			if (control_register_b.bits.cartridge_interrupt_enable)
			{
				/* FIRQ Interrupts - PIA 2 is connected to the FIRQ interrupt signal going to the
				 * MPU. The READY signal form the serial I/O port is connected to CA1. (ED: AKA,
				 * Carrier Detected or CD). A device connected to the serial I/O port, when it goes
				 * from a non-ready state to a ready state, causes the READY signal to go from a
				 * high to a low. If PIA 2 is configured to sense a high-to-low transition at CA1,
				 * an FIRQ interrupt will be generated when the device bcomes ready.
				 */
				mpu_pins_state->set_firq(true);
			}
		}
	}
}

void Mc6821_PIA2::set_data_register_a(std::uint8_t new_value)
{
	if (data_register_a.byte != new_value)
	{
		if (!debug)
		{
			data_register_a.byte = new_value;
		}
		else
		{
			bool old_cassette_data_input = data_register_a.bits.cassette_data_input;
			bool old_rs_232c_data_output = data_register_a.bits.rs_232c_data_output;
			std::uint8_t old_dac = data_register_a.dac.value;
			bool old_dac_0 = data_register_a.bits.dac_0;
			bool old_dac_1 = data_register_a.bits.dac_1;
			bool old_dac_2 = data_register_a.bits.dac_2;
			bool old_dac_3 = data_register_a.bits.dac_3;
			bool old_dac_4 = data_register_a.bits.dac_4;
			bool old_dac_5 = data_register_a.bits.dac_5;

			data_register_a.byte = new_value;

			if (old_cassette_data_input != data_register_a.bits.cassette_data_input)
			{
				emit_signal("cassette_data_input_toggled", data_register_a.bits.cassette_data_input);
			}

			if (old_rs_232c_data_output != data_register_a.bits.rs_232c_data_output)
			{
				emit_signal("rs_232c_data_output_toggled", data_register_a.bits.rs_232c_data_output);
			}

			if (old_dac != data_register_a.dac.value)
			{
				if (old_dac_0 != data_register_a.bits.dac_0)
				{
					emit_signal("dac_0_toggled", data_register_a.bits.dac_0);
				}

				if (old_dac_1 != data_register_a.bits.dac_1)
				{
					emit_signal("dac_1_toggled", data_register_a.bits.dac_1);
				}

				if (old_dac_2 != data_register_a.bits.dac_2)
				{
					emit_signal("dac_2_toggled", data_register_a.bits.dac_2);
				}

				if (old_dac_3 != data_register_a.bits.dac_3)
				{
					emit_signal("dac_3_toggled", data_register_a.bits.dac_3);
				}

				if (old_dac_4 != data_register_a.bits.dac_4)
				{
					emit_signal("dac_4_toggled", data_register_a.bits.dac_4);
				}

				if (old_dac_5 != data_register_a.bits.dac_5)
				{
					emit_signal("dac_5_toggled", data_register_a.bits.dac_5);
				}

				emit_signal("dac_set", data_register_a.dac.value);
			}

			emit_signal("data_register_a_set", new_value);
		}
	}
}

std::uint8_t Mc6821_PIA2::get_data_register_a()
{
	return data_register_a.byte;
}

void Mc6821_PIA2::set_cassette_data_input(bool new_value)
{
	if (data_register_a.bits.cassette_data_input != new_value)
	{
		data_register_a.bits.cassette_data_input = new_value;
		if (debug)
		{
			emit_signal("cassette_data_input_toggled", new_value);
			emit_signal("data_register_a_set", data_register_a.byte);
		}
	}
}

bool Mc6821_PIA2::get_cassette_data_input()
{
	return data_register_a.bits.cassette_data_input;
}

void Mc6821_PIA2::set_rs_232c_data_output(bool new_value)
{
	if (data_register_a.bits.rs_232c_data_output != new_value)
	{
		data_register_a.bits.rs_232c_data_output = new_value;
		if (debug)
		{
			emit_signal("rs_232c_data_output_toggled", new_value);
			emit_signal("data_register_a_set", data_register_a.byte);
		}
	}
}

bool Mc6821_PIA2::get_rs_232c_data_output()
{
	return data_register_a.bits.rs_232c_data_output;
}

void Mc6821_PIA2::set_dac_0(bool new_value)
{
	if (data_register_a.bits.dac_0 != new_value)
	{
		data_register_a.bits.dac_0 = new_value;
		if (debug)
		{
			emit_signal("dac_0_toggled", new_value);
			emit_signal("dac_set", data_register_a.dac.value);
			emit_signal("data_register_a_set", data_register_a.byte);
		}
	}
}

bool Mc6821_PIA2::get_dac_0()
{
	return data_register_a.bits.dac_0;
}

void Mc6821_PIA2::set_dac_1(bool new_value)
{
	if (data_register_a.bits.dac_1 != new_value)
	{
		data_register_a.bits.dac_1 = new_value;
		if (debug)
		{
			emit_signal("dac_1_toggled", new_value);
			emit_signal("dac_set", data_register_a.dac.value);
			emit_signal("data_register_a_set", data_register_a.byte);
		}
	}
}

bool Mc6821_PIA2::get_dac_1()
{
	return data_register_a.bits.dac_1;
}

void Mc6821_PIA2::set_dac_2(bool new_value)
{
	if (data_register_a.bits.dac_2 != new_value)
	{
		data_register_a.bits.dac_2 = new_value;
		if (debug)
		{
			emit_signal("dac_2_toggled", new_value);
			emit_signal("dac_set", data_register_a.dac.value);
			emit_signal("data_register_a_set", data_register_a.byte);
		}
	}
}

bool Mc6821_PIA2::get_dac_2()
{
	return data_register_a.bits.dac_2;
}

void Mc6821_PIA2::set_dac_3(bool new_value)
{
	if (data_register_a.bits.dac_3 != new_value)
	{
		data_register_a.bits.dac_3 = new_value;
		if (debug)
		{
			emit_signal("dac_3_toggled", new_value);
			emit_signal("dac_set", data_register_a.dac.value);
			emit_signal("data_register_a_set", data_register_a.byte);
		}
	}
}

bool Mc6821_PIA2::get_dac_3()
{
	return data_register_a.bits.dac_3;
}

void Mc6821_PIA2::set_dac_4(bool new_value)
{
	if (data_register_a.bits.dac_4 != new_value)
	{
		data_register_a.bits.dac_4 = new_value;
		if (debug)
		{
			emit_signal("dac_4_toggled", new_value);
			emit_signal("dac_set", data_register_a.dac.value);
			emit_signal("data_register_a_set", data_register_a.byte);
		}
	}
}

bool Mc6821_PIA2::get_dac_4()
{
	return data_register_a.bits.dac_4;
}

void Mc6821_PIA2::set_dac_5(bool new_value)
{
	if (data_register_a.bits.dac_5 != new_value)
	{
		data_register_a.bits.dac_5 = new_value;
		if (debug)
		{
			emit_signal("dac_5_toggled", new_value);
			emit_signal("dac_set", data_register_a.dac.value);
			emit_signal("data_register_a_set", data_register_a.byte);
		}
	}
}

bool Mc6821_PIA2::get_dac_5()
{
	return data_register_a.bits.dac_5;
}

void Mc6821_PIA2::set_dac(std::uint8_t new_value)
{
	if (data_register_a.dac.value != new_value)
	{
		bool old_dac_0 = data_register_a.bits.dac_0;
		bool old_dac_1 = data_register_a.bits.dac_1;
		bool old_dac_2 = data_register_a.bits.dac_2;
		bool old_dac_3 = data_register_a.bits.dac_3;
		bool old_dac_4 = data_register_a.bits.dac_4;
		bool old_dac_5 = data_register_a.bits.dac_5;

		data_register_a.dac.value = new_value;

		if (old_dac_0 != data_register_a.bits.dac_0)
		{
			emit_signal("dac_0_toggled", data_register_a.bits.dac_0);
		}

		if (old_dac_1 != data_register_a.bits.dac_1)
		{
			emit_signal("dac_1_toggled", data_register_a.bits.dac_1);
		}

		if (old_dac_2 != data_register_a.bits.dac_2)
		{
			emit_signal("dac_2_toggled", data_register_a.bits.dac_2);
		}

		if (old_dac_3 != data_register_a.bits.dac_3)
		{
			emit_signal("dac_3_toggled", data_register_a.bits.dac_3);
		}

		if (old_dac_4 != data_register_a.bits.dac_4)
		{
			emit_signal("dac_4_toggled", data_register_a.bits.dac_4);
		}

		if (old_dac_5 != data_register_a.bits.dac_5)
		{
			emit_signal("dac_5_toggled", data_register_a.bits.dac_5);
		}

		emit_signal("dac_set", data_register_a.dac.value);
	}
}

std::uint8_t Mc6821_PIA2::get_dac()
{
	return data_register_a.dac.value;
}

void Mc6821_PIA2::set_control_register_a(std::uint8_t new_value)
{
	if (data_register_a.byte != new_value)
	{
		if (!debug)
		{
			data_register_a.byte = new_value;
		}
		else
		{
			bool old_carrier_detect_interrupt_enable = control_register_a.bits.carrier_detect_interrupt_enable;
			bool old_carrier_detect_interrupt_polarity = control_register_a.bits.carrier_detect_interrupt_polarity;
			bool old_accessing_data_register = control_register_a.bits.accessing_data_register;
			bool old_cassette_motor_control = control_register_a.bits.cassette_motor_control;
			bool old_carrier_detect_interrupt_flag = control_register_a.bits.carrier_detect_interrupt_flag;

			control_register_a.byte = new_value;

			if (old_carrier_detect_interrupt_enable != control_register_a.bits.carrier_detect_interrupt_enable)
			{
				emit_signal("carrier_detect_interrupt_enable_toggled", control_register_a.bits.carrier_detect_interrupt_enable);
			}

			if (old_carrier_detect_interrupt_polarity != control_register_a.bits.carrier_detect_interrupt_polarity)
			{
				emit_signal("carrier_detect_interrupt_polarity_toggled", control_register_a.bits.carrier_detect_interrupt_polarity);
			}

			if (old_accessing_data_register != control_register_a.bits.accessing_data_register)
			{
				emit_signal("data_direction_a_toggled", control_register_a.bits.accessing_data_register);
			}

			if (old_cassette_motor_control != control_register_a.bits.cassette_motor_control)
			{
				emit_signal("cassette_motor_control_toggled", control_register_a.bits.cassette_motor_control);
			}

			if (old_carrier_detect_interrupt_flag != control_register_a.bits.carrier_detect_interrupt_flag)
			{
				emit_signal("carrier_detect_interrupt_flag_toggled", control_register_a.bits.carrier_detect_interrupt_flag);
			}

			emit_signal("control_register_a_set", new_value);
		}
	}
}

std::uint8_t Mc6821_PIA2::get_control_register_a()
{
	return control_register_a.byte;
}

void Mc6821_PIA2::set_carrier_detect_interrupt_enable(bool new_value)
{
	if (control_register_a.bits.carrier_detect_interrupt_enable != new_value)
	{
		control_register_a.bits.carrier_detect_interrupt_enable = new_value;
		if (debug)
		{
			emit_signal("carrier_detect_interrupt_enable_toggled", new_value);
			emit_signal("control_register_a_set", control_register_a.byte);
		}
	}
}

bool Mc6821_PIA2::get_carrier_detect_interrupt_enable()
{
	return control_register_a.bits.carrier_detect_interrupt_enable;
}

void Mc6821_PIA2::set_carrier_detect_interrupt_polarity(bool new_value)
{
	if (control_register_a.bits.carrier_detect_interrupt_polarity != new_value)
	{
		control_register_a.bits.carrier_detect_interrupt_polarity = new_value;
		if (debug)
		{
			emit_signal("carrier_detect_interrupt_polarity_toggled", new_value);
			emit_signal("control_register_a_set", control_register_a.byte);
		}
	}
}

bool Mc6821_PIA2::get_carrier_detect_interrupt_polarity()
{
	return control_register_a.bits.carrier_detect_interrupt_polarity;
}

void Mc6821_PIA2::set_data_direction_a(bool new_value)
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

bool Mc6821_PIA2::get_data_direction_a()
{
	return control_register_a.bits.accessing_data_register;
}

void Mc6821_PIA2::set_cassette_motor_control(bool new_value)
{
	if (control_register_a.bits.cassette_motor_control != new_value)
	{
		control_register_a.bits.cassette_motor_control = new_value;
		if (debug)
		{
			emit_signal("cassette_motor_control_toggled", new_value);
			emit_signal("control_register_a_set", control_register_a.byte);
		}
	}
}

bool Mc6821_PIA2::get_cassette_motor_control()
{
	return control_register_a.bits.cassette_motor_control;
}

void Mc6821_PIA2::set_carrier_detect_interrupt_flag(bool new_value)
{
	if (control_register_a.bits.carrier_detect_interrupt_flag != new_value)
	{
		control_register_a.bits.carrier_detect_interrupt_flag = new_value;
		if (debug)
		{
			emit_signal("carrier_detect_interrupt_flag_toggled", new_value);
			emit_signal("control_register_a_set", control_register_a.byte);
		}
	}
}

bool Mc6821_PIA2::get_carrier_detect_interrupt_flag()
{
	return control_register_a.bits.carrier_detect_interrupt_flag;
}

void Mc6821_PIA2::set_data_direction_register_a(std::uint8_t new_value)
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

std::uint8_t Mc6821_PIA2::get_data_direction_register_a()
{
	return data_direction_register_a.byte;
}

void Mc6821_PIA2::set_ddra_0(bool new_value)
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

bool Mc6821_PIA2::get_ddra_0()
{
	return data_direction_register_a.bits.ddra_0;
}

void Mc6821_PIA2::set_ddra_1(bool new_value)
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

bool Mc6821_PIA2::get_ddra_1()
{
	return data_direction_register_a.bits.ddra_1;
}

void Mc6821_PIA2::set_ddra_2(bool new_value)
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

bool Mc6821_PIA2::get_ddra_2()
{
	return data_direction_register_a.bits.ddra_2;
}

void Mc6821_PIA2::set_ddra_3(bool new_value)
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

bool Mc6821_PIA2::get_ddra_3()
{
	return data_direction_register_a.bits.ddra_3;
}

void Mc6821_PIA2::set_ddra_4(bool new_value)
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

bool Mc6821_PIA2::get_ddra_4()
{
	return data_direction_register_a.bits.ddra_4;
}

void Mc6821_PIA2::set_ddra_5(bool new_value)
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

bool Mc6821_PIA2::get_ddra_5()
{
	return data_direction_register_a.bits.ddra_5;
}

void Mc6821_PIA2::set_ddra_6(bool new_value)
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

bool Mc6821_PIA2::get_ddra_6()
{
	return data_direction_register_a.bits.ddra_6;
}

void Mc6821_PIA2::set_ddra_7(bool new_value)
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

bool Mc6821_PIA2::get_ddra_7()
{
	return data_direction_register_a.bits.ddra_7;
}

void Mc6821_PIA2::set_data_register_b(std::uint8_t new_value)
{
	if (data_direction_register_a.byte != new_value)
	{
		if (!debug)
		{
			data_direction_register_a.byte = new_value;
		}
		else
		{
			bool old_rs_232c_data_input = data_register_b.bits.rs_232c_data_input;
			bool old_single_bit_sound_output = data_register_b.bits.single_bit_sound_output;
			bool old_ram_size_input = data_register_b.bits.ram_size_input;
			bool old_color_set_select = data_register_b.bits.color_set_select;
			bool old_vdg_control_output_gm0 = data_register_b.bits.vdg_control_output_gm0;
			bool old_vdg_control_output_gm1 = data_register_b.bits.vdg_control_output_gm1;
			bool old_vdg_control_output_gm2 = data_register_b.bits.vdg_control_output_gm2;
			bool old_vdg_control_output_ag = data_register_b.bits.vdg_control_output_ag;
			bool old_rgb_monitor_sensing_input = data_register_b.input.rgb_monitor_sensing_input;

			data_register_b.byte = new_value;

			if (old_rs_232c_data_input != data_register_b.bits.rs_232c_data_input)
			{
				emit_signal("rs_232c_data_input_toggled", data_register_b.bits.rs_232c_data_input);
			}

			if (old_single_bit_sound_output != data_register_b.bits.single_bit_sound_output)
			{
				emit_signal("single_bit_sound_output_toggled", data_register_b.bits.single_bit_sound_output);
			}

			if (old_ram_size_input != data_register_b.bits.ram_size_input)
			{
				emit_signal("ram_size_input_toggled", data_register_b.bits.ram_size_input);
			}

			if (old_color_set_select != data_register_b.bits.color_set_select)
			{
				emit_signal("color_set_select_toggled", data_register_b.bits.color_set_select);
			}

			if (old_vdg_control_output_gm0 != data_register_b.bits.vdg_control_output_gm0)
			{
				emit_signal("vdg_control_output_gm0", data_register_b.bits.vdg_control_output_gm0);
			}

			if (old_vdg_control_output_gm1 != data_register_b.bits.vdg_control_output_gm1)
			{
				emit_signal("vdg_control_output_gm1", data_register_b.bits.vdg_control_output_gm1);
			}

			if (old_vdg_control_output_gm2 != data_register_b.bits.vdg_control_output_gm2)
			{
				emit_signal("vdg_control_output_gm2", data_register_b.bits.vdg_control_output_gm2);
			}

			if (old_vdg_control_output_ag != data_register_b.bits.vdg_control_output_ag)
			{
				emit_signal("vdg_control_output_ag", data_register_b.bits.vdg_control_output_ag);
			}

			if (old_rgb_monitor_sensing_input != data_register_b.input.rgb_monitor_sensing_input)
			{
				emit_signal("rgb_monitor_sensing_input_toggled", data_register_b.input.rgb_monitor_sensing_input);
			}

			emit_signal("data_register_b_set", new_value);
		}
	}
}

std::uint8_t Mc6821_PIA2::get_data_register_b()
{
	return data_register_b.byte;
}

void Mc6821_PIA2::set_rs_232c_data_input(bool new_value)
{
	if (data_register_b.bits.rs_232c_data_input != new_value)
	{
		data_register_b.bits.rs_232c_data_input = new_value;
		if (debug)
		{
			emit_signal("rs_232c_data_input_toggled", new_value);
			emit_signal("data_register_b_set", data_register_b.byte);
		}
	}
}

bool Mc6821_PIA2::get_rs_232c_data_input()
{
	return data_register_b.bits.rs_232c_data_input;
}

void Mc6821_PIA2::set_single_bit_sound_output(bool new_value)
{
	if (data_register_b.bits.single_bit_sound_output != new_value)
	{
		data_register_b.bits.single_bit_sound_output = new_value;
		if (debug)
		{
			emit_signal("single_bit_sound_output_toggled", new_value);
			emit_signal("data_register_b_set", data_register_b.byte);
		}
	}
}

bool Mc6821_PIA2::get_single_bit_sound_output()
{
	return data_register_b.bits.single_bit_sound_output;
}

void Mc6821_PIA2::set_ram_size_input(bool new_value)
{
	if (data_register_b.bits.ram_size_input != new_value)
	{
		data_register_b.bits.ram_size_input = new_value;
		if (debug)
		{
			emit_signal("ram_size_input_toggled", new_value);
			emit_signal("data_register_b_set", data_register_b.byte);
		}
	}
}

bool Mc6821_PIA2::get_ram_size_input()
{
	return data_register_b.bits.ram_size_input;
}

void Mc6821_PIA2::set_color_set_select(bool new_value)
{
	if (data_register_b.bits.color_set_select != new_value)
	{
		data_register_b.bits.color_set_select = new_value;
		if (debug)
		{
			emit_signal("rgb_monitor_sensing_input_toggled", new_value);
			emit_signal("color_set_select_toggled", new_value);
			emit_signal("data_register_b_set", data_register_b.byte);
		}
	}
}

bool Mc6821_PIA2::get_color_set_select()
{
	return data_register_b.bits.color_set_select;
}

void Mc6821_PIA2::set_vdg_control_output_gm0(bool new_value)
{
	if (data_register_b.bits.vdg_control_output_gm0 != new_value)
	{
		data_register_b.bits.vdg_control_output_gm0 = new_value;
		if (debug)
		{
			emit_signal("vdg_control_output_gm0_toggled", new_value);
			emit_signal("data_register_b_set", data_register_b.byte);
		}
	}
}

bool Mc6821_PIA2::get_vdg_control_output_gm0()
{
	return data_register_b.bits.vdg_control_output_gm0;
}

void Mc6821_PIA2::set_vdg_control_output_gm1(bool new_value)
{
	if (data_register_b.bits.vdg_control_output_gm1 != new_value)
	{
		data_register_b.bits.vdg_control_output_gm1 = new_value;
		if (debug)
		{
			emit_signal("vdg_control_output_gm1_toggled", new_value);
			emit_signal("data_register_b_set", data_register_b.byte);
		}
	}
}

bool Mc6821_PIA2::get_vdg_control_output_gm1()
{
	return data_register_b.bits.vdg_control_output_gm1;
}

void Mc6821_PIA2::set_vdg_control_output_gm2(bool new_value)
{
	if (data_register_b.bits.vdg_control_output_gm2 != new_value)
	{
		data_register_b.bits.vdg_control_output_gm2 = new_value;
		if (debug)
		{
			emit_signal("vdg_control_output_gm2_toggled", new_value);
			emit_signal("data_register_b_set", data_register_b.byte);
		}
	}
}

bool Mc6821_PIA2::get_vdg_control_output_gm2()
{
	return data_register_b.bits.vdg_control_output_gm2;
}

void Mc6821_PIA2::set_vdg_control_output_ag(bool new_value)
{
	if (data_register_b.bits.vdg_control_output_ag != new_value)
	{
		data_register_b.bits.vdg_control_output_ag = new_value;
		if (debug)
		{
			emit_signal("vdg_control_output_ag_toggled", new_value);
			emit_signal("data_register_b_set", data_register_b.byte);
		}
	}
}

bool Mc6821_PIA2::get_vdg_control_output_ag()
{
	return data_register_b.bits.vdg_control_output_ag;
}

void Mc6821_PIA2::set_rgb_monitor_sensing_input(bool new_value)
{
	if (data_register_b.input.rgb_monitor_sensing_input != new_value)
	{
		data_register_b.input.rgb_monitor_sensing_input = new_value;
		if (debug)
		{
			emit_signal("rgb_monitor_sensing_input_toggled", new_value);
			emit_signal("data_register_b_set", data_register_b.byte);
		}
	}
}

bool Mc6821_PIA2::get_rgb_monitor_sensing_input()
{
	return data_register_b.input.rgb_monitor_sensing_input;
}

void Mc6821_PIA2::set_control_register_b(std::uint8_t new_value)
{
	if (control_register_b.byte != new_value)
	{
		if (!debug)
		{
			control_register_b.byte = new_value;
		}
		else
		{
			bool old_cartridge_interrupt_enable = control_register_b.bits.cartridge_interrupt_enable;
			bool old_cartridge_interrupt_polarity = control_register_b.bits.cartridge_interrupt_polarity;
			bool old_accessing_data_register = control_register_b.bits.accessing_data_register;
			bool old_sound_enable = control_register_b.bits.sound_enable;
			bool old_cartridge_interrupt_flag = control_register_b.bits.cartridge_interrupt_flag;

			control_register_b.byte = new_value;

			if (old_cartridge_interrupt_enable != control_register_b.bits.cartridge_interrupt_enable)
			{
				emit_signal("cartridge_interrupt_enable_toggled", control_register_b.bits.cartridge_interrupt_enable);
			}

			if (old_cartridge_interrupt_polarity != control_register_b.bits.cartridge_interrupt_polarity)
			{
				emit_signal("cartridge_interrupt_polarity_toggled", control_register_b.bits.cartridge_interrupt_polarity);
			}

			if (old_accessing_data_register != control_register_b.bits.accessing_data_register)
			{
				emit_signal("data_direction_b_toggled", control_register_b.bits.accessing_data_register);
			}

			if (old_sound_enable != control_register_b.bits.sound_enable)
			{
				emit_signal("sound_enable_toggled", control_register_b.bits.sound_enable);
			}

			if (old_cartridge_interrupt_flag != control_register_b.bits.cartridge_interrupt_flag)
			{
				emit_signal("cartridge_interrupt_flag_toggled", control_register_b.bits.cartridge_interrupt_flag);
			}

			emit_signal("control_register_b_set", new_value);
		}
	}
}

std::uint8_t Mc6821_PIA2::get_control_register_b()
{
	return control_register_b.byte;
}

void Mc6821_PIA2::set_cartridge_interrupt_enable(bool new_value)
{
	if (control_register_b.bits.cartridge_interrupt_enable != new_value)
	{
		control_register_b.bits.cartridge_interrupt_enable = new_value;
		if (debug)
		{
			emit_signal("cartridge_interrupt_enable_toggled", new_value);
			emit_signal("control_register_b_set", control_register_b.byte);
		}
	}
}

bool Mc6821_PIA2::get_cartridge_interrupt_enable()
{
	return control_register_b.bits.cartridge_interrupt_enable;
}

void Mc6821_PIA2::set_cartridge_interrupt_polarity(bool new_value)
{
	if (control_register_b.bits.cartridge_interrupt_polarity != new_value)
	{
		control_register_b.bits.cartridge_interrupt_polarity = new_value;
		if (debug)
		{
			emit_signal("cartridge_interrupt_polarity_toggled", new_value);
			emit_signal("control_register_b_set", control_register_b.byte);
		}
	}
}

bool Mc6821_PIA2::get_cartridge_interrupt_polarity()
{
	return control_register_b.bits.cartridge_interrupt_polarity;
}

void Mc6821_PIA2::set_data_direction_b(bool new_value)
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

bool Mc6821_PIA2::get_data_direction_b()
{
	return control_register_b.bits.accessing_data_register;
}

void Mc6821_PIA2::set_sound_enable(bool new_value)
{
	if (control_register_b.bits.sound_enable != new_value)
	{
		control_register_b.bits.sound_enable = new_value;
		if (debug)
		{
			emit_signal("sound_enable_toggled", new_value);
			emit_signal("control_register_b_set", control_register_b.byte);
		}
	}
}

bool Mc6821_PIA2::get_sound_enable()
{
	return control_register_b.bits.sound_enable;
}

void Mc6821_PIA2::set_cartridge_interrupt_flag(bool new_value)
{
	if (control_register_b.bits.cartridge_interrupt_flag != new_value)
	{
		control_register_b.bits.cartridge_interrupt_flag = new_value;
		if (debug)
		{
			emit_signal("cartridge_interrupt_flag_toggled", new_value);
			emit_signal("control_register_b_set", control_register_b.byte);
		}
	}
}

bool Mc6821_PIA2::get_cartridge_interrupt_flag()
{
	return control_register_b.bits.cartridge_interrupt_flag;
}

void Mc6821_PIA2::set_data_direction_register_b(std::uint8_t new_value)
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

std::uint8_t Mc6821_PIA2::get_data_direction_register_b()
{
	return data_direction_register_b.byte;
}

void Mc6821_PIA2::set_ddrb_0(bool new_value)
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

bool Mc6821_PIA2::get_ddrb_0()
{
	return data_direction_register_b.bits.ddrb_0;
}

void Mc6821_PIA2::set_ddrb_1(bool new_value)
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

bool Mc6821_PIA2::get_ddrb_1()
{
	return data_direction_register_b.bits.ddrb_1;
}

void Mc6821_PIA2::set_ddrb_2(bool new_value)
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

bool Mc6821_PIA2::get_ddrb_2()
{
	return data_direction_register_b.bits.ddrb_2;
}

void Mc6821_PIA2::set_ddrb_3(bool new_value)
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

bool Mc6821_PIA2::get_ddrb_3()
{
	return data_direction_register_b.bits.ddrb_3;
}

void Mc6821_PIA2::set_ddrb_4(bool new_value)
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

bool Mc6821_PIA2::get_ddrb_4()
{
	return data_direction_register_b.bits.ddrb_4;
}

void Mc6821_PIA2::set_ddrb_5(bool new_value)
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

bool Mc6821_PIA2::get_ddrb_5()
{
	return data_direction_register_b.bits.ddrb_5;
}

void Mc6821_PIA2::set_ddrb_6(bool new_value)
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

bool Mc6821_PIA2::get_ddrb_6()
{
	return data_direction_register_b.bits.ddrb_6;
}

void Mc6821_PIA2::set_ddrb_7(bool new_value)
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

bool Mc6821_PIA2::get_ddrb_7()
{
	return data_direction_register_b.bits.ddrb_7;
}
