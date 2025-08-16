/**
 * FF99:       Video Resolution Register
 *
 *        BIT 7   -
 *        BIT 6 = LPF1        Lines per field (See table below)
 *        BIT 5 = LPF0        Lines per field
 *        BIT 4 = HRES2       Horizontal resolution (See Video resolution on page 17)
 *        BIT 3 = HRES1       Horizontal resolution
 *        BIT 2 = HRES0       Horizontal resolution
 *        BIT 1 = CRES1       Color resolution (See Video resolution)
 *        BIT 0 = CRES0       Color resolution
 *
 * ---------------------------------
 * | LPF1 | LPF0 | Lines per field |
 * ---------------------------------
 * |  0   |  0   |       192       |
 * |  0   |  1   |       200       |
 * |  1   |  0   |     Reserved    |
 * |  1   |  1   |       225       |
 * ---------------------------------
 *
 * From page 18:
 *
 * VIDEO RESOLUTION
 *
 * The combination of HRES and CRES bits determine the resolution of the screen. The following
 * resolutions are supported:
 *
 * Alphanumerics: BP = 0, CoCo = 0
 *
 * --------------------------------------------------------
 * |              |       |       |       |       |       |
 * | \    RES bit |       |       |       |       |       |
 * |     \        | HRES2 | HRES1 | HRES0 | CRES1 | CRES0 |
 * |         \    |       |       |       |       |       |
 * |  MODE      \ |       |       |       |       |       |
 * |              |       |       |       |       |       |
 * --------------------------------------------------------
 * |              |       |       |       |       |       |
 * | 32 character |   0   |   -   |   0   |   -   |   1   |
 * |              |       |       |       |       |       |
 * | 40 character |   0   |   -   |   1   |   -   |   1   |
 * |              |       |       |       |       |       |
 * | 80 character |   1   |   -   |   1   |   -   |   1   |
 * --------------------------------------------------------
 *
 * Graphics: BP = 1, CoCo = 0
 *
 * -----------------------------------------------------------
 * | Pixels | Colors | HRES2 | HRES1 | HRES0 | CRES1 | CRES0 |
 * -----------------------------------------------------------
 * |  640   |   4    |   1   |   1   |   1   |   0   |   1   |
 * |  640   |   2    |   1   |   0   |   1   |   0   |   0   |
 * -----------------------------------------------------------
 * |  512   |   4    |   1   |   1   |   0   |   0   |   1   |
 * |  512   |   2    |   1   |   0   |   0   |   0   |   0   |
 * -----------------------------------------------------------
 * |  320   |   16   |   1   |   1   |   1   |   1   |   0   |
 * |  320   |   4    |   1   |   0   |   1   |   0   |   1   |
 * -----------------------------------------------------------
 * |  256   |   16   |   1   |   1   |   0   |   1   |   0   |
 * |  256   |   4    |   1   |   0   |   0   |   0   |   1   |
 * |  256   |   2    |   0   |   1   |   0   |   0   |   0   |
 * -----------------------------------------------------------
 * |  160   |   16   |   1   |   0   |   1   |   1   |   0   |
 * -----------------------------------------------------------
 */

#ifndef VIDEORESOLUTIONREGISTER_H
#define VIDEORESOLUTIONREGISTER_H

#include <cstdint>

#include <Godot.hpp>
#include <Reference.hpp>

namespace godot
{
	class VideoResolutionRegister : public Reference
	{
		GODOT_CLASS(VideoResolutionRegister, Reference)

	private:
	public:
		enum Bits
		{
			COLOR_RESOLUTION_0 = 0x01,
			COLOR_RESOLUTION_1 = 0x02,
			HORIZONTAL_RESOLUTION_0 = 0x04,
			HORIZONTAL_RESOLUTION_1 = 0x08,
			HORIZONTAL_RESOLUTION_2 = 0x10,
			LINES_PER_FIELD_0 = 0x20,
			LINES_PER_FIELD_1 = 0x40
		};

		enum LinesPerField
		{
			ONE_NINETY_TWO,
			TWO_HUNDERD = 0x20,
			TWO_TWENTY_FIVE = 0x60
		};

		enum AlphanumericsWidth
		{
			THIRTY_TWO_CHARACTER = 0x01, // xxx0x0x1
			FORTY_CHARACTER = 0x05,		 // xxx0x1x1
			/**
			 * Laurence Tepolt's Assembly Language Programming for the Color Computer 3 claims
			 * there is a sixty-four character column mode when HRES2 is 1 and HRES0 is 0.
			 */
			SIXTY_FOUR_CHARACTER = 0x11, // xxx1x0x1
			EIGHTY_CHARACTER = 0x15		 // xxx1x1x1
		};

		enum GraphicsResolution
		{
			TWO_FIFTY_SIX_2 = 0x08,	 // xxx01000
			FIVE_TWELVE_2 = 0x10,	 // xxx10000
			TWO_FIFTY_SIX_4 = 0x11,	 // xxx10001
			SIX_FORTY_2 = 0x14,		 // xxx10100
			THREE_TWENTY_4 = 0x15,	 // xxx10101
			ONE_SIXTY_16 = 0x16,	 // xxx10110
			FIVE_TWELVE_4 = 0x19,	 // xxx11001
			TWO_FIFTY_SIX_16 = 0x1A, // xxx11010
			SIX_FORTY_4 = 0x1D,		 // xxx11101
			THREE_TWENTY_16 = 0x1E	 // xxx11110
		};

		enum ColorResolution
		{
			TWO = 0x00,
			FOUR,
			SIXTEEN
		};

		union
		{
			std::uint8_t byte;
			struct
			{
				bool color_resolution_0 : 1;
				bool color_resolution_1 : 1;
				bool horizontal_resolution_0 : 1;
				bool horizontal_resolution_1 : 1;
				bool horizontal_resolution_2 : 1;
				bool lines_per_field_0 : 1;
				bool lines_per_field_1 : 1;
			} bits;
		};

		// C++ Specific Definitions

		VideoResolutionRegister();

		~VideoResolutionRegister();

		// Godot Specific Definitions

		static void _register_methods();

		void _init(); // our initializer called by Godot

		// Class Definitions

		void set_register(std::uint8_t new_value);
		std::uint8_t get_register();

		void set_bit(int bit, bool new_value);
		bool get_bit(int bit);

		void set_color_resolution_0(bool new_value);
		bool get_color_resolution_0();

		void set_color_resolution_1(bool new_value);
		bool get_color_resolution_1();

		void set_color_resolution(std::uint8_t new_value);
		std::uint8_t get_color_resolution();

		void set_horizontal_resolution_0(bool new_value);
		bool get_horizontal_resolution_0();

		void set_horizontal_resolution_1(bool new_value);
		bool get_horizontal_resolution_1();

		void set_horizontal_resolution_2(bool new_value);
		bool get_horizontal_resolution_2();

		void set_horizontal_resolution(std::uint8_t new_value);
		std::uint8_t get_horizontal_resolution();

		void set_resolution(std::uint8_t new_value);
		std::uint8_t get_resolution();

		void set_lines_per_field_0(bool new_value);
		bool get_lines_per_field_0();

		void set_lines_per_field_1(bool new_value);
		bool get_lines_per_field_1();

		void set_lines_per_field(std::uint8_t new_value);
		std::uint8_t get_lines_per_field();
	};
}

#endif