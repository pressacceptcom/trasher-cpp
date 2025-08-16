#include "TRS80CoCo3.hpp"

using namespace godot;

TRS80CoCo3::TRS80CoCo3()
{
}

TRS80CoCo3::~TRS80CoCo3()
{
}

void TRS80CoCo3::_register_methods()
{
	// Methods
	register_method("get_memory", &TRS80CoCo3::get_memory);
	register_method("get_processor", &TRS80CoCo3::get_processor);
	register_method("get_tcc1014", &TRS80CoCo3::get_tcc1014);
	register_method("get_video_graphics", &TRS80CoCo3::get_video_graphics);
}

void TRS80CoCo3::_init()
{
	memory = Ref<Memory>(Memory::_new());

	tcc1014 = Ref<Tcc1014>(Tcc1014::_new());
	tcc1014->memory = memory;

	processor = Ref<Mc68b09e>(Mc68b09e::_new());

	processor->tcc1014 = tcc1014;
	tcc1014->set_cpu_pins(processor->pins_state);

	video_graphics = Ref<VideoGraphics>(VideoGraphics::_new());
	video_graphics->memory = memory;
	video_graphics->processor = processor;
	video_graphics->set_tcc1014(tcc1014);
	//	video_graphics->update_graphics_mode();
}

Ref<Mc68b09e> TRS80CoCo3::get_processor()
{
	return processor;
}

Ref<Memory> TRS80CoCo3::get_memory()
{
	return memory;
}

Ref<Tcc1014> TRS80CoCo3::get_tcc1014()
{
	return tcc1014;
}

Ref<VideoGraphics> TRS80CoCo3::get_video_graphics()
{
	return video_graphics;
}
