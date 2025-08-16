/**
 * SAM CONTROL REGISTERS:    (FFC0 - FFDF)
 *
 * Clear    Set
 * FFC0  -  FFC1   V0   CoCo graphics mode select
 * FFC2  -  FFC3   V1   CoCo graphics mode select
 * FFC4  -  FFC5   V2   CoCo graphics mode select
 * FFC6  -  FFC7   F0   CoCo Vertical offset
 * FFC8  -  FFC9   F1   CoCo Vertical offset
 * FFCA  -  FFCB   F2   CoCo Vertical offset
 * FFCC  -  FFCD   F3   CoCo Vertical offset
 * FFCE  -  FFCF   F4   CoCo Vertical offset
 * FFD0  -  FFD1   F5   CoCo Vertical offset
 * FFD2  -  FFD3   F6   CoCo Vertical offset
 * FFD8  -  FFD9   R1   MPU Speed
 * FFDE  -  FFDF   TY   ROM disable
 *
 * NOTE: These bits work like the ones in the Motorola SAM chip (MC6883/SN74LS785) in that by
 *       writing to the upper address of each two-address group (data is don't care), the bit is
 *       set; by writing to the lower address, the bit is cleared. The graphics modes and vertical
 *       offset bits are valid only in the CoCo mode, but the other two bits are valid anytime. Note
 *       the only semigraphics mode supported is Semi Four
 *
 * ----------------------------
 * | FFDF |  S  |      | MAP  |
 * --------------  TY  | TYPE |
 * | FFDE |  C  |      |      |
 * ---------------------------------------------------
 * | FFD9 |  S  |      | CPU  |          |     |     |
 * --------------  R1  | Rate |          |  1  |  0  |
 * | FFD8 |  C  |      |      |          |     |     |
 * ---------------------------------------------------
 * | FFD3 |  S  |      |          |\        |     |
 * --------------  F6  |          |  \      |     0.89 MHz
 * | FFD2 |  C  |      |          |  |      |
 * ---------------------          |  |      1.78 MHz
 * | FFD1 |  S  |      |          |  |
 * --------------  F5  |          |  |
 * | FFD0 |  C  |      |          |  |
 * ---------------------          |  |
 * | FFCF |  S  |      |          |  |
 * --------------  F4  | DISPLAY  |  |- Address of Upper-Left-Most
 * | FFCE |  C  |      | OFFSET   |  |  Display Element = 0000 + (1/2K Offset)
 * ---------------------          |  |
 * | FFCD |  S  |      |          |  |     N.U.
 * --------------  F3  |          |  |        |     RG6, CG6
 * | FFCC |  C  |      |          |  |        |     |
 * --------------------- (BINARY) |  |        |     |     RG3
 * | FFCB |  S  |      |          |  |        |     |     |
 * --------------  F2  |          |  |        |     |     |     CG3
 * | FFCA |  C  |      |          |  /        |     |     |     |
 * ---------------------          |/          |     |     |     |     RG2
 * | FFC9 |  S  |      |          |           |     |     |     |     |
 * --------------  F1  |          |           |     |     |     |     |     CG2
 * | FFC8 |  C  |      |          |           |     |     |     |     |     |
 * ---------------------          |           |     |     |     |     |     |     CG1, RG1
 * | FFC7 |  S  |      |          |           |     |     |     |     |     |     |
 * --------------  F0  |          |           |     |     |     |     |     |     |     AI, AE, S4
 * | FFC6 |  C  |      |          |           |     |     |     |     |     |     |     |
 * -----------------------------------------------------------------------------------------
 * | FFC5 |  S  |      |          |        |     |     |     |     |     |     |     |     |
 * --------------  V2  | DISPLAY  |        |  1  |  1  |  1  |  1  |  0  |  0  |  0  |  0  |
 * | FFC4 |  C  |      |  MODE    |        |     |     |     |     |     |     |     |     |
 * --------------------- CONTROL  ----------------------------------------------------------
 * | FFC3 |  S  |      |          |        |     |     |     |     |     |     |     |     |
 * --------------  V1  |          |        |  1  |  1  |  0  |  0  |  1  |  1  |  0  |  0  |
 * | FFC2 |  C  |      |  (SAM)   |        |     |     |     |     |     |     |     |     |
 * ---------------------          ----------------------------------------------------------
 * | FFC1 |  S  |      |          |        |     |     |     |     |     |     |     |     |
 * --------------  V0  |          |        |  1  |  0  |  1  |  0  |  1  |  0  |  1  |  0  |
 * | FFC0 |  C  |      |          |        |     |     |     |     |     |     |     |     |
 * -----------------------------------------------------------------------------------------
 *
 * (S = Set Bit, C = Clear Bit, all Bits are cleared when SAM is reset)
 *
 * Figure 1-3. Memory Map for SAM Control Register
 */

