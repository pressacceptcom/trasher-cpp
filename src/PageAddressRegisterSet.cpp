#include "PageAddressRegisterSet.hpp"

using namespace godot;

PageAddressRegisterSet::PageAddressRegisterSet()
{
}

PageAddressRegisterSet::~PageAddressRegisterSet()
{
}

void PageAddressRegisterSet::_register_methods()
{
	// Methods
	register_method("map_address", &PageAddressRegisterSet::map_address);

	// Properties
	register_property<PageAddressRegisterSet, std::uint16_t>("register_0", &PageAddressRegisterSet::set_register_0, &PageAddressRegisterSet::get_register_0, 0);
	register_property<PageAddressRegisterSet, std::uint16_t>("register_1", &PageAddressRegisterSet::set_register_1, &PageAddressRegisterSet::get_register_1, 0);
	register_property<PageAddressRegisterSet, std::uint16_t>("register_2", &PageAddressRegisterSet::set_register_2, &PageAddressRegisterSet::get_register_2, 0);
	register_property<PageAddressRegisterSet, std::uint16_t>("register_3", &PageAddressRegisterSet::set_register_3, &PageAddressRegisterSet::get_register_3, 0);
	register_property<PageAddressRegisterSet, std::uint16_t>("register_4", &PageAddressRegisterSet::set_register_4, &PageAddressRegisterSet::get_register_4, 0);
	register_property<PageAddressRegisterSet, std::uint16_t>("register_5", &PageAddressRegisterSet::set_register_5, &PageAddressRegisterSet::get_register_5, 0);
	register_property<PageAddressRegisterSet, std::uint16_t>("register_6", &PageAddressRegisterSet::set_register_6, &PageAddressRegisterSet::get_register_6, 0);
	register_property<PageAddressRegisterSet, std::uint16_t>("register_7", &PageAddressRegisterSet::set_register_7, &PageAddressRegisterSet::get_register_7, 0);

	// Signals
	register_signal<PageAddressRegisterSet>("register_set", "index", GODOT_VARIANT_TYPE_INT, "new_value", GODOT_VARIANT_TYPE_INT);
}

void PageAddressRegisterSet::_init()
{
	// initialize any variables here
}

void PageAddressRegisterSet::set_register(std::uint8_t index, std::uint16_t new_value)
{
	if (index < NUM_REGISTERS)
	{
		if (page_address_registers[index] != new_value)
		{
			page_address_registers[index] = new_value;
			emit_signal("register_set", index, new_value);
		}
	}
}

std::uint16_t PageAddressRegisterSet::get_register(std::uint8_t index)
{
	if (index < NUM_REGISTERS)
	{
		return page_address_registers[index];
	}

	return 0;
}

std::uint64_t PageAddressRegisterSet::map_address(std::uint16_t address)
{
	std::uint8_t index = (address & TOP_ADDRESS_MASK) >> 5;
	return ((page_address_registers[index] & mask) << 13) | (address & BOTTOM_ADDRESS_MASK);
}

void PageAddressRegisterSet::set_register_0(std::uint16_t new_value)
{
	set_register(0, new_value);
}

std::uint16_t PageAddressRegisterSet::get_register_0()
{
	return get_register(0);
}

void PageAddressRegisterSet::set_register_1(std::uint16_t new_value)
{
	set_register(1, new_value);
}

std::uint16_t PageAddressRegisterSet::get_register_1()
{
	return get_register(1);
}

void PageAddressRegisterSet::set_register_2(std::uint16_t new_value)
{
	set_register(2, new_value);
}

std::uint16_t PageAddressRegisterSet::get_register_2()
{
	return get_register(2);
}

void PageAddressRegisterSet::set_register_3(std::uint16_t new_value)
{
	set_register(3, new_value);
}

std::uint16_t PageAddressRegisterSet::get_register_3()
{
	return get_register(3);
}

void PageAddressRegisterSet::set_register_4(std::uint16_t new_value)
{
	set_register(4, new_value);
}

std::uint16_t PageAddressRegisterSet::get_register_4()
{
	return get_register(4);
}

void PageAddressRegisterSet::set_register_5(std::uint16_t new_value)
{
	set_register(5, new_value);
}

std::uint16_t PageAddressRegisterSet::get_register_5()
{
	return get_register(5);
}

void PageAddressRegisterSet::set_register_6(std::uint16_t new_value)
{
	set_register(6, new_value);
}

std::uint16_t PageAddressRegisterSet::get_register_6()
{
	return get_register(6);
}

void PageAddressRegisterSet::set_register_7(std::uint16_t new_value)
{
	set_register(7, new_value);
}

std::uint16_t PageAddressRegisterSet::get_register_7()
{
	return get_register(7);
}
