/**
 * # ALPHANUMERIC MODES
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

#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <cstdint>

#include <Godot.hpp>
#include <Reference.hpp>

namespace godot
{
	class Attribute : public Reference
	{
		GODOT_CLASS(Attribute, Reference)

	private:
	public:
		enum Bits
		{
			BACKGROUND_0 = 0x01,
			BACKGROUND_1 = 0x02,
			BACKGROUND_2 = 0x04,
			FOREGROUND_0 = 0x08,
			FOREGROUND_1 = 0x10,
			FOREGROUND_2 = 0x20,
			UNDERLINE = 0x40,
			BLINK = 0x80
		};

		static const std::uint8_t BACKGROUND_MASK = 0x7;
		static const std::uint8_t FOREGROUND_MASK = 0x38;

		union
		{
			std::uint8_t byte;
			struct
			{
				bool background_0 : 1;
				bool background_1 : 1;
				bool background_2 : 1;
				bool foreground_0 : 1;
				bool foreground_1 : 1;
				bool foreground_2 : 1;
				bool underline : 1;
				bool blink : 1;
			} bits;
		};

		// C++ Specific Definitions

		Attribute();

		~Attribute();

		// Godot Specific Definitions

		static void _register_methods();

		void _init(); // our initializer called by Godot

		// Class Definitions

		void set_byte(std::uint8_t new_value);
		std::uint8_t get_byte();

		void set_background_0(bool new_value);
		bool get_background_0();

		void set_background_1(bool new_value);
		bool get_background_1();

		void set_background_2(bool new_value);
		bool get_background_2();

		void set_background(std::uint8_t new_value);
		std::uint8_t get_background();

		void set_foreground_0(bool new_value);
		bool get_foreground_0();

		void set_foreground_1(bool new_value);
		bool get_foreground_1();

		void set_foreground_2(bool new_value);
		bool get_foreground_2();

		void set_foreground(std::uint8_t new_value);
		std::uint8_t get_foreground();

		void set_underline(bool new_value);
		bool get_underline();

		void set_blink(bool new_value);
		bool get_blink();
	};
}

#endif