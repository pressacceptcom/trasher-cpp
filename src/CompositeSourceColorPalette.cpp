#include "CompositeSourceColorPalette.hpp"

using namespace godot;

CompositeSourceColorPalette::CompositeSourceColorPalette()
{
}

CompositeSourceColorPalette::~CompositeSourceColorPalette()
{
}

void CompositeSourceColorPalette::_register_methods()
{
	// Methods
	register_method("set_color", &CompositeSourceColorPalette::set_color);
	register_method("get_color", &CompositeSourceColorPalette::get_color);
}

void CompositeSourceColorPalette::_init()
{
	int counter = 0;
	float angle = 120.0;

	colors.resize(MAX_SIZE);

	colors[counter++] = Color();

	for (int i = 0; i < 15; i++)
	{
		Color color;
		color.set_hsv(angle / 360.0, 1.0, 0.55, 1.0);
		colors[counter++] = color;
		angle -= DEGREES_PER_HUE;
		if (angle < 0)
		{
			angle += 360.0;
		}
	}

	colors[counter++] = Color(0.33, 0.33, 0.33);

	angle = 120.0;
	for (int i = 0; i < 15; i++)
	{
		Color color;
		color.set_hsv(angle / 360.0, .808, 0.816, 1.0);
		colors[counter++] = color;
		angle -= DEGREES_PER_HUE;
		if (angle < 0)
		{
			angle += 360.0;
		}
	}

	colors[counter++] = Color(0.66, 0.66, 0.66);

	for (int i = 0; i < 15; i++)
	{
		Color color;
		color.set_hsv(angle / 360.0, 0.682, 1.0, 1.0);
		colors[counter++] = color;
		angle -= DEGREES_PER_HUE;
		if (angle < 0)
		{
			angle += 360.0;
		}
	}

	colors[counter++] = Color(0.99, 0.99, 0.99);

	for (int i = 0; i < 15; i++)
	{
		Color color;
		color.set_hsv(angle / 360.0, 0.549, 0.992, 1.0);
		colors[counter++] = color;
		angle -= DEGREES_PER_HUE;
		if (angle < 0)
		{
			angle += 360.0;
		}
	}

	colors[counter++] = Color(1.0, 1.0, 1.0);

	colors_data.resize(MAX_SIZE);

	for (int i = 0; i < MAX_SIZE; i++)
	{
		colors_data[i] = colors[i].to_ABGR32();
	}
}

void CompositeSourceColorPalette::set_color(std::uint8_t index, Variant new_value)
{
	Color new_color = new_value;

	if (index >= MAX_SIZE)
	{
		return;
	}

	colors[index] = new_color;
	colors_data[index] = colors[index].to_ABGR32();
}

Color CompositeSourceColorPalette::get_color(std::uint8_t index)
{
	if (index >= MAX_SIZE)
	{
		return Color();
	}

	return colors[index];
}
