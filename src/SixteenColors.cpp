#include "SixteenColors.hpp"

using namespace godot;

SixteenColors::SixteenColors() : byte{0}
{
}

SixteenColors::~SixteenColors()
{
}

void SixteenColors::_register_methods()
{
	// Properties
	register_property<SixteenColors, std::uint8_t>("byte", &SixteenColors::set_byte, &SixteenColors::get_byte, 0);
	register_property<SixteenColors, bool>("palette_0_second_pixel", &SixteenColors::set_palette_0_second_pixel, &SixteenColors::get_palette_0_second_pixel, false);
	register_property<SixteenColors, bool>("palette_1_second_pixel", &SixteenColors::set_palette_1_second_pixel, &SixteenColors::get_palette_1_second_pixel, false);
	register_property<SixteenColors, bool>("palette_2_second_pixel", &SixteenColors::set_palette_2_second_pixel, &SixteenColors::get_palette_2_second_pixel, false);
	register_property<SixteenColors, bool>("palette_3_second_pixel", &SixteenColors::set_palette_3_second_pixel, &SixteenColors::get_palette_3_second_pixel, false);
	register_property<SixteenColors, std::uint8_t>("palette_second_pixel", &SixteenColors::set_palette_second_pixel, &SixteenColors::get_palette_second_pixel, 0);
	register_property<SixteenColors, bool>("palette_0_first_pixel", &SixteenColors::set_palette_0_first_pixel, &SixteenColors::get_palette_0_first_pixel, false);
	register_property<SixteenColors, bool>("palette_1_first_pixel", &SixteenColors::set_palette_1_first_pixel, &SixteenColors::get_palette_1_first_pixel, false);
	register_property<SixteenColors, bool>("palette_2_first_pixel", &SixteenColors::set_palette_2_first_pixel, &SixteenColors::get_palette_2_first_pixel, false);
	register_property<SixteenColors, bool>("palette_3_first_pixel", &SixteenColors::set_palette_3_first_pixel, &SixteenColors::get_palette_3_first_pixel, false);
	register_property<SixteenColors, std::uint8_t>("palette_first_pixel", &SixteenColors::set_palette_first_pixel, &SixteenColors::get_palette_first_pixel, 0);
}

void SixteenColors::_init()
{
}

void SixteenColors::set_byte(std::uint8_t new_value)
{
	byte = new_value;
}

std::uint8_t SixteenColors::get_byte()
{
	return byte;
}

void SixteenColors::set_palette_0_second_pixel(bool new_value)
{
	bits.palette_0_second_pixel = new_value;
}

bool SixteenColors::get_palette_0_second_pixel()
{
	return bits.palette_0_second_pixel;
}

void SixteenColors::set_palette_1_second_pixel(bool new_value)
{
	bits.palette_1_second_pixel = new_value;
}

bool SixteenColors::get_palette_1_second_pixel()
{
	return bits.palette_1_second_pixel;
}

void SixteenColors::set_palette_2_second_pixel(bool new_value)
{
	bits.palette_2_second_pixel = new_value;
}

bool SixteenColors::get_palette_2_second_pixel()
{
	return bits.palette_2_second_pixel;
}

void SixteenColors::set_palette_3_second_pixel(bool new_value)
{
	bits.palette_3_second_pixel = new_value;
}

bool SixteenColors::get_palette_3_second_pixel()
{
	return bits.palette_3_second_pixel;
}

void SixteenColors::set_palette_second_pixel(std::uint8_t new_value)
{
	palette_index.second_pixel = new_value;
}

std::uint8_t SixteenColors::get_palette_second_pixel()
{
	return palette_index.second_pixel;
}

void SixteenColors::set_palette_0_first_pixel(bool new_value)
{
	bits.palette_0_first_pixel = new_value;
}

bool SixteenColors::get_palette_0_first_pixel()
{
	return bits.palette_0_first_pixel;
}

void SixteenColors::set_palette_1_first_pixel(bool new_value)
{
	bits.palette_1_first_pixel = new_value;
}

bool SixteenColors::get_palette_1_first_pixel()
{
	return bits.palette_1_first_pixel;
}

void SixteenColors::set_palette_2_first_pixel(bool new_value)
{
	bits.palette_2_first_pixel = new_value;
}

bool SixteenColors::get_palette_2_first_pixel()
{
	return bits.palette_2_first_pixel;
}

void SixteenColors::set_palette_3_first_pixel(bool new_value)
{
	bits.palette_3_first_pixel = new_value;
}

bool SixteenColors::get_palette_3_first_pixel()
{
	return bits.palette_3_first_pixel;
}

void SixteenColors::set_palette_first_pixel(std::uint8_t new_value)
{
	palette_index.first_pixel = new_value;
}

std::uint8_t SixteenColors::get_palette_first_pixel()
{
	return palette_index.first_pixel;
}
