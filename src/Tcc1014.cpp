#include "Tcc1014.hpp"

using namespace godot;

Tcc1014::Tcc1014()
{
	control_registers.resize(CONTROL_REGISTER_SIZE);
}

Tcc1014::~Tcc1014()
{
}

void Tcc1014::_register_methods()
{
	// Methods
	register_method("get_initialization_register_0", &Tcc1014::get_initialization_register_0);
	register_method("get_initialization_register_1", &Tcc1014::get_initialization_register_1);
	register_method("get_interrupt_request_enable_register", &Tcc1014::get_interrupt_request_enable_register);
	register_method("get_fast_interrupt_request_enable_register", &Tcc1014::get_fast_interrupt_request_enable_register);
	register_method("get_interrupt_timer", &Tcc1014::get_interrupt_timer);
	register_method("get_video_mode_register", &Tcc1014::get_video_mode_register);
	register_method("get_video_resolution_register", &Tcc1014::get_video_resolution_register);
	register_method("get_border_register", &Tcc1014::get_border_register);
	register_method("get_vertical_scroll_register", &Tcc1014::get_vertical_scroll_register);
	register_method("get_vertical_offset_registers", &Tcc1014::get_vertical_offset_registers);
	register_method("get_horizontal_offset_register", &Tcc1014::get_horizontal_offset_register);
	register_method("get_memory_management_unit", &Tcc1014::get_memory_management_unit);
	register_method("get_color_palette", &Tcc1014::get_color_palette);
	register_method("get_monitor", &Tcc1014::get_monitor);
	register_method("get_sam_control_registers", &Tcc1014::get_sam_control_registers);
	register_method("get_pia_1", &Tcc1014::get_pia_1);
	register_method("get_pia_2", &Tcc1014::get_pia_2);
	register_method("get_vdg_control_register", &Tcc1014::get_vdg_control_register);
}

void Tcc1014::_init()
{
	initialization_register_0 = Ref<InitializationRegister0>(InitializationRegister0::_new());
	initialization_register_1 = Ref<InitializationRegister1>(InitializationRegister1::_new());
	interrupt_request_enable_register = Ref<InterruptRequestEnableRegister>(InterruptRequestEnableRegister::_new());
	fast_interrupt_request_enable_register = Ref<InterruptRequestEnableRegister>(InterruptRequestEnableRegister::_new());
	interrupt_timer = Ref<IntervalTimer>(IntervalTimer::_new());
	video_mode_register = Ref<VideoModeRegister>(VideoModeRegister::_new());
	video_resolution_register = Ref<VideoResolutionRegister>(VideoResolutionRegister::_new());
	border_register = Ref<BorderRegister>(BorderRegister::_new());
	vertical_scroll_register = Ref<VerticalScrollRegister>(VerticalScrollRegister::_new());
	vertical_offset_registers = Ref<VerticalOffset>(VerticalOffset::_new());
	horizontal_offset_register = Ref<HorizontalOffsetRegister>(HorizontalOffsetRegister::_new());
	memory_management_unit = Ref<MemoryManagementUnit>(MemoryManagementUnit::_new());
	color_palette = Ref<ColorPalette>(ColorPalette::_new());
	monitor = Ref<Monitor>(Monitor::_new());
	sam_control_registers = Ref<SAMControlRegisters>(SAMControlRegisters::_new());

	vdg_control_register = Ref<VDGControl>(VDGControl::_new());

	pia1 = Ref<Mc6821_PIA1>(Mc6821_PIA1::_new());
	pia2 = Ref<Mc6821_PIA2>(Mc6821_PIA2::_new());

	pia2->vdg_control_register = vdg_control_register;
}

void Tcc1014::set_cpu_pins(Ref<PinsState> new_value)
{
	cpu_pins = new_value;
	pia1->mpu_pins_state = cpu_pins;
	pia2->mpu_pins_state = cpu_pins;
}

Ref<InitializationRegister0> Tcc1014::get_initialization_register_0()
{
	return initialization_register_0;
}

Ref<InitializationRegister1> Tcc1014::get_initialization_register_1()
{
	return initialization_register_1;
}

Ref<InterruptRequestEnableRegister> Tcc1014::get_interrupt_request_enable_register()
{
	return interrupt_request_enable_register;
}

Ref<InterruptRequestEnableRegister> Tcc1014::get_fast_interrupt_request_enable_register()
{
	return fast_interrupt_request_enable_register;
}

