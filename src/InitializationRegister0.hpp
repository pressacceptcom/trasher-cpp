/**
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
 */

#ifndef INITIALIZATIONREGISTER0_H
#define INITIALIZATIONREGISTER0_H

#include <cstdint>

#include <Godot.hpp>
#include <Reference.hpp>

namespace godot
{
	class InitializationRegister0 : public Reference
	{
		GODOT_CLASS(InitializationRegister0, Reference)

	private:
	public:
		enum Bits
		{
			ROM_MAP_CONTROL_0 = 0x01,
			ROM_MAP_CONTROL_1 = 0x02,
			STANDARD_SCS = 0x04,
			DRAM_CONSTANT = 0x08,
			FIRQ_OUTPUT_ENABLED = 0x10,
			IRQ_OUTPUT_ENABLED = 0x20,
			MMU_ENABLED = 0x40,
			COCO_COMPATIBILITY_ENABLED = 0x80
		};

		union
		{
			std::uint8_t byte;
			struct
			{
				bool rom_map_control_0 : 1;
				bool rom_map_control_1 : 1;
				bool standard_scs : 1;
				bool dram_constant : 1;
				bool firq_output_enabled : 1;
				bool irq_output_enabled : 1;
				bool mmu_enabled : 1;
				bool coco_compatibility_enabled : 1;
			} bits;
		};

		// C++ Specific Definitions

		InitializationRegister0();

		~InitializationRegister0();

		// Godot Specific Definitions

		static void _register_methods();

		void _init(); // our initializer called by Godot

		// Class Definitions

		void set_register(std::uint8_t new_value);
		std::uint8_t get_register();

		void set_bit(int bit, bool new_value);
		bool get_bit(int bit);

		void set_rom_map_control_0(bool new_value);
		bool get_rom_map_control_0();

		void set_rom_map_control_1(bool new_value);
		bool get_rom_map_control_1();

		void set_rom_map_control(std::uint8_t new_value);
		std::uint8_t get_rom_map_control();

		void set_standard_scs(bool new_value);
		bool get_standard_scs();

		void set_dram_constant(bool new_value);
		bool get_dram_constant();

		void set_firq_output_enabled(bool new_value);
		bool get_firq_output_enabled();

		void set_irq_output_enabled(bool new_value);
		bool get_irq_output_enabled();

		void set_mmu_enabled(bool new_value);
		bool get_mmu_enabled();

		void set_coco_compatibility_enabled(bool new_value);
		bool get_coco_compatibility_enabled();
	};
}

#endif
