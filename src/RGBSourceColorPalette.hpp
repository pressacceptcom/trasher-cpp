#ifndef RGBSOURCECOLORPALETTE_H
#define RGBSOURCECOLORPALETTE_H

#include <cstdint>
#include <vector>

#include <Godot.hpp>
#include <Reference.hpp>
#include <Color.hpp>
#include <Variant.hpp>

namespace godot
{
	class RGBSourceColorPalette : public Reference
	{
		GODOT_CLASS(RGBSourceColorPalette, Reference)

	private:
	public:
		static const std::uint8_t MAX_SIZE = 64;
		static const float constexpr COLOR_INTENSITIES[] = {0, 0.33, 0.66, 0.99};

		std::vector<Color> colors;
		std::vector<std::uint32_t> colors_data;

		// C++ Specific Definitions

		RGBSourceColorPalette();

		~RGBSourceColorPalette();

		// Godot Specific Definitions

		static void _register_methods();

		void _init(); // our initializer called by Godot

		// Class Definitions

		void set_color(std::uint8_t index, Variant new_value);
		Color get_color(std::uint8_t index);
	};
}

#endif