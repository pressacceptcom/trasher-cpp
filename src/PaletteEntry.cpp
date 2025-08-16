#include "PaletteEntry.hpp"

using namespace godot;

PaletteEntry::PaletteEntry() : byte{0}
{
}

PaletteEntry::~PaletteEntry()
{
}

void PaletteEntry::_register_methods()
{
	// Methods
	register_method("set_bit", &PaletteEntry::set_bit);
	register_method("get_bit", &PaletteEntry::get_bit);

	// Properties
	register_property<PaletteEntry, std::uint8_t>("register", &PaletteEntry::set_register, &PaletteEntry::get_register, 0);
	register_property<PaletteEntry, bool>("blue_0", &PaletteEntry::set_blue_0, &PaletteEntry::get_blue_0, false);
	register_property<PaletteEntry, bool>("blue_1", &PaletteEntry::set_blue_1, &PaletteEntry::get_blue_1, false);
	register_property<PaletteEntry, std::uint8_t>("blue", &PaletteEntry::set_blue, &PaletteEntry::get_blue, 0);
	register_property<PaletteEntry, bool>("green_0", &PaletteEntry::set_green_0, &PaletteEntry::get_green_0, false);
	register_property<PaletteEntry, bool>("green_1", &PaletteEntry::set_green_1, &PaletteEntry::get_green_1, false);
	register_property<PaletteEntry, std::uint8_t>("green", &PaletteEntry::set_green, &PaletteEntry::get_green, 0);
	register_property<PaletteEntry, bool>("red_0", &PaletteEntry::set_red_0, &PaletteEntry::get_red_0, false);
	register_property<PaletteEntry, bool>("red_1", &PaletteEntry::set_red_1, &PaletteEntry::get_red_1, false);
	register_property<PaletteEntry, std::uint8_t>("red", &PaletteEntry::set_red, &PaletteEntry::get_red, 0);
	register_property<PaletteEntry, bool>("phase_0", &PaletteEntry::set_phase_0, &PaletteEntry::get_phase_0, false);
	register_property<PaletteEntry, bool>("phase_1", &PaletteEntry::set_phase_1, &PaletteEntry::get_phase_1, false);
	register_property<PaletteEntry, bool>("phase_2", &PaletteEntry::set_phase_2, &PaletteEntry::get_phase_2, false);
	register_property<PaletteEntry, bool>("phase_3", &PaletteEntry::set_phase_3, &PaletteEntry::get_phase_3, false);
	register_property<PaletteEntry, std::uint8_t>("phase", &PaletteEntry::set_phase, &PaletteEntry::get_phase, 0);
	register_property<PaletteEntry, bool>("intensity_0", &PaletteEntry::set_intensity_0, &PaletteEntry::get_intensity_0, false);
	register_property<PaletteEntry, bool>("intensity_1", &PaletteEntry::set_intensity_1, &PaletteEntry::get_intensity_1, false);
	register_property<PaletteEntry, std::uint8_t>("intensity", &PaletteEntry::set_intensity, &PaletteEntry::get_intensity, 0);

	// Signals
	register_signal<PaletteEntry>("register_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<PaletteEntry>("blue_0_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<PaletteEntry>("blue_1_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<PaletteEntry>("blue_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<PaletteEntry>("green_0_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<PaletteEntry>("green_1_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<PaletteEntry>("green_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<PaletteEntry>("red_0_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<PaletteEntry>("red_1_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<PaletteEntry>("red_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<PaletteEntry>("phase_0_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<PaletteEntry>("phase_1_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<PaletteEntry>("phase_2_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<PaletteEntry>("phase_3_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<PaletteEntry>("phase_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<PaletteEntry>("intensity_0_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<PaletteEntry>("intensity_1_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<PaletteEntry>("intensity_set", "new_value", GODOT_VARIANT_TYPE_INT);
}

void PaletteEntry::_init()
{
}

void PaletteEntry::set_register(std::uint8_t new_value)
{
	if (byte != new_value)
	{
		if (!debug)
		{
			byte = new_value;
		}
		else
		{
			bool old_blue_0 = rgb.blue_0;
			bool old_blue_1 = rgb.blue_1;
			std::uint8_t old_blue = get_blue();
			bool old_green_0 = rgb.green_0;
			bool old_green_1 = rgb.green_1;
			std::uint8_t old_green = get_green();
			bool old_red_0 = rgb.red_0;
			bool old_red_1 = rgb.red_1;
			std::uint8_t old_red = get_red();

			bool old_phase_0 = composite.phase_0;
			bool old_phase_1 = composite.phase_1;
			bool old_phase_2 = composite.phase_2;
			bool old_phase_3 = composite.phase_3;
			std::uint8_t old_phase = get_phase();
			bool old_intensity_0 = composite.intensity_0;
			bool old_intensity_1 = composite.intensity_1;
			std::uint8_t old_intensity = get_intensity();

			byte = new_value;

			if (old_blue != get_blue())
			{
				if (old_blue_0 != rgb.blue_0)
				{
					emit_signal("blue_0_toggled", rgb.blue_0);
				}

				if (old_blue_1 != rgb.blue_1)
				{
					emit_signal("blue_1_toggled", rgb.blue_1);
				}

				emit_signal("blue_set", get_blue());
			}

			if (old_green != get_green())
			{
				if (old_green_0 != rgb.green_0)
				{
					emit_signal("green_0_toggled", rgb.green_0);
				}

				if (old_green_1 != rgb.green_1)
				{
					emit_signal("green_1_toggled", rgb.green_1);
				}

				emit_signal("green_set", get_green());
			}

			if (old_red != get_red())
			{
				if (old_red_0 != rgb.red_0)
				{
					emit_signal("red_0_toggled", rgb.red_0);
				}

				if (old_red_1 != rgb.red_1)
				{
					emit_signal("red_1_toggled", rgb.red_1);
				}

				emit_signal("red_set", get_red());
			}

			if (old_phase != get_phase())
			{
				if (old_phase_0 != composite.phase_0)
				{
					emit_signal("phase_0_toggled", composite.phase_0);
				}

				if (old_phase_1 != composite.phase_1)
				{
					emit_signal("phase_1_toggled", composite.phase_1);
				}

				if (old_phase_2 != composite.phase_2)
				{
					emit_signal("phase_2_toggled", composite.phase_1);
				}

				if (old_phase_3 != composite.phase_3)
				{
					emit_signal("phase_3_toggled", composite.phase_1);
				}

				emit_signal("phase_set", get_phase());
			}

			if (old_intensity != get_intensity())
			{
				if (old_intensity_0 != composite.intensity_0)
				{
					emit_signal("intensity_0_toggled", composite.intensity_0);
				}

				if (old_intensity_1 != composite.intensity_1)
				{
					emit_signal("intensity_1_toggled", composite.intensity_1);
				}

				emit_signal("intensity_set", get_intensity());
			}

			emit_signal("register_set", byte);
		}
	}
}

std::uint8_t PaletteEntry::get_register()
{
	return byte;
}

void PaletteEntry::set_bit(int bit, bool new_value)
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

bool PaletteEntry::get_bit(int bit)
{
	switch (bit)
	{

	case Bits::BLUE_0:
		return rgb.blue_0;

	case Bits::GREEN_0:
		return rgb.green_0;

	case Bits::RED_0:
		return rgb.red_0;

	case Bits::BLUE_1:
		return rgb.blue_1;

	case Bits::GREEN_1:
		return rgb.green_1;

	case Bits::RED_1:
		return rgb.red_1;
	}

	return false;
}

void PaletteEntry::set_blue_0(bool new_value)
{
	if (rgb.blue_0 != new_value)
	{
		rgb.blue_0 = new_value;
		if (debug)
		{
			emit_signal("blue_0_toggled", new_value);
			emit_signal("phase_0_toggled", new_value);
			emit_signal("blue_set", get_blue());
			emit_signal("phase_set", get_phase());
			emit_signal("register_set", byte);
		}
	}
}

bool PaletteEntry::get_blue_0()
{
	return rgb.blue_0;
}

void PaletteEntry::set_blue_1(bool new_value)
{
	if (rgb.blue_1 != new_value)
	{
		rgb.blue_1 = new_value;
		if (debug)
		{
			emit_signal("blue_1_toggled", new_value);
			emit_signal("phase_3_toggled", new_value);
			emit_signal("blue_set", get_blue());
			emit_signal("phase_set", get_phase());
			emit_signal("register_set", byte);
		}
	}
}

bool PaletteEntry::get_blue_1()
{
	return rgb.blue_1;
}

void PaletteEntry::set_blue(std::uint8_t new_value)
{
	if (get_blue() != new_value)
	{
		if (!debug)
		{
			byte = (byte & (255 - static_cast<std::uint8_t>(Blue::BLUE_3))) |
				   (new_value & static_cast<std::uint8_t>(Blue::BLUE_3));
		}
		else
		{
			bool old_blue_0 = rgb.blue_0;
			bool old_blue_1 = rgb.blue_1;

			byte = (byte & (255 - static_cast<std::uint8_t>(Blue::BLUE_3))) |
				   (new_value & static_cast<std::uint8_t>(Blue::BLUE_3));

			if (old_blue_0 != rgb.blue_0)
			{
				emit_signal("blue_0_toggled", rgb.blue_0);
				emit_signal("phase_0_toggled", composite.phase_0);
			}

			if (old_blue_1 != rgb.blue_1)
			{
				emit_signal("blue_1_toggled", rgb.blue_1);
				emit_signal("phase_3_toggled", composite.phase_1);
			}

			emit_signal("blue_set", get_blue());
			emit_signal("phase_set", get_phase());
			emit_signal("register_set", byte);
		}
	}
}

std::uint8_t PaletteEntry::get_blue()
{
	return byte & static_cast<std::uint8_t>(Blue::BLUE_3);
}

void PaletteEntry::set_green_0(bool new_value)
{
	if (rgb.green_0 != new_value)
	{
		rgb.green_0 = new_value;
		if (debug)
		{
			emit_signal("green_0_toggled", new_value);
			emit_signal("phase_1_toggled", new_value);
			emit_signal("green_set", get_green());
			emit_signal("phase_set", get_phase());
			emit_signal("register_set", byte);
		}
	}
}

bool PaletteEntry::get_green_0()
{
	return rgb.green_0;
}

void PaletteEntry::set_green_1(bool new_value)
{
	if (rgb.green_1 != new_value)
	{
		rgb.green_1 = new_value;
		if (debug)
		{
			emit_signal("green_1_toggled", new_value);
			emit_signal("intensity_0_toggled", new_value);
			emit_signal("green_set", get_green());
			emit_signal("intensity_set", get_intensity());
			emit_signal("register_set", byte);
		}
	}
}

bool PaletteEntry::get_green_1()
{
	return rgb.green_1;
}

void PaletteEntry::set_green(std::uint8_t new_value)
{
	if (get_green() != new_value)
	{
		if (!debug)
		{
			byte = (byte & (255 - static_cast<std::uint8_t>(Green::GREEN_3))) |
				   (new_value & static_cast<std::uint8_t>(Green::GREEN_3));
		}
		else
		{
			bool old_green_0 = rgb.green_0;
			bool old_green_1 = rgb.green_1;

			byte = (byte & (255 - static_cast<std::uint8_t>(Green::GREEN_3))) |
				   (new_value & static_cast<std::uint8_t>(Green::GREEN_3));

			if (old_green_0 != rgb.green_0)
			{
				emit_signal("green_0_toggled", rgb.green_0);
				emit_signal("phase_1_toggled", composite.phase_1);
				emit_signal("phase_set", get_phase());
			}

			if (old_green_1 != rgb.green_1)
			{
				emit_signal("green_1_toggled", rgb.green_1);
				emit_signal("intensity_0_toggled", composite.intensity_0);
				emit_signal("intensity_set", get_intensity());
			}

			emit_signal("green_set", get_green());
			emit_signal("register_set", byte);
		}
	}
}

std::uint8_t PaletteEntry::get_green()
{
	return byte & static_cast<std::uint8_t>(Green::GREEN_3);
}

void PaletteEntry::set_red_0(bool new_value)
{
	if (rgb.red_0 != new_value)
	{
		rgb.red_0 = new_value;
		if (debug)
		{
			emit_signal("red_0_toggled", new_value);
			emit_signal("phase_2_toggled", new_value);
			emit_signal("red_set", get_red());
			emit_signal("phase_set", get_phase());
			emit_signal("register_set", byte);
		}
	}
}

bool PaletteEntry::get_red_0()
{
	return rgb.red_0;
}

void PaletteEntry::set_red_1(bool new_value)
{
	if (rgb.red_1 != new_value)
	{
		rgb.red_1 = new_value;
		if (debug)
		{
			emit_signal("red_1_toggled", new_value);
			emit_signal("intensity_1_toggled", new_value);
			emit_signal("red_set", get_red());
			emit_signal("intensity_set", get_intensity());
			emit_signal("register_set", byte);
		}
	}
}

bool PaletteEntry::get_red_1()
{
	return rgb.red_1;
}

void PaletteEntry::set_red(std::uint8_t new_value)
{
	if (get_red() != new_value)
	{
		if (!debug)
		{
			byte = (byte & (255 - static_cast<std::uint8_t>(Red::RED_3))) |
				   (new_value & static_cast<std::uint8_t>(Red::RED_3));
		}
		else
		{
			bool old_red_0 = rgb.red_0;
			bool old_red_1 = rgb.red_1;

			byte = (byte & (255 - static_cast<std::uint8_t>(Red::RED_3))) |
				   (new_value & static_cast<std::uint8_t>(Red::RED_3));

			if (old_red_0 != rgb.red_0)
			{
				emit_signal("red_0_toggled", rgb.red_0);
				emit_signal("phase_2_toggled", composite.phase_2);
				emit_signal("phase_set", get_phase());
			}

			if (old_red_1 != rgb.red_1)
			{
				emit_signal("red_1_toggled", rgb.red_1);
				emit_signal("intensity_1_toggled", composite.intensity_1);
				emit_signal("intensity_set", get_intensity());
			}

			emit_signal("red_set", get_red());
			emit_signal("register_set", byte);
		}
	}
}

std::uint8_t PaletteEntry::get_red()
{
	return byte & static_cast<std::uint8_t>(Red::RED_3);
}

void PaletteEntry::set_phase_0(bool new_value)
{
	if (composite.phase_0 != new_value)
	{
		composite.phase_0 = new_value;
		if (debug)
		{
			emit_signal("phase_0_toggled", new_value);
			emit_signal("blue_0_toggled", new_value);
			emit_signal("phase_set", get_phase());
			emit_signal("blue_set", get_blue());
			emit_signal("register_set", byte);
		}
	}
}

bool PaletteEntry::get_phase_0()
{
	return composite.phase_0;
}

void PaletteEntry::set_phase_1(bool new_value)
{
	if (composite.phase_1 != new_value)
	{
		composite.phase_1 = new_value;
		if (debug)
		{
			emit_signal("phase_1_toggled", new_value);
			emit_signal("green_0_toggled", new_value);
			emit_signal("phase_set", get_phase());
			emit_signal("green_set", get_green());
			emit_signal("register_set", byte);
		}
	}
}

bool PaletteEntry::get_phase_1()
{
	return composite.phase_1;
}

void PaletteEntry::set_phase_2(bool new_value)
{
	if (composite.phase_2 != new_value)
	{
		composite.phase_2 = new_value;
		if (debug)
		{
			emit_signal("phase_2_toggled", new_value);
			emit_signal("red_0_toggled", new_value);
			emit_signal("phase_set", get_phase());
			emit_signal("red_set", get_red());
			emit_signal("register_set", byte);
		}
	}
}

bool PaletteEntry::get_phase_2()
{
	return composite.phase_2;
}

void PaletteEntry::set_phase_3(bool new_value)
{
	if (composite.phase_3 != new_value)
	{
		composite.phase_3 = new_value;
		if (debug)
		{
			emit_signal("phase_3_toggled", new_value);
			emit_signal("blue_1_toggled", new_value);
			emit_signal("phase_set", get_phase());
			emit_signal("blue_set", get_blue());
			emit_signal("register_set", byte);
		}
	}
}

bool PaletteEntry::get_phase_3()
{
	return composite.phase_3;
}

void PaletteEntry::set_phase(std::uint8_t new_value)
{
	if (get_phase() != new_value)
	{
		if (!debug)
		{
			byte = (byte & (255 - PHASE_MASK)) | (new_value & PHASE_MASK);
		}
		else
		{
			bool old_phase_0 = composite.phase_0;
			bool old_phase_1 = composite.phase_1;
			bool old_phase_2 = composite.phase_2;
			bool old_phase_3 = composite.phase_3;

			byte = (byte & (255 - PHASE_MASK)) | (new_value & PHASE_MASK);

			if (old_phase_0 != composite.phase_0 || old_phase_3 != composite.phase_3)
			{
				if (old_phase_0 != composite.phase_0)
				{
					emit_signal("phase_0_toggled", composite.phase_0);
					emit_signal("blue_0_toggled", rgb.blue_0);
				}

				if (old_phase_3 != composite.phase_3)
				{
					emit_signal("phase_3_toggled", composite.phase_3);
					emit_signal("blue_1_toggled", rgb.blue_1);
				}

				emit_signal("blue_set", get_blue());
			}

			if (old_phase_1 != composite.phase_1)
			{
				emit_signal("phase_1_toggled", composite.phase_1);
				emit_signal("green_0_toggled", rgb.green_0);
				emit_signal("green_set", get_green());
			}

			if (old_phase_2 != composite.phase_2)
			{
				emit_signal("phase_2_toggled", composite.phase_2);
				emit_signal("red_0_toggled", rgb.red_0);
				emit_signal("red_set", get_red());
			}

			emit_signal("phase_set", get_phase());
			emit_signal("register_set", byte);
		}
	}
}

std::uint8_t PaletteEntry::get_phase()
{
	return byte & PHASE_MASK;
}

void PaletteEntry::set_intensity_0(bool new_value)
{
	if (composite.intensity_0 != new_value)
	{
		composite.intensity_0 = new_value;
		if (debug)
		{
			emit_signal("intensity_0_toggled", new_value);
			emit_signal("green_1_toggled", new_value);
			emit_signal("intensity_set", get_intensity());
			emit_signal("green_set", get_green());
			emit_signal("register_set", byte);
		}
	}
}

bool PaletteEntry::get_intensity_0()
{
	return composite.intensity_0;
}

void PaletteEntry::set_intensity_1(bool new_value)
{
	if (composite.intensity_1 != new_value)
	{
		composite.intensity_1 = new_value;
		if (debug)
		{
			emit_signal("intensity_1_toggled", new_value);
			emit_signal("red_1_toggled", new_value);
			emit_signal("intensity_set", get_intensity());
			emit_signal("red_set", get_red());
			emit_signal("register_set", byte);
		}
	}
}

bool PaletteEntry::get_intensity_1()
{
	return composite.intensity_1;
}

void PaletteEntry::set_intensity(std::uint8_t new_value)
{
	if (get_intensity() != new_value)
	{
		if (!debug)
		{
			byte = (byte & (255 - INTENSITY_MASK)) | (new_value & INTENSITY_MASK);
		}
		else
		{
			bool old_intensity_0 = composite.intensity_0;
			bool old_intensity_1 = composite.intensity_1;

			byte = (byte & (255 - INTENSITY_MASK)) | (new_value & INTENSITY_MASK);

			if (old_intensity_0 != composite.intensity_0)
			{
				emit_signal("intensity_0_toggled", composite.intensity_0);
				emit_signal("green_1_toggled", rgb.green_1);
				emit_signal("green_set", get_green());
			}

			if (old_intensity_1 != composite.intensity_1)
			{
				emit_signal("intensity_1_toggled", composite.intensity_1);
				emit_signal("red_1_toggled", rgb.red_1);
				emit_signal("red_set", get_red());
			}

			emit_signal("intensity_set", get_intensity());
			emit_signal("register_set", byte);
		}
	}
}

std::uint8_t PaletteEntry::get_intensity()
{
	return byte & INTENSITY_MASK;
}
