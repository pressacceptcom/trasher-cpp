#include "ConditionCodeRegister.hpp"

using namespace godot;

void ConditionCodeRegister::_register_methods()
{
	// Methods
	register_method("set_bit", &ConditionCodeRegister::set_bit);
	register_method("get_bit", &ConditionCodeRegister::get_bit);

	// Properties
	register_property<ConditionCodeRegister, std::uint8_t>("register", &ConditionCodeRegister::set_register, &ConditionCodeRegister::get_register, 0);
	register_property<ConditionCodeRegister, bool>("carry", &ConditionCodeRegister::set_carry, &ConditionCodeRegister::get_carry, false);
	register_property<ConditionCodeRegister, bool>("overflow", &ConditionCodeRegister::set_overflow, &ConditionCodeRegister::get_overflow, false);
	register_property<ConditionCodeRegister, bool>("zero", &ConditionCodeRegister::set_zero, &ConditionCodeRegister::get_zero, false);
	register_property<ConditionCodeRegister, bool>("negative", &ConditionCodeRegister::set_negative, &ConditionCodeRegister::get_negative, false);
	register_property<ConditionCodeRegister, bool>("irq_mask", &ConditionCodeRegister::set_irq_mask, &ConditionCodeRegister::get_irq_mask, false);
	register_property<ConditionCodeRegister, bool>("half_carry", &ConditionCodeRegister::set_half_carry, &ConditionCodeRegister::get_half_carry, false);
	register_property<ConditionCodeRegister, bool>("firq_mask", &ConditionCodeRegister::set_firq_mask, &ConditionCodeRegister::get_firq_mask, false);
	register_property<ConditionCodeRegister, bool>("entire", &ConditionCodeRegister::set_entire, &ConditionCodeRegister::get_entire, false);

	// Signals
	register_signal<ConditionCodeRegister>("register_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<ConditionCodeRegister>("carry_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<ConditionCodeRegister>("overflow_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<ConditionCodeRegister>("zero_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<ConditionCodeRegister>("negative_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<ConditionCodeRegister>("irq_mask_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<ConditionCodeRegister>("half_carry_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<ConditionCodeRegister>("firq_mask_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<ConditionCodeRegister>("entire_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
}

ConditionCodeRegister::ConditionCodeRegister() : slot{0}
{
}

ConditionCodeRegister::~ConditionCodeRegister()
{
}

void ConditionCodeRegister::_init()
{
	// initialize any variables here
}

void ConditionCodeRegister::set_register(std::uint8_t new_register)
{
	if (slot != new_register)
	{
		bool old_carry = bits.carry;
		bool old_overflow = bits.overflow;
		bool old_zero = bits.zero;
		bool old_negative = bits.negative;
		bool old_irq_mask = bits.irq_mask;
		bool old_half_carry = bits.half_carry;
		bool old_firq_mask = bits.firq_mask;
		bool old_entire = bits.entire;

		slot = new_register;

		if (!debug)
		{
			return;
		}

		if (old_carry != bits.carry)
		{
			emit_signal("carry_toggled", bits.carry);
		}

		if (old_overflow != bits.overflow)
		{
			emit_signal("overflow_toggled", bits.overflow);
		}

		if (old_zero != bits.zero)
		{
			emit_signal("zero_toggled", bits.zero);
		}

		if (old_negative != bits.negative)
		{
			emit_signal("negative_toggled", bits.negative);
		}

		if (old_irq_mask != bits.irq_mask)
		{
			emit_signal("irq_mask_toggled", bits.irq_mask);
		}

		if (old_half_carry != bits.half_carry)
		{
			emit_signal("half_carry_toggled", bits.half_carry);
		}

		if (old_firq_mask != bits.firq_mask)
		{
			emit_signal("firq_mask_toggled", bits.firq_mask);
		}

		if (old_entire != bits.entire)
		{
			emit_signal("entire_toggled", bits.entire);
		}

		emit_signal("register_set", slot);
	}
}

void ConditionCodeRegister::or_register(std::uint8_t or_value)
{
	bool old_carry = bits.carry;
	bool old_overflow = bits.overflow;
	bool old_zero = bits.zero;
	bool old_negative = bits.negative;
	bool old_irq_mask = bits.irq_mask;
	bool old_half_carry = bits.half_carry;
	bool old_firq_mask = bits.firq_mask;
	bool old_entire = bits.entire;

	slot |= or_value;

	if (!debug)
	{
		return;
	}

	if (old_carry != bits.carry)
	{
		emit_signal("carry_toggled", bits.carry);
	}

	if (old_overflow != bits.overflow)
	{
		emit_signal("overflow_toggled", bits.overflow);
	}

	if (old_zero != bits.zero)
	{
		emit_signal("zero_toggled", bits.zero);
	}

	if (old_negative != bits.negative)
	{
		emit_signal("negative_toggled", bits.negative);
	}

	if (old_irq_mask != bits.irq_mask)
	{
		emit_signal("irq_mask_toggled", bits.irq_mask);
	}

	if (old_half_carry != bits.half_carry)
	{
		emit_signal("half_carry_toggled", bits.half_carry);
	}

	if (old_firq_mask != bits.firq_mask)
	{
		emit_signal("firq_mask_toggled", bits.firq_mask);
	}

	if (old_entire != bits.entire)
	{
		emit_signal("entire_toggled", bits.entire);
	}

	emit_signal("register_set", slot);
}

void ConditionCodeRegister::and_register(std::uint8_t and_value)
{
	bool old_carry = bits.carry;
	bool old_overflow = bits.overflow;
	bool old_zero = bits.zero;
	bool old_negative = bits.negative;
	bool old_irq_mask = bits.irq_mask;
	bool old_half_carry = bits.half_carry;
	bool old_firq_mask = bits.firq_mask;
	bool old_entire = bits.entire;

	slot &= and_value;

	if (!debug)
	{
		return;
	}

	if (old_carry != bits.carry)
	{
		emit_signal("carry_toggled", bits.carry);
	}

	if (old_overflow != bits.overflow)
	{
		emit_signal("overflow_toggled", bits.overflow);
	}

	if (old_zero != bits.zero)
	{
		emit_signal("zero_toggled", bits.zero);
	}

	if (old_negative != bits.negative)
	{
		emit_signal("negative_toggled", bits.negative);
	}

	if (old_irq_mask != bits.irq_mask)
	{
		emit_signal("irq_mask_toggled", bits.irq_mask);
	}

	if (old_half_carry != bits.half_carry)
	{
		emit_signal("half_carry_toggled", bits.half_carry);
	}

	if (old_firq_mask != bits.firq_mask)
	{
		emit_signal("firq_mask_toggled", bits.firq_mask);
	}

	if (old_entire != bits.entire)
	{
		emit_signal("entire_toggled", bits.entire);
	}

	emit_signal("register_set", slot);
}

std::uint8_t ConditionCodeRegister::get_register()
{
	return slot;
}

void ConditionCodeRegister::set_bit(int bit, bool new_value)
{
	switch (bit)
	{
	case ConditionCode::C_CARRY:
		set_carry(new_value);
		break;

	case ConditionCode::V_OVERFLOW:
		set_overflow(new_value);
		break;

	case ConditionCode::Z_ZERO:
		set_zero(new_value);
		break;

	case ConditionCode::N_NEGATIVE:
		set_negative(new_value);
		break;

	case ConditionCode::I_IRQ_MASK:
		set_irq_mask(new_value);
		break;

	case ConditionCode::H_HALF_CARRY:
		set_half_carry(new_value);
		break;

	case ConditionCode::F_FIRQ_MASK:
		set_firq_mask(new_value);
		break;

	case ConditionCode::E_ENTIRE:
		set_entire(new_value);
		break;
	}
}

bool ConditionCodeRegister::get_bit(int bit)
{
	switch (bit)
	{
	case ConditionCode::C_CARRY:
		return bits.carry;

	case ConditionCode::V_OVERFLOW:
		return bits.overflow;

	case ConditionCode::Z_ZERO:
		return bits.zero;

	case ConditionCode::N_NEGATIVE:
		return bits.negative;

	case ConditionCode::I_IRQ_MASK:
		return bits.irq_mask;

	case ConditionCode::H_HALF_CARRY:
		return bits.half_carry;

	case ConditionCode::F_FIRQ_MASK:
		return bits.firq_mask;

	case ConditionCode::E_ENTIRE:
		return bits.entire;
	}

	return false;
}

void ConditionCodeRegister::set_carry(bool new_value)
{
	if (bits.carry != new_value)
	{
		bits.carry = new_value;
		if (debug)
		{
			emit_signal("carry_toggled", new_value);
			emit_signal("register_set", slot);
		}
	}
}

bool ConditionCodeRegister::get_carry()
{
	return bits.carry;
}

void ConditionCodeRegister::set_overflow(bool new_value)
{
	if (bits.overflow != new_value)
	{
		bits.overflow = new_value;
		if (debug)
		{
			emit_signal("overflow_toggled", new_value);
			emit_signal("register_set", slot);
		}
	}
}

bool ConditionCodeRegister::get_overflow()
{
	return bits.overflow;
}

void ConditionCodeRegister::set_zero(bool new_value)
{
	if (bits.zero != new_value)
	{
		bits.zero = new_value;
		if (debug)
		{
			emit_signal("zero_toggled", new_value);
			emit_signal("register_set", slot);
		}
	}
}

bool ConditionCodeRegister::get_zero()
{
	return bits.zero;
}

void ConditionCodeRegister::set_negative(bool new_value)
{
	if (bits.negative != new_value)
	{
		bits.negative = new_value;
		if (debug)
		{
			emit_signal("negative_toggled", new_value);
			emit_signal("register_set", slot);
		}
	}
}

bool ConditionCodeRegister::get_negative()
{
	return bits.negative;
}

void ConditionCodeRegister::set_irq_mask(bool new_value)
{
	if (bits.irq_mask != new_value)
	{
		bits.irq_mask = new_value;
		if (debug)
		{
			emit_signal("irq_mask_toggled", new_value);
			emit_signal("register_set", slot);
		}
	}
}

bool ConditionCodeRegister::get_irq_mask()
{
	return bits.irq_mask;
}

void ConditionCodeRegister::set_half_carry(bool new_value)
{
	if (bits.half_carry != new_value)
	{
		bits.half_carry = new_value;
		if (debug)
		{
			emit_signal("half_carry_toggled", new_value);
			emit_signal("register_set", slot);
		}
	}
}

bool ConditionCodeRegister::get_half_carry()
{
	return bits.half_carry;
}

void ConditionCodeRegister::set_firq_mask(bool new_value)
{
	if (bits.firq_mask != new_value)
	{
		bits.firq_mask = new_value;
		if (debug)
		{
			emit_signal("firq_mask_toggled", new_value);
			emit_signal("register_set", slot);
		}
	}
}

bool ConditionCodeRegister::get_firq_mask()
{
	return bits.firq_mask;
}

void ConditionCodeRegister::set_entire(bool new_value)
{
	if (bits.entire != new_value)
	{
		bits.entire = new_value;
		if (debug)
		{
			emit_signal("entire_toggled", new_value);
			emit_signal("register_set", slot);
		}
	}
}

bool ConditionCodeRegister::get_entire()
{
	return bits.entire;
}
