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
 * FFB0 - Green   (12)    - semigraphics mode 4 color 0, color graphics set 0 color 0
 * FFB1 - Yellow  (36)    - semigraphics mode 4 color 1, color graphics set 0 color 1
 * FFB2 - Blue    (09)    - semigraphics mode 4 color 2, color graphics set 0 color 2
 * FFB3 - Red     (24)    - semigraphics mode 4 color 3, color graphics set 0 color 3
 * FFB4 - Buff    (3F)    - semigraphics mode 4 color 4, color graphics set 1 color 0
 * FFB5 - Cyan    (10)    - semigraphics mode 4 color 5, color graphics set 1 color 1
 * FFB6 - Magenta (2D)    - semigraphics mode 4 color 6, color graphics set 1 color 2
 * FFB7 - Orange  (26)    - semigraphics mode 4 color 7, color graphics set 1 color 3
 * FFB8 - Black   (00)    - resolution graphics color set 0
 * FFB9 - Green   (12)
 * FFBA - Black   (00)    - resolution graphics color set 1
 * FFBB - Buff    (3F)
 * FFBC - Black   (00)    - text color, color set 0
 * FFBD - Green   (12)    - text background, color set 0
 * FFBE - Black   (00)    - text color, color set 1
 * FFBF - Orange  (26)    - text background, color set 1
 *
 * NOTE: For the PAL version, ignore the table attributed to composite.
 */

#ifndef COLORPALETTE_H
#define COLORPALETTE_H

#include <cstdint>
#include <vector>

#include <Godot.hpp>
#include <Reference.hpp>
#include <Ref.hpp>

#include "PaletteEntry.hpp"

namespace godot
{
	class ColorPalette : public Reference
	{
		GODOT_CLASS(ColorPalette, Reference)

	private:
	public:
		static const int MAX_SIZE = 16;

		std::vector<Ref<PaletteEntry>> palette;

		// C++ Specific Definitions

		ColorPalette();

		~ColorPalette();

		// Godot Specific Definitions

		static void _register_methods();

		void _init(); // our initializer called by Godot

		// Class Definitions

		void set_palette_entry(std::uint8_t index, std::uint8_t new_value);
		std::uint8_t get_palette_entry(std::uint8_t index);

		Ref<PaletteEntry> get_palette_entry_reference(std::uint8_t index);
	};
}

#endif