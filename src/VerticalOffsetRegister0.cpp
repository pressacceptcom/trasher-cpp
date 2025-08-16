#include "VerticalOffsetRegister0.hpp"

using namespace godot;

VerticalOffsetRegister0::VerticalOffsetRegister0() : byte{0}
{
}

VerticalOffsetRegister0::~VerticalOffsetRegister0()
{
}

void VerticalOffsetRegister0::_register_methods()
{
	// Methods
	register_method("set_bit", &VerticalOffsetRegister0::set_bit);
	register_method("get_bit", &VerticalOffsetRegister0::get_bit);

	// Properties
	register_property<VerticalOffsetRegister0, std::uint8_t>("register", &VerticalOffsetRegister0::set_register, &VerticalOffsetRegister0::get_register, 0);
	register_property<VerticalOffsetRegister0, bool>("y_3", &VerticalOffsetRegister0::set_y_3, &VerticalOffsetRegister0::get_y_3, false);
	register_property<VerticalOffsetRegister0, bool>("y_4", &VerticalOffsetRegister0::set_y_4, &VerticalOffsetRegister0::get_y_4, false);
	register_property<VerticalOffsetRegister0, bool>("y_5", &VerticalOffsetRegister0::set_y_5, &VerticalOffsetRegister0::get_y_5, false);
	register_property<VerticalOffsetRegister0, bool>("y_6", &VerticalOffsetRegister0::set_y_6, &VerticalOffsetRegister0::get_y_6, false);
	register_property<VerticalOffsetRegister0, bool>("y_7", &VerticalOffsetRegister0::set_y_7, &VerticalOffsetRegister0::get_y_7, false);
	register_property<VerticalOffsetRegister0, bool>("y_8", &VerticalOffsetRegister0::set_y_8, &VerticalOffsetRegister0::get_y_8, false);
	register_property<VerticalOffsetRegister0, bool>("y_9", &VerticalOffsetRegister0::set_y_9, &VerticalOffsetRegister0::get_y_9, false);
	register_property<VerticalOffsetRegister0, bool>("y_10", &VerticalOffsetRegister0::set_y_10, &VerticalOffsetRegister0::get_y_10, false);

	// Signals
	register_signal<VerticalOffsetRegister0>("register_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<VerticalOffsetRegister0>("y_3_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<VerticalOffsetRegister0>("y_4_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<VerticalOffsetRegister0>("y_5_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<VerticalOffsetRegister0>("y_6_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<VerticalOffsetRegister0>("y_7_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<VerticalOffsetRegister0>("y_8_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<VerticalOffsetRegister0>("y_9_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<VerticalOffsetRegister0>("y_10_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
}

void VerticalOffsetRegister0::_init()
{
}

void VerticalOffsetRegister0::set_register(std::uint8_t new_value)
{
	if (byte != new_value)
	{
		bool old_y_3 = bits.y_3;
		bool old_y_4 = bits.y_4;
		bool old_y_5 = bits.y_5;
		bool old_y_6 = bits.y_6;
		bool old_y_7 = bits.y_7;
		bool old_y_8 = bits.y_8;
		bool old_y_9 = bits.y_9;
		bool old_y_10 = bits.y_10;

		byte = new_value;

		if (old_y_3 != bits.y_3)
		{
			emit_signal("y_3_toggled", bits.y_3);
		}

		if (old_y_4 != bits.y_4)
		{
			emit_signal("y_4_toggled", bits.y_4);
		}

		if (old_y_5 != bits.y_5)
		{
			emit_signal("y_5_toggled", bits.y_5);
		}

		if (old_y_6 != bits.y_6)
		{
			emit_signal("y_6_toggled", bits.y_6);
		}

		if (old_y_7 != bits.y_7)
		{
			emit_signal("y_7_toggled", bits.y_7);
		}

		if (old_y_8 != bits.y_8)
		{
			emit_signal("y_8_toggled", bits.y_8);
		}

		if (old_y_9 != bits.y_9)
		{
			emit_signal("y_9_toggled", bits.y_9);
		}

		if (old_y_10 != bits.y_10)
		{
			emit_signal("y_10_toggled", bits.y_10);
		}

		emit_signal("register_set", byte);
	}
}

std::uint8_t VerticalOffsetRegister0::get_register()
{
	return byte;
}

void VerticalOffsetRegister0::set_bit(int bit, bool new_value)
{
	switch (bit)
	{

	case Bits::Y_3:
		set_y_3(new_value);
		break;

	case Bits::Y_4:
		set_y_4(new_value);
		break;

	case Bits::Y_5:
		set_y_5(new_value);
		break;

	case Bits::Y_6:
		set_y_6(new_value);
		break;

	case Bits::Y_7:
		set_y_7(new_value);
		break;

	case Bits::Y_8:
		set_y_8(new_value);
		break;

	case Bits::Y_9:
		set_y_9(new_value);
		break;

	case Bits::Y_10:
		set_y_10(new_value);
		break;
	}
}

bool VerticalOffsetRegister0::get_bit(int bit)
{
	switch (bit)
	{

	case Bits::Y_3:
		return bits.y_3;

	case Bits::Y_4:
		return bits.y_4;

	case Bits::Y_5:
		return bits.y_5;

	case Bits::Y_6:
		return bits.y_6;

	case Bits::Y_7:
		return bits.y_7;

	case Bits::Y_8:
		return bits.y_8;

	case Bits::Y_9:
		return bits.y_9;

	case Bits::Y_10:
		return bits.y_10;
	}

	return false;
}

void VerticalOffsetRegister0::set_y_3(bool new_value)
{
	if (bits.y_3 != new_value)
	{
		bits.y_3 = new_value;
		emit_signal("y_3_toggled", new_value);
		emit_signal("register_set", byte);
	}
}

bool VerticalOffsetRegister0::get_y_3()
{
	return bits.y_3;
}

void VerticalOffsetRegister0::set_y_4(bool new_value)
{
	if (bits.y_4 != new_value)
	{
		bits.y_4 = new_value;
		emit_signal("y_4_toggled", new_value);
		emit_signal("register_set", byte);
	}
}

bool VerticalOffsetRegister0::get_y_4()
{
	return bits.y_4;
}

void VerticalOffsetRegister0::set_y_5(bool new_value)
{
	if (bits.y_5 != new_value)
	{
		bits.y_5 = new_value;
		emit_signal("y_5_toggled", new_value);
		emit_signal("register_set", byte);
	}
}

bool VerticalOffsetRegister0::get_y_5()
{
	return bits.y_5;
}

void VerticalOffsetRegister0::set_y_6(bool new_value)
{
	if (bits.y_6 != new_value)
	{
		bits.y_6 = new_value;
		emit_signal("y_6_toggled", new_value);
		emit_signal("register_set", byte);
	}
}

bool VerticalOffsetRegister0::get_y_6()
{
	return bits.y_6;
}

void VerticalOffsetRegister0::set_y_7(bool new_value)
{
	if (bits.y_7 != new_value)
	{
		bits.y_7 = new_value;
		emit_signal("y_7_toggled", new_value);
		emit_signal("register_set", byte);
	}
}

bool VerticalOffsetRegister0::get_y_7()
{
	return bits.y_7;
}

void VerticalOffsetRegister0::set_y_8(bool new_value)
{
	if (bits.y_8 != new_value)
	{
		bits.y_8 = new_value;
		emit_signal("y_8_toggled", new_value);
		emit_signal("register_set", byte);
	}
}

bool VerticalOffsetRegister0::get_y_8()
{
	return bits.y_8;
}

void VerticalOffsetRegister0::set_y_9(bool new_value)
{
	if (bits.y_9 != new_value)
	{
		bits.y_9 = new_value;
		emit_signal("y_9_toggled", new_value);
		emit_signal("register_set", byte);
	}
}

bool VerticalOffsetRegister0::get_y_9()
{
	return bits.y_9;
}

void VerticalOffsetRegister0::set_y_10(bool new_value)
{
	if (bits.y_10 != new_value)
	{
		bits.y_10 = new_value;
		emit_signal("y_10_toggled", new_value);
		emit_signal("register_set", byte);
	}
}

bool VerticalOffsetRegister0::get_y_10()
{
	return bits.y_10;
}
