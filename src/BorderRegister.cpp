#include "BorderRegister.hpp"

using namespace godot;

BorderRegister::BorderRegister() : byte{0}
{
}

BorderRegister::~BorderRegister()
{
}

// Godot Specific Definitions

void BorderRegister::_register_methods()
{
	// Methods
	register_method("set_bit", &BorderRegister::set_bit);
	register_method("get_bit", &BorderRegister::get_bit);

	// Properties
	register_property<BorderRegister, std::uint8_t>("register", &BorderRegister::set_register, &BorderRegister::get_register, 0);
	register_property<BorderRegister, bool>("blue_0", &BorderRegister::set_blue_0, &BorderRegister::get_blue_0, false);
	register_property<BorderRegister, bool>("blue_1", &BorderRegister::set_blue_1, &BorderRegister::get_blue_1, false);
	register_property<BorderRegister, std::uint8_t>("blue", &BorderRegister::set_blue, &BorderRegister::get_blue, 0);
	register_property<BorderRegister, bool>("green_0", &BorderRegister::set_green_0, &BorderRegister::get_green_0, false);
	register_property<BorderRegister, bool>("green_1", &BorderRegister::set_green_1, &BorderRegister::get_green_1, false);
	register_property<BorderRegister, std::uint8_t>("green", &BorderRegister::set_green, &BorderRegister::get_green, 0);
	register_property<BorderRegister, bool>("red_0", &BorderRegister::set_red_0, &BorderRegister::get_red_0, false);
	register_property<BorderRegister, bool>("red_1", &BorderRegister::set_red_1, &BorderRegister::get_red_1, false);
	register_property<BorderRegister, std::uint8_t>("blue", &BorderRegister::set_red, &BorderRegister::get_red, 0);

	// Signals
	register_signal<BorderRegister>("register_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<BorderRegister>("blue_0_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<BorderRegister>("blue_1_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<BorderRegister>("blue_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<BorderRegister>("green_0_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<BorderRegister>("green_1_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<BorderRegister>("green_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<BorderRegister>("red_0_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<BorderRegister>("red_1_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<BorderRegister>("red_set", "new_value", GODOT_VARIANT_TYPE_INT);
}

void BorderRegister::_init()
{
}

// Class Definitions

void BorderRegister::set_register(std::uint8_t new_value)
{
	if (byte != new_value)
	{
		bool old_blue_0 = bits.blue_0;
		bool old_blue_1 = bits.blue_1;
		std::uint8_t old_blue = get_blue();
		bool old_green_0 = bits.green_0;
		bool old_green_1 = bits.green_1;
		std::uint8_t old_green = get_green();
		bool old_red_0 = bits.red_0;
		bool old_red_1 = bits.red_1;
		std::uint8_t old_red = get_red();

		byte = new_value;

		if (old_blue != get_blue())
		{
			if (old_blue_0 != bits.blue_0)
			{
				emit_signal("blue_0_toggled", bits.blue_0);
			}

			if (old_blue_1 != bits.blue_1)
			{
				emit_signal("blue_1_toggled", bits.blue_1);
			}

			emit_signal("blue_set", get_blue());
		}

		if (old_green != get_green())
		{
			if (old_green_0 != bits.green_0)
			{
				emit_signal("green_0_toggled", bits.green_0);
			}

			if (old_green_1 != bits.green_1)
			{
				emit_signal("green_1_toggled", bits.green_1);
			}

			emit_signal("geen_set", get_green());
		}

		if (old_red != get_red())
		{
			if (old_red_0 != bits.red_0)
			{
				emit_signal("red_0_toggled", bits.red_0);
			}

			if (old_red_1 != bits.red_1)
			{
				emit_signal("red_1_toggled", bits.red_1);
			}

			emit_signal("red_set", get_red());
		}

		emit_signal("register_set", byte);
	}
}

std::uint8_t BorderRegister::get_register()
{
	return byte;
}

void BorderRegister::set_bit(int bit, bool new_value)
{
	switch (bit)
	{

	case Bits::BLUE_0:
		set_blue_0(new_value);
		break;

	case Bits::GREEN_0:
		set_green_0(new_value);
		break;

	case Bits::RED_0:
		set_red_0(new_value);
		break;

	case Bits::BLUE_1:
		set_blue_1(new_value);
		break;

	case Bits::GREEN_1:
		set_green_1(new_value);
		break;

	case Bits::RED_1:
		set_red_1(new_value);
		break;
	}
}

bool BorderRegister::get_bit(int bit)
{
	switch (bit)
	{

	case Bits::BLUE_0:
		return bits.blue_0;

	case Bits::GREEN_0:
		return bits.green_0;

	case Bits::RED_0:
		return bits.red_0;

	case Bits::BLUE_1:
		return bits.blue_1;

	case Bits::GREEN_1:
		return bits.green_1;

	case Bits::RED_1:
		return bits.red_1;
	}

	return false;
}

void BorderRegister::set_blue_0(bool new_value)
{
	if (bits.blue_0 != new_value)
	{
		bits.blue_0 = new_value;
		emit_signal("blue_0_toggled", new_value);
		emit_signal("blue_set", get_blue());
		emit_signal("register_set", byte);
	}
}

bool BorderRegister::get_blue_0()
{
	return bits.blue_0;
}

void BorderRegister::set_blue_1(bool new_value)
{
	if (bits.blue_1 != new_value)
	{
		bits.blue_1 = new_value;
		emit_signal("blue_1_toggled", new_value);
		emit_signal("blue_set", get_blue());
		emit_signal("register_set", byte);
	}
}

bool BorderRegister::get_blue_1()
{
	return bits.blue_1;
}

void BorderRegister::set_blue(std::uint8_t new_value)
{
	if (get_blue() != new_value)
	{
		bool old_blue_0 = bits.blue_0;
		bool old_blue_1 = bits.blue_1;

		byte = (byte & (255 - static_cast<std::uint8_t>(Blue::BLUE_3))) |
			   (new_value & static_cast<std::uint8_t>(Blue::BLUE_3));

		if (old_blue_0 != bits.blue_0)
		{
			emit_signal("blue_0_toggled", bits.blue_0);
		}

		if (old_blue_1 != bits.blue_1)
		{
			emit_signal("blue_1_toggled", bits.blue_1);
		}

		emit_signal("blue_set", get_blue());
		emit_signal("register_set", byte);
	}
}

std::uint8_t BorderRegister::get_blue()
{
	return byte & static_cast<std::uint8_t>(Blue::BLUE_3);
}

void BorderRegister::set_green_0(bool new_value)
{
	if (bits.green_0 != new_value)
	{
		bits.green_0 = new_value;
		emit_signal("green_0_toggled", new_value);
		emit_signal("green_set", get_green());
		emit_signal("register_set", byte);
	}
}

bool BorderRegister::get_green_0()
{
	return bits.green_0;
}

void BorderRegister::set_green_1(bool new_value)
{
	if (bits.green_1 != new_value)
	{
		bits.green_1 = new_value;
		emit_signal("green_1_toggled", new_value);
		emit_signal("green_set", get_green());
		emit_signal("register_set", byte);
	}
}

bool BorderRegister::get_green_1()
{
	return bits.green_1;
}

void BorderRegister::set_green(std::uint8_t new_value)
{
	if (get_green() != new_value)
	{
		bool old_green_0 = bits.green_0;
		bool old_green_1 = bits.green_1;

		byte = (byte & (255 - static_cast<std::uint8_t>(Green::GREEN_3))) |
			   (new_value & static_cast<std::uint8_t>(Green::GREEN_3));

		if (old_green_0 != bits.green_0)
		{
			emit_signal("green_0_toggled", bits.green_0);
		}

		if (old_green_1 != bits.green_1)
		{
			emit_signal("green_1_toggled", bits.green_1);
		}

		emit_signal("green_set", get_green());
		emit_signal("register_set", byte);
	}
}

std::uint8_t BorderRegister::get_green()
{
	return byte & static_cast<std::uint8_t>(Green::GREEN_3);
}

void BorderRegister::set_red_0(bool new_value)
{
	if (bits.red_0 != new_value)
	{
		bits.red_0 = new_value;
		emit_signal("red_0_toggled", new_value);
		emit_signal("red_set", get_red());
		emit_signal("register_set", byte);
	}
}

bool BorderRegister::get_red_0()
{
	return bits.red_0;
}

void BorderRegister::set_red_1(bool new_value)
{
	if (bits.red_1 != new_value)
	{
		bits.red_1 = new_value;
		emit_signal("red_1_toggled", new_value);
		emit_signal("red_set", get_red());
		emit_signal("register_set", byte);
	}
}

bool BorderRegister::get_red_1()
{
	return bits.red_1;
}

void BorderRegister::set_red(std::uint8_t new_value)
{
	if (get_red() != new_value)
	{
		bool old_red_0 = bits.red_0;
		bool old_red_1 = bits.red_1;

		byte = (byte & (255 - static_cast<std::uint8_t>(Red::RED_3))) |
			   (new_value & static_cast<std::uint8_t>(Red::RED_3));

		if (old_red_0 != bits.red_0)
		{
			emit_signal("red_0_toggled", bits.red_0);
		}

		if (old_red_1 != bits.red_1)
		{
			emit_signal("red_1_toggled", bits.red_1);
		}

		emit_signal("red_set", get_red());
		emit_signal("register_set", byte);
	}
}

std::uint8_t BorderRegister::get_red()
{
	return byte & static_cast<std::uint8_t>(Red::RED_3);
}
