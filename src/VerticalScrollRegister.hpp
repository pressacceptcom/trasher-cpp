/**
 * FF9C:       Vertical Scroll Register
 *
 *        BIT 7   -
 *        BIT 6   -
 *        BIT 5   -
 *        BIT 4   -
 *        BIT 3 = VSC3        (Vert. Scroll)
 *        BIT 2 = VSC2
 *        BIT 1 = VSC1
 *        BIT 0 = VSC0
 *
 *        NOTE: In the CoCo mode, the VSC's must be initialized to 0F hex.
 *
 * ------
 * | Vertical Scroll Register
 * |
 * |     This register is only effective in the text modes and provides the smooth scroll
 * | capability. A smooth scroll is when the text appears to move up smoothly instead of jumping up
 * | one line at a time. The bit assignments of htis register are shown in Table 4-5.
 * |
 * | Bit 7 - Not used
 * | Bit 6 - Not used
 * | Bit 5 - Not used
 * | Bit 4 - Not used
 * | Bit 3 - SCEN (Scroll Enable)
 * | Bit 2 - SC2
 * | Bit 1 - SC1
 * | Bit 2 - SC0
 * |
 * | + SCEN - Vertical scrolling is enabled when this bit is clear and disabled when set.
 * | + SC2-0 - In these bits is put a verical scroll value of 0-7. Each incrementally higher value
 * |           will scroll the text up one eighth of a line. In doing so, new buffer area is
 * |           exposed at the bottom of the screen. This should have been previously loaded with
 * |           the new line of text to appear at the screen bottom. When the scroll value reaches
 * |           seven, the smooth scroll process is completed by setting the scroll value to zero
 * |           and incrementing the buffer starting address (vertical offset 1 and 0) such that the
 * |           buffer starts at the new top line of text.
 * |
 * -- Assembly Language Programmign for the Color Computer 3 by Laurence Tepolt
 *
 */

#ifndef VERTICALSCROLLREGISTER_H
#define VERTICALSCROLLREGISTER_H

#include <cstdint>

#include <Godot.hpp>
#include <Reference.hpp>

namespace godot
{
	class VerticalScrollRegister : public Reference
	{
		GODOT_CLASS(VerticalScrollRegister, Reference)

	private:
	public:
		enum Bits
		{
			VERTICAL_SCROLL_0 = 0x01,
			VERTICAL_SCROLL_1 = 0x02,
			VERTICAL_SCROLL_2 = 0x04,
			VERTICAL_SCROLL_3 = 0x08
		};

		union
		{
			std::uint8_t byte;
			struct
			{
				bool vertical_scroll_0 : 1;
				bool vertical_scroll_1 : 1;
				bool vertical_scroll_2 : 1;
				bool vertical_scroll_3 : 1;
			} bits;
		};

		// C++ Specific Definitions

		VerticalScrollRegister();

		~VerticalScrollRegister();

		// Godot Specific Definitions

		static void _register_methods();

		void _init(); // our initializer called by Godot

		// Class Definitions

		void set_register(std::uint8_t new_value);
		std::uint8_t get_register();

		void set_bit(int bit, bool new_value);
		bool get_bit(int bit);

		void set_vertical_scroll_0(bool new_value);
		bool get_vertical_scroll_0();

		void set_vertical_scroll_1(bool new_value);
		bool get_vertical_scroll_1();

		void set_vertical_scroll_2(bool new_value);
		bool get_vertical_scroll_2();

		void set_vertical_scroll_3(bool new_value);
		bool get_vertical_scroll_3();
	};
}

#endif