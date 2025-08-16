#include "HorizontalOffsetRegister.hpp"

using namespace godot;

HorizontalOffsetRegister::HorizontalOffsetRegister() : byte{0}
{
}

HorizontalOffsetRegister::~HorizontalOffsetRegister()
{
}

void HorizontalOffsetRegister::_register_methods()
{
	// Methods
	register_method("set_bit", &HorizontalOffsetRegister::set_bit);
	register_method("get_bit", &HorizontalOffsetRegister::get_bit);

	// Properties
	register_property<HorizontalOffsetRegister, std::uint8_t>("register", &HorizontalOffsetRegister::set_register, &HorizontalOffsetRegister::get_register, 0);
	register_property<HorizontalOffsetRegister, bool>("horizontal_offset_0", &HorizontalOffsetRegister::set_horizontal_offset_0, &HorizontalOffsetRegister::get_horizontal_offset_0, false);
	register_property<HorizontalOffsetRegister, bool>("horizontal_offset_1", &HorizontalOffsetRegister::set_horizontal_offset_1, &HorizontalOffsetRegister::get_horizontal_offset_1, false);
	register_property<HorizontalOffsetRegister, bool>("horizontal_offset_2", &HorizontalOffsetRegister::set_horizontal_offset_2, &HorizontalOffsetRegister::get_horizontal_offset_2, false);
	register_property<HorizontalOffsetRegister, bool>("horizontal_offset_3", &HorizontalOffsetRegister::set_horizontal_offset_3, &HorizontalOffsetRegister::get_horizontal_offset_3, false);
	register_property<HorizontalOffsetRegister, bool>("horizontal_offset_4", &HorizontalOffsetRegister::set_horizontal_offset_4, &HorizontalOffsetRegister::get_horizontal_offset_4, false);
	register_property<HorizontalOffsetRegister, bool>("horizontal_offset_5", &HorizontalOffsetRegister::set_horizontal_offset_5, &HorizontalOffsetRegister::get_horizontal_offset_5, false);
	register_property<HorizontalOffsetRegister, bool>("horizontal_offset_6", &HorizontalOffsetRegister::set_horizontal_offset_6, &HorizontalOffsetRegister::get_horizontal_offset_6, false);
	register_property<HorizontalOffsetRegister, std::uint8_t>("horizontal_offset", &HorizontalOffsetRegister::set_horizontal_offset, &HorizontalOffsetRegister::get_horizontal_offset, 0);
	register_property<HorizontalOffsetRegister, bool>("horizontal_extended", &HorizontalOffsetRegister::set_horizontal_extended, &HorizontalOffsetRegister::get_horizontal_extended, false);

	// Signals
	register_signal<HorizontalOffsetRegister>("register_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<HorizontalOffsetRegister>("horizontal_offset_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<HorizontalOffsetRegister>("horizontal_offset_0_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<HorizontalOffsetRegister>("horizontal_offset_1_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<HorizontalOffsetRegister>("horizontal_offset_2_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<HorizontalOffsetRegister>("horizontal_offset_3_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<HorizontalOffsetRegister>("horizontal_offset_4_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<HorizontalOffsetRegister>("horizontal_offset_5_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<HorizontalOffsetRegister>("horizontal_offset_6_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<HorizontalOffsetRegister>("horizontal_extended_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
}

void HorizontalOffsetRegister::_init()
{
}

void HorizontalOffsetRegister::set_register(std::uint8_t new_value)
{
	if (byte != new_value)
	{
		bool old_horizontal_offset_0 = bits.horizontal_offset_0;
		bool old_horizontal_offset_1 = bits.horizontal_offset_1;
		bool old_horizontal_offset_2 = bits.horizontal_offset_2;
		bool old_horizontal_offset_3 = bits.horizontal_offset_3;
		bool old_horizontal_offset_4 = bits.horizontal_offset_4;
		bool old_horizontal_offset_5 = bits.horizontal_offset_5;
		bool old_horizontal_offset_6 = bits.horizontal_offset_6;
		std::uint8_t old_horizontal_offset = get_horizontal_offset();
		bool old_horizontal_extended = bits.horizontal_extended;

		byte = new_value;

		if (old_horizontal_offset != get_horizontal_offset())
		{
			if (old_horizontal_offset_0 != bits.horizontal_offset_0)
			{
				emit_signal("horizontal_offset_0_toggled", bits.horizontal_offset_0);
			}

			if (old_horizontal_offset_1 != bits.horizontal_offset_1)
			{
				emit_signal("horizontal_offset_1_toggled", bits.horizontal_offset_1);
			}

			if (old_horizontal_offset_2 != bits.horizontal_offset_2)
			{
				emit_signal("horizontal_offset_2_toggled", bits.horizontal_offset_2);
			}

			if (old_horizontal_offset_3 != bits.horizontal_offset_3)
			{
				emit_signal("horizontal_offset_3_toggled", bits.horizontal_offset_3);
			}

			if (old_horizontal_offset_4 != bits.horizontal_offset_4)
			{
				emit_signal("horizontal_offset_4_toggled", bits.horizontal_offset_4);
			}

			if (old_horizontal_offset_5 != bits.horizontal_offset_5)
			{
				emit_signal("horizontal_offset_5_toggled", bits.horizontal_offset_5);
			}

			if (old_horizontal_offset_6 != bits.horizontal_offset_6)
			{
				emit_signal("horizontal_offset_6_toggled", bits.horizontal_offset_6);
			}

			emit_signal("horizontal_offset_set", get_horizontal_offset());
		}

		if (old_horizontal_extended != bits.horizontal_extended)
		{
			emit_signal("horizontal_extended_toggled", bits.horizontal_extended);
		}

		emit_signal("register_set", byte);
	}
}

std::uint8_t HorizontalOffsetRegister::get_register()
{
	return byte;
}

void HorizontalOffsetRegister::set_bit(int bit, bool new_value)
{
	switch (bit)
	{

	case Bits::HORIZONTAL_OFFSET_0:
		set_horizontal_offset_0(new_value);
		break;

	case Bits::HORIZONTAL_OFFSET_1:
		set_horizontal_offset_1(new_value);
		break;

	case Bits::HORIZONTAL_OFFSET_2:
		set_horizontal_offset_2(new_value);
		break;

	case Bits::HORIZONTAL_OFFSET_3:
		set_horizontal_offset_3(new_value);
		break;

	case Bits::HORIZONTAL_OFFSET_4:
		set_horizontal_offset_4(new_value);
		break;

	case Bits::HORIZONTAL_OFFSET_5:
		set_horizontal_offset_5(new_value);
		break;

	case Bits::HORIZONTAL_OFFSET_6:
		set_horizontal_offset_6(new_value);
		break;

	case Bits::HORIZONTAL_EXTENDED:
		set_horizontal_extended(new_value);
		break;
	}
}

bool HorizontalOffsetRegister::get_bit(int bit)
{
	switch (bit)
	{

	case Bits::HORIZONTAL_OFFSET_0:
		return bits.horizontal_offset_0;

	case Bits::HORIZONTAL_OFFSET_1:
		return bits.horizontal_offset_1;

	case Bits::HORIZONTAL_OFFSET_2:
		return bits.horizontal_offset_2;

	case Bits::HORIZONTAL_OFFSET_3:
		return bits.horizontal_offset_3;

	case Bits::HORIZONTAL_OFFSET_4:
		return bits.horizontal_offset_4;

	case Bits::HORIZONTAL_OFFSET_5:
		return bits.horizontal_offset_5;

	case Bits::HORIZONTAL_OFFSET_6:
		return bits.horizontal_offset_6;

	case Bits::HORIZONTAL_EXTENDED:
		return bits.horizontal_extended;
	}

	return false;
}

void HorizontalOffsetRegister::set_horizontal_offset_0(bool new_value)
{
	if (bits.horizontal_offset_0 != new_value)
	{
		bits.horizontal_offset_0 = new_value;
		emit_signal("horizontal_offset_0_toggled", new_value);
		emit_signal("horizontal_offset_set", get_horizontal_offset());
		emit_signal("register_set", byte);
	}
}

bool HorizontalOffsetRegister::get_horizontal_offset_0()
{
	return bits.horizontal_offset_0;
}

void HorizontalOffsetRegister::set_horizontal_offset_1(bool new_value)
{
	if (bits.horizontal_offset_1 != new_value)
	{
		bits.horizontal_offset_1 = new_value;
		emit_signal("horizontal_offset_1_toggled", new_value);
		emit_signal("horizontal_offset_set", get_horizontal_offset());
		emit_signal("register_set", byte);
	}
}

bool HorizontalOffsetRegister::get_horizontal_offset_1()
{
	return bits.horizontal_offset_1;
}

void HorizontalOffsetRegister::set_horizontal_offset_2(bool new_value)
{
	if (bits.horizontal_offset_2 != new_value)
	{
		bits.horizontal_offset_2 = new_value;
		emit_signal("horizontal_offset_2_toggled", new_value);
		emit_signal("horizontal_offset_set", get_horizontal_offset());
		emit_signal("register_set", byte);
	}
}

bool HorizontalOffsetRegister::get_horizontal_offset_2()
{
	return bits.horizontal_offset_2;
}

void HorizontalOffsetRegister::set_horizontal_offset_3(bool new_value)
{
	if (bits.horizontal_offset_3 != new_value)
	{
		bits.horizontal_offset_3 = new_value;
		emit_signal("horizontal_offset_3_toggled", new_value);
		emit_signal("horizontal_offset_set", get_horizontal_offset());
		emit_signal("register_set", byte);
	}
}

bool HorizontalOffsetRegister::get_horizontal_offset_3()
{
	return bits.horizontal_offset_3;
}

void HorizontalOffsetRegister::set_horizontal_offset_4(bool new_value)
{
	if (bits.horizontal_offset_4 != new_value)
	{
		bits.horizontal_offset_4 = new_value;
		emit_signal("horizontal_offset_4_toggled", new_value);
		emit_signal("horizontal_offset_set", get_horizontal_offset());
		emit_signal("register_set", byte);
	}
}

bool HorizontalOffsetRegister::get_horizontal_offset_4()
{
	return bits.horizontal_offset_4;
}

void HorizontalOffsetRegister::set_horizontal_offset_5(bool new_value)
{
	if (bits.horizontal_offset_5 != new_value)
	{
		bits.horizontal_offset_5 = new_value;
		emit_signal("horizontal_offset_5_toggled", new_value);
		emit_signal("horizontal_offset_set", get_horizontal_offset());
		emit_signal("register_set", byte);
	}
}

bool HorizontalOffsetRegister::get_horizontal_offset_5()
{
	return bits.horizontal_offset_5;
}

void HorizontalOffsetRegister::set_horizontal_offset_6(bool new_value)
{
	if (bits.horizontal_offset_6 != new_value)
	{
		bits.horizontal_offset_6 = new_value;
		emit_signal("horizontal_offset_6_toggled", new_value);
		emit_signal("horizontal_offset_set", get_horizontal_offset());
		emit_signal("register_set", byte);
	}
}

bool HorizontalOffsetRegister::get_horizontal_offset_6()
{
	return bits.horizontal_offset_6;
}

void HorizontalOffsetRegister::set_horizontal_offset(std::uint8_t new_value)
{
	if (get_horizontal_offset() != new_value)
	{
		bool old_horizontal_offset_0 = bits.horizontal_offset_0;
		bool old_horizontal_offset_1 = bits.horizontal_offset_1;
		bool old_horizontal_offset_2 = bits.horizontal_offset_2;
		bool old_horizontal_offset_3 = bits.horizontal_offset_3;
		bool old_horizontal_offset_4 = bits.horizontal_offset_4;
		bool old_horizontal_offset_5 = bits.horizontal_offset_5;
		bool old_horizontal_offset_6 = bits.horizontal_offset_6;

		byte = (byte & 0x80) | (new_value & 0x7F);

		if (old_horizontal_offset_0 != bits.horizontal_offset_0)
		{
			emit_signal("horizontal_offset_0_toggled", bits.horizontal_offset_0);
		}

		if (old_horizontal_offset_1 != bits.horizontal_offset_1)
		{
			emit_signal("horizontal_offset_1_toggled", bits.horizontal_offset_1);
		}

		if (old_horizontal_offset_2 != bits.horizontal_offset_2)
		{
			emit_signal("horizontal_offset_2_toggled", bits.horizontal_offset_2);
		}

		if (old_horizontal_offset_3 != bits.horizontal_offset_3)
		{
			emit_signal("horizontal_offset_3_toggled", bits.horizontal_offset_3);
		}

		if (old_horizontal_offset_4 != bits.horizontal_offset_4)
		{
			emit_signal("horizontal_offset_4_toggled", bits.horizontal_offset_4);
		}

		if (old_horizontal_offset_5 != bits.horizontal_offset_5)
		{
			emit_signal("horizontal_offset_5_toggled", bits.horizontal_offset_5);
		}

		if (old_horizontal_offset_6 != bits.horizontal_offset_6)
		{
			emit_signal("horizontal_offset_6_toggled", bits.horizontal_offset_6);
		}

		emit_signal("horizontal_offset_set", get_horizontal_offset());
	}
}

std::uint8_t HorizontalOffsetRegister::get_horizontal_offset()
{
	return byte & 0x7F;
}

void HorizontalOffsetRegister::set_horizontal_extended(bool new_value)
{
	if (bits.horizontal_extended != new_value)
	{
		bits.horizontal_extended = new_value;
		emit_signal("horizontal_extended_toggled", new_value);
		emit_signal("register_set", byte);
	}
}

bool HorizontalOffsetRegister::get_horizontal_extended()
{
	return bits.horizontal_extended;
}
