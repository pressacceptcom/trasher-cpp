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
 */

#ifndef VERTICALOFFSET_H
#define VERTICALOFFSET_H

#include <cstdint>

#include <Godot.hpp>
#include <Reference.hpp>
#include <Ref.hpp>

#include "VerticalOffsetRegister1.hpp"
#include "VerticalOffsetRegister0.hpp"

namespace godot
{
	class VerticalOffset : public Reference
	{
		GODOT_CLASS(VerticalOffset, Reference)

	private:
	public:
		Ref<VerticalOffsetRegister1> vertical_offset_register_1;
		Ref<VerticalOffsetRegister0> vertical_offset_register_0;

		// C++ Specific Definitions

		VerticalOffset();

		~VerticalOffset();

		// Godot Specific Definitions

		static void _register_methods();

		void _init(); // our initializer called by Godot

		// Class Definitions

		void set_registers(std::uint16_t new_value);
		std::uint16_t get_registers();

		void set_bit(int bit, bool new_value);
		bool get_bit(int bit);

		std::uint32_t get_offset();

		void _on_vertical_offset_register_set(std::int64_t new_value);
	};
}

#endif