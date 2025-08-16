#include "Monitor.hpp"

using namespace godot;

Monitor::Monitor()
{
}

Monitor::~Monitor()
{
}

void Monitor::_register_methods()
{
	// Methods
	register_method("get_rgb_palette", &Monitor::get_rgb_palette);
	register_method("get_composite_palette", &Monitor::get_composite_palette);
	register_method("get_source_palette_color", &Monitor::get_source_palette_color);

	// Properties
	register_property<Monitor, int>("type", &Monitor::set_type, &Monitor::get_type, 0);
}

void Monitor::_init()
{
	rgb_palette = Ref<RGBSourceColorPalette>(RGBSourceColorPalette::_new());
	composite_palette = Ref<CompositeSourceColorPalette>(CompositeSourceColorPalette::_new());
}

void Monitor::set_type(int new_type)
{
	type = new_type;

	if (type != Type::RGB && type != Type::COMPOSITE)
	{
		type = Type::RGB;
	}
}

int Monitor::get_type()
{
	return type;
}

Ref<RGBSourceColorPalette> Monitor::get_rgb_palette()
{
	return rgb_palette;
}

Ref<CompositeSourceColorPalette> Monitor::get_composite_palette()
{
	return composite_palette;
}

Color Monitor::get_source_palette_color(std::uint8_t index)
{
	if (type == Type::RGB)
	{
		return rgb_palette->colors[index];
	}
	else
	{
		return composite_palette->colors[index];
	}
}

std::uint32_t Monitor::get_source_palette_color_data(std::uint8_t index)
{
	if (type == Type::RGB)
	{
		return rgb_palette->colors_data[index];
	}
	else
	{
		return composite_palette->colors_data[index];
	}
}
