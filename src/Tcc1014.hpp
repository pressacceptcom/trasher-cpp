/**
 * The TCC1014 (VC2645QC: ACVC) "GIME" Emulator Component
 *
 * Emulates the ACVC
 */

/**
 * # From page 8 of the TANDY Service Manual for the Color Computer 3 (26-3334)
 *
 * 1.3 Memory Map
 *
 * Figure 1-2 shows the breakdown of the large blocks of memory in the Color Computer 3.
 *
 * The rest of the section itemizes the following registers:
 *
 * + I/O Control Register
 * + Chip Control Register
 * + 68B09E Vector Register
 *
 * ----------
 * ... In the CoCo 3, physical memory is considered to start at the 512K address point and extend
 * downward toward zero. ...
 *
 * ... The exception ... is when the virtual address is in the range FF00-FFFF. These (the dedicated
 * addresses) are not expanded and sent to RAM but are always routed to their appropriate device
 * and/or control register. ...
 * |
 * |
 * ---From "Assembly Language Programming for the CoCo3" by Laurence A. Tepolt
 *
 * $7FFFF ----------------------------
 *            |                      | 68B09E VECTOR
 *            |                      | CONTROL
 *            |                      | REGISTER, I/O
 * $7FF00 ----------------------------
 *            |                      |
 *            | SUPER EXTENDED BASIC |
 *            |                      |
 * $7E000 ----------------------------
 *            |                      |
 *            | CARTRIDGE ROM        |
 *            |                      |
 * $7C000 ----------------------------
 *            |                      |
 *            | COLOR BASIC          |
 *            |                      |
 * $7A000 ----------------------------
 *            |                      |
 *            | EXTEND COLOR BASIC   |
 *            |                      |
 * $78000 ----------------------------
 *            |                      |
 *            |                      |
 *            |                      |
 * $70600 ----------------------------
 *            | STANDARD TEXT SCREEN |
 * $70400 ----------------------------
 *            |                      |
 * $70000 ----------------------------
 *            |                      |
 *            |                      |
 *            |                      |
 * $6E000 ----------------------------
 *            |                      |
 *            | HIGH RESOLUTION      |
 *            | TEXT SCREEN          |
 *            |                      |
 * $6C000 ----------------------------
 *            |                      |
 *            |                      |
 *            |                      |
 * $68000 ----------------------------
 *            |                      |
 *            | HIGH RESOLUTION      |
 *            | GRAPHIC SCREEN       |
 *            |                      |
 * $60000 ----------------------------
 *            |                      |
 *            |                      |
 *            |                      |
 *
 *            ~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *            |                      |
 *            |                      |
 *            |                      |
 * $00000 ----------------------------
 *
 * Figure 1-2. Color Computer 3 Memory Map
 *
 * ... [See mc6821.gd for this material]
 *
 * 1.5 Chip Control Registers
 *
 * ----------------------------
 * | FF90 - FFDF | ACVC | IC6 |
 * ----------------------------
 *
 * FF90:       Initialization Register 0 (INIT0)
 *
 *        BIT 7 = COCO        1 = Color Computer 1 and 2 Compatible
 *        BIT 6 = M/P         1 = MMU enabled
 *        BIT 5 = IEN         1 = Chip IRQ output enabled
 *        BIT 4 = FEN         1 = Chip FIRQ output enabled
 *        BIT 3 = MC3         1 = DRAM at XFEXX is constant
 *        BIT 2 = MC2         1 = Standard SCS
 *        BIT 1 = MC1         ROM map control (See table below)
 *        BIT 0 = MC0         ROM map control (See table below)
 *
 * -------------------------------------------------
 * | MC1 | MC0 | ROM mapping                       |
 * |  0  |  x  | 16K Internal, 16K External        |
 * |  1  |  0  | 32K Internal                      |
 * |  1  |  1  | 32K External (except for vectors) |
 * -------------------------------------------------
 *
 * -----------
 * | ROM Memory Mapping
 * |
 * |     Physical memory may be selected as all RAM or part RAM and ROM. The all-RAM mode is selected
 * | by setting the SAM TY bit. It is set by writing anything into dedicated address FFDF.
 * |
 * |     The ROM/RAM mode is selected by clearing the SAM TY bit. IT is cleared by writing anything
 * | into dedicated address FFDE. In the ROM/RAM mode the physical memory of pages 3C-3F are not RAM
 * | but ROM. The specific ROM assigned to these pages is controlled by bits 0 and 1 of dedicated
 * | address FF90. The three possible ROM configurations are shown in Fig. 3-5.
 * |
 * | Physical Page #        3C       3D         3E             3F           Bit 1    Bit 0
 * |                  --------------------------------------------------
 * |                  | Ext BASIC | BASIC |         Cart. ROM          |      0        x
 * |                  --------------------------------------------------
 * |                  | Ext BASIC | BASIC | Reset Init | Sup Ext BASIC |      1        0
 * |                  --------------------------------------------------
 * |                  |                 Cartridge ROM                  |      1        1
 * |                  --------------------------------------------------
 * |
 * | Fig. 3-5. Physical ROM Configurations
 * |
 * |
 * ---From "Assembly Language Programming for the CoCo3" by Laurence A. Tepolt
 *
 * FF91:       Initialization Register 1 (INIT1)
 *
 *        BIT 7   -
 *        BIT 6   -
 *        BIT 5 = TINS        Timer Input Select: 1 = 70 nsec / 0 = 63 µsec
 *        BIT 4   -
 *        BIT 3   -
 *        BIT 2   -
 *        BIT 1   -
 *        BIT 0 = TR          MMU Task Register Select
 *
 * FF92:       Interrupt Request Enable Register (IRQENR)
 *
 *        BIT 7   -
 *        BIT 6   -
 *        BIT 5 = TMR         Interrupt from Timer enabled
 *        BIT 4 = HBORD       Horizontal Border IRQ enabled
 *        BIT 3 = VBORD       Vertical Border IRQ enabled
 *        BIT 2 = EI2         Serial Data IRQ enabled
 *        BIT 1 = EI1         Keyboard IRQ enabled
 *        BIT 0 = EI0         Cartridge IRQ enabled
 *
 * FF93:       Fast Interrupt Request Enable Register (FIRQENR)
 *
 *        BIT 7   -
 *        BIT 6   -
 *        BIT 5 = TMR         Interrupt from Timer enabled
 *        BIT 4 = HBORD       Horizontal Border IRQ enabled
 *        BIT 3 = VBORD       Vertical Border IRQ enabled
 *        BIT 2 = EI2         Serial Data IRQ enabled
 *        BIT 1 = EI1         Keyboard IRQ enabled
 *        BIT 0 = EI0         Cartridge IRQ enabled
 *
 * FF94:       Timer Most Significant Nibble
 * FF95:       Timer Least Significant Byte
 *
 *        TIMER: This is a 12-bit interval timer. When a value is loaded into the MSB, the count is
 *               begun. The input clock is either 14 MHz or horizontal sync, as selected by TINS
 *               (bit 5 of FF91). As the count falls through zero, an interrupt is generated (if
 *               enabled), and the count is automatically reloaded.
 *
 * FF96:       Reserved
 * FF97:       Reserved
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
 *
 * FF99:       Video Resolution Register
 *
 *        BIT 7   -
 *        BIT 6 = LPF1        Lines per field (See table below)    [VRES1]
 *        BIT 5 = LPF0        Lines per field                      [VRES0]
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
 *
 * In addition to the above modes, the previous CoCo modes are available.
 *
 * -------------------------------------------------------------
 * |                          | MC6883 (SAM) |                 |
 * |                          | DISPLAY MODE |                 |
 * |                          |              | Reg. FF22       |
 * |                          |   V2 V1 V0   | 7  6  5  4  3   |
 * -------------------------------------------------------------
 * | Alphanumerics            |   0  0  0    | 0  X  X  0  CSS |
 * | Alphanumerics Inverted   |   0  0  0    | 0  X  X  0  CSS |
 * -------------------------------------------------------------
 * | Semigraphics - 4         |   0  0  0    | 0  X  X  0  X   |
 * -------------------------------------------------------------
 * | 64 x 64 Color Graphics   |   0  0  1    | 1  0  0  0  CSS |
 * -------------------------------------------------------------
 * | 128 x 64 Graphics        |   0  0  1    | 1  0  0  1  CSS |
 * | 128 x 64 Color Graphics  |   0  1  0    | 1  0  1  0  CSS |
 * -------------------------------------------------------------
 * | 128 x 96 Graphics        |   0  1  1    | 1  0  1  1  CSS |
 * | 128 x 96 Color Graphics  |   1  0  0    | 1  1  0  0  CSS |
 * -------------------------------------------------------------
 * | 128 x 192 Graphics       |   1  0  1    | 1  1  0  1  CSS |
 * | 128 x 192 Color Graphics |   1  1  0    | 1  1  1  0  CSS |
 * -------------------------------------------------------------
 * | 256 x 192 Graphics       |   1  1  0    | 1  1  1  1  CSS |
 * -------------------------------------------------------------
 *
 * FF9A:       Border Register (All bits are 0 for CoCo 1 and CoCo 2 compatibility)
 *
 *        BIT 7   -
 *        BIT 6   -
 *        BIT 5 = RED1        Most significant red bit
 *        BIT 4 = GRN1        Most significant green bit
 *        BIT 3 = BLU1        Most significant blue bit
 *        BIT 2 = RED0        Least significant red bit
 *        BIT 1 = GRN0        Least significant green bit
 *        BIT 0 = BLU0        Least significant blue bit
 *
 * FF9B:       Reserved
 *
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
 * FF9D:       Vertical Offset 1 Register
 *
 *        BIT 7 = Y18         (Vert. Offset)
 *        BIT 6 = Y17
 *        BIT 5 = Y16
 *        BIT 4 = Y15
 *        BIT 3 = Y14
 *        BIT 2 = Y13
 *        BIT 1 = Y12
 *        BIT 0 = Y11
 *
 * FF9E:       Vertical Offset 0 Register
 *
 *        BIT 7 = Y10         (Vert. Offset)
 *        BIT 6 = Y9
 *        BIT 5 = Y8
 *        BIT 4 = Y7
 *        BIT 3 = Y6
 *        BIT 2 = Y5
 *        BIT 1 = Y4
 *        BIT 0 = Y3
 *
 *        NOTE: In CoCo mode, Y15 - Y9 are not effective, and are controlled by SAM bits F6 - F0.
 *              Also in CoCo mode, Y18 - Y16 should be 1, all others 0.
 *
 * -------
 * |
 * | Vertical Offset Registers 0 & 1
 * |
 * |     The starting address of a buffer area is indicated to the ACVC via these registers. This is
 * | done by writing the upper sixteen bits (Y18-Y3) of the starting physical address into these
 * | registers. Y18-Y11 are written to vertical offset 1 and Y10-Y3 are written to vertical offset
 * | 0. Thus, a buffer is limited to starting on an 8-bit boundary, or the starting address is
 * | limited to the binary form: xxx xxxx xxxx xxxx x000
 * |
 * - From Assembly Language Programming For The COCO 3 by Laurence A. Tepolt
 *
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

#ifndef TCC1014_H
#define TCC1014_H

#include <Godot.hpp>
#include <Reference.hpp>
#include <Ref.hpp>

#include "Memory.hpp"
#include "PinsState.hpp"
#include "InitializationRegister0.hpp"
#include "InitializationRegister1.hpp"
#include "InterruptRequestEnableRegister.hpp"
#include "IntervalTimer.hpp"
#include "VideoModeRegister.hpp"
#include "VideoResolutionRegister.hpp"
#include "BorderRegister.hpp"
#include "VerticalScrollRegister.hpp"
#include "VerticalOffset.hpp"
#include "HorizontalOffsetRegister.hpp"
#include "MemoryManagementUnit.hpp"
#include "ColorPalette.hpp"
#include "Monitor.hpp"
#include "SAMControlRegisters.hpp"
#include "Mc6821_pia1.hpp"
#include "Mc6821_pia2.hpp"
#include "VDGControl.hpp"

namespace godot
{
	class Tcc1014 : public Reference
	{
		GODOT_CLASS(Tcc1014, Reference)

	private:
	public:
		static const int CONTROL_REGISTER_SIZE = 256;

		Ref<Memory> memory;
		Ref<PinsState> cpu_pins;
		Ref<InitializationRegister0> initialization_register_0;
		Ref<InitializationRegister1> initialization_register_1;
		Ref<InterruptRequestEnableRegister> interrupt_request_enable_register;
		Ref<InterruptRequestEnableRegister> fast_interrupt_request_enable_register;
		Ref<IntervalTimer> interrupt_timer;
		Ref<VideoModeRegister> video_mode_register;
		Ref<VideoResolutionRegister> video_resolution_register;
		Ref<BorderRegister> border_register;
		Ref<VerticalScrollRegister> vertical_scroll_register;
		Ref<VerticalOffset> vertical_offset_registers;
		Ref<HorizontalOffsetRegister> horizontal_offset_register;
		Ref<MemoryManagementUnit> memory_management_unit;
		Ref<ColorPalette> color_palette;
		Ref<Monitor> monitor;
		Ref<SAMControlRegisters> sam_control_registers;

		Ref<Mc6821_PIA1> pia1;
		Ref<Mc6821_PIA2> pia2;
		Ref<VDGControl> vdg_control_register;

		std::vector<std::uint8_t> control_registers;

		/**
		 * 1.6 68B09E Vector Registers
		 *
		 * ---------------------------
		 * | FFE0 - FFFF | CPU | IC1 |
		 * ---------------------------
		 *
		 *     FFFF: Reset vector LS
		 *     FFFE: Reset vector MS
		 *     FFFD: NMI vector LS
		 *     FFFC: NMI vector MS
		 *     FFFB: SWI1 vector LS
		 *     FFFA: SWI1 vector MS
		 *     FFF9: IRQ vector LS
		 *     FFF8: IRQ vector MS
		 *     FFF7: FIRQ vector LS
		 *     FFF6: FIRQ vector MS
		 *     FFF5: SWI2 vecor LS
		 *     FFF4: SWI2 vector MS
		 *     FFF3: SWI3 vector LS
		 *     FFF2: SWI3 vector MS
		 *     FFF1: Reserved
		 *     FFF0: Reserved
		 *     FFEF - FFE0: Not used
		 *
		 *     LS: Least significant address byte
		 *     MS: Most significant address byte
		 */

		enum ControlChipRegisters
		{
			INITIALIZATION_0 = 0x90,
			INITIALIZERION_1,
			INTERRUPT_REQUEST_ENABLE,
			FAST_INTERRUPT_REQUEST_ENABLE,
			TIMER_MOST_SIGNIFICANT_NIBBLE,
			TIMER_LEAST_SIGNIFICANT_BYTE,
			VIDEO_MODE = 0x98,
			VIDEO_RESOLUTION,
			BORDER,
			VERTICAL_SCROLL = 0x9C,
			VERTICAL_OFFSET_1,
			VERTICAL_OFFSET_0,
			HORIZONTAL_OFFSET_0,
			MMU_LOCATION_0 = 0xA0,
			MMU_LOCATION_1,
			MMU_LOCATION_2,
			MMU_LOCATION_3,
			MMU_LOCATION_4,
			MMU_LOCATION_5,
			MMU_LOCATION_6,
			MMU_LOCATION_7,
			MMU_LOCATION_8,
			MMU_LOCATION_9,
			MMU_LOCATION_10,
			MMU_LOCATION_11,
			MMU_LOCATION_12,
			MMU_LOCATION_13,
			MMU_LOCATION_14,
			MMU_LOCATION_15,
			PALETTE_0 = 0xB0,
			PALETTE_1,
			PALETTE_2,
			PALETTE_3,
			PALETTE_4,
			PALETTE_5,
			PALETTE_6,
			PALETTE_7,
			PALETTE_8,
			PALETTE_9,
			PALETTE_10,
			PALETTE_11,
			PALETTE_12,
			PALETTE_13,
			PALETTE_14,
			PALETTE_15,
			SAM_GRAPHICS_MODE_SELECT_V0_SET = 0xC0,
			SAM_GRAPHICS_MODE_SELECT_V0_CLEAR,
			SAM_GRAPHICS_MODE_SELECT_V1_SET,
			SAM_GRAPHICS_MODE_SELECT_V1_CLEAR,
			SAM_GRAPHICS_MODE_SELECT_V2_SET,
			SAM_GRAPHICS_MODE_SELECT_V2_CLEAR,
			SAM_VERTICAL_OFFSET_F0_SET,
			SAM_VERTICAL_OFFSET_F0_CLEAR,
			SAM_VERTICAL_OFFSET_F1_SET,
			SAM_VERTICAL_OFFSET_F1_CLEAR,
			SAM_VERTICAL_OFFSET_F2_SET,
			SAM_VERTICAL_OFFSET_F2_CLEAR,
			SAM_VERTICAL_OFFSET_F3_SET,
			SAM_VERTICAL_OFFSET_F3_CLEAR,
			SAM_VERTICAL_OFFSET_F4_SET,
			SAM_VERTICAL_OFFSET_F4_CLEAR,
			SAM_VERTICAL_OFFSET_F5_SET,
			SAM_VERTICAL_OFFSET_F5_CLEAR,
			SAM_VERTICAL_OFFSET_F6_SET,
			SAM_VERTICAL_OFFSET_F6_CLEAR,
			SAM_MPU_SPEED_SET,
			SAM_MPU_SPEED_CLEAR,
			SAM_ROM_DISABLE_SET,
			SAM_ROM_DISABLE_CLEAR
		};

		// C++ Specific Definitions

		Tcc1014();

		~Tcc1014();

		// Godot Specific Definitions

		static void _register_methods();

		void _init(); // our initializer called by Godot

		// Class Definitions

		void set_cpu_pins(Ref<PinsState> new_value);

		Ref<InitializationRegister0> get_initialization_register_0();
		Ref<InitializationRegister1> get_initialization_register_1();
		Ref<InterruptRequestEnableRegister> get_interrupt_request_enable_register();
		Ref<InterruptRequestEnableRegister> get_fast_interrupt_request_enable_register();
		Ref<IntervalTimer> get_interrupt_timer();
		Ref<VideoModeRegister> get_video_mode_register();
		Ref<VideoResolutionRegister> get_video_resolution_register();
		Ref<BorderRegister> get_border_register();
		Ref<VerticalScrollRegister> get_vertical_scroll_register();
		Ref<VerticalOffset> get_vertical_offset_registers();
		Ref<HorizontalOffsetRegister> get_horizontal_offset_register();
		Ref<MemoryManagementUnit> get_memory_management_unit();
		Ref<ColorPalette> get_color_palette();
		Ref<Monitor> get_monitor();
		Ref<SAMControlRegisters> get_sam_control_registers();
		Ref<Mc6821_PIA1> get_pia_1();
		Ref<Mc6821_PIA2> get_pia_2();
		Ref<VDGControl> get_vdg_control_register();

		void resize_memory(std::uint64_t new_size);

		void set_byte_at(std::uint16_t address, std::uint8_t data);
		std::uint8_t get_byte_at(std::uint16_t address);

		void set_bytes_at(std::uint16_t address, std::uint16_t data);
		std::uint16_t get_bytes_at(std::uint16_t address);

		void set_control_register(std::uint16_t address, std::uint8_t data);
		std::uint8_t get_control_register(std::uint16_t address);
	};

	inline void Tcc1014::set_byte_at(std::uint16_t address, std::uint8_t data)
	{
		if ((address & 0xFF00) != 0xFF00)
		{
			// we're accessing memory normally

			if (initialization_register_0->bits.mmu_enabled)
			{
				// Memory Management Unit enabled

				std::uint64_t real_address;

				real_address = memory_management_unit->map_address(
					initialization_register_1->bits.mmu_task_register_select,
					address);

				memory->set_memory_at(real_address, data);
			}
			else
			{
				memory->set_memory_at(address, data);
			}
		}
		else
		{
			// We're accessing reserved memory

			set_control_register(address, data);
		}
	}

	inline std::uint8_t Tcc1014::get_byte_at(std::uint16_t address)
	{
		if ((address & 0xFF00) != 0xFF00)
		{
			// we're accessing memory normally

			if (initialization_register_0->bits.mmu_enabled)
			{
				// Memory Management Unit enabled

				std::uint64_t real_address;

				real_address = memory_management_unit->map_address(
					initialization_register_1->bits.mmu_task_register_select,
					address);

				return memory->get_memory_at(real_address);
			}
			else
			{
				return memory->get_memory_at(address);
			}
		}
		else
		{
			// We're accessing reserved memory

			return get_control_register(address);
		}

		return 0;
	}

	inline void Tcc1014::set_bytes_at(std::uint16_t address, std::uint16_t data)
	{
		if (initialization_register_0->bits.mmu_enabled)
		{
			std::uint64_t real_address;

			real_address = memory_management_unit->map_address(
				initialization_register_1->bits.mmu_task_register_select,
				address);

			memory->set_memory_at(real_address, data >> 8);
			memory->set_memory_at(real_address + 1, data & 0xFF);
		}
		else
		{
			memory->set_memory_at(address, data >> 8);
			memory->set_memory_at(address + 1, data & 0xFF);
		}
	}

	inline std::uint16_t Tcc1014::get_bytes_at(std::uint16_t address)
	{
		if (initialization_register_0->bits.mmu_enabled)
		{
			std::uint64_t real_address;

			real_address = memory_management_unit->map_address(
				initialization_register_1->bits.mmu_task_register_select,
				address);

			return (memory->get_memory_at(real_address) << 8) | memory->get_memory_at(real_address + 1);
		}
		else
		{
			return (memory->get_memory_at(address) << 8) | memory->get_memory_at(address + 1);
		}
	}
}

#endif