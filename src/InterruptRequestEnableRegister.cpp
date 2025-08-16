#include "InterruptRequestEnableRegister.hpp"

using namespace godot;

InterruptRequestEnableRegister::InterruptRequestEnableRegister() : byte{0}
{
}

InterruptRequestEnableRegister::~InterruptRequestEnableRegister()
{
}

void InterruptRequestEnableRegister::_register_methods()
{
	// Methods
	register_method("set_bit", &InterruptRequestEnableRegister::set_bit);
	register_method("get_bit", &InterruptRequestEnableRegister::get_bit);

	// Properties
	register_property<InterruptRequestEnableRegister, std::uint8_t>("register", &InterruptRequestEnableRegister::set_register, &InterruptRequestEnableRegister::get_register, 0);
	register_property<InterruptRequestEnableRegister, bool>("serial_data_irq_enabled", &InterruptRequestEnableRegister::set_serial_data_irq_enabled, &InterruptRequestEnableRegister::get_serial_data_irq_enabled, false);
	register_property<InterruptRequestEnableRegister, bool>("keyboard_irq_enabled", &InterruptRequestEnableRegister::set_keyboard_irq_enabled, &InterruptRequestEnableRegister::get_keyboard_irq_enabled, false);
	register_property<InterruptRequestEnableRegister, bool>("cartridge_irq_enabled", &InterruptRequestEnableRegister::set_cartridge_irq_enabled, &InterruptRequestEnableRegister::get_cartridge_irq_enabled, false);
	register_property<InterruptRequestEnableRegister, bool>("vertical_border_irq_enabled", &InterruptRequestEnableRegister::set_vertical_border_irq_enabled, &InterruptRequestEnableRegister::get_vertical_border_irq_enabled, false);
	register_property<InterruptRequestEnableRegister, bool>("horizontal_border_irq_enabled", &InterruptRequestEnableRegister::set_horizontal_border_irq_enabled, &InterruptRequestEnableRegister::get_horizontal_border_irq_enabled, false);
	register_property<InterruptRequestEnableRegister, bool>("timer_irq_enabled", &InterruptRequestEnableRegister::set_timer_irq_enabled, &InterruptRequestEnableRegister::get_timer_irq_enabled, false);

	// Signals
	register_signal<InterruptRequestEnableRegister>("register_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<InterruptRequestEnableRegister>("serial_data_irq_enabled_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<InterruptRequestEnableRegister>("keyboard_irq_enabled_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<InterruptRequestEnableRegister>("cartridge_irq_enabled_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<InterruptRequestEnableRegister>("vertical_border_irq_enabled_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<InterruptRequestEnableRegister>("horizontal_border_irq_enabled_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<InterruptRequestEnableRegister>("timer_irq_enabled_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
}

void InterruptRequestEnableRegister::_init()
{
}

void InterruptRequestEnableRegister::set_register(std::uint8_t new_value)
{
	if (byte != new_value)
	{
		bool old_serial_data_irq_enabled = bits.serial_data_irq_enabled;
		bool old_keyboard_irq_enabled = bits.keyboard_irq_enabled;
		bool old_cartridge_irq_enabled = bits.cartridge_irq_enabled;
		bool old_vertical_border_irq_enabled = bits.vertical_border_irq_enabled;
		bool old_horizontal_border_irq_enabled = bits.horizontal_border_irq_enabled;
		bool old_timer_irq_enabled = bits.timer_irq_enabled;

		byte = new_value;

		if (old_serial_data_irq_enabled != bits.serial_data_irq_enabled)
		{
			emit_signal("serial_data_irq_enabled_toggled", bits.serial_data_irq_enabled);
		}

		if (old_keyboard_irq_enabled != bits.keyboard_irq_enabled)
		{
			emit_signal("keyboard_irq_enabled_toggled", bits.keyboard_irq_enabled);
		}

		if (old_cartridge_irq_enabled != bits.cartridge_irq_enabled)
		{
			emit_signal("cartridge_irq_enabled_toggled", bits.cartridge_irq_enabled);
		}

		if (old_vertical_border_irq_enabled != bits.vertical_border_irq_enabled)
		{
			emit_signal("vertical_border_irq_enabled_toggled", bits.vertical_border_irq_enabled);
		}

		if (old_horizontal_border_irq_enabled != bits.horizontal_border_irq_enabled)
		{
			emit_signal("horizonal_border_irq_enabled_toggled", bits.horizontal_border_irq_enabled);
		}

		if (old_timer_irq_enabled != bits.timer_irq_enabled)
		{
			emit_signal("timer_irq_enabled_toggled", bits.timer_irq_enabled);
		}

		emit_signal("register_set", byte);
	}
}

std::uint8_t InterruptRequestEnableRegister::get_register()
{
	return byte;
}

void InterruptRequestEnableRegister::set_bit(int bit, bool new_value)
{
	switch (bit)
	{

	case Bits::SERIAL_DATA_IRQ_ENABLED:
		set_serial_data_irq_enabled(new_value);
		break;

	case Bits::KEYBOARD_IRQ_ENABLED:
		set_keyboard_irq_enabled(new_value);
		break;

	case Bits::CARTRIDGE_IRQ_ENABLED:
		set_cartridge_irq_enabled(new_value);
		break;

	case Bits::VERTICAL_BORDER_IRQ_ENABLED:
		set_vertical_border_irq_enabled(new_value);
		break;

	case Bits::HORIZONTAL_BORDER_IRQ_ENABLED:
		set_horizontal_border_irq_enabled(new_value);
		break;

	case Bits::TIMER_IRQ_ENABLED:
		set_timer_irq_enabled(new_value);
		break;
	}
}

bool InterruptRequestEnableRegister::get_bit(int bit)
{
	switch (bit)
	{

	case Bits::SERIAL_DATA_IRQ_ENABLED:
		return bits.serial_data_irq_enabled;

	case Bits::KEYBOARD_IRQ_ENABLED:
		return bits.keyboard_irq_enabled;

	case Bits::CARTRIDGE_IRQ_ENABLED:
		return bits.cartridge_irq_enabled;

	case Bits::VERTICAL_BORDER_IRQ_ENABLED:
		return bits.vertical_border_irq_enabled;

	case Bits::HORIZONTAL_BORDER_IRQ_ENABLED:
		return bits.horizontal_border_irq_enabled;

	case Bits::TIMER_IRQ_ENABLED:
		return bits.timer_irq_enabled;
	}

	return false;
}

void InterruptRequestEnableRegister::set_serial_data_irq_enabled(bool new_value)
{
	if (bits.serial_data_irq_enabled != new_value)
	{
		bits.serial_data_irq_enabled = new_value;
		emit_signal("serial_data_irq_enabled_toggled", bits.serial_data_irq_enabled);
		emit_signal("register_set", byte);
	}
}

bool InterruptRequestEnableRegister::get_serial_data_irq_enabled()
{
	return bits.serial_data_irq_enabled;
}

void InterruptRequestEnableRegister::set_keyboard_irq_enabled(bool new_value)
{
	if (bits.keyboard_irq_enabled != new_value)
	{
		bits.keyboard_irq_enabled = new_value;
		emit_signal("keyboard_irq_enabled_toggled", bits.keyboard_irq_enabled);
		emit_signal("register_set", byte);
	}
}

bool InterruptRequestEnableRegister::get_keyboard_irq_enabled()
{
	return bits.keyboard_irq_enabled;
}

void InterruptRequestEnableRegister::set_cartridge_irq_enabled(bool new_value)
{
	if (bits.cartridge_irq_enabled != new_value)
	{
		bits.cartridge_irq_enabled = new_value;
		emit_signal("cartrdige_irq_enabled_toggled", bits.cartridge_irq_enabled);
		emit_signal("register_set", byte);
	}
}

bool InterruptRequestEnableRegister::get_cartridge_irq_enabled()
{
	return bits.cartridge_irq_enabled;
}

void InterruptRequestEnableRegister::set_vertical_border_irq_enabled(bool new_value)
{
	if (bits.vertical_border_irq_enabled != new_value)
	{
		bits.vertical_border_irq_enabled = new_value;
		emit_signal("vertical_border_irq_enabled_toggled", bits.vertical_border_irq_enabled);
		emit_signal("register_set", byte);
	}
}

bool InterruptRequestEnableRegister::get_vertical_border_irq_enabled()
{
	return bits.vertical_border_irq_enabled;
}

void InterruptRequestEnableRegister::set_horizontal_border_irq_enabled(bool new_value)
{
	if (bits.horizontal_border_irq_enabled != new_value)
	{
		bits.horizontal_border_irq_enabled = new_value;
		emit_signal("horizontal_border_irq_enabled_toggled", bits.horizontal_border_irq_enabled);
		emit_signal("register_set", byte);
	}
}

bool InterruptRequestEnableRegister::get_horizontal_border_irq_enabled()
{
	return bits.horizontal_border_irq_enabled;
}

void InterruptRequestEnableRegister::set_timer_irq_enabled(bool new_value)
{
	if (bits.timer_irq_enabled != new_value)
	{
		bits.timer_irq_enabled = new_value;
		emit_signal("timer_irq_enabled_toggled", bits.timer_irq_enabled);
		emit_signal("register_set", byte);
	}
}

bool InterruptRequestEnableRegister::get_timer_irq_enabled()
{
	return bits.timer_irq_enabled;
}