Ref<IntervalTimer> Tcc1014::get_interrupt_timer()
{
	return interrupt_timer;
}

Ref<VideoModeRegister> Tcc1014::get_video_mode_register()
{
	return video_mode_register;
}

Ref<VideoResolutionRegister> Tcc1014::get_video_resolution_register()
{
	return video_resolution_register;
}

Ref<BorderRegister> Tcc1014::get_border_register()
{
	return border_register;
}

Ref<VerticalScrollRegister> Tcc1014::get_vertical_scroll_register()
{
	return vertical_scroll_register;
}

Ref<VerticalOffset> Tcc1014::get_vertical_offset_registers()
{
	return vertical_offset_registers;
}

Ref<HorizontalOffsetRegister> Tcc1014::get_horizontal_offset_register()
{
	return horizontal_offset_register;
}

Ref<MemoryManagementUnit> Tcc1014::get_memory_management_unit()
{
	return memory_management_unit;
}

Ref<ColorPalette> Tcc1014::get_color_palette()
{
	return color_palette;
}

Ref<Monitor> Tcc1014::get_monitor()
{
	return monitor;
}

Ref<SAMControlRegisters> Tcc1014::get_sam_control_registers()
{
	return sam_control_registers;
}

Ref<Mc6821_PIA1> Tcc1014::get_pia_1()
{
	return pia1;
}

Ref<Mc6821_PIA2> Tcc1014::get_pia_2()
{
	return pia2;
}

Ref<VDGControl> Tcc1014::get_vdg_control_register()
{
	return vdg_control_register;
}

void Tcc1014::resize_memory(std::uint64_t new_size)
{
	// we can't be smaller than 512k
	if (new_size < Memory::FIVE_TWELVE_K)
	{
		new_size = Memory::FIVE_TWELVE_K;
	}

	if (new_size >= Memory::EIGHT_MEGABYTES)
	{
		if (memory_management_unit.is_valid())
		{
			memory_management_unit->set_mask(0x3FF);
		}
	}
	else if (new_size >= Memory::FOUR_MEGABYTES)
	{
		if (memory_management_unit.is_valid())
		{
			memory_management_unit->set_mask(0x1FF);
		}
	}
	else if (new_size >= Memory::TWO_MEGABYTES)
	{
		if (memory_management_unit.is_valid())
		{
			memory_management_unit->set_mask(0xFF);
		}
	}
	else if (new_size >= Memory::ONE_MEGABYTE)
	{
		if (memory_management_unit.is_valid())
		{
			memory_management_unit->set_mask(0x7F);
		}
	}
	else
	{
		if (memory_management_unit.is_valid())
		{
			memory_management_unit->set_mask(0x3F);
		}
	}
}

