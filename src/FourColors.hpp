/**
 * GRAPHICS MODES
 *
 * 16 Color Modes: (CRES1 = 1, CRES0 = 0)
 *     Byte from DRAM
 *         Bit 7             PA3, First Pixel
 *         Bit 6             PA2, First Pixel
 *         Bit 5             PA1, First Pixel
 *         Bit 4             PA0, First Pixel
 *         Bit 3             PA3, Second Pixel
 *         Bit 2             PA2, Second Pixel
 *         Bit 1             PA1, Second Pixel
 *         Bit 0             PA0, Second Pixel
 *
 * 4 Color Modes: (CRES1 = 0, CRES0 = 1)
 *     Byte from DRAM
 *         Bit 7             PA1, First Pixel
 *         Bit 6             PA0, First Pixel
 *         Bit 5             PA1, Second Pixel
 *         Bit 4             PA0, Second Pixel
 *         Bit 3             PA1, Third Pixel
 *         Bit 2             PA0, Third Pixel
 *         Bit 1             PA1, Fourth Pixel
 *         Bit 0             PA0, Fourth Pixel
 *
 * 2 Color Modes: (CRES1 = 0, CRES0 = 0)
 *     Byte from DRAM
 *         Bit 7             PA0, First Pixel
 *         Bit 6             PA0, Second Pixel
 *         Bit 5             PA0, Third Pixel
 *         Bit 4             PA0, Fourth Pixel
 *         Bit 3             PA0, Fifth Pixel
 *         Bit 2             PA0, Sixth Pixel
 *         Bit 1             PA0, Seventh Pixel
 *         Bit 0             PA0, Eighth Pixel
 *
 * Palette Addresses
 *
 * ---------------------------------------------------------
 * | PA3 | PA2 | PA1 | PA0 | Address of Contents Displayed |
 * ---------------------------------------------------------
 * |  0  |  0  |  0  |  0  |             FFB0              |
 * |  0  |  0  |  0  |  1  |             FFB1              |
 * |  0  |  0  |  1  |  0  |             FFB2              |
 * |  0  |  0  |  1  |  1  |             FFB3              |
 * |  0  |  1  |  0  |  0  |             FFB4              |
 * |  0  |  1  |  0  |  1  |             FFB5              |
 * |  0  |  1  |  1  |  0  |             FFB6              |
 * |  0  |  1  |  1  |  1  |             FFB7              |
 * |  1  |  0  |  0  |  0  |             FFB8              |
 * |  1  |  0  |  0  |  1  |             FFB9              |
 * |  1  |  0  |  1  |  0  |             FFBA              |
 * |  1  |  0  |  1  |  1  |             FFBB              |
 * |  1  |  1  |  0  |  0  |             FFBC              |
 * |  1  |  1  |  0  |  1  |             FFBD              |
 * |  1  |  1  |  1  |  0  |             FFBE              |
 * |  1  |  1  |  1  |  1  |             FFBF              |
 * ---------------------------------------------------------
 */

#ifndef FOURCOLORS_H
#define FOURCOLORS_H

#include <cstdint>

#include <Godot.hpp>
#include <Reference.hpp>

namespace godot
{
	class FourColors : public Reference
	{
		GODOT_CLASS(FourColors, Reference)

	private:
	public:
		enum Bits
		{
			PALETTE_0_FOURTH_PIXEL = 0x01,
			PALETTE_1_FOURTH_PIXEL = 0x02,
			PALETTE_0_THIRD_PIXEL = 0x04,
			PALETTE_1_THIRD_PIXEL = 0x08,
			PALETTE_0_SECOND_PIXEL = 0x10,
			PALETTE_1_SECOND_PIXEL = 0x20,
			PALETTE_0_FIRST_PIXEL = 0x40,
			PALETTE_1_FIRST_PIXEL = 0x80
		};

		union
		{
			std::uint8_t byte;
			struct
			{
				bool palette_0_fourth_pixel : 1;
				bool palette_1_fourth_pixel : 1;
				bool palette_0_third_pixel : 1;
				bool palette_1_third_pixel : 1;
				bool palette_0_second_pixel : 1;
				bool palette_1_second_pixel : 1;
				bool palette_0_first_pixel : 1;
				bool palette_1_first_pixel : 1;
			} bits;
			struct
			{
				unsigned int fourth_pixel : 2;
				unsigned int third_pixel : 2;
				unsigned int second_pixel : 2;
				unsigned int first_pixel : 2;
			} palette_index;
		};

		// C++ Specific Definitions

		FourColors();

		~FourColors();

		// Godot Specific Definitions

		static void _register_methods();

		void _init(); // our initializer called by Godot

		// Class Definitions

		void set_byte(std::uint8_t new_value);
		std::uint8_t get_byte();

		void set_palette_0_fourth_pixel(bool new_value);
		bool get_palette_0_fourth_pixel();

		void set_palette_1_fourth_pixel(bool new_value);
		bool get_palette_1_fourth_pixel();

		void set_palette_fourth_pixel(std::uint8_t new_value);
		std::uint8_t get_palette_fourth_pixel();

		void set_palette_0_third_pixel(bool new_value);
		bool get_palette_0_third_pixel();

		void set_palette_1_third_pixel(bool new_value);
		bool get_palette_1_third_pixel();

		void set_palette_third_pixel(std::uint8_t new_value);
		std::uint8_t get_palette_third_pixel();

		void set_palette_0_second_pixel(bool new_value);
		bool get_palette_0_second_pixel();

		void set_palette_1_second_pixel(bool new_value);
		bool get_palette_1_second_pixel();

		void set_palette_second_pixel(std::uint8_t new_value);
		std::uint8_t get_palette_second_pixel();

		void set_palette_0_first_pixel(bool new_value);
		bool get_palette_0_first_pixel();

		void set_palette_1_first_pixel(bool new_value);
		bool get_palette_1_first_pixel();

		void set_palette_first_pixel(std::uint8_t new_value);
		std::uint8_t get_palette_first_pixel();
	};
}

#endif