#include "InitializationRegister1.hpp"

using namespace godot;

InitializationRegister1::InitializationRegister1() : byte{0}
{
}

InitializationRegister1::~InitializationRegister1()
{
}

void InitializationRegister1::_register_methods()
{
	// Methods
	register_method("set_bit", &InitializationRegister1::set_bit);
	register_method("get_bit", &InitializationRegister1::get_bit);

	// Properties
	register_property<InitializationRegister1, std::uint8_t>("register", &InitializationRegister1::set_register, &InitializationRegister1::get_register, false);
	register_property<InitializationRegister1, bool>("mmu_task_register_select", &InitializationRegister1::set_mmu_task_register_select, &InitializationRegister1::get_mmu_task_register_select, false);
	register_property<InitializationRegister1, bool>("timer_input_select", &InitializationRegister1::set_timer_input_select, &InitializationRegister1::get_timer_input_select, false);

	// Signals
	register_signal<InitializationRegister1>("register_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<InitializationRegister1>("mmu_task_register_select_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<InitializationRegister1>("timer_input_select_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
}

void InitializationRegister1::_init()
{
}

void InitializationRegister1::set_register(std::uint8_t new_value)
{
	if (byte != new_value)
	{
		bool old_mmu_task_register_select = bits.mmu_task_register_select;
		bool old_timer_input_select = bits.timer_input_select;

		byte = new_value;

		if (old_mmu_task_register_select != bits.mmu_task_register_select)
		{
			emit_signal("mmu_task_register_select_toggled", bits.mmu_task_register_select);
		}

		if (old_timer_input_select != bits.timer_input_select)
		{
			emit_signal("timer_input_select_toggled", bits.timer_input_select);
		}

		emit_signal("register_set", byte);
	}
}

std::uint8_t InitializationRegister1::get_register()
{
	return byte;
}

void InitializationRegister1::set_bit(int bit, bool new_value)
{
	switch (bit)
	{

	case Bits::MMU_TASK_REGISTER_SELECT:
		set_mmu_task_register_select(new_value);
		break;

	case Bits::TIMER_INPUT_SELECT:
		set_timer_input_select(new_value);
		break;
	}
}

bool InitializationRegister1::get_bit(int bit)
{
	switch (bit)
	{

	case Bits::MMU_TASK_REGISTER_SELECT:
		return bits.mmu_task_register_select;

	case Bits::TIMER_INPUT_SELECT:
		return bits.timer_input_select;
	}

	return false;
}

void InitializationRegister1::set_mmu_task_register_select(bool new_value)
{
	if (bits.mmu_task_register_select != new_value)
	{
		bits.mmu_task_register_select = new_value;
		emit_signal("mmu_task_register_select_toggled", new_value);
		emit_signal("register_set", byte);
	}
}

bool InitializationRegister1::get_mmu_task_register_select()
{
	return bits.mmu_task_register_select;
}

void InitializationRegister1::set_timer_input_select(bool new_value)
{
	if (bits.timer_input_select != new_value)
	{
		bits.timer_input_select = new_value;
		emit_signal("timer_input_select_toggled", new_value);
		emit_signal("register_set", byte);
	}
}

bool InitializationRegister1::get_timer_input_select()
{
	return bits.timer_input_select;
}
