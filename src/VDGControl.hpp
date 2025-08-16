/**
 * FF22:
 *     BIT 0 = RS-232C DATA INPUT
 *     BIT 1 = SINGLE BIT SOUND OUTPUT
 *     BIT 2 = RAM SIZE INPUT
 *     BIT 3 = RGB Monitor Sensing INPUT    CSS
 *     BIT 4 = VDG CONTROL OUTPUT           GM0 & UPPER/LOWER CASE*
 *     BIT 5 = VDG CONTROL OUTPUT           GM1 & INVERT
 *     BIT 6 = VDG CONTROL OUTPUT           GM2
 *     BIT 7 = VDG CONTROL OUTPUT           A* / G
 */

#ifndef VDGCONTROL_H
#define VDGCONTROL_H

#include <cstdint>

#include <Godot.hpp>
#include <Reference.hpp>

namespace godot
{
	class VDGControl : public Reference
	{
		GODOT_CLASS(VDGControl, Reference)

	private:
	public:
		enum Bits
		{
			COLOR_SET_SELECT = 0x08,
			GM_0 = 0x10,
			GM_1 = 0x20,
			GM_2 = 0x40,
			AG = 0x80
		};

		union
		{
			std::uint8_t byte;
			struct
			{
				bool bit_0 : 1;
				bool bit_1 : 1;
				bool bit_2 : 1;
				bool color_set_select : 1;
				bool gm_0 : 1;
				bool gm_1 : 1;
				bool gm_2 : 1;
				bool ag : 1;
			} bits;
		};

		// C++ Specific Definitions

		VDGControl();

		~VDGControl();

		// Godot Specific Definitions

		static void _register_methods();

		void _init(); // our initializer called by Godot

		// Class Definitions

		void set_bit(int bit, bool new_value);
		bool get_bit(int bit);

		void set_register(std::uint8_t new_value);
		std::uint8_t get_register();

		void set_color_set_select(bool new_value);
		bool get_color_set_select();

		void set_gm_0(bool new_value);
		bool get_gm_0();

		void set_gm_1(bool new_value);
		bool get_gm_1();

		void set_gm_2(bool new_value);
		bool get_gm_2();

		void set_ag(bool new_value);
		bool get_ag();
	};
}

#endif