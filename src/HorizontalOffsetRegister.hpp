/**
 * FF9F:       Horizontal Offset 0 Register
 *
 *        BIT 7 = HVEN        Horizontal Virtual Enable
 *        BIT 6 = X6          Horizontal Offset address
 *        BIT 5 = X5          Horizontal Offset address
 *        BIT 4 = X4          Horizontal Offset address
 *        BIT 3 = X3          Horizontal Offset address
 *        BIT 2 = X2          Horizontal Offset address
 *        BIT 1 = X1          Horizontal Offset address
 *        BIT 0 = X0          Horizontal Offset address
 *
 *        NOTE: HVEN enables a horizontal screen width of 128 bytes regardless of the HRES bits and
 *              CRES bits selected. This will allow a "virtual" screen somewhat larger than the
 *              displayed screen. The user can move the "window" (the displayed screen) by means of
 *              the horizontal offset bits. In character mode, the screen width is 128 characters
 *              regardless of attribute (or 64, if double-wide is selected).
 */

#ifndef HORIZONTALOFFSETREGISTER_H
#define HORIZONTALOFFSETREGISTER_H

#include <cstdint>

#include <Godot.hpp>
#include <Reference.hpp>

namespace godot
{
	class HorizontalOffsetRegister : public Reference
	{
		GODOT_CLASS(HorizontalOffsetRegister, Reference)

	private:
	public:
		enum Bits
		{
			HORIZONTAL_OFFSET_0 = 0x01,
			HORIZONTAL_OFFSET_1 = 0x02,
			HORIZONTAL_OFFSET_2 = 0x04,
			HORIZONTAL_OFFSET_3 = 0x08,
			HORIZONTAL_OFFSET_4 = 0x10,
			HORIZONTAL_OFFSET_5 = 0x20,
			HORIZONTAL_OFFSET_6 = 0x40,
			HORIZONTAL_EXTENDED = 0x80
		};

		union
		{
			std::uint8_t byte;
			struct
			{
				bool horizontal_offset_0 : 1;
				bool horizontal_offset_1 : 1;
				bool horizontal_offset_2 : 1;
				bool horizontal_offset_3 : 1;
				bool horizontal_offset_4 : 1;
				bool horizontal_offset_5 : 1;
				bool horizontal_offset_6 : 1;
				bool horizontal_extended : 1;
			} bits;
			struct
			{
				unsigned int value : 7;
			} offset;
		};

		// C++ Specific Definitions

		HorizontalOffsetRegister();

		~HorizontalOffsetRegister();

		// Godot Specific Definitions

		static void _register_methods();

		void _init(); // our initializer called by Godot

		// Class Definitions

		void set_register(std::uint8_t new_value);
		std::uint8_t get_register();

		void set_bit(int bit, bool new_value);
		bool get_bit(int bit);

		void set_horizontal_offset_0(bool new_value);
		bool get_horizontal_offset_0();

		void set_horizontal_offset_1(bool new_value);
		bool get_horizontal_offset_1();

		void set_horizontal_offset_2(bool new_value);
		bool get_horizontal_offset_2();

		void set_horizontal_offset_3(bool new_value);
		bool get_horizontal_offset_3();

		void set_horizontal_offset_4(bool new_value);
		bool get_horizontal_offset_4();

		void set_horizontal_offset_5(bool new_value);
		bool get_horizontal_offset_5();

		void set_horizontal_offset_6(bool new_value);
		bool get_horizontal_offset_6();

		void set_horizontal_offset(std::uint8_t new_value);
		std::uint8_t get_horizontal_offset();

		void set_horizontal_extended(bool new_value);
		bool get_horizontal_extended();
	};
}

#endif