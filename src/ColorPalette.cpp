#include "ColorPalette.hpp"

using namespace godot;

ColorPalette::ColorPalette()
{
}

ColorPalette::~ColorPalette()
{
}

void ColorPalette::_register_methods()
{
	// Methods
	register_method("set_palette_entry", &ColorPalette::set_palette_entry);
	register_method("get_palette_entry", &ColorPalette::get_palette_entry);
	register_method("get_palette_entry_reference", &ColorPalette::get_palette_entry_reference);

	// Signals
	register_signal<ColorPalette>("palette_entry_changed", "index", GODOT_VARIANT_TYPE_INT, "new_value", GODOT_VARIANT_TYPE_INT);
}

void ColorPalette::_init()
{
	palette.resize(MAX_SIZE);

	for (int i = 0; i < MAX_SIZE; i++)
	{
		palette[i] = Ref<PaletteEntry>(PaletteEntry::_new());
	}
}

void ColorPalette::set_palette_entry(std::uint8_t index, std::uint8_t new_value)
{
	if (index >= MAX_SIZE)
	{
		return;
	}

	if (palette[index]->byte != new_value)
	{
		palette[index]->set_register(new_value);
		emit_signal("palette_entry_changed", index, new_value);
	}
}

std::uint8_t ColorPalette::get_palette_entry(std::uint8_t index)
{
	if (index >= MAX_SIZE)
	{
		return palette[0]->byte;
	}

	return palette[index]->byte;
}

Ref<PaletteEntry> ColorPalette::get_palette_entry_reference(std::uint8_t index)
{
	if (index >= MAX_SIZE)
	{
		return palette[0];
	}

	return palette[index];
}
