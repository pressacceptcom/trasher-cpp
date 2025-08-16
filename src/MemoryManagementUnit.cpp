#include "MemoryManagementUnit.hpp"

using namespace godot;

MemoryManagementUnit::MemoryManagementUnit()
{
}

MemoryManagementUnit::~MemoryManagementUnit()
{
}

void MemoryManagementUnit::_register_methods()
{
	// Methods
	register_method("map_address", &MemoryManagementUnit::map_address);
	register_method("get_executive", &MemoryManagementUnit::get_executive);
	register_method("get_task", &MemoryManagementUnit::get_task);
}

void MemoryManagementUnit::_init()
{
	executive = Ref<PageAddressRegisterSet>(PageAddressRegisterSet::_new());
	task = Ref<PageAddressRegisterSet>(PageAddressRegisterSet::_new());
}

Ref<PageAddressRegisterSet> MemoryManagementUnit::get_executive()
{
	return executive;
}

Ref<PageAddressRegisterSet> MemoryManagementUnit::get_task()
{
	return task;
}

void MemoryManagementUnit::set_mask(std::uint16_t new_mask)
{
	executive->mask = new_mask;
	task->mask = new_mask;
}

void MemoryManagementUnit::set_register(bool select, std::uint8_t index, std::uint16_t new_value)
{
	if (select)
	{
		task->set_register(index, new_value);
	}
	else
	{
		executive->set_register(index, new_value);
	}
}

std::uint16_t MemoryManagementUnit::get_register(bool select, std::uint8_t index)
{
	if (select)
	{
		task->get_register(index);
	}
	else
	{
		executive->get_register(index);
	}

	return 0;
}

std::uint64_t MemoryManagementUnit::map_address(bool select, std::uint16_t address)
{
	if (select)
	{
		return task->map_address(address);
	}
	else
	{
		return executive->map_address(address);
	}
}
