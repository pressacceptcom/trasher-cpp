#include "VideoModeRegister.hpp"

using namespace godot;

VideoModeRegister::VideoModeRegister() : byte{0}
{
}

VideoModeRegister::~VideoModeRegister()
{
}

void VideoModeRegister::_register_methods()
{
	// Methods
	register_method("set_bit", &VideoModeRegister::set_bit);
	register_method("get_bit", &VideoModeRegister::get_bit);

	// Properties
	register_property<VideoModeRegister, std::uint8_t>("register", &VideoModeRegister::set_register, &VideoModeRegister::get_register, 0);
	register_property<VideoModeRegister, bool>("lines_per_row_0", &VideoModeRegister::set_lines_per_row_0, &VideoModeRegister::get_lines_per_row_0, false);
	register_property<VideoModeRegister, bool>("lines_per_row_1", &VideoModeRegister::set_lines_per_row_1, &VideoModeRegister::get_lines_per_row_1, false);
	register_property<VideoModeRegister, bool>("lines_per_row_2", &VideoModeRegister::set_lines_per_row_2, &VideoModeRegister::get_lines_per_row_2, false);
	register_property<VideoModeRegister, std::uint8_t>("lines_per_row", &VideoModeRegister::set_lines_per_row, &VideoModeRegister::get_lines_per_row, 0);
	register_property<VideoModeRegister, bool>("fifty_hertz_vertical_sync", &VideoModeRegister::set_fifty_hertz_vertical_sync, &VideoModeRegister::get_fifty_hertz_vertical_sync, false);
	register_property<VideoModeRegister, bool>("monochrome", &VideoModeRegister::set_monochrome, &VideoModeRegister::get_monochrome, false);
	register_property<VideoModeRegister, bool>("burst_phase_inverted", &VideoModeRegister::set_burst_phase_inverted, &VideoModeRegister::get_burst_phase_inverted, false);
	register_property<VideoModeRegister, bool>("bit_plane", &VideoModeRegister::set_bit_plane, &VideoModeRegister::get_bit_plane, false);

	// Signals
	register_signal<VideoModeRegister>("register_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<VideoModeRegister>("lines_per_row_0_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<VideoModeRegister>("lines_per_row_1_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<VideoModeRegister>("lines_per_row_2_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<VideoModeRegister>("lines_per_row_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<VideoModeRegister>("fifty_hertz_vertical_sync_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<VideoModeRegister>("monochrome_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<VideoModeRegister>("burst_phase_inverted_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<VideoModeRegister>("bit_plane_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
}

void VideoModeRegister::_init()
{
	// initialize any variables here
}

void VideoModeRegister::set_register(std::uint8_t new_value)
{
	if (byte != new_value)
	{
		bool old_lines_per_row_0 = bits.lines_per_row_0;
		bool old_lines_per_row_1 = bits.lines_per_row_1;
		bool old_lines_per_row_2 = bits.lines_per_row_2;
		std::uint8_t old_lines_per_row = get_lines_per_row();
		bool old_fifty_hertz_vertical_sync = bits.fifty_hertz_vertical_sync;
		bool old_monochrome = bits.monochrome;
		bool old_burst_phase_inverted = bits.burst_phase_inverted;
		bool old_bit_plane = bits.bit_plane;

		byte = new_value;

		if (old_lines_per_row != get_lines_per_row())
		{
			if (old_lines_per_row_0 != bits.lines_per_row_0)
			{
				emit_signal("lines_per_row_0_toggled", bits.lines_per_row_0);
			}

			if (old_lines_per_row_1 != bits.lines_per_row_1)
			{
				emit_signal("lines_per_row_1_toggled", bits.lines_per_row_1);
			}

			if (old_lines_per_row_2 != bits.lines_per_row_2)
			{
				emit_signal("lines_per_row_2_toggled", bits.lines_per_row_2);
			}

			emit_signal("lines_per_row_set", get_lines_per_row());
		}

		if (old_fifty_hertz_vertical_sync != bits.fifty_hertz_vertical_sync)
		{
			emit_signal("fifty_hertz_vertical_sync_toggled", bits.fifty_hertz_vertical_sync);
		}

		if (old_monochrome != bits.monochrome)
		{
			emit_signal("monochrome_toggled", bits.monochrome);
		}

		if (old_burst_phase_inverted != bits.burst_phase_inverted)
		{
			emit_signal("burst_phase_inverted_toggled", bits.burst_phase_inverted);
		}

		if (old_bit_plane != bits.bit_plane)
		{
			emit_signal("bit_plane_toggled", bits.bit_plane);
		}

		emit_signal("register_set", byte);
	}
}

std::uint8_t VideoModeRegister::get_register()
{
	return byte;
}

void VideoModeRegister::set_bit(int bit, bool new_value)
{
	switch (bit)
	{

	case Bits::LINES_PER_ROW_0:
		set_lines_per_row_0(new_value);
		break;

	case Bits::LINES_PER_ROW_1:
		set_lines_per_row_1(new_value);
		break;

	case Bits::LINES_PER_ROW_2:
		set_lines_per_row_2(new_value);
		break;

	case Bits::FIFTY_HERTZ_VERTICAL_SYNC:
		set_fifty_hertz_vertical_sync(new_value);
		break;

	case Bits::MONOCHROME:
		set_monochrome(new_value);
		break;

	case Bits::BURST_PHASE_INVERTED:
		set_burst_phase_inverted(new_value);
		break;

	case Bits::BIT_PLANE:
		set_bit_plane(new_value);
		break;
	}
}

bool VideoModeRegister::get_bit(int bit)
{
	switch (bit)
	{

	case Bits::LINES_PER_ROW_0:
		return get_lines_per_row_0();

	case Bits::LINES_PER_ROW_1:
		return get_lines_per_row_1();

	case Bits::LINES_PER_ROW_2:
		return get_lines_per_row_2();

	case Bits::FIFTY_HERTZ_VERTICAL_SYNC:
		return get_fifty_hertz_vertical_sync();

	case Bits::MONOCHROME:
		return get_monochrome();

	case Bits::BURST_PHASE_INVERTED:
		return get_burst_phase_inverted();

	case Bits::BIT_PLANE:
		return get_bit_plane();
	}

	return false;
}

void VideoModeRegister::set_lines_per_row_0(bool new_value)
{
	if (bits.lines_per_row_0 != new_value)
	{
		bits.lines_per_row_0 = new_value;
		emit_signal("lines_per_row_0_toggled", bits.lines_per_row_0);
		emit_signal("lines_per_row_set", get_lines_per_row());
		emit_signal("register_set", byte);
	}
}

bool VideoModeRegister::get_lines_per_row_0()
{
	return bits.lines_per_row_0;
}

void VideoModeRegister::set_lines_per_row_1(bool new_value)
{
	if (bits.lines_per_row_1 != new_value)
	{
		bits.lines_per_row_1 = new_value;
		emit_signal("lines_per_row_1_toggled", bits.lines_per_row_1);
		emit_signal("lines_per_row_set", get_lines_per_row());
		emit_signal("register_set", byte);
	}
}

bool VideoModeRegister::get_lines_per_row_1()
{
	return bits.lines_per_row_1;
}

void VideoModeRegister::set_lines_per_row_2(bool new_value)
{
	if (bits.lines_per_row_2 != new_value)
	{
		bits.lines_per_row_2 = new_value;
		emit_signal("lines_per_row_2_toggled", bits.lines_per_row_2);
		emit_signal("lines_per_row_set", get_lines_per_row());
		emit_signal("register_set", byte);
	}
}

bool VideoModeRegister::get_lines_per_row_2()
{
	return bits.lines_per_row_2;
}

void VideoModeRegister::set_lines_per_row(std::uint8_t new_value)
{
	if (get_lines_per_row() != new_value)
	{
		bool old_lines_per_row_0 = bits.lines_per_row_0;
		bool old_lines_per_row_1 = bits.lines_per_row_1;
		bool old_lines_per_row_2 = bits.lines_per_row_2;

		byte = (byte & 0xF8) | (new_value & 0x07);

		if (old_lines_per_row_0 != bits.lines_per_row_0)
		{
			emit_signal("lines_per_row_0_toggled", bits.lines_per_row_0);
		}

		if (old_lines_per_row_1 != bits.lines_per_row_1)
		{
			emit_signal("lines_per_row_1_toggled", bits.lines_per_row_1);
		}

		if (old_lines_per_row_2 != bits.lines_per_row_2)
		{
			emit_signal("lines_per_row_2_toggled", bits.lines_per_row_2);
		}

		emit_signal("lines_per_row_set", get_lines_per_row());
		emit_signal("register_set", byte);
	}
}

std::uint8_t VideoModeRegister::get_lines_per_row()
{
	return byte & 0x07;
}

void VideoModeRegister::set_fifty_hertz_vertical_sync(bool new_value)
{
	if (bits.fifty_hertz_vertical_sync != new_value)
	{
		bits.fifty_hertz_vertical_sync = new_value;
		emit_signal("fifty_hertz_vertical_sync_toggled", new_value);
		emit_signal("register_set", byte);
	}
}

bool VideoModeRegister::get_fifty_hertz_vertical_sync()
{
	return bits.fifty_hertz_vertical_sync;
}

void VideoModeRegister::set_monochrome(bool new_value)
{
	if (bits.monochrome != new_value)
	{
		bits.monochrome = new_value;
		emit_signal("monochrome_toggled", new_value);
		emit_signal("register_set", byte);
	}
}

bool VideoModeRegister::get_monochrome()
{
	return bits.monochrome;
}

void VideoModeRegister::set_burst_phase_inverted(bool new_value)
{
	if (bits.burst_phase_inverted != new_value)
	{
		bits.burst_phase_inverted = new_value;
		emit_signal("burst_phase_inverted_toggled", new_value);
		emit_signal("register_set", byte);
	}
}

bool VideoModeRegister::get_burst_phase_inverted()
{
	return bits.burst_phase_inverted;
}

void VideoModeRegister::set_bit_plane(bool new_value)
{
	if (bits.bit_plane != new_value)
	{
		bits.bit_plane = new_value;
		emit_signal("bit_plane_toggled", new_value);
		emit_signal("register_set", byte);
	}
}

bool VideoModeRegister::get_bit_plane()
{
	return bits.bit_plane;
}
