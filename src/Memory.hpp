#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <vector>

#include <Godot.hpp>
#include <Reference.hpp>

namespace godot
{
	class Memory : public Reference
	{
		GODOT_CLASS(Memory, Reference)

	private:
	public:
		const static size_t EIGHT_MEGABYTES = 8388609;
		const static size_t FOUR_MEGABYTES = 4194305;
		const static size_t TWO_MEGABYTES = 2097153;
		const static size_t ONE_MEGABYTE = 1048577;
		const static size_t FIVE_TWELVE_K = 524289;

		std::vector<std::uint8_t> memory;

		// C++ Specific Definitions

		Memory();

		~Memory();

		// Godot Specific Definitions

		static void _register_methods();

		void _init(); // our initialize called by Godot

		// Class Specific Definitions
		void set_memory(std::int64_t start, Array new_data);
		Array get_memory(std::int64_t from, std::int64_t to);

		void set_memory_at(std::int64_t at, std::int8_t new_value);
		std::int64_t get_memory_at(std::int64_t at);

		std::vector<std::uint8_t> get_video_memory(size_t from, size_t to);
	};

	inline void Memory::set_memory_at(std::int64_t at, std::int8_t new_value)
	{
		if (at < memory.size())
		{
			memory[at] = new_value;
		}
	}

	inline std::int64_t Memory::get_memory_at(std::int64_t at)
	{
		if (at < memory.size())
		{
			return static_cast<std::int64_t>(memory[at]);
		}

		return 0;
	}
}

#endif