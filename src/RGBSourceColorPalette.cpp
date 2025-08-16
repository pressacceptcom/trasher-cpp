#include "RGBSourceColorPalette.hpp"

#include "PaletteEntry.hpp"

using namespace godot;

RGBSourceColorPalette::RGBSourceColorPalette()
{
}

RGBSourceColorPalette::~RGBSourceColorPalette()
{
}

void RGBSourceColorPalette::_register_methods()
{
	// Methods
	register_method("set_color", &RGBSourceColorPalette::set_color);
	register_method("get_color", &RGBSourceColorPalette::get_color);
}

void RGBSourceColorPalette::_init()
{
	float red = 0.0;
	float green = 0.0;
	float blue = 0.0;

	colors.resize(MAX_SIZE);

	for (int i = 0; i < MAX_SIZE; i++)
	{
		red = COLOR_INTENSITIES[((i & PaletteEntry::Bits::RED_1) >> 4) | ((i & PaletteEntry::Bits::RED_0) >> 2)];
		green = COLOR_INTENSITIES[((i & PaletteEntry::Bits::GREEN_1) >> 3) | ((i & PaletteEntry::Bits::GREEN_0) >> 1)];
		blue = COLOR_INTENSITIES[((i & PaletteEntry::Bits::BLUE_1) >> 2) | (i & PaletteEntry::Bits::BLUE_0)];
		colors[i] = Color(red, green, blue);
	}

	colors_data.resize(MAX_SIZE);

	for (int i = 0; i < MAX_SIZE; i++)
	{
		colors_data[i] = colors[i].to_ABGR32();
	}
}

void RGBSourceColorPalette::set_color(std::uint8_t index, Variant new_value)
{
	Color new_color = new_value;

	if (index >= MAX_SIZE)
	{
		return;
	}

	colors[index] = new_color;
}

Color RGBSourceColorPalette::get_color(std::uint8_t index)
{
	if (index >= MAX_SIZE)
	{
		return Color();
	}

	return colors[index];
}
