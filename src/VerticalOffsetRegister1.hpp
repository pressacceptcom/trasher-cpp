/**
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
 */

#ifndef VERTICALOFFSETREGISTER1_H
#define VERTICALOFFSETREGISTER1_H

#include <cstdint>

#include <Godot.hpp>
#include <Reference.hpp>

namespace godot
{
	class VerticalOffsetRegister1 : public Reference
	{
		GODOT_CLASS(VerticalOffsetRegister1, Reference)

	private:
	public:
		enum Bits
		{
			Y_11 = 0x0100,
			Y_12 = 0x0200,
			Y_13 = 0x0400,
			Y_14 = 0x0800,
			Y_15 = 0x1000,
			Y_16 = 0x2000,
			Y_17 = 0x4000,
			Y_18 = 0x8000
		};

		union
		{
			std::int8_t byte;
			struct
			{
				bool y_11 : 1;
				bool y_12 : 1;
				bool y_13 : 1;
				bool y_14 : 1;
				bool y_15 : 1;
				bool y_16 : 1;
				bool y_17 : 1;
				bool y_18 : 1;
			} bits;
		};

		// C++ Specific Definitions

		VerticalOffsetRegister1();

		~VerticalOffsetRegister1();

		// Godot Specific Definitions

		static void _register_methods();

		void _init(); // our initializer called by Godot

		// Class Definitions

		void set_register(std::uint8_t new_value);
		std::uint8_t get_register();

		void set_bit(int bit, bool new_value);
		bool get_bit(int bit);

		void set_y_11(bool new_value);
		bool get_y_11();

		void set_y_12(bool new_value);
		bool get_y_12();

		void set_y_13(bool new_value);
		bool get_y_13();

		void set_y_14(bool new_value);
		bool get_y_14();

		void set_y_15(bool new_value);
		bool get_y_15();

		void set_y_16(bool new_value);
		bool get_y_16();

		void set_y_17(bool new_value);
		bool get_y_17();

		void set_y_18(bool new_value);
		bool get_y_18();
	};
}

#endif