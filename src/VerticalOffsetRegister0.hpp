/**
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
 */

#ifndef VERTICALOFFSETREGISTER0_H
#define VERTICALOFFSETREGISTER0_H

#include <cstdint>

#include <Godot.hpp>
#include <Reference.hpp>

namespace godot
{
	class VerticalOffsetRegister0 : public Reference
	{
		GODOT_CLASS(VerticalOffsetRegister0, Reference)

	private:
	public:
		enum Bits
		{
			Y_3 = 0x01,
			Y_4 = 0x02,
			Y_5 = 0x04,
			Y_6 = 0x08,
			Y_7 = 0x10,
			Y_8 = 0x20,
			Y_9 = 0x40,
			Y_10 = 0x80
		};

		union
		{
			std::int8_t byte;
			struct
			{
				bool y_3 : 1;
				bool y_4 : 1;
				bool y_5 : 1;
				bool y_6 : 1;
				bool y_7 : 1;
				bool y_8 : 1;
				bool y_9 : 1;
				bool y_10 : 1;
			} bits;
		};

		// C++ Specific Definitions

		VerticalOffsetRegister0();

		~VerticalOffsetRegister0();

		// Godot Specific Definitions

		static void _register_methods();

		void _init(); // our initializer called by Godot

		// Class Definitions

		void set_register(std::uint8_t new_value);
		std::uint8_t get_register();

		void set_bit(int bit, bool new_value);
		bool get_bit(int bit);

		void set_y_3(bool new_value);
		bool get_y_3();

		void set_y_4(bool new_value);
		bool get_y_4();

		void set_y_5(bool new_value);
		bool get_y_5();

		void set_y_6(bool new_value);
		bool get_y_6();

		void set_y_7(bool new_value);
		bool get_y_7();

		void set_y_8(bool new_value);
		bool get_y_8();

		void set_y_9(bool new_value);
		bool get_y_9();

		void set_y_10(bool new_value);
		bool get_y_10();
	};
}

#endif