#include "VDGControl.hpp"

using namespace godot;

VDGControl::VDGControl() : byte{0}
{
}

VDGControl::~VDGControl()
{
}

void VDGControl::_register_methods()
{
	// Methods
	register_method("set_bit", &VDGControl::set_bit);
	register_method("get_bit", &VDGControl::get_bit);

	// Properties
	register_property<VDGControl, std::uint8_t>("register", &VDGControl::set_register, &VDGControl::get_register, false);
	register_property<VDGControl, bool>("color_set_select", &VDGControl::set_color_set_select, &VDGControl::get_color_set_select, false);
	register_property<VDGControl, bool>("gm_0", &VDGControl::set_gm_0, &VDGControl::get_gm_0, false);
	register_property<VDGControl, bool>("gm_1", &VDGControl::set_gm_1, &VDGControl::get_gm_1, false);
	register_property<VDGControl, bool>("gm_2", &VDGControl::set_gm_2, &VDGControl::get_gm_2, false);
	register_property<VDGControl, bool>("ag", &VDGControl::set_ag, &VDGControl::get_ag, false);

	// Signals
	register_signal<VDGControl>("register_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<VDGControl>("color_set_select_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<VDGControl>("gm_0_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<VDGControl>("gm_1_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<VDGControl>("gm_2_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<VDGControl>("ag_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
}

void VDGControl::_init()
{
}

void VDGControl::set_bit(int bit, bool new_value)
{
	switch (bit)
	{
	case Bits::COLOR_SET_SELECT:
		set_color_set_select(new_value);
		break;

	case Bits::GM_0:
		set_gm_0(new_value);
		break;

	case Bits::GM_1:
		set_gm_1(new_value);
		break;

	case Bits::GM_2:
		set_gm_2(new_value);
		break;

	case Bits::AG:
		set_ag(new_value);
		break;
	}
}

bool VDGControl::get_bit(int bit)
{
	switch (bit)
	{
	case Bits::COLOR_SET_SELECT:
		return bits.color_set_select;

	case Bits::GM_0:
		return bits.gm_0;

	case Bits::GM_1:
		return bits.gm_1;

	case Bits::GM_2:
		return bits.gm_2;

	case Bits::AG:
		return bits.ag;
	}

	return false;
}

void VDGControl::set_register(std::uint8_t new_value)
{
	if (byte != new_value)
	{
		bool old_color_set_select = bits.color_set_select;
		bool old_gm_0 = bits.gm_0;
		bool old_gm_1 = bits.gm_1;
		bool old_gm_2 = bits.gm_2;
		bool old_ag = bits.ag;

		byte = new_value;

		if (old_color_set_select != bits.color_set_select)
		{
			emit_signal("color_set_select_toggled", bits.color_set_select);
		}

		if (old_gm_0 != bits.gm_0)
		{
			emit_signal("gm_0_toggled", bits.gm_0);
		}

		if (old_gm_1 != bits.gm_1)
		{
			emit_signal("gm_1_toggled", bits.gm_1);
		}

		if (old_gm_2 != bits.gm_2)
		{
			emit_signal("gm_2_toggled", bits.gm_2);
		}

		if (old_ag != bits.ag)
		{
			emit_signal("ag_toggled", bits.ag);
		}

		emit_signal("register_set", byte);
	}
}

std::uint8_t VDGControl::get_register()
{
	return byte;
}

void VDGControl::set_color_set_select(bool new_value)
{
	if (bits.color_set_select != new_value)
	{
		bits.color_set_select = new_value;
		emit_signal("color_set_select_toggled", new_value);
		emit_signal("register_set", byte);
	}
}

bool VDGControl::get_color_set_select()
{
	return bits.color_set_select;
}

void VDGControl::set_gm_0(bool new_value)
{
	if (bits.gm_0 != new_value)
	{
		bits.gm_0 = new_value;
		emit_signal("gm_0_toggled", new_value);
		emit_signal("register_set", byte);
	}
}

bool VDGControl::get_gm_0()
{
	return bits.gm_0;
}

void VDGControl::set_gm_1(bool new_value)
{
	if (bits.gm_1 != new_value)
	{
		bits.gm_1 = new_value;
		emit_signal("gm_1_toggled", new_value);
		emit_signal("register_set", byte);
	}
}

bool VDGControl::get_gm_1()
{
	return bits.gm_1;
}

void VDGControl::set_gm_2(bool new_value)
{
	if (bits.gm_2 != new_value)
	{
		bits.gm_2 = new_value;
		emit_signal("gm_2_toggled", new_value);
		emit_signal("register_set", byte);
	}
}

bool VDGControl::get_gm_2()
{
	return bits.gm_2;
}

void VDGControl::set_ag(bool new_value)
{
	if (bits.ag != new_value)
	{
		bits.ag = new_value;
		emit_signal("ag_toggled", new_value);
		emit_signal("register_set", byte);
	}
}

bool VDGControl::get_ag()
{
	return bits.ag;
}
