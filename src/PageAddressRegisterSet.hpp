/**
 * See Memory Management Unit for more information.
 */

#ifndef PAGEADDRESSREGISTERSET_H
#define PAGEADDRESSREGISTERSET_H

#include <cstdint>
#include <vector>

#include <Godot.hpp>
#include <Reference.hpp>

namespace godot
{
	class PageAddressRegisterSet : public Reference
	{
		GODOT_CLASS(PageAddressRegisterSet, Reference);

	private:
	public:
		const static size_t NUM_REGISTERS = 8;
		const static std::uint16_t DEFAULT_MASK = 0x3F;
		const static std::uint16_t TOP_ADDRESS_MASK = 0xE000;
		const static std::uint16_t BOTTOM_ADDRESS_MASK = 0x1FFF;

		std::uint16_t mask;
		std::vector<std::uint16_t> page_address_registers = {0, 0, 0, 0, 0, 0, 0, 0};

		// C++ Specific Definitions

		PageAddressRegisterSet();

		~PageAddressRegisterSet();

		// Godot Specific Definitions

		static void _register_methods();

		void _init(); // our initializer called by Godot

		// Class Specific Definitions

		void set_register(std::uint8_t index, std::uint16_t new_value);
		std::uint16_t get_register(std::uint8_t index);

		std::uint64_t map_address(std::uint16_t address);

		void set_register_0(std::uint16_t new_value);
		std::uint16_t get_register_0();

		void set_register_1(std::uint16_t new_value);
		std::uint16_t get_register_1();

		void set_register_2(std::uint16_t new_value);
		std::uint16_t get_register_2();

		void set_register_3(std::uint16_t new_value);
		std::uint16_t get_register_3();

		void set_register_4(std::uint16_t new_value);
		std::uint16_t get_register_4();

		void set_register_5(std::uint16_t new_value);
		std::uint16_t get_register_5();

		void set_register_6(std::uint16_t new_value);
		std::uint16_t get_register_6();

		void set_register_7(std::uint16_t new_value);
		std::uint16_t get_register_7();
	};
}

#endif
