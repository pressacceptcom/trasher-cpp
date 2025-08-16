#include "Attribute.hpp"

using namespace godot;

Attribute::Attribute() : byte{0}
{
}

Attribute::~Attribute()
{
}

void Attribute::_register_methods()
{
	// Properties
	register_property<Attribute, std::uint8_t>("byte", &Attribute::set_byte, &Attribute::get_byte, 0);
	register_property<Attribute, bool>("background_0", &Attribute::set_background_0, &Attribute::get_background_0, false);
	register_property<Attribute, bool>("background_1", &Attribute::set_background_1, &Attribute::get_background_1, false);
	register_property<Attribute, bool>("background_2", &Attribute::set_background_2, &Attribute::get_background_2, false);
	register_property<Attribute, std::uint8_t>("background", &Attribute::set_background, &Attribute::get_background, 0);
	register_property<Attribute, bool>("foreground_0", &Attribute::set_foreground_0, &Attribute::get_foreground_0, false);
	register_property<Attribute, bool>("foreground_1", &Attribute::set_foreground_1, &Attribute::get_foreground_1, false);
	register_property<Attribute, bool>("foreground_2", &Attribute::set_foreground_2, &Attribute::get_foreground_2, false);
	register_property<Attribute, std::uint8_t>("foreground", &Attribute::set_foreground, &Attribute::get_foreground, 0);
	register_property<Attribute, bool>("underline", &Attribute::set_underline, &Attribute::get_underline, false);
	register_property<Attribute, bool>("blink", &Attribute::set_blink, &Attribute::get_blink, false);
}

void Attribute::_init()
{
}

void Attribute::set_byte(std::uint8_t new_value)
{
	byte = new_value;
}

std::uint8_t Attribute::get_byte()
{
	return byte;
}

void Attribute::set_background_0(bool new_value)
{
	bits.background_0 = new_value;
}

bool Attribute::get_background_0()
{
	return bits.background_0;
}

void Attribute::set_background_1(bool new_value)
{
	bits.background_1 = new_value;
}

bool Attribute::get_background_1()
{
	return bits.background_1;
}

void Attribute::set_background_2(bool new_value)
{
	bits.background_2 = new_value;
}

bool Attribute::get_background_2()
{
	return bits.background_2;
}

void Attribute::set_background(std::uint8_t new_value)
{
	byte = (byte & (255 - BACKGROUND_MASK)) | (new_value & BACKGROUND_MASK);
}

std::uint8_t Attribute::get_background()
{
	return byte & BACKGROUND_MASK;
}

void Attribute::set_foreground_0(bool new_value)
{
	bits.foreground_0 = new_value;
}

bool Attribute::get_foreground_0()
{
	return bits.foreground_0;
}

void Attribute::set_foreground_1(bool new_value)
{
	bits.foreground_1 = new_value;
}

bool Attribute::get_foreground_1()
{
	return bits.foreground_1;
}

void Attribute::set_foreground_2(bool new_value)
{
	bits.foreground_2 = new_value;
}

bool Attribute::get_foreground_2()
{
	return bits.foreground_2;
}

void Attribute::set_foreground(std::uint8_t new_value)
{
	byte = (byte & (255 - FOREGROUND_MASK)) | (new_value & FOREGROUND_MASK);
}

std::uint8_t Attribute::get_foreground()
{
	return byte & FOREGROUND_MASK;
}

void Attribute::set_underline(bool new_value)
{
	bits.underline = new_value;
}

bool Attribute::get_underline()
{
	return bits.underline;
}

void Attribute::set_blink(bool new_value)
{
	bits.blink = new_value;
}

bool Attribute::get_blink()
{
	return bits.blink;
}
