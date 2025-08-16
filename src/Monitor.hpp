/**
 * Monitors
 *
 *     The standard (NTSC) Color Computer 3 can interface with RGB and Composite signals. The color
 * output signals (mappable to a 64 value 6 bit space) have different interpretations depending on
 * whether it's an RGB monitor, or a standard Composite monitor. This object keeps track of which
 * monitor is being used, and returns the appropriate color through its interface.
 */

#ifndef MONITOR_H
#define MONITOR_H

#include <Godot.hpp>
#include <Reference.hpp>
#include <Ref.hpp>
#include <Color.hpp>

#include "RGBSourceColorPalette.hpp"
#include "CompositeSourceColorPalette.hpp"

namespace godot
{
	class Monitor : public Reference
	{
		GODOT_CLASS(Monitor, Reference)

	private:
	public:
		enum Type
		{
			RGB,
			COMPOSITE
		};

		int type = Type::RGB;

		Ref<RGBSourceColorPalette> rgb_palette;
		Ref<CompositeSourceColorPalette> composite_palette;

		// C++ Specific Definitions

		Monitor();

		~Monitor();

		// Godot Specific Definitions

		static void _register_methods();

		void _init(); // our initializer called by Godot

		// Class Definitions

		void set_type(int new_type);
		int get_type();

		Ref<RGBSourceColorPalette> get_rgb_palette();

		Ref<CompositeSourceColorPalette> get_composite_palette();

		Color get_source_palette_color(std::uint8_t index);

		std::uint32_t get_source_palette_color_data(std::uint8_t index);
	};
}

#endif