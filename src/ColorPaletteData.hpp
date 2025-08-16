#ifndef COLORPALETTEDATA_H
#define COLORPALETTEDATA_H

#include <cstdint>
#include <vector>

#include <Godot.hpp>
#include <Reference.hpp>
#include <Ref.hpp>

#include "Monitor.hpp"
#include "ColorPalette.hpp"

namespace godot
{
	class ColorPaletteData : public Reference
	{
		GODOT_CLASS(ColorPaletteData, Reference)

	private:
	public:
		Ref<Monitor> monitor;
		Ref<ColorPalette> color_palette;

		std::vector<std::uint32_t> data;

		// C++ Specific Definitions

		ColorPaletteData();

		~ColorPaletteData();

		// Godot Specific Definitions

		static void _register_methods();

		void _init(); // our initializer called by Godot

		void _on_color_palette_palette_entry_changed(std::uint8_t index, std::uint8_t new_value);

		// Class Definitions

		Ref<Monitor> get_monitor();
		Ref<ColorPalette> get_color_palette();

		void set_monitor(Ref<Monitor> new_value);

		void set_color_palette(Ref<ColorPalette> new_value);
	};
}

#endif