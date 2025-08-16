#include "VerticalOffsetRegister1.hpp"

using namespace godot;

VerticalOffsetRegister1::VerticalOffsetRegister1() : byte{0}
{
}

VerticalOffsetRegister1::~VerticalOffsetRegister1()
{
}

void VerticalOffsetRegister1::_register_methods()
{
	// Methods
	register_method("set_bit", &VerticalOffsetRegister1::set_bit);
	register_method("get_bit", &VerticalOffsetRegister1::get_bit);

	// Properties
	register_property<VerticalOffsetRegister1, std::uint8_t>("register", &VerticalOffsetRegister1::set_register, &VerticalOffsetRegister1::get_register, 0);
	register_property<VerticalOffsetRegister1, bool>("y_11", &VerticalOffsetRegister1::set_y_11, &VerticalOffsetRegister1::get_y_11, false);
	register_property<VerticalOffsetRegister1, bool>("y_12", &VerticalOffsetRegister1::set_y_12, &VerticalOffsetRegister1::get_y_12, false);
	register_property<VerticalOffsetRegister1, bool>("y_13", &VerticalOffsetRegister1::set_y_13, &VerticalOffsetRegister1::get_y_13, false);
	register_property<VerticalOffsetRegister1, bool>("y_14", &VerticalOffsetRegister1::set_y_14, &VerticalOffsetRegister1::get_y_14, false);
	register_property<VerticalOffsetRegister1, bool>("y_15", &VerticalOffsetRegister1::set_y_15, &VerticalOffsetRegister1::get_y_15, false);
	register_property<VerticalOffsetRegister1, bool>("y_16", &VerticalOffsetRegister1::set_y_16, &VerticalOffsetRegister1::get_y_16, false);
	register_property<VerticalOffsetRegister1, bool>("y_17", &VerticalOffsetRegister1::set_y_17, &VerticalOffsetRegister1::get_y_17, false);
	register_property<VerticalOffsetRegister1, bool>("y_18", &VerticalOffsetRegister1::set_y_18, &VerticalOffsetRegister1::get_y_18, false);

	// Signals
	register_signal<VerticalOffsetRegister1>("register_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<VerticalOffsetRegister1>("y_11_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<VerticalOffsetRegister1>("y_12_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<VerticalOffsetRegister1>("y_13_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<VerticalOffsetRegister1>("y_14_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<VerticalOffsetRegister1>("y_15_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<VerticalOffsetRegister1>("y_16_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<VerticalOffsetRegister1>("y_17_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<VerticalOffsetRegister1>("y_18_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
}

void VerticalOffsetRegister1::_init()
{
}

void VerticalOffsetRegister1::set_register(std::uint8_t new_value)
{
	if (byte != new_value)
	{
		bool old_y_11 = bits.y_11;
		bool old_y_12 = bits.y_12;
		bool old_y_13 = bits.y_13;
		bool old_y_14 = bits.y_14;
		bool old_y_15 = bits.y_15;
		bool old_y_16 = bits.y_16;
		bool old_y_17 = bits.y_17;
		bool old_y_18 = bits.y_18;

		byte = new_value;

		if (old_y_11 != bits.y_11)
		{
			emit_signal("y_11_toggled", bits.y_11);
		}

		if (old_y_12 != bits.y_12)
		{
			emit_signal("y_12_toggled", bits.y_12);
		}

		if (old_y_13 != bits.y_13)
		{
			emit_signal("y_13_toggled", bits.y_13);
		}

		if (old_y_14 != bits.y_14)
		{
			emit_signal("y_14_toggled", bits.y_14);
		}

		if (old_y_15 != bits.y_15)
		{
			emit_signal("y_15_toggled", bits.y_15);
		}

		if (old_y_16 != bits.y_16)
		{
			emit_signal("y_16_toggled", bits.y_16);
		}

		if (old_y_17 != bits.y_17)
		{
			emit_signal("y_17_toggled", bits.y_17);
		}

		if (old_y_18 != bits.y_18)
		{
			emit_signal("y_18_toggled", bits.y_18);
		}

		emit_signal("register_set", byte);
	}
}

std::uint8_t VerticalOffsetRegister1::get_register()
{
	return byte;
}

void VerticalOffsetRegister1::set_bit(int bit, bool new_value)
{
	switch (bit)
	{

	case Bits::Y_11:
		set_y_11(new_value);
		break;

	case Bits::Y_12:
		set_y_12(new_value);
		break;

	case Bits::Y_13:
		set_y_13(new_value);
		break;

	case Bits::Y_14:
		set_y_14(new_value);
		break;

	case Bits::Y_15:
		set_y_15(new_value);
		break;

	case Bits::Y_16:
		set_y_16(new_value);
		break;

	case Bits::Y_17:
		set_y_17(new_value);
		break;

	case Bits::Y_18:
		set_y_18(new_value);
		break;
	}
}

bool VerticalOffsetRegister1::get_bit(int bit)
{
	switch (bit)
	{

	case Bits::Y_11:
		return bits.y_11;

	case Bits::Y_12:
		return bits.y_12;

	case Bits::Y_13:
		return bits.y_13;

	case Bits::Y_14:
		return bits.y_14;

	case Bits::Y_15:
		return bits.y_15;

	case Bits::Y_16:
		return bits.y_16;

	case Bits::Y_17:
		return bits.y_17;

	case Bits::Y_18:
		return bits.y_18;
	}

	return false;
}

void VerticalOffsetRegister1::set_y_11(bool new_value)
{
	if (bits.y_11 != new_value)
	{
		bits.y_11 = new_value;
		emit_signal("y_11_toggled", new_value);
		emit_signal("register_set", byte);
	}
}

bool VerticalOffsetRegister1::get_y_11()
{
	return bits.y_11;
}

void VerticalOffsetRegister1::set_y_12(bool new_value)
{
	if (bits.y_12 != new_value)
	{
		bits.y_12 = new_value;
		emit_signal("y_12_toggled", new_value);
		emit_signal("register_set", byte);
	}
}

bool VerticalOffsetRegister1::get_y_12()
{
	return bits.y_12;
}

void VerticalOffsetRegister1::set_y_13(bool new_value)
{
	if (bits.y_13 != new_value)
	{
		bits.y_13 = new_value;
		emit_signal("y_13_toggled", new_value);
		emit_signal("register_set", byte);
	}
}

bool VerticalOffsetRegister1::get_y_13()
{
	return bits.y_13;
}

void VerticalOffsetRegister1::set_y_14(bool new_value)
{
	if (bits.y_14 != new_value)
	{
		bits.y_14 = new_value;
		emit_signal("y_14_toggled", new_value);
		emit_signal("register_set", byte);
	}
}

bool VerticalOffsetRegister1::get_y_14()
{
	return bits.y_14;
}

void VerticalOffsetRegister1::set_y_15(bool new_value)
{
	if (bits.y_15 != new_value)
	{
		bits.y_15 = new_value;
		emit_signal("y_15_toggled", new_value);
		emit_signal("register_set", byte);
	}
}

bool VerticalOffsetRegister1::get_y_15()
{
	return bits.y_15;
}

void VerticalOffsetRegister1::set_y_16(bool new_value)
{
	if (bits.y_16 != new_value)
	{
		bits.y_16 = new_value;
		emit_signal("y_16_toggled", new_value);
		emit_signal("register_set", byte);
	}
}

bool VerticalOffsetRegister1::get_y_16()
{
	return bits.y_16;
}

void VerticalOffsetRegister1::set_y_17(bool new_value)
{
	if (bits.y_17 != new_value)
	{
		bits.y_17 = new_value;
		emit_signal("y_17_toggled", new_value);
		emit_signal("register_set", byte);
	}
}

bool VerticalOffsetRegister1::get_y_17()
{
	return bits.y_17;
}

void VerticalOffsetRegister1::set_y_18(bool new_value)
{
	if (bits.y_18 != new_value)
	{
		bits.y_18 = new_value;
		emit_signal("y_18_toggled", new_value);
		emit_signal("register_set", byte);
	}
}

bool VerticalOffsetRegister1::get_y_18()
{
	return bits.y_18;
}
