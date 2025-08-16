#include "Memory.hpp"

using namespace godot;

Memory::Memory()
{
}

Memory::~Memory()
{
}

void Memory::_register_methods()
{
	// Methods
	register_method("set_memory", &Memory::set_memory);
	register_method("get_memory", &Memory::get_memory);
	register_method("set_memory_at", &Memory::set_memory_at);
	register_method("get_memory_at", &Memory::get_memory_at);
}

void Memory::_init()
{
	// initialize memory space to a default size
	memory.resize(FIVE_TWELVE_K);
}

void Memory::set_memory(std::int64_t start, Array new_data)
{
	std::int64_t new_data_size = new_data.size();

	if ((start + new_data_size) >= memory.size())
	{
		return;
	}

	for (size_t i = start; i < new_data_size + start; i++)
	{
		memory[i] = new_data[i - start];
	}
}

Array Memory::get_memory(std::int64_t from, std::int64_t to)
{
	Array ret;

	if (to < from)
	{
		return ret;
	}

	if (from >= memory.size())
	{
		return ret;
	}

	ret.resize(to - from);

	for (size_t i = from; i < to; i++)
	{
		ret[i - from] = memory[i];
	}

	return ret;
}

std::vector<std::uint8_t> Memory::get_video_memory(size_t from, size_t to)
{
	std::vector<std::uint8_t> ret;

	if (to < from)
	{
		return ret;
	}

	ret.resize(to - from);

	for (size_t i = from; i < to; i++)
	{
		if (i >= memory.size())
		{
			ret[i - from] = 0;
		}
		else
		{
			ret[i - from] = memory[i];
		}
	}

	return ret;
}