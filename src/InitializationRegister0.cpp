#include "InitializationRegister0.hpp"

using namespace godot;

InitializationRegister0::InitializationRegister0() : byte{0}
{
}

InitializationRegister0::~InitializationRegister0()
{
}

void InitializationRegister0::_register_methods()
{
	// Methods
	register_method("set_bit", &InitializationRegister0::set_bit);
	register_method("get_bit", &InitializationRegister0::get_bit);

	// Properties
	register_property<InitializationRegister0, std::uint8_t>("register", &InitializationRegister0::set_register, &InitializationRegister0::get_register, false);
	register_property<InitializationRegister0, bool>("rom_map_control_0", &InitializationRegister0::set_rom_map_control_0, &InitializationRegister0::get_rom_map_control_0, false);
	register_property<InitializationRegister0, bool>("rom_map_control_1", &InitializationRegister0::set_rom_map_control_1, &InitializationRegister0::get_rom_map_control_1, false);
	register_property<InitializationRegister0, std::uint8_t>("rom_map_control", &InitializationRegister0::set_rom_map_control, &InitializationRegister0::get_rom_map_control, false);
	register_property<InitializationRegister0, bool>("standard_scs", &InitializationRegister0::set_standard_scs, &InitializationRegister0::get_standard_scs, false);
	register_property<InitializationRegister0, bool>("dram_constant", &InitializationRegister0::set_dram_constant, &InitializationRegister0::get_dram_constant, false);
	register_property<InitializationRegister0, bool>("firq_output_enabled", &InitializationRegister0::set_firq_output_enabled, &InitializationRegister0::get_firq_output_enabled, false);
	register_property<InitializationRegister0, bool>("irq_output_enabled", &InitializationRegister0::set_irq_output_enabled, &InitializationRegister0::get_irq_output_enabled, false);
	register_property<InitializationRegister0, bool>("mmu_enabled", &InitializationRegister0::set_mmu_enabled, &InitializationRegister0::get_mmu_enabled, false);
	register_property<InitializationRegister0, bool>("coco_compatibility_enabled", &InitializationRegister0::set_coco_compatibility_enabled, &InitializationRegister0::get_coco_compatibility_enabled, false);

	// Signals
	register_signal<InitializationRegister0>("register_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<InitializationRegister0>("rom_map_control_0_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<InitializationRegister0>("rom_map_control_1_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<InitializationRegister0>("rom_map_control_set", "new_value", GODOT_VARIANT_TYPE_INT);
	register_signal<InitializationRegister0>("standard_scs_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<InitializationRegister0>("dram_constant_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<InitializationRegister0>("firq_output_enabled_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<InitializationRegister0>("irq_output_enabled_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<InitializationRegister0>("mmu_enabled_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
	register_signal<InitializationRegister0>("coco_compatibility_enabled_toggled", "new_value", GODOT_VARIANT_TYPE_BOOL);
}

void InitializationRegister0::_init()
{
	// initialize any variables here
}

void InitializationRegister0::set_register(std::uint8_t new_value)
{
	if (byte != new_value)
	{
		bool old_rom_map_control_0 = bits.rom_map_control_0;
		bool old_rom_map_control_1 = bits.rom_map_control_1;
		std::uint8_t old_rom_map_control = get_rom_map_control();
		bool old_standard_scs = bits.standard_scs;
		bool old_dram_constant = bits.dram_constant;
		bool old_firq_output_enabled = bits.firq_output_enabled;
		bool old_irq_output_enabled = bits.irq_output_enabled;
		bool old_mmu_enabled = bits.mmu_enabled;
		bool old_coco_compatibility_enabled = bits.coco_compatibility_enabled;

		byte = new_value;

		if (old_rom_map_control != get_rom_map_control())
		{
			if (old_rom_map_control_0 != bits.rom_map_control_0)
			{
				emit_signal("rom_map_control_0_toggled", bits.rom_map_control_0);
			}

			if (old_rom_map_control_1 != bits.rom_map_control_1)
			{
				emit_signal("rom_map_control_1_toggled", bits.rom_map_control_1);
			}

			emit_signal("rom_map_control_set", get_rom_map_control());
		}

		if (old_standard_scs != bits.standard_scs)
		{
			emit_signal("standard_scs_toggled", bits.standard_scs);
		}

		if (old_dram_constant != bits.dram_constant)
		{
			emit_signal("dram_constant_toggled", bits.dram_constant);
		}

		if (old_firq_output_enabled != bits.firq_output_enabled)
		{
			emit_signal("firq_output_enabled_toggled", bits.firq_output_enabled);
		}

		if (old_irq_output_enabled != bits.irq_output_enabled)
		{
			emit_signal("irq_output_enabled_toggled", bits.irq_output_enabled);
		}

		if (old_mmu_enabled != bits.mmu_enabled)
		{
			emit_signal("mmu_enabled_toggled", bits.mmu_enabled);
		}

		if (old_coco_compatibility_enabled != bits.coco_compatibility_enabled)
		{
			emit_signal("coco_compatibility_enabled_toggled", bits.coco_compatibility_enabled);
		}

		emit_signal("register_set", byte);
	}
}

std::uint8_t InitializationRegister0::get_register()
{
	return byte;
}

void InitializationRegister0::set_bit(int bit, bool new_value)
{
	switch (bit)
	{

	case Bits::ROM_MAP_CONTROL_0:
		set_rom_map_control_0(new_value);
		break;

	case Bits::ROM_MAP_CONTROL_1:
		set_rom_map_control_1(new_value);
		break;

	case Bits::STANDARD_SCS:
		set_standard_scs(new_value);
		break;

	case Bits::DRAM_CONSTANT:
		set_dram_constant(new_value);
		break;

	case Bits::FIRQ_OUTPUT_ENABLED:
		set_firq_output_enabled(new_value);
		break;

	case Bits::IRQ_OUTPUT_ENABLED:
		set_irq_output_enabled(new_value);
		break;

	case Bits::MMU_ENABLED:
		set_mmu_enabled(new_value);
		break;

	case Bits::COCO_COMPATIBILITY_ENABLED:
		set_coco_compatibility_enabled(new_value);
		break;
	}
}

bool InitializationRegister0::get_bit(int bit)
{
	switch (bit)
	{

	case Bits::ROM_MAP_CONTROL_0:
		return bits.rom_map_control_0;

	case Bits::ROM_MAP_CONTROL_1:
		return bits.rom_map_control_1;

	case Bits::STANDARD_SCS:
		return bits.standard_scs;

	case Bits::DRAM_CONSTANT:
		return bits.dram_constant;

	case Bits::FIRQ_OUTPUT_ENABLED:
		return bits.firq_output_enabled;

	case Bits::IRQ_OUTPUT_ENABLED:
		return bits.irq_output_enabled;

	case Bits::MMU_ENABLED:
		return bits.mmu_enabled;

	case Bits::COCO_COMPATIBILITY_ENABLED:
		return bits.coco_compatibility_enabled;
	}

	return false;
}

void InitializationRegister0::set_rom_map_control_0(bool new_value)
{
	if (bits.rom_map_control_0 != new_value)
	{
		bits.rom_map_control_0 = new_value;
		emit_signal("rom_map_control_0_toggled", bits.rom_map_control_0);
		emit_signal("rom_map_control_set", get_rom_map_control());
		emit_signal("register_set", byte);
	}
}

bool InitializationRegister0::get_rom_map_control_0()
{
	return bits.rom_map_control_0;
}

void InitializationRegister0::set_rom_map_control_1(bool new_value)
{
	if (bits.rom_map_control_1 != new_value)
	{
		bits.rom_map_control_1 = new_value;
		emit_signal("rom_map_control_1_toggled", bits.rom_map_control_1);
		emit_signal("rom_map_control_set", get_rom_map_control());
		emit_signal("register_set", byte);
	}
}

bool InitializationRegister0::get_rom_map_control_1()
{
	return bits.rom_map_control_1;
}

void InitializationRegister0::set_rom_map_control(std::uint8_t new_value)
{
	if (get_rom_map_control() != new_value)
	{
		bool old_rom_map_control_0 = bits.rom_map_control_0;
		bool old_rom_map_control_1 = bits.rom_map_control_1;

		byte = (byte & 0xFC) | (new_value & 0x03);

		if (old_rom_map_control_0 != bits.rom_map_control_0)
		{
			emit_signal("rom_map_control_0_toggled", bits.rom_map_control_0);
		}

		if (old_rom_map_control_1 != bits.rom_map_control_1)
		{
			emit_signal("rom_map_control_1_toggled", bits.rom_map_control_1);
		}

		emit_signal("rom_map_control_set", get_rom_map_control());
		emit_signal("register_set", byte);
	}
}

std::uint8_t InitializationRegister0::get_rom_map_control()
{
	return byte & 0x03;
}

void InitializationRegister0::set_standard_scs(bool new_value)
{
	if (bits.standard_scs != new_value)
	{
		bits.standard_scs = new_value;
		emit_signal("standard_scs_toggled", new_value);
		emit_signal("register_set", byte);
	}
}

bool InitializationRegister0::get_standard_scs()
{
	return bits.standard_scs;
}

void InitializationRegister0::set_dram_constant(bool new_value)
{
	if (bits.dram_constant != new_value)
	{
		bits.dram_constant = new_value;
		emit_signal("dram_constant_toggled", new_value);
		emit_signal("register_set", byte);
	}
}

bool InitializationRegister0::get_dram_constant()
{
	return bits.dram_constant;
}

void InitializationRegister0::set_firq_output_enabled(bool new_value)
{
	if (bits.firq_output_enabled != new_value)
	{
		bits.firq_output_enabled = new_value;
		emit_signal("firq_output_enabled_toggled", new_value);
		emit_signal("register_set", byte);
	}
}

bool InitializationRegister0::get_firq_output_enabled()
{
	return bits.firq_output_enabled;
}

void InitializationRegister0::set_irq_output_enabled(bool new_value)
{
	if (bits.irq_output_enabled != new_value)
	{
		bits.irq_output_enabled = new_value;
		emit_signal("irq_output_enabled_toggled", new_value);
		emit_signal("register_set", byte);
	}
}

bool InitializationRegister0::get_irq_output_enabled()
{
	return bits.irq_output_enabled;
}

void InitializationRegister0::set_mmu_enabled(bool new_value)
{
	if (bits.mmu_enabled != new_value)
	{
		bits.mmu_enabled = new_value;
		emit_signal("mmu_enabled_toggled", new_value);
		emit_signal("register_set", byte);
	}
}

bool InitializationRegister0::get_mmu_enabled()
{
	return bits.mmu_enabled;
}

void InitializationRegister0::set_coco_compatibility_enabled(bool new_value)
{
	if (bits.coco_compatibility_enabled != new_value)
	{
		bits.coco_compatibility_enabled = new_value;
		emit_signal("coco_compatibility_enabled_toggled", new_value);
		emit_signal("register_set", byte);
	}
}

bool InitializationRegister0::get_coco_compatibility_enabled()
{
	return bits.coco_compatibility_enabled;
}