void Tcc1014::set_control_register(std::uint16_t address, std::uint8_t data)
{
	address &= 0xFF;

	switch (address)
	{

	case ControlChipRegisters::INITIALIZATION_0:
		initialization_register_0->set_register(data);
		break;

	case ControlChipRegisters::INITIALIZERION_1:
		initialization_register_1->set_register(data);
		break;

	case ControlChipRegisters::INTERRUPT_REQUEST_ENABLE:
		interrupt_request_enable_register->set_register(data);
		break;

	case ControlChipRegisters::FAST_INTERRUPT_REQUEST_ENABLE:
		fast_interrupt_request_enable_register->set_register(data);
		break;

	case ControlChipRegisters::TIMER_MOST_SIGNIFICANT_NIBBLE:
		interrupt_timer->set_most_significant_nibble(data);
		break;

	case ControlChipRegisters::TIMER_LEAST_SIGNIFICANT_BYTE:
		interrupt_timer->set_least_significant_byte(data);
		break;

	case ControlChipRegisters::VIDEO_MODE:
		video_mode_register->set_register(data);
		break;

	case ControlChipRegisters::VIDEO_RESOLUTION:
		video_resolution_register->set_register(data);
		break;

	case ControlChipRegisters::BORDER:
		border_register->set_register(data);
		break;

	case ControlChipRegisters::VERTICAL_SCROLL:
		vertical_scroll_register->set_register(data);
		break;

	case ControlChipRegisters::VERTICAL_OFFSET_1:
		vertical_offset_registers->vertical_offset_register_1->set_register(data);
		break;

	case ControlChipRegisters::VERTICAL_OFFSET_0:
		vertical_offset_registers->vertical_offset_register_0->set_register(data);
		break;

	case ControlChipRegisters::HORIZONTAL_OFFSET_0:
		horizontal_offset_register->set_register(data);
		break;

	case ControlChipRegisters::MMU_LOCATION_0:
		memory_management_unit->executive->set_register(0, data);
		break;

	case ControlChipRegisters::MMU_LOCATION_1:
		memory_management_unit->executive->set_register(1, data);
		break;

	case ControlChipRegisters::MMU_LOCATION_2:
		memory_management_unit->executive->set_register(2, data);
		break;

	case ControlChipRegisters::MMU_LOCATION_3:
		memory_management_unit->executive->set_register(3, data);
		break;

	case ControlChipRegisters::MMU_LOCATION_4:
		memory_management_unit->executive->set_register(4, data);
		break;

	case ControlChipRegisters::MMU_LOCATION_5:
		memory_management_unit->executive->set_register(5, data);
		break;

	case ControlChipRegisters::MMU_LOCATION_6:
		memory_management_unit->executive->set_register(6, data);
		break;

	case ControlChipRegisters::MMU_LOCATION_7:
		memory_management_unit->executive->set_register(7, data);
		break;

	case ControlChipRegisters::MMU_LOCATION_8:
		memory_management_unit->task->set_register(0, data);
		break;

	case ControlChipRegisters::MMU_LOCATION_9:
		memory_management_unit->task->set_register(1, data);
		break;

	case ControlChipRegisters::MMU_LOCATION_10:
		memory_management_unit->task->set_register(2, data);
		break;

	case ControlChipRegisters::MMU_LOCATION_11:
		memory_management_unit->task->set_register(3, data);
		break;

	case ControlChipRegisters::MMU_LOCATION_12:
		memory_management_unit->task->set_register(4, data);
		break;

	case ControlChipRegisters::MMU_LOCATION_13:
		memory_management_unit->task->set_register(5, data);
		break;

	case ControlChipRegisters::MMU_LOCATION_14:
		memory_management_unit->task->set_register(6, data);
		break;

	case ControlChipRegisters::MMU_LOCATION_15:
		memory_management_unit->task->set_register(7, data);
		break;

	case ControlChipRegisters::PALETTE_0:
		color_palette->set_palette_entry(0, data);
		break;

	case ControlChipRegisters::PALETTE_1:
		color_palette->set_palette_entry(1, data);
		break;

	case ControlChipRegisters::PALETTE_2:
		color_palette->set_palette_entry(2, data);
		break;

	case ControlChipRegisters::PALETTE_3:
		color_palette->set_palette_entry(3, data);
		break;

	case ControlChipRegisters::PALETTE_4:
		color_palette->set_palette_entry(4, data);
		break;

	case ControlChipRegisters::PALETTE_5:
		color_palette->set_palette_entry(5, data);
		break;

	case ControlChipRegisters::PALETTE_6:
		color_palette->set_palette_entry(6, data);
		break;

	case ControlChipRegisters::PALETTE_7:
		color_palette->set_palette_entry(7, data);
		break;

	case ControlChipRegisters::PALETTE_8:
		color_palette->set_palette_entry(8, data);
		break;

	case ControlChipRegisters::PALETTE_9:
		color_palette->set_palette_entry(9, data);
		break;

	case ControlChipRegisters::PALETTE_10:
		color_palette->set_palette_entry(10, data);
		break;

	case ControlChipRegisters::PALETTE_11:
		color_palette->set_palette_entry(11, data);
		break;

	case ControlChipRegisters::PALETTE_12:
		color_palette->set_palette_entry(12, data);
		break;

	case ControlChipRegisters::PALETTE_13:
		color_palette->set_palette_entry(13, data);
		break;

	case ControlChipRegisters::PALETTE_14:
		color_palette->set_palette_entry(14, data);
		break;

	case ControlChipRegisters::PALETTE_15:
		color_palette->set_palette_entry(15, data);
		break;

	case ControlChipRegisters::SAM_GRAPHICS_MODE_SELECT_V0_SET:
		sam_control_registers->set_v_0(true);
		break;

	case ControlChipRegisters::SAM_GRAPHICS_MODE_SELECT_V0_CLEAR:
		sam_control_registers->set_v_0(false);

	case ControlChipRegisters::SAM_GRAPHICS_MODE_SELECT_V1_SET:
		sam_control_registers->set_v_1(true);
		break;

	case ControlChipRegisters::SAM_GRAPHICS_MODE_SELECT_V1_CLEAR:
		sam_control_registers->set_v_1(false);
		break;

	case ControlChipRegisters::SAM_GRAPHICS_MODE_SELECT_V2_SET:
		sam_control_registers->set_v_2(true);
		break;

	case ControlChipRegisters::SAM_GRAPHICS_MODE_SELECT_V2_CLEAR:
		sam_control_registers->set_v_2(false);
		break;

	case ControlChipRegisters::SAM_VERTICAL_OFFSET_F0_SET:
		sam_control_registers->set_f_0(true);
		break;

	case ControlChipRegisters::SAM_VERTICAL_OFFSET_F0_CLEAR:
		sam_control_registers->set_f_0(false);
		break;

	case ControlChipRegisters::SAM_VERTICAL_OFFSET_F1_SET:
		sam_control_registers->set_f_1(true);
		break;

	case ControlChipRegisters::SAM_VERTICAL_OFFSET_F1_CLEAR:
		sam_control_registers->set_f_1(false);
		break;

	case ControlChipRegisters::SAM_VERTICAL_OFFSET_F2_SET:
		sam_control_registers->set_f_2(true);
		break;

	case ControlChipRegisters::SAM_VERTICAL_OFFSET_F2_CLEAR:
		sam_control_registers->set_f_2(false);
		break;

	case ControlChipRegisters::SAM_VERTICAL_OFFSET_F3_SET:
		sam_control_registers->set_f_3(true);
		break;

	case ControlChipRegisters::SAM_VERTICAL_OFFSET_F3_CLEAR:
		sam_control_registers->set_f_3(false);
		break;

	case ControlChipRegisters::SAM_VERTICAL_OFFSET_F4_SET:
		sam_control_registers->set_f_4(true);
		break;

	case ControlChipRegisters::SAM_VERTICAL_OFFSET_F4_CLEAR:
		sam_control_registers->set_f_4(false);
		break;

	case ControlChipRegisters::SAM_VERTICAL_OFFSET_F5_SET:
		sam_control_registers->set_f_5(true);
		break;

	case ControlChipRegisters::SAM_VERTICAL_OFFSET_F5_CLEAR:
		sam_control_registers->set_f_5(false);
		break;

	case ControlChipRegisters::SAM_VERTICAL_OFFSET_F6_SET:
		sam_control_registers->set_f_6(true);
		break;

	case ControlChipRegisters::SAM_VERTICAL_OFFSET_F6_CLEAR:
		sam_control_registers->set_f_6(false);
		break;

	case ControlChipRegisters::SAM_MPU_SPEED_SET:
		sam_control_registers->set_r1_cpu_rate(true);
		break;

	case ControlChipRegisters::SAM_MPU_SPEED_CLEAR:
		sam_control_registers->set_r1_cpu_rate(false);
		break;

	case ControlChipRegisters::SAM_ROM_DISABLE_SET:
		sam_control_registers->set_ty_rom_disable(true);
		break;

	case ControlChipRegisters::SAM_ROM_DISABLE_CLEAR:
		sam_control_registers->set_ty_rom_disable(false);
		break;

	default:
		control_registers[address] = data;
	}
}

