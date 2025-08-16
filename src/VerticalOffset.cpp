#include "VerticalOffset.hpp"

using namespace godot;

VerticalOffset::VerticalOffset()
{
}

VerticalOffset::~VerticalOffset()
{
}

void VerticalOffset::_register_methods()
{
	// Methods
	register_method("set_bit", &VerticalOffset::set_bit);
	register_method("get_bit", &VerticalOffset::get_bit);

	// Properties
	register_property<VerticalOffset, std::uint16_t>("registers", &VerticalOffset::set_registers, &VerticalOffset::get_registers, 0);

	// Signals
	register_signal<VerticalOffset>("registers_set", "new_value", GODOT_VARIANT_TYPE_INT);
}

void VerticalOffset::_init()
{
	vertical_offset_register_1 = Ref<VerticalOffsetRegister1>(VerticalOffsetRegister1::_new());
	vertical_offset_register_0 = Ref<VerticalOffsetRegister0>(VerticalOffsetRegister0::_new());

	vertical_offset_register_1->connect("register_set", this, "_on_vertical_offset_register_set");
	vertical_offset_register_0->connect("register_set", this, "_on_vertical_offset_register_set");
}

void VerticalOffset::_on_vertical_offset_register_set(std::int64_t new_value)
{
	emit_signal("registers_set", get_registers());
}

void VerticalOffset::set_registers(std::uint16_t new_value)
{
	vertical_offset_register_1->set_register(new_value >> 8);
	vertical_offset_register_0->set_register(new_value & 0xFF);
}

std::uint16_t VerticalOffset::get_registers()
{
	return (vertical_offset_register_1->byte << 8) | vertical_offset_register_0->byte;
}

void VerticalOffset::set_bit(int bit, bool new_value)
{
	switch (bit)
	{

	case VerticalOffsetRegister0::Bits::Y_3:
	case VerticalOffsetRegister0::Bits::Y_4:
	case VerticalOffsetRegister0::Bits::Y_5:
	case VerticalOffsetRegister0::Bits::Y_6:
	case VerticalOffsetRegister0::Bits::Y_7:
	case VerticalOffsetRegister0::Bits::Y_8:
	case VerticalOffsetRegister0::Bits::Y_9:
	case VerticalOffsetRegister0::Bits::Y_10:
		vertical_offset_register_0->set_bit(bit, new_value);
		break;

	case VerticalOffsetRegister1::Bits::Y_11:
	case VerticalOffsetRegister1::Bits::Y_12:
	case VerticalOffsetRegister1::Bits::Y_13:
	case VerticalOffsetRegister1::Bits::Y_14:
	case VerticalOffsetRegister1::Bits::Y_15:
	case VerticalOffsetRegister1::Bits::Y_16:
	case VerticalOffsetRegister1::Bits::Y_17:
	case VerticalOffsetRegister1::Bits::Y_18:
		vertical_offset_register_1->set_bit(bit, new_value);
		break;
	}
}

bool VerticalOffset::get_bit(int bit)
{
	switch (bit)
	{

	case VerticalOffsetRegister0::Bits::Y_3:
	case VerticalOffsetRegister0::Bits::Y_4:
	case VerticalOffsetRegister0::Bits::Y_5:
	case VerticalOffsetRegister0::Bits::Y_6:
	case VerticalOffsetRegister0::Bits::Y_7:
	case VerticalOffsetRegister0::Bits::Y_8:
	case VerticalOffsetRegister0::Bits::Y_9:
	case VerticalOffsetRegister0::Bits::Y_10:
		return vertical_offset_register_0->get_bit(bit);

	case VerticalOffsetRegister1::Bits::Y_11:
	case VerticalOffsetRegister1::Bits::Y_12:
	case VerticalOffsetRegister1::Bits::Y_13:
	case VerticalOffsetRegister1::Bits::Y_14:
	case VerticalOffsetRegister1::Bits::Y_15:
	case VerticalOffsetRegister1::Bits::Y_16:
	case VerticalOffsetRegister1::Bits::Y_17:
	case VerticalOffsetRegister1::Bits::Y_18:
		return vertical_offset_register_1->get_bit(bit);
	}

	return false;
}

std::uint32_t VerticalOffset::get_offset()
{
	return get_registers() << 3;
}
