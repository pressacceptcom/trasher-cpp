#include "SAMControlRegisters.hpp"

using namespace godot;

SAMControlRegisters::SAMControlRegisters()
{
	display_mode_control.byte = 0;
	display_offset.byte = 0;
}

SAMControlRegisters::~SAMControlRegisters()
{
}

void SAMControlRegisters::_register_methods()
{
	// Properties
	register_property<SAMControlRegisters, bool>("ty_rom_disable", &SAMControlRegisters::set_ty_rom_disable, &SAMControlRegisters::get_ty_rom_disable, false);
	register_property<SAMControlRegisters, bool>("r1_cpu_rate", &SAMControlRegisters::set_r1_cpu_rate, &SAMControlRegisters::get_r1_cpu_rate, false);
	register_property<SAMControlRegisters, bool>("f_0", &SAMControlRegisters::set_f_0, &SAMControlRegisters::get_f_0, false);
	register_property<SAMControlRegisters, bool>("f_1", &SAMControlRegisters::set_f_1, &SAMControlRegisters::get_f_1, false);
	register_property<SAMControlRegisters, bool>("f_2", &SAMControlRegisters::set_f_2, &SAMControlRegisters::get_f_2, false);
	register_property<SAMControlRegisters, bool>("f_3", &SAMControlRegisters::set_f_3, &SAMControlRegisters::get_f_3, false);
	register_property<SAMControlRegisters, bool>("f_4", &SAMControlRegisters::set_f_4, &SAMControlRegisters::get_f_4, false);
	register_property<SAMControlRegisters, bool>("f_5", &SAMControlRegisters::set_f_5, &SAMControlRegisters::get_f_5, false);
	register_property<SAMControlRegisters, bool>("f_6", &SAMControlRegisters::set_f_6, &SAMControlRegisters::get_f_6, false);
	register_property<SAMControlRegisters, std::uint8_t>("display_offset", &SAMControlRegisters::set_display_offset, &SAMControlRegisters::get_display_offset, 0);
	register_property<SAMControlRegisters, bool>("v_0", &SAMControlRegisters::set_v_0, &SAMControlRegisters::get_v_0, false);
	register_property<SAMControlRegisters, bool>("v_1", &SAMControlRegisters::set_v_1, &SAMControlRegisters::get_v_1, false);
	register_property<SAMControlRegisters, bool>("v_2", &SAMControlRegisters::set_v_2, &SAMControlRegisters::get_v_2, false);
	register_property<SAMControlRegisters, std::uint8_t>("display_mode_control", &SAMControlRegisters::set_display_mode_control, &SAMControlRegisters::get_display_mode_control, 0);

	// Signals
	register_signal<SAMControlRegisters>("ty_rom_disable_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<SAMControlRegisters>("r1_cpu_rate_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<SAMControlRegisters>("f_0_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<SAMControlRegisters>("f_1_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<SAMControlRegisters>("f_2_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<SAMControlRegisters>("f_3_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<SAMControlRegisters>("f_4_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<SAMControlRegisters>("f_5_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<SAMControlRegisters>("f_6_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<SAMControlRegisters>("display_offset_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<SAMControlRegisters>("v_0_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<SAMControlRegisters>("v_1_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<SAMControlRegisters>("v_2_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<SAMControlRegisters>("display_mode_control_set", "new_value", GODOT_VARIANT_TYPE_INT);
}

void SAMControlRegisters::_init()
{
}

void SAMControlRegisters::set_v_0(bool new_value)
{
	if (display_mode_control.bits.v_0 != new_value)
	{
		display_mode_control.bits.v_0 = new_value;
		emit_signal("v_0_toggled", new_value);
		emit_signal("display_mode_control_set", display_mode_control.byte);
	}
}

bool SAMControlRegisters::get_v_0()
{
	return display_mode_control.bits.v_0;
}

void SAMControlRegisters::set_v_1(bool new_value)
{
	if (display_mode_control.bits.v_1 != new_value)
	{
		display_mode_control.bits.v_1 = new_value;
		emit_signal("v_1_toggled", new_value);
		emit_signal("display_mode_control_set", display_mode_control.byte);
	}
}

bool SAMControlRegisters::get_v_1()
{
	return display_mode_control.bits.v_1;
}

void SAMControlRegisters::set_v_2(bool new_value)
{
	if (display_mode_control.bits.v_2 != new_value)
	{
		display_mode_control.bits.v_2 = new_value;
		emit_signal("v_2_toggled", new_value);
		emit_signal("display_mode_control_set", display_mode_control.byte);
	}
}

bool SAMControlRegisters::get_v_2()
{
	return display_mode_control.bits.v_2;
}

void SAMControlRegisters::set_display_mode_control(std::uint8_t new_value)
{
	if (display_mode_control.byte != new_value)
	{
		bool old_v_0 = display_mode_control.bits.v_0;
		bool old_v_1 = display_mode_control.bits.v_1;
		bool old_v_2 = display_mode_control.bits.v_2;

		display_mode_control.byte = new_value;

		if (old_v_0 != display_mode_control.bits.v_0)
		{
			emit_signal("v_0_toggled", display_mode_control.bits.v_0);
		}

		if (old_v_1 != display_mode_control.bits.v_1)
		{
			emit_signal("v_1_toggled", display_mode_control.bits.v_1);
		}

		if (old_v_2 != display_mode_control.bits.v_2)
		{
			emit_signal("v_2_toggled", display_mode_control.bits.v_2);
		}

		emit_signal("display_mode_control_set", display_mode_control.byte);
	}
}

std::uint8_t SAMControlRegisters::get_display_mode_control()
{
	return display_mode_control.byte & 0x07;
}

void SAMControlRegisters::set_f_0(bool new_value)
{
	if (display_offset.bits.f_0 != new_value)
	{
		display_offset.bits.f_0 = new_value;
		emit_signal("f_0_toggled", display_offset.bits.f_0);
		emit_signal("display_offset_set", display_offset.byte);
	}
}

bool SAMControlRegisters::get_f_0()
{
	return display_offset.bits.f_0;
}

void SAMControlRegisters::set_f_1(bool new_value)
{
	if (display_offset.bits.f_1 != new_value)
	{
		display_offset.bits.f_1 = new_value;
		emit_signal("f_1_toggled", display_offset.bits.f_1);
		emit_signal("display_offset_set", display_offset.byte);
	}
}

bool SAMControlRegisters::get_f_1()
{
	return display_offset.bits.f_1;
}

void SAMControlRegisters::set_f_2(bool new_value)
{
	if (display_offset.bits.f_2 != new_value)
	{
		display_offset.bits.f_2 = new_value;
		emit_signal("f_2_toggled", display_offset.bits.f_2);
		emit_signal("display_offset_set", display_offset.byte);
	}
}

bool SAMControlRegisters::get_f_2()
{
	return display_offset.bits.f_2;
}

void SAMControlRegisters::set_f_3(bool new_value)
{
	if (display_offset.bits.f_3 != new_value)
	{
		display_offset.bits.f_3 = new_value;
		emit_signal("f_3_toggled", display_offset.bits.f_3);
		emit_signal("display_offset_set", display_offset.byte);
	}
}

bool SAMControlRegisters::get_f_3()
{
	return display_offset.bits.f_3;
}

void SAMControlRegisters::set_f_4(bool new_value)
{
	if (display_offset.bits.f_4 != new_value)
	{
		display_offset.bits.f_4 = new_value;
		emit_signal("f_4_toggled", display_offset.bits.f_4);
		emit_signal("display_offset_set", display_offset.byte);
	}
}

bool SAMControlRegisters::get_f_4()
{
	return display_offset.bits.f_4;
}

void SAMControlRegisters::set_f_5(bool new_value)
{
	if (display_offset.bits.f_5 != new_value)
	{
		display_offset.bits.f_5 = new_value;
		emit_signal("f_5_toggled", display_offset.bits.f_5);
		emit_signal("display_offset_set", display_offset.byte);
	}
}

bool SAMControlRegisters::get_f_5()
{
	return display_offset.bits.f_5;
}

void SAMControlRegisters::set_f_6(bool new_value)
{
	if (display_offset.bits.f_6 != new_value)
	{
		display_offset.bits.f_6 = new_value;
		emit_signal("f_6_toggled", display_offset.bits.f_6);
		emit_signal("display_offset_set", display_offset.byte);
	}
}

bool SAMControlRegisters::get_f_6()
{
	return display_offset.bits.f_6;
}

void SAMControlRegisters::set_display_offset(std::uint8_t new_value)
{
	if (display_offset.byte != new_value)
	{
		bool old_f_0 = display_offset.bits.f_0;
		bool old_f_1 = display_offset.bits.f_1;
		bool old_f_2 = display_offset.bits.f_2;
		bool old_f_3 = display_offset.bits.f_3;
		bool old_f_4 = display_offset.bits.f_4;
		bool old_f_5 = display_offset.bits.f_5;
		bool old_f_6 = display_offset.bits.f_6;

		display_offset.byte = new_value;

		if (old_f_0 != display_offset.bits.f_0)
		{
			emit_signal("f_0_toggled", display_offset.bits.f_0);
		}

		if (old_f_1 != display_offset.bits.f_1)
		{
			emit_signal("f_1_toggled", display_offset.bits.f_1);
		}

		if (old_f_2 != display_offset.bits.f_2)
		{
			emit_signal("f_2_toggled", display_offset.bits.f_2);
		}

		if (old_f_3 != display_offset.bits.f_3)
		{
			emit_signal("f_3_toggled", display_offset.bits.f_3);
		}

		if (old_f_4 != display_offset.bits.f_4)
		{
			emit_signal("f_4_toggled", display_offset.bits.f_4);
		}

		if (old_f_5 != display_offset.bits.f_5)
		{
			emit_signal("f_5_toggled", display_offset.bits.f_5);
		}

		if (old_f_6 != display_offset.bits.f_6)
		{
			emit_signal("f_6_toggled", display_offset.bits.f_6);
		}

		emit_signal("display_offset_set", display_offset.byte);
	}
}

std::uint8_t SAMControlRegisters::get_display_offset()
{
	return display_offset.byte & 0x7F;
}

void SAMControlRegisters::set_r1_cpu_rate(bool new_value)
{
	if (r1_cpu_rate != new_value)
	{
		r1_cpu_rate = new_value;
		emit_signal("r1_cpu_rate_toggled", new_value);
	}
}

bool SAMControlRegisters::get_r1_cpu_rate()
{
	return r1_cpu_rate;
}

void SAMControlRegisters::set_ty_rom_disable(bool new_value)
{
	if (ty_rom_disable != new_value)
	{
		ty_rom_disable = new_value;
		emit_signal("ty_rom_disable_toggled", new_value);
	}
}

bool SAMControlRegisters::get_ty_rom_disable()
{
	return ty_rom_disable;
}
