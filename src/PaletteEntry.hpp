/**
 * COLOR PALETTE
 *
 * FFB0 - FFBF: 16 addresses, 6 bits each
 *
 * For the RGB output, the bits are defined as follows:
 *
 * -------------------------------------
 * | Data Bit      | D5 D4 D3 D2 D1 D0 |
 * -------------------------------------
 * | Corresponding |                   |
 * |  RGB output   | R1 G1 B1 R0 G0 B0 |
 * |               |                   |
 * -------------------------------------
 *
 * For the Composite output, the bits are defined as follows, where I is intensity level and P is
 * phase:
 *
 * -------------------------------------
 * | Data Bit      | D5 D4 D3 D2 D1 D0 |
 * -------------------------------------
 * | Corresponding |                   |
 * |   composite   | I1 I0 P3 P2 P1 P0 |
 * |    output     |                   |
 * -------------------------------------
 *
 * Some Color Examples:
 *
 * --------------------------------------------------
 * |                |      RGB      |   Composite   |
 * |                ---------------------------------
 * | Color          | Binary | Hex  | Binary | Hex  |
 * --------------------------------------------------
 * | White          | 111111 | (3F) | 110000 | (30) |
 * | Black          | 000000 | (00) | 000000 | (00) |
 * | Bright Green   | 010010 | (12) | 100010 | (22) |
 * | Medium Green   | 010000 | (10) | 010010 | (12) |
 * | Dark Green     | 000010 | (02) | 000010 | (02) |
 * | Medium Magenta | 101000 | (28) | 010101 | (15) |
 * --------------------------------------------------
 *
 * For CoCo compatibility, the following values should be loaded upon initialization. (NOTE: These
 * are the RGB values.)
 *
 * FFB0 - Green   (12)
 * FFB1 - Yellow  (36)
 * FFB2 - Blue    (09)
 * FFB3 - Red     (24)
 * FFB4 - Buff    (3F)
 * FFB5 - Cyan    (10)
 * FFB6 - Magenta (2D)
 * FFB7 - Orange  (26)
 * FFB8 - Black   (00)
 * FFB9 - Green   (12)
 * FFBA - Black   (00)
 * FFBB - Buff    (3F)
 * FFBC - Black   (00)
 * FFBD - Green   (12)
 * FFBE - Black   (00)
 * FFBF - Orange  (26)
 *
 * NOTE: For the PAL version, ignore the table attributed to composite.
 */

#ifndef PALETTEENTRY_H
#define PALETTEENTRY_H

#include <cstdint>

#include <Godot.hpp>
#include <Reference.hpp>

namespace godot
{
	class PaletteEntry : public Reference
	{
		GODOT_CLASS(PaletteEntry, Reference)

	private:
	public:
		enum Bits
		{
			BLUE_0 = 0x01,
			GREEN_0 = 0x02,
			RED_0 = 0x04,
			BLUE_1 = 0x08,
			GREEN_1 = 0x10,
			RED_1 = 0x20,
			PHASE_0 = 0x01,
			PHASE_1 = 0x02,
			PHASE_2 = 0x04,
			PHASE_4 = 0x08,
			INTENSITY_0 = 0x10,
			INTENSITY_1 = 0x20
		};

		enum class Blue
		{
			BLUE_0 = 0x00,
			BLUE_1 = Bits::BLUE_0,
			BLUE_2 = Bits::BLUE_1,
			BLUE_3 = Bits::BLUE_0 | Bits::BLUE_1
		};

		enum class Green
		{
			GREEN_0 = 0x00,
			GREEN_1 = Bits::GREEN_0,
			GREEN_2 = Bits::GREEN_1,
			GREEN_3 = Bits::GREEN_0 | Bits::GREEN_1
		};

		enum class Red
		{
			RED_0 = 0x00,
			RED_1 = Bits::RED_0,
			RED_2 = Bits::RED_1,
			RED_3 = Bits::RED_0 | Bits::RED_1
		};

		static const std::uint8_t PHASE_MASK = 0x0F;
		static const std::uint8_t INTENSITY_MASK = 0x30;

		bool debug = false;

		union
		{
			std::uint8_t byte;
			struct
			{
				bool blue_0 : 1;
				bool green_0 : 1;
				bool red_0 : 1;
				bool blue_1 : 1;
				bool green_1 : 1;
				bool red_1 : 1;
			} rgb;
			struct
			{
				bool phase_0 : 1;
				bool phase_1 : 1;
				bool phase_2 : 1;
				bool phase_3 : 1;
				bool intensity_0 : 1;
				bool intensity_1 : 1;
			} composite;
		};

		// C++ Specific Definitions

		PaletteEntry();

		~PaletteEntry();

		// Godot Specific Definitions

		static void _register_methods();

		void _init(); // our initializer called by Godot

		// Class Definitions

		void set_register(std::uint8_t new_value);
		std::uint8_t get_register();

		void set_bit(int bit, bool new_value);
		bool get_bit(int bit);

		void set_blue_0(bool new_value);
		bool get_blue_0();

		void set_blue_1(bool new_value);
		bool get_blue_1();

		void set_blue(std::uint8_t new_value);
		std::uint8_t get_blue();

		void set_green_0(bool new_value);
		bool get_green_0();

		void set_green_1(bool new_value);
		bool get_green_1();

		void set_green(std::uint8_t new_value);
		std::uint8_t get_green();

		void set_red_0(bool new_value);
		bool get_red_0();

		void set_red_1(bool new_value);
		bool get_red_1();

		void set_red(std::uint8_t new_value);
		std::uint8_t get_red();

		void set_phase_0(bool new_value);
		bool get_phase_0();

		void set_phase_1(bool new_value);
		bool get_phase_1();

		void set_phase_2(bool new_value);
		bool get_phase_2();

		void set_phase_3(bool new_value);
		bool get_phase_3();

		void set_phase(std::uint8_t new_value);
		std::uint8_t get_phase();

		void set_intensity_0(bool new_value);
		bool get_intensity_0();

		void set_intensity_1(bool new_value);
		bool get_intensity_1();

		void set_intensity(std::uint8_t new_value);
		std::uint8_t get_intensity();
	};
}

#endif