#include "VerticalScrollRegister.hpp"

using namespace godot;

VerticalScrollRegister::VerticalScrollRegister()
{
}

VerticalScrollRegister::~VerticalScrollRegister()
{
}

void VerticalScrollRegister::_register_methods()
{
	// Methods
	register_method("set_bit", &VerticalScrollRegister::set_bit);
	register_method("get_bit", &VerticalScrollRegister::get_bit);

	// Properties
	register_property<VerticalScrollRegister, std::uint8_t>("register", &VerticalScrollRegister::set_register, &VerticalScrollRegister::get_register, 0);
	register_property<VerticalScrollRegister, bool>("vertical_scroll_0", &VerticalScrollRegister::set_vertical_scroll_0, &VerticalScrollRegister::get_vertical_scroll_0, false);
	register_property<VerticalScrollRegister, bool>("vertical_scroll_1", &VerticalScrollRegister::set_vertical_scroll_1, &VerticalScrollRegister::get_vertical_scroll_1, false);
	register_property<VerticalScrollRegister, bool>("vertical_scroll_2", &VerticalScrollRegister::set_vertical_scroll_2, &VerticalScrollRegister::get_vertical_scroll_2, false);
	register_property<VerticalScrollRegister, bool>("vertical_scroll_3", &VerticalScrollRegister::set_vertical_scroll_3, &VerticalScrollRegister::get_vertical_scroll_3, false);

	// Signals
	register_signal<VerticalScrollRegister>("register_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<VerticalScrollRegister>("vertical_scroll_0_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<VerticalScrollRegister>("vertical_scroll_1_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<VerticalScrollRegister>("vertical_scroll_2_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<VerticalScrollRegister>("vertical_scroll_3_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
}

void VerticalScrollRegister::_init()
{
}

void VerticalScrollRegister::set_register(std::uint8_t new_value)
{
	if (byte != new_value)
	{
		bool old_vertical_scroll_0 = bits.vertical_scroll_0;
		bool old_vertical_scroll_1 = bits.vertical_scroll_1;
		bool old_vertical_scroll_2 = bits.vertical_scroll_2;
		bool old_vertical_scroll_3 = bits.vertical_scroll_3;

		byte = new_value;

		if (old_vertical_scroll_0 != bits.vertical_scroll_0)
		{
			emit_signal("vertical_scroll_0_toggled", bits.vertical_scroll_0);
		}

		if (old_vertical_scroll_1 != bits.vertical_scroll_1)
		{
			emit_signal("vertical_scroll_1_toggled", bits.vertical_scroll_1);
		}

		if (old_vertical_scroll_2 != bits.vertical_scroll_2)
		{
			emit_signal("vertical_scroll_2_toggled", bits.vertical_scroll_2);
		}

		if (old_vertical_scroll_3 != bits.vertical_scroll_3)
		{
			emit_signal("vertical_scroll_3_toggled", bits.vertical_scroll_3);
		}

		emit_signal("register_set", byte);
	}
}

std::uint8_t VerticalScrollRegister::get_register()
{
	return byte;
}

void VerticalScrollRegister::set_bit(int bit, bool new_value)
{
	switch (bit)
	{

	case Bits::VERTICAL_SCROLL_0:
		set_vertical_scroll_0(new_value);
		return;

	case Bits::VERTICAL_SCROLL_1:
		set_vertical_scroll_1(new_value);
		return;

	case Bits::VERTICAL_SCROLL_2:
		set_vertical_scroll_2(new_value);
		return;

	case Bits::VERTICAL_SCROLL_3:
		set_vertical_scroll_3(new_value);
		return;
	}
}

bool VerticalScrollRegister::get_bit(int bit)
{
	switch (bit)
	{

	case Bits::VERTICAL_SCROLL_0:
		return bits.vertical_scroll_0;

	case Bits::VERTICAL_SCROLL_1:
		return bits.vertical_scroll_1;

	case Bits::VERTICAL_SCROLL_2:
		return bits.vertical_scroll_2;

	case Bits::VERTICAL_SCROLL_3:
		return bits.vertical_scroll_3;
	}

	return false;
}

void VerticalScrollRegister::set_vertical_scroll_0(bool new_value)
{
	if (bits.vertical_scroll_0 != new_value)
	{
		bits.vertical_scroll_0 = new_value;
		emit_signal("vertical_scroll_0_toggled", bits.vertical_scroll_0);
		emit_signal("register_set", byte);
	}
}

bool VerticalScrollRegister::get_vertical_scroll_0()
{
	return bits.vertical_scroll_0;
}

void VerticalScrollRegister::set_vertical_scroll_1(bool new_value)
{
	if (bits.vertical_scroll_1 != new_value)
	{
		bits.vertical_scroll_1 = new_value;
		emit_signal("vertical_scroll_1_toggled", bits.vertical_scroll_1);
		emit_signal("register_set", byte);
	}
}

bool VerticalScrollRegister::get_vertical_scroll_1()
{
	return bits.vertical_scroll_1;
}

void VerticalScrollRegister::set_vertical_scroll_2(bool new_value)
{
	if (bits.vertical_scroll_2 != new_value)
	{
		bits.vertical_scroll_2 = new_value;
		emit_signal("vertical_scroll_2_toggled", bits.vertical_scroll_2);
		emit_signal("register_set", byte);
	}
}

bool VerticalScrollRegister::get_vertical_scroll_2()
{
	return bits.vertical_scroll_2;
}

void VerticalScrollRegister::set_vertical_scroll_3(bool new_value)
{
	if (bits.vertical_scroll_3 != new_value)
	{
		bits.vertical_scroll_3 = new_value;
		emit_signal("vertical_scroll_3_toggled", bits.vertical_scroll_3);
		emit_signal("register_set", byte);
	}
}

bool VerticalScrollRegister::get_vertical_scroll_3()
{
	return bits.vertical_scroll_3;
}
