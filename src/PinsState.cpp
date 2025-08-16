#include "PinsState.hpp"

using namespace godot;

void PinsState::_register_methods()
{
	// Methods
	register_method("set_pin", &PinsState::set_pin);
	register_method("get_pin", &PinsState::get_pin);

	// Properties
	register_property<PinsState, bool>("nmi", &PinsState::set_nmi, &PinsState::get_nmi, false);
	register_property<PinsState, bool>("irq", &PinsState::set_irq, &PinsState::get_irq, false);
	register_property<PinsState, bool>("firq", &PinsState::set_firq, &PinsState::get_firq, false);
	register_property<PinsState, bool>("bs", &PinsState::set_bs, &PinsState::get_bs, false);
	register_property<PinsState, bool>("ba", &PinsState::set_ba, &PinsState::get_ba, false);
	register_property<PinsState, bool>("reset", &PinsState::set_reset, &PinsState::get_reset, false);
	register_property<PinsState, bool>("halt", &PinsState::set_halt, &PinsState::get_nmi, false);
	register_property<PinsState, int>("state", &PinsState::set_state, &PinsState::get_state, false);

	// Signals
	register_signal<PinsState>("nmi_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<PinsState>("irq_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<PinsState>("firq_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<PinsState>("bs_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<PinsState>("ba_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<PinsState>("reset_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<PinsState>("halt_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
}

PinsState::PinsState()
{
}

PinsState::~PinsState()
{
}

void PinsState::_init()
{
	// initialize any variables here
}

void PinsState::set_pin(int pin, bool new_value)
{
	switch (pin)
	{
	case Pins::NMI:
		set_nmi(new_value);
		break;

	case Pins::IRQ:
		set_irq(new_value);
		break;

	case Pins::FIRQ:
		set_firq(new_value);
		break;

	case Pins::BS:
		set_bs(new_value);
		break;

	case Pins::BA:
		set_ba(new_value);
		break;

	case Pins::RESET:
		set_reset(new_value);
		break;

	case Pins::HALT:
		set_halt(new_value);
		break;
	}
}

bool PinsState::get_pin(int pin)
{
	switch (pin)
	{
	case Pins::NMI:
		return nmi;

	case Pins::IRQ:
		return irq;

	case Pins::FIRQ:
		return firq;

	case Pins::BS:
		return bs;

	case Pins::BA:
		return ba;

	case Pins::RESET:
		return reset;

	case Pins::HALT:
		return halt;
	}

	return false;
}

void PinsState::set_nmi(bool new_value)
{
	if (nmi != new_value)
	{
		nmi = new_value;
		if (debug)
		{
			emit_signal("nmi_toggled", new_value);
		}
	}
}

bool PinsState::get_nmi()
{
	return nmi;
}

void PinsState::set_irq(bool new_value)
{
	if (irq != new_value)
	{
		irq = new_value;
		if (debug)
		{
			emit_signal("irq_toggled", new_value);
		}
	}
}

bool PinsState::get_irq()
{
	return irq;
}

void PinsState::set_firq(bool new_value)
{
	if (firq != new_value)
	{
		firq = new_value;
		if (debug)
		{
			emit_signal("firq_toggled", new_value);
		}
	}
}

bool PinsState::get_firq()
{
	return firq;
}

void PinsState::set_bs(bool new_value)
{
	if (bs != new_value)
	{
		bs = new_value;
		if (debug)
		{
			emit_signal("bs_toggled", new_value);
		}
	}
}

bool PinsState::get_bs()
{
	return bs;
}

void PinsState::set_ba(bool new_value)
{
	if (ba != new_value)
	{
		ba = new_value;
		if (debug)
		{
			emit_signal("ba_toggled", new_value);
		}
	}
}

bool PinsState::get_ba()
{
	return ba;
}

void PinsState::set_reset(bool new_value)
{
	if (reset != new_value)
	{
		reset = new_value;
		if (debug)
		{
			emit_signal("reset_toggled", new_value);
		}
	}
}

bool PinsState::get_reset()
{
	return reset;
}

void PinsState::set_halt(bool new_value)
{
	if (halt != new_value)
	{
		halt = new_value;
		if (debug)
		{
			emit_signal("halt_toggled", new_value);
		}
	}
}

bool PinsState::get_halt()
{
	return halt;
}

void PinsState::set_state(int ignored)
{
}

int PinsState::get_state()
{
	if (bs)
	{
		if (ba)
		{
			return MPUState::HALT_ACKNOLWEDGE;
		}

		return MPUState::INTERRUPT_RESET_ACKNOWLEDGE;
	}

	if (ba)
	{
		return MPUState::SYNC_ACKNOWLEDGE;
	}

	return MPUState::NORMAL;
}
