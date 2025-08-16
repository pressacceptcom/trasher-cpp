/**
 * ALPHANUMERIC MODES
 *
 * Text screen memory:
 *
 *     Even Byte (Character byte)
 *     --------------------------
 *     BIT 7    -
 *     BIT 6 = Character bit 6
 *     BIT 5 = Character bit 5
 *     BIT 4 = Character bit 4
 *     BIT 3 = Character bit 3
 *     BIT 2 = Character bit 2
 *     BIT 1 = Character bit 1
 *     BIT 0 = Character bit 0
 *
 *     Odd Byte (Attribute byte)
 *     -------------------------
 *     BIT 7 = BLINK       Characters blink at 1/2 sec. rate
 *     BIT 6 = UNDLN       Characters are underline
 *     BIT 5 = FGND2       Foreground color bit (palette addr.)
 *     BIT 4 = FGND1       Foreground color bit (palette addr.)
 *     BIT 3 = FGND0       Foreground color bit (palette addr.)
 *     BIT 2 = BGND2       Background color bit (palette addr.)
 *     BIT 1 = BGND1       Background color bit (palette addr.)
 *     BIT 0 = BGND0       Background color bit (palette addr.)
 *
 * NOTE: Attributes are not available when CoCo = 1
 */

#ifndef ALPHANUMERIC_H
#define ALPHANUMERIC_H

#include <Godot.hpp>
#include <Reference.hpp>
#include <Ref.hpp>

#include "Character.hpp"
#include "Attribute.hpp"

namespace godot
{
	class Alphanumeric : public Reference
	{
		GODOT_CLASS(Alphanumeric, Reference)

	private:
	public:
		Ref<Character> character;
		Ref<Attribute> attribute;

		// C++ Specific Definitions

		Alphanumeric();

		~Alphanumeric();

		// Godot Specific Definitions

		static void _register_methods();

		void _init(); // our initializer called by Godot

		// Class Definitions

		void set_bytes(std::uint16_t bytes);
		std::uint16_t get_bytes();

		void set_character(Ref<Character> new_character);
		Ref<Character> get_character();

		void set_attribute(Ref<Attribute> new_attribute);
		Ref<Attribute> get_attribute();
	};
}

#endif