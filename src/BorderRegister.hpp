/**
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
 */

#ifndef BORDERREGISTER_H
#define BORDERREGISTER_H

#include <cstdint>

#include <Godot.hpp>
#include <Reference.hpp>

namespace godot
{
	class BorderRegister : public Reference
	{
		GODOT_CLASS(BorderRegister, Reference)

	private:
	public:
		enum Bits
		{
			BLUE_0 = 0x01,
			GREEN_0 = 0x02,
			RED_0 = 0x04,
			BLUE_1 = 0x08,
			GREEN_1 = 0x10,
			RED_1 = 0x20
		};

		enum class Blue
		{
			BLUE_0 = 0x00,
			BLUE_1 = Bits::BLUE_0,
			BLUE_2 = Bits::BLUE_1,
			BLUE_3 = Bits::BLUE_0 | Bits::BLUE_1
		};

		enum class Green
		{
			GREEN_0 = 0x00,
			GREEN_1 = Bits::GREEN_0,
			GREEN_2 = Bits::GREEN_1,
			GREEN_3 = Bits::GREEN_0 | Bits::GREEN_1
		};

		enum class Red
		{
			RED_0 = 0x00,
			RED_1 = Bits::RED_0,
			RED_2 = Bits::RED_1,
			RED_3 = Bits::RED_0 | Bits::RED_1
		};

		union
		{
			std::uint8_t byte;
			struct
			{
				bool blue_0 : 1;
				bool green_0 : 1;
				bool red_0 : 1;
				bool blue_1 : 1;
				bool green_1 : 1;
				bool red_1 : 1;
			} bits;
		};

		// C++ Specific Definitions

		BorderRegister();

		~BorderRegister();

		// Godot Specific Definitions

		static void _register_methods();

		void _init(); // our initializer called by Godot

		// Class Definitions

		void set_register(std::uint8_t new_value);
		std::uint8_t get_register();

		void set_bit(int bit, bool new_value);
		bool get_bit(int bit);

		void set_blue_0(bool new_value);
		bool get_blue_0();

		void set_blue_1(bool new_value);
		bool get_blue_1();

		void set_blue(std::uint8_t new_value);
		std::uint8_t get_blue();

		void set_green_0(bool new_value);
		bool get_green_0();

		void set_green_1(bool new_value);
		bool get_green_1();

		void set_green(std::uint8_t new_value);
		std::uint8_t get_green();

		void set_red_0(bool new_value);
		bool get_red_0();

		void set_red_1(bool new_value);
		bool get_red_1();

		void set_red(std::uint8_t new_value);
		std::uint8_t get_red();
	};
}

#endif