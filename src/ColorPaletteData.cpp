#include "ColorPaletteData.hpp"

using namespace godot;

ColorPaletteData::ColorPaletteData()
{
	data.resize(ColorPalette::MAX_SIZE);
}

ColorPaletteData::~ColorPaletteData()
{
}

void ColorPaletteData::_register_methods()
{
	// Methods
	register_method("_on_color_palette_palette_entry_changed", &ColorPaletteData::_on_color_palette_palette_entry_changed);

	register_method("get_monitor", &ColorPaletteData::get_monitor);
	register_method("get_color_palette", &ColorPaletteData::get_color_palette);
}

void ColorPaletteData::_init()
{
}

void ColorPaletteData::_on_color_palette_palette_entry_changed(std::uint8_t index, std::uint8_t new_value)
{
	data[index] = monitor->get_source_palette_color_data(new_value);
}

Ref<Monitor> ColorPaletteData::get_monitor()
{
	return monitor;
}

Ref<ColorPalette> ColorPaletteData::get_color_palette()
{
	return color_palette;
}

void ColorPaletteData::set_monitor(Ref<Monitor> new_value)
{
	monitor = new_value;

	if (color_palette.is_valid())
	{
		// get palette entry color data
		for (int i = 0; i < ColorPalette::MAX_SIZE; i++)
		{
			data[i] = monitor->get_source_palette_color_data(color_palette->palette[i]->byte);
		}
	}
}

void ColorPaletteData::set_color_palette(Ref<ColorPalette> new_value)
{
	color_palette = new_value;

	color_palette->connect("palette_entry_changed", this, "_on_color_palette_palette_entry_changed");

	// get palette entry color data
	for (int i = 0; i < ColorPalette::MAX_SIZE; i++)
	{
		data[i] = monitor->get_source_palette_color_data(color_palette->palette[i]->byte);
	}
}