std::uint8_t Tcc1014::get_control_register(std::uint16_t address)
{
	address &= 0xFF;

	switch (address)
	{

	case ControlChipRegisters::INITIALIZATION_0:
		return initialization_register_0->byte;

	case ControlChipRegisters::INITIALIZERION_1:
		return initialization_register_1->byte;

	case ControlChipRegisters::INTERRUPT_REQUEST_ENABLE:
	{
		std::uint8_t temp = interrupt_request_enable_register->byte;
		interrupt_request_enable_register->set_register(0);
		return temp;
	}

	case ControlChipRegisters::FAST_INTERRUPT_REQUEST_ENABLE:
	{
		std::uint8_t temp = fast_interrupt_request_enable_register->byte;
		fast_interrupt_request_enable_register->set_register(0);
		return temp;
	}

	case ControlChipRegisters::TIMER_MOST_SIGNIFICANT_NIBBLE:
		return interrupt_timer->get_most_significant_nibble();

	case ControlChipRegisters::TIMER_LEAST_SIGNIFICANT_BYTE:
		return interrupt_timer->get_least_significant_byte();

	case ControlChipRegisters::VIDEO_MODE:
		return video_mode_register->byte;

	case ControlChipRegisters::VIDEO_RESOLUTION:
		return video_resolution_register->byte;

	case ControlChipRegisters::BORDER:
		return border_register->byte;

	case ControlChipRegisters::VERTICAL_SCROLL:
		return vertical_scroll_register->byte;

	case ControlChipRegisters::VERTICAL_OFFSET_1:
		return vertical_offset_registers->vertical_offset_register_1->byte;

	case ControlChipRegisters::VERTICAL_OFFSET_0:
		return vertical_offset_registers->vertical_offset_register_0->byte;

	case ControlChipRegisters::HORIZONTAL_OFFSET_0:
		return horizontal_offset_register->byte;

	case ControlChipRegisters::MMU_LOCATION_0:
		return static_cast<std::uint8_t>(memory_management_unit->executive->get_register(0));

	case ControlChipRegisters::MMU_LOCATION_1:
		return static_cast<std::uint8_t>(memory_management_unit->executive->get_register(1));

	case ControlChipRegisters::MMU_LOCATION_2:
		return static_cast<std::uint8_t>(memory_management_unit->executive->get_register(2));

	case ControlChipRegisters::MMU_LOCATION_3:
		return static_cast<std::uint8_t>(memory_management_unit->executive->get_register(3));

	case ControlChipRegisters::MMU_LOCATION_4:
		return static_cast<std::uint8_t>(memory_management_unit->executive->get_register(4));

	case ControlChipRegisters::MMU_LOCATION_5:
		return static_cast<std::uint8_t>(memory_management_unit->executive->get_register(5));

	case ControlChipRegisters::MMU_LOCATION_6:
		return static_cast<std::uint8_t>(memory_management_unit->executive->get_register(6));

	case ControlChipRegisters::MMU_LOCATION_7:
		return static_cast<std::uint8_t>(memory_management_unit->executive->get_register(7));

	case ControlChipRegisters::MMU_LOCATION_8:
		return static_cast<std::uint8_t>(memory_management_unit->task->get_register(0));

	case ControlChipRegisters::MMU_LOCATION_9:
		return static_cast<std::uint8_t>(memory_management_unit->task->get_register(1));

	case ControlChipRegisters::MMU_LOCATION_10:
		return static_cast<std::uint8_t>(memory_management_unit->task->get_register(2));

	case ControlChipRegisters::MMU_LOCATION_11:
		return static_cast<std::uint8_t>(memory_management_unit->task->get_register(3));

	case ControlChipRegisters::MMU_LOCATION_12:
		return static_cast<std::uint8_t>(memory_management_unit->task->get_register(4));

	case ControlChipRegisters::MMU_LOCATION_13:
		return static_cast<std::uint8_t>(memory_management_unit->task->get_register(5));

	case ControlChipRegisters::MMU_LOCATION_14:
		return static_cast<std::uint8_t>(memory_management_unit->task->get_register(6));

	case ControlChipRegisters::MMU_LOCATION_15:
		return static_cast<std::uint8_t>(memory_management_unit->task->get_register(7));

	case ControlChipRegisters::PALETTE_0:
		return color_palette->get_palette_entry(0);

	case ControlChipRegisters::PALETTE_1:
		return color_palette->get_palette_entry(1);

	case ControlChipRegisters::PALETTE_2:
		return color_palette->get_palette_entry(2);

	case ControlChipRegisters::PALETTE_3:
		return color_palette->get_palette_entry(3);

	case ControlChipRegisters::PALETTE_4:
		return color_palette->get_palette_entry(4);

	case ControlChipRegisters::PALETTE_5:
		return color_palette->get_palette_entry(5);

	case ControlChipRegisters::PALETTE_6:
		return color_palette->get_palette_entry(6);

	case ControlChipRegisters::PALETTE_7:
		return color_palette->get_palette_entry(7);

	case ControlChipRegisters::PALETTE_8:
		return color_palette->get_palette_entry(8);

	case ControlChipRegisters::PALETTE_9:
		return color_palette->get_palette_entry(9);

	case ControlChipRegisters::PALETTE_10:
		return color_palette->get_palette_entry(10);

	case ControlChipRegisters::PALETTE_11:
		return color_palette->get_palette_entry(11);

	case ControlChipRegisters::PALETTE_12:
		return color_palette->get_palette_entry(12);

	case ControlChipRegisters::PALETTE_13:
		return color_palette->get_palette_entry(13);

	case ControlChipRegisters::PALETTE_14:
		return color_palette->get_palette_entry(14);

	case ControlChipRegisters::PALETTE_15:
		return color_palette->get_palette_entry(15);

	default:
		return control_registers[address];
	}
}
