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

#ifndef CHARACTER_H
#define CHARACTER_H

#include <cstdint>

#include <Godot.hpp>
#include <Reference.hpp>

namespace godot
{
	class Character : public Reference
	{
		GODOT_CLASS(Character, Reference)

	private:
	public:
		enum Bits
		{
			BIT_0 = 0x01,
			BIT_1 = 0x02,
			BIT_2 = 0x04,
			BIT_3 = 0x08,
			BIT_4 = 0x10,
			BIT_5 = 0x20,
			BIT_6 = 0x40
		};

		union
		{
			std::uint8_t byte;
			struct
			{
				bool bit_0 : 1;
				bool bit_1 : 1;
				bool bit_2 : 1;
				bool bit_3 : 1;
				bool bit_4 : 1;
				bool bit_5 : 1;
				bool bit_6 : 1;
			} bits;
		};

		// C++ Specific Definitions

		Character();

		~Character();

		// Godot Specific Definitions

		static void _register_methods();

		void _init(); // our initializer called by Godot

		// Class Definitions

		void set_byte(std::uint8_t new_value);
		std::uint8_t get_byte();

		void set_bit_0(bool new_value);
		bool get_bit_0();

		void set_bit_1(bool new_value);
		bool get_bit_1();

		void set_bit_2(bool new_value);
		bool get_bit_2();

		void set_bit_3(bool new_value);
		bool get_bit_3();

		void set_bit_4(bool new_value);
		bool get_bit_4();

		void set_bit_5(bool new_value);
		bool get_bit_5();

		void set_bit_6(bool new_value);
		bool get_bit_6();
	};
}

#endif