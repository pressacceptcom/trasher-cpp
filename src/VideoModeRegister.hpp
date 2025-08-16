/**
 * FF98:       Video Mode Register
 *
 *        BIT 7 = BP          0 = alphanumeric, 1 = bit plane
 *        BIT 6   -
 *        BIT 5 = BPI         1 = Burst phase inverted
 *        BIT 4 = MOCH        1 = monochrome (on composite)
 *        BIT 3 = H50         1 = 50 Hz vertical sync
 *        BIT 2 = LPR2        Lines per row (See table below)
 *        BIT 1 = LPR1        Lines per row (See table below)
 *        BIT 0 = LPR0        Lines per row (See table below)
 *
 * -------
 * |
 * | Alternate Color Set
 * |
 * |     Setting bit 5 of dedicated address FF98 invokes the alternate color set. In this set all
 * | the same colors are available; they are just specified by a different color code. Enabling the
 * | alternate color set has the effect of shifting all the colors, except gray, half way around the
 * | hue wheel in Fig. 2-4. Thus the hue angle specifies a different color than with the normal
 * | color set. The purpose of the alternate color set is to simulate the original artifact effect
 * | on a TV. With the original CoCo, sometimes the artifact colors woul dbe of one set and other
 * | times of another set.
 * |
 * - From Assembly Language Programming For The COCO 3 by Laurence A. Tepolt
 *
 * ------------------------------------------------------------
 * | LPR2 | LPR1 | LPR0 | Lines per character row             |
 * ------------------------------------------------------------
 * |  0   |  0   |  0   | one        (Graphics modes)         |
 * |  0   |  0   |  1   | two        (CoCo 1 and CoCo 2 only) |
 * |  0   |  1   |  0   | three      (CoCo 1 and CoCo 2 only) |
 * |  0   |  1   |  1   | eight                               |
 * |  1   |  0   |  0   | nine                                |
 * |  1   |  0   |  1   | (reserved)                          |
 * |  1   |  1   |  0   | twelve     (CoCo 1 and CoCo 2 only) |
 * |  1   |  1   |  1   | (reserved)                          |
 * ------------------------------------------------------------
 */

#ifndef VIDEOMODEREGISTER_H
#define VIDEOMODEREGISTER_H

#include <cstdint>

#include <Godot.hpp>
#include <Reference.hpp>

namespace godot
{
	class VideoModeRegister : public Reference
	{
		GODOT_CLASS(VideoModeRegister, Reference)

	private:
	public:
		enum Bits
		{
			LINES_PER_ROW_0 = 0x01,
			LINES_PER_ROW_1 = 0x02,
			LINES_PER_ROW_2 = 0x04,
			FIFTY_HERTZ_VERTICAL_SYNC = 0x08,
			MONOCHROME = 0x10,
			BURST_PHASE_INVERTED = 0x20,
			BIT_PLANE = 0x80
		};

		enum LinesPerRow
		{
			ONE = 0,
			TWO,
			THREE,
			EIGHT,
			NINE,
			TWELVE = 6
		};

		union
		{
			std::uint8_t byte;
			struct
			{
				bool lines_per_row_0 : 1;
				bool lines_per_row_1 : 1;
				bool lines_per_row_2 : 1;
				bool fifty_hertz_vertical_sync : 1;
				bool monochrome : 1;
				bool burst_phase_inverted : 1;
				bool ignore : 1;
				bool bit_plane : 1;
			} bits;
		};

		// C++ Specific Definitions

		VideoModeRegister();

		~VideoModeRegister();

		// Godot Specific Definitions

		static void _register_methods();

		void _init(); // our initializer called by Godot

		// Class Definitions

		void set_register(std::uint8_t new_value);
		std::uint8_t get_register();

		void set_bit(int bit, bool new_value);
		bool get_bit(int bit);

		void set_lines_per_row_0(bool new_value);
		bool get_lines_per_row_0();

		void set_lines_per_row_1(bool new_value);
		bool get_lines_per_row_1();

		void set_lines_per_row_2(bool new_value);
		bool get_lines_per_row_2();

		void set_lines_per_row(std::uint8_t new_value);
		std::uint8_t get_lines_per_row();

		void set_fifty_hertz_vertical_sync(bool new_value);
		bool get_fifty_hertz_vertical_sync();

		void set_monochrome(bool new_value);
		bool get_monochrome();

		void set_burst_phase_inverted(bool new_value);
		bool get_burst_phase_inverted();

		void set_bit_plane(bool new_value);
		bool get_bit_plane();
	};
}

#endif