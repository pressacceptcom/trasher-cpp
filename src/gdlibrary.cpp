#include "Memory.hpp"
#include "InitializationRegister0.hpp"
#include "InitializationRegister1.hpp"
#include "InterruptRequestEnableRegister.hpp"
#include "IntervalTimer.hpp"
#include "VideoModeRegister.hpp"
#include "VideoResolutionRegister.hpp"
#include "BorderRegister.hpp"
#include "VerticalScrollRegister.hpp"
#include "VerticalOffsetRegister1.hpp"
#include "VerticalOffsetRegister0.hpp"
#include "VerticalOffset.hpp"
#include "HorizontalOffsetRegister.hpp"
#include "PageAddressRegisterSet.hpp"
#include "MemoryManagementUnit.hpp"
#include "PaletteEntry.hpp"
#include "ColorPalette.hpp"
#include "RGBSourceColorPalette.hpp"
#include "CompositeSourceColorPalette.hpp"
#include "Monitor.hpp"
#include "SAMControlRegisters.hpp"
#include "Mc6821_pia1.hpp"
#include "Mc6821_pia2.hpp"
#include "VDGControl.hpp"
#include "Tcc1014.hpp"
#include "Character.hpp"
#include "Attribute.hpp"
#include "Alphanumeric.hpp"
#include "SixteenColors.hpp"
#include "FourColors.hpp"
#include "TwoColors.hpp"
#include "ColorPaletteData.hpp"
#include "VideoGraphics.hpp"
#include "PinsState.hpp"
#include "ConditionCodeRegister.hpp"
#include "Mc68b09e.hpp"
#include "TRS80CoCo3.hpp"

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o)
{
	godot::Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o)
{
	godot::Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle)
{
	godot::Godot::nativescript_init(handle);

	godot::register_class<godot::Memory>();

	godot::register_class<godot::InitializationRegister0>();
	godot::register_class<godot::InitializationRegister1>();
	godot::register_class<godot::InterruptRequestEnableRegister>();
	godot::register_class<godot::IntervalTimer>();
	godot::register_class<godot::VideoModeRegister>();
	godot::register_class<godot::VideoResolutionRegister>();
	godot::register_class<godot::BorderRegister>();
	godot::register_class<godot::VerticalScrollRegister>();
	godot::register_class<godot::VerticalOffsetRegister1>();
	godot::register_class<godot::VerticalOffsetRegister0>();
	godot::register_class<godot::VerticalOffset>();
	godot::register_class<godot::HorizontalOffsetRegister>();
	godot::register_class<godot::PageAddressRegisterSet>();
	godot::register_class<godot::MemoryManagementUnit>();
	godot::register_class<godot::PaletteEntry>();
	godot::register_class<godot::ColorPalette>();
	godot::register_class<godot::RGBSourceColorPalette>();
	godot::register_class<godot::CompositeSourceColorPalette>();
	godot::register_class<godot::Monitor>();
	godot::register_class<godot::SAMControlRegisters>();
	godot::register_class<godot::Mc6821_PIA1>();
	godot::register_class<godot::Mc6821_PIA2>();
	godot::register_class<godot::VDGControl>();
	godot::register_class<godot::Tcc1014>();

	godot::register_class<godot::Character>();
	godot::register_class<godot::Attribute>();
	godot::register_class<godot::Alphanumeric>();
	godot::register_class<godot::SixteenColors>();
	godot::register_class<godot::FourColors>();
	godot::register_class<godot::TwoColors>();
	godot::register_class<godot::ColorPaletteData>();

	godot::register_class<godot::VideoGraphics>();

	godot::register_class<godot::PinsState>();
	godot::register_class<godot::ConditionCodeRegister>();
	godot::register_class<godot::Mc68b09e>();

	godot::register_class<godot::TRS80CoCo3>();
}