#ifndef SAMCONTROLREGISTERS_H
#define SAMCONTROLREGISTERS_H

#include <cstdint>

#include <Godot.hpp>
#include <Reference.hpp>

namespace godot
{
	class SAMControlRegisters : public Reference
	{
		GODOT_CLASS(SAMControlRegisters, Reference)

	private:
	public:
		enum DisplayOffsetBits
		{
			F_0 = 0x01,
			F_1 = 0x02,
			F_2 = 0x04,
			F_3 = 0x08,
			F_4 = 0x10,
			F_5 = 0x20,
			F_6 = 0x40
		};

		enum Modes
		{
			ALPHANUMERIC,
			COLOR_RESOLUTION_GRAPHICS_1,
			COLOR_GRAPHICS_2,
			RESOLUTION_GRAPHICS_2,
			COLOR_GRAPHICS_3,
			RESOLUTION_GRAPHICS_3,
			COLOR_GRAPHICS_6,
			RESOLUTION_GRAPHICS_6
		};

		enum DisplayModeControlBits
		{
			V_0 = 0x01,
			V_1 = 0x02,
			V_2 = 0x04
		};

		union DisplayModeControl
		{
			std::uint8_t byte;
			struct
			{
				bool v_0 : 1;
				bool v_1 : 1;
				bool v_2 : 1;
			} bits;
		} display_mode_control;

		union DisplayOffset
		{
			std::uint8_t byte;
			struct
			{
				bool f_0 : 1;
				bool f_1 : 1;
				bool f_2 : 1;
				bool f_3 : 1;
				bool f_4 : 1;
				bool f_5 : 1;
				bool f_6 : 1;
			} bits;
		} display_offset;

		bool r1_cpu_rate = false;
		bool ty_rom_disable = false;

		// C++ Specific Definitions

		SAMControlRegisters();

		~SAMControlRegisters();

		// Godot Specific Definitions

		static void _register_methods();

		void _init(); // our initializer called by Godot

		// Class Definitions

		void set_v_0(bool new_value);
		bool get_v_0();

		void set_v_1(bool new_value);
		bool get_v_1();

		void set_v_2(bool new_value);
		bool get_v_2();

		void set_display_mode_control(std::uint8_t new_value);
		std::uint8_t get_display_mode_control();

		void set_f_0(bool new_value);
		bool get_f_0();

		void set_f_1(bool new_value);
		bool get_f_1();

		void set_f_2(bool new_value);
		bool get_f_2();

		void set_f_3(bool new_value);
		bool get_f_3();

		void set_f_4(bool new_value);
		bool get_f_4();

		void set_f_5(bool new_value);
		bool get_f_5();

		void set_f_6(bool new_value);
		bool get_f_6();

		void set_display_offset(std::uint8_t new_value);
		std::uint8_t get_display_offset();

		void set_r1_cpu_rate(bool new_value);
		bool get_r1_cpu_rate();

		void set_ty_rom_disable(bool new_value);
		bool get_ty_rom_disable();
	};
}

#endif