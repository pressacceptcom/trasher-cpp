#include "VideoResolutionRegister.hpp"

using namespace godot;

VideoResolutionRegister::VideoResolutionRegister() : byte{0}
{
}

VideoResolutionRegister::~VideoResolutionRegister()
{
}

void VideoResolutionRegister::_register_methods()
{
	// Methods
	register_method("set_bit", &VideoResolutionRegister::set_bit);
	register_method("get_bit", &VideoResolutionRegister::get_bit);

	// Properties
	register_property<VideoResolutionRegister, std::uint8_t>("register", &VideoResolutionRegister::set_register, &VideoResolutionRegister::get_register, 0);
	register_property<VideoResolutionRegister, bool>("color_resolution_0", &VideoResolutionRegister::set_color_resolution_0, &VideoResolutionRegister::get_color_resolution_0, false);
	register_property<VideoResolutionRegister, bool>("color_resolution_1", &VideoResolutionRegister::set_color_resolution_1, &VideoResolutionRegister::get_color_resolution_1, false);
	register_property<VideoResolutionRegister, std::uint8_t>("color_resolution", &VideoResolutionRegister::set_color_resolution, &VideoResolutionRegister::get_color_resolution, 0);
	register_property<VideoResolutionRegister, bool>("horizontal_resolution_0", &VideoResolutionRegister::set_horizontal_resolution_0, &VideoResolutionRegister::get_horizontal_resolution_0, false);
	register_property<VideoResolutionRegister, bool>("horizontal_resolution_1", &VideoResolutionRegister::set_horizontal_resolution_1, &VideoResolutionRegister::get_horizontal_resolution_1, false);
	register_property<VideoResolutionRegister, bool>("horizontal_resolution_2", &VideoResolutionRegister::set_horizontal_resolution_2, &VideoResolutionRegister::get_horizontal_resolution_2, false);
	register_property<VideoResolutionRegister, std::uint8_t>("horizontal_resolution", &VideoResolutionRegister::set_horizontal_resolution, &VideoResolutionRegister::get_horizontal_resolution, 0);
	register_property<VideoResolutionRegister, bool>("lines_per_field_0", &VideoResolutionRegister::set_lines_per_field_0, &VideoResolutionRegister::get_lines_per_field_0, false);
	register_property<VideoResolutionRegister, bool>("lines_per_field_1", &VideoResolutionRegister::set_lines_per_field_1, &VideoResolutionRegister::get_lines_per_field_1, false);
	register_property<VideoResolutionRegister, std::uint8_t>("lines_per_field", &VideoResolutionRegister::set_lines_per_field, &VideoResolutionRegister::get_lines_per_field, 0);

	// Signals
	register_signal<VideoResolutionRegister>("register_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<VideoResolutionRegister>("color_resolution_0_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<VideoResolutionRegister>("color_resolution_1_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<VideoResolutionRegister>("color_resolution_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<VideoResolutionRegister>("horizontal_resolution_0_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<VideoResolutionRegister>("horizontal_resolution_1_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<VideoResolutionRegister>("horizontal_resolution_2_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<VideoResolutionRegister>("horizontal_resolution_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<VideoResolutionRegister>("lines_per_field_0_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<VideoResolutionRegister>("lines_per_field_1_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<VideoResolutionRegister>("lines_per_field_set", "new_value", GODOT_VARIANT_TYPE_INT);
}

void VideoResolutionRegister::_init()
{
}

void VideoResolutionRegister::set_register(std::uint8_t new_value)
{
	if (byte != new_value)
	{
		bool old_color_resolution_0 = bits.color_resolution_0;
		bool old_color_resolution_1 = bits.color_resolution_1;
		std::uint8_t old_color_resolution = get_color_resolution();
		bool old_horizontal_resolution_0 = bits.horizontal_resolution_0;
		bool old_horizontal_resolution_1 = bits.horizontal_resolution_1;
		bool old_horizontal_resolution_2 = bits.horizontal_resolution_2;
		std::uint8_t old_horizontal_resolution = get_horizontal_resolution();
		bool old_lines_per_field_0 = bits.lines_per_field_0;
		bool old_lines_per_field_1 = bits.lines_per_field_1;
		std::uint8_t old_lines_per_field = get_lines_per_field();

		byte = new_value;

		if (old_color_resolution != get_color_resolution())
		{
			if (old_color_resolution_0 != bits.color_resolution_0)
			{
				emit_signal("color_resolution_0_toggled", bits.color_resolution_0);
			}

			if (old_color_resolution_1 != bits.color_resolution_1)
			{
				emit_signal("color_resolution_1_toggled", bits.color_resolution_1);
			}

			emit_signal("color_resolution_set", get_color_resolution());
		}

		if (old_horizontal_resolution != get_horizontal_resolution())
		{
			if (old_horizontal_resolution_0 != bits.horizontal_resolution_0)
			{
				emit_signal("horizontal_resolution_0_toggled", bits.horizontal_resolution_0);
			}

			if (old_horizontal_resolution_1 != bits.horizontal_resolution_1)
			{
				emit_signal("horizontal_resolution_1_toggled", bits.horizontal_resolution_1);
			}

			if (old_horizontal_resolution_2 != bits.horizontal_resolution_2)
			{
				emit_signal("horizontal_resolution_2_toggled", bits.horizontal_resolution_2);
			}

			emit_signal("horizontal_resolution_set", get_horizontal_resolution());
		}

		if (old_lines_per_field != get_lines_per_field())
		{
			if (old_lines_per_field_0 != bits.lines_per_field_0)
			{
				emit_signal("lines_per_field_0_toggled", bits.lines_per_field_0);
			}

			if (old_lines_per_field_1 != bits.lines_per_field_1)
			{
				emit_signal("lines_per_field_1_toggled", bits.lines_per_field_1);
			}

			emit_signal("lines_per_field_set", get_lines_per_field());
		}

		emit_signal("register_set", byte);
	}
}

std::uint8_t VideoResolutionRegister::get_register()
{
	return byte;
}

void VideoResolutionRegister::set_bit(int bit, bool new_value)
{
	switch (bit)
	{

	case Bits::COLOR_RESOLUTION_0:
		set_color_resolution_0(new_value);
		break;

	case Bits::COLOR_RESOLUTION_1:
		set_color_resolution_1(new_value);
		break;

	case Bits::HORIZONTAL_RESOLUTION_0:
		set_horizontal_resolution_0(new_value);
		break;

	case Bits::HORIZONTAL_RESOLUTION_1:
		set_horizontal_resolution_1(new_value);
		break;

	case Bits::HORIZONTAL_RESOLUTION_2:
		set_horizontal_resolution_2(new_value);
		break;

	case LINES_PER_FIELD_0:
		set_lines_per_field_0(new_value);
		break;

	case LINES_PER_FIELD_1:
		set_lines_per_field_1(new_value);
		break;
	}
}

bool VideoResolutionRegister::get_bit(int bit)
{
	switch (bit)
	{

	case Bits::COLOR_RESOLUTION_0:
		return bits.color_resolution_0;

	case Bits::COLOR_RESOLUTION_1:
		return bits.color_resolution_1;

	case Bits::HORIZONTAL_RESOLUTION_0:
		return bits.horizontal_resolution_0;

	case Bits::HORIZONTAL_RESOLUTION_1:
		return bits.horizontal_resolution_1;

	case Bits::HORIZONTAL_RESOLUTION_2:
		return bits.horizontal_resolution_2;

	case Bits::LINES_PER_FIELD_0:
		return bits.lines_per_field_0;

	case Bits::LINES_PER_FIELD_1:
		return bits.lines_per_field_1;
	}

	return false;
}

void VideoResolutionRegister::set_color_resolution_0(bool new_value)
{
	if (bits.color_resolution_0 != new_value)
	{
		bits.color_resolution_0 = new_value;
		emit_signal("color_resolution_0_toggled", bits.color_resolution_0);
		emit_signal("color_resolution_set", get_color_resolution());
		emit_signal("register_set", byte);
	}
}

bool VideoResolutionRegister::get_color_resolution_0()
{
	return bits.color_resolution_0;
}

void VideoResolutionRegister::set_color_resolution_1(bool new_value)
{
	if (bits.color_resolution_1 != new_value)
	{
		bits.color_resolution_1 = new_value;
		emit_signal("color_resolution_1_toggled", bits.color_resolution_1);
		emit_signal("color_resolution_set", get_color_resolution());
		emit_signal("register_set", byte);
	}
}

bool VideoResolutionRegister::get_color_resolution_1()
{
	return bits.color_resolution_1;
}

void VideoResolutionRegister::set_color_resolution(std::uint8_t new_value)
{
	if (get_color_resolution() != new_value)
	{
		bool old_color_resolution_0 = bits.color_resolution_0;
		bool old_color_resolution_1 = bits.color_resolution_1;

		byte = (byte & 0xFC) | (new_value & 0x03);

		if (old_color_resolution_0 != bits.color_resolution_0)
		{
			emit_signal("color_resolution_0_toggled", bits.color_resolution_0);
		}

		if (old_color_resolution_1 != bits.color_resolution_1)
		{
			emit_signal("color_resolution_1_toggled", bits.color_resolution_1);
		}

		emit_signal("color_resolution_set", get_color_resolution());
		emit_signal("register_set", byte);
	}
}

std::uint8_t VideoResolutionRegister::get_color_resolution()
{
	return byte & 0x03;
}

void VideoResolutionRegister::set_horizontal_resolution_0(bool new_value)
{
	if (bits.horizontal_resolution_0 != new_value)
	{
		bits.horizontal_resolution_0 = new_value;
		emit_signal("horizontal_resolution_0_toggled", bits.horizontal_resolution_0);
		emit_signal("horizontal_resolution_set", get_horizontal_resolution());
		emit_signal("register_set", byte);
	}
}

bool VideoResolutionRegister::get_horizontal_resolution_0()
{
	return bits.horizontal_resolution_0;
}

void VideoResolutionRegister::set_horizontal_resolution_1(bool new_value)
{
	if (bits.horizontal_resolution_1 != new_value)
	{
		bits.horizontal_resolution_1 = new_value;
		emit_signal("horizontal_resolution_1_toggled", bits.horizontal_resolution_1);
		emit_signal("horizontal_resolution_set", get_horizontal_resolution());
		emit_signal("register_set", byte);
	}
}

bool VideoResolutionRegister::get_horizontal_resolution_1()
{
	return bits.horizontal_resolution_1;
}

void VideoResolutionRegister::set_horizontal_resolution_2(bool new_value)
{
	if (bits.horizontal_resolution_2 != new_value)
	{
		bits.horizontal_resolution_2 = new_value;
		emit_signal("horizontal_resolution_2_toggled", bits.horizontal_resolution_2);
		emit_signal("horizontal_resolution_set", get_horizontal_resolution());
		emit_signal("register_set", byte);
	}
}

bool VideoResolutionRegister::get_horizontal_resolution_2()
{
	return bits.horizontal_resolution_2;
}

void VideoResolutionRegister::set_horizontal_resolution(std::uint8_t new_value)
{
	if (get_horizontal_resolution() != new_value)
	{
		bool old_horizontal_resolution_0 = bits.horizontal_resolution_0;
		bool old_horizontal_resolution_1 = bits.horizontal_resolution_1;
		bool old_horizontal_resolution_2 = bits.horizontal_resolution_2;

		byte = (byte & 0xE3) | (new_value & 0x1C);

		if (old_horizontal_resolution_0 != bits.horizontal_resolution_0)
		{
			emit_signal("horizontal_resolution_0_toggled", bits.horizontal_resolution_0);
		}

		if (old_horizontal_resolution_1 != bits.horizontal_resolution_1)
		{
			emit_signal("horizontal_resolution_1_toggled", bits.horizontal_resolution_1);
		}

		if (old_horizontal_resolution_2 != bits.horizontal_resolution_2)
		{
			emit_signal("horizontal_resolution_2_toggled", bits.horizontal_resolution_2);
		}

		emit_signal("horizontal_resolution_set", get_horizontal_resolution());
		emit_signal("register_set", byte);
	}
}

std::uint8_t VideoResolutionRegister::get_horizontal_resolution()
{
	return byte & 0x1C;
}

void VideoResolutionRegister::set_resolution(std::uint8_t new_value)
{
	if ((byte & 0x1F) != new_value)
	{
		bool old_color_resolution_0 = bits.color_resolution_0;
		bool old_color_resolution_1 = bits.color_resolution_1;
		bool old_horizontal_resolution_0 = bits.horizontal_resolution_0;
		bool old_horizontal_resolution_1 = bits.horizontal_resolution_1;
		bool old_horizontal_resolution_2 = bits.horizontal_resolution_2;

		byte = (byte & 0xE0) | (new_value & 0x1F);

		if (old_color_resolution_0 != bits.color_resolution_0)
		{
			emit_signal("color_resolution_0_toggled", bits.color_resolution_0);
		}

		if (old_color_resolution_1 != bits.color_resolution_1)
		{
			emit_signal("color_resolution_1_toggled", bits.color_resolution_1);
		}

		emit_signal("color_resolution_set", get_color_resolution());

		if (old_horizontal_resolution_0 != bits.horizontal_resolution_0)
		{
			emit_signal("horizontal_resolution_0_toggled", bits.horizontal_resolution_0);
		}

		if (old_horizontal_resolution_1 != bits.horizontal_resolution_1)
		{
			emit_signal("horizontal_resolution_1_toggled", bits.horizontal_resolution_1);
		}

		if (old_horizontal_resolution_2 != bits.horizontal_resolution_2)
		{
			emit_signal("horizontal_resolution_2_toggled", bits.horizontal_resolution_2);
		}

		emit_signal("horizontal_resolution_set", get_horizontal_resolution());
		emit_signal("register_set", byte);
	}
}

std::uint8_t VideoResolutionRegister::get_resolution()
{
	return byte & 0x1F;
}

void VideoResolutionRegister::set_lines_per_field_0(bool new_value)
{
	if (bits.lines_per_field_0 != new_value)
	{
		bits.lines_per_field_0 = new_value;
		emit_signal("lines_per_field_0_toggled", bits.lines_per_field_0);
		emit_signal("lines_per_field_set", get_lines_per_field());
		emit_signal("register_set", byte);
	}
}

bool VideoResolutionRegister::get_lines_per_field_0()
{
	return bits.lines_per_field_0;
}

void VideoResolutionRegister::set_lines_per_field_1(bool new_value)
{
	if (bits.lines_per_field_1 != new_value)
	{
		bits.lines_per_field_1 = new_value;
		emit_signal("lines_per_field_1_toggled", bits.lines_per_field_1);
		emit_signal("lines_per_field_set", get_lines_per_field());
		emit_signal("register_set", byte);
	}
}

bool VideoResolutionRegister::get_lines_per_field_1()
{
	return bits.lines_per_field_1;
}

void VideoResolutionRegister::set_lines_per_field(std::uint8_t new_value)
{
	if ((byte & 0x1F) != new_value)
	{
		bool old_lines_per_field_0 = bits.lines_per_field_0;
		bool old_lines_per_field_1 = bits.lines_per_field_1;

		byte = (byte & 0x1F) | (new_value & 0xE0);

		if (old_lines_per_field_0 != bits.lines_per_field_0)
		{
			emit_signal("lines_per_field_0_toggled", bits.lines_per_field_0);
		}

		if (old_lines_per_field_1 != bits.lines_per_field_1)
		{
			emit_signal("lines_per_field_1_toggled", bits.lines_per_field_1);
		}

		emit_signal("lines_per_field_set", get_lines_per_field());
		emit_signal("register_set", byte);
	}
}

std::uint8_t VideoResolutionRegister::get_lines_per_field()
{
	return byte & 0xE0;
}
