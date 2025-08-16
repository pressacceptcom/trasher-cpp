#include "FourColors.hpp"

using namespace godot;

FourColors::FourColors() : byte{0}
{
}

FourColors::~FourColors()
{
}

void FourColors::_register_methods()
{
	// Properties
	register_property<FourColors, std::uint8_t>("byte", &FourColors::set_byte, &FourColors::get_byte, 0);
	register_property<FourColors, bool>("palette_0_fourth_pixel", &FourColors::set_palette_0_fourth_pixel, &FourColors::get_palette_0_fourth_pixel, false);
	register_property<FourColors, bool>("palette_1_fourth_pixel", &FourColors::set_palette_1_fourth_pixel, &FourColors::get_palette_1_fourth_pixel, false);
	register_property<FourColors, std::uint8_t>("palette_fourth_pixel", &FourColors::set_palette_fourth_pixel, &FourColors::get_palette_fourth_pixel, 0);
	register_property<FourColors, bool>("palette_0_third_pixel", &FourColors::set_palette_0_third_pixel, &FourColors::get_palette_0_third_pixel, false);
	register_property<FourColors, bool>("palette_1_third_pixel", &FourColors::set_palette_1_third_pixel, &FourColors::get_palette_1_third_pixel, false);
	register_property<FourColors, std::uint8_t>("palette_third_pixel", &FourColors::set_palette_third_pixel, &FourColors::get_palette_third_pixel, 0);
	register_property<FourColors, bool>("palette_0_second_pixel", &FourColors::set_palette_0_second_pixel, &FourColors::get_palette_0_second_pixel, false);
	register_property<FourColors, bool>("palette_1_second_pixel", &FourColors::set_palette_1_second_pixel, &FourColors::get_palette_1_second_pixel, false);
	register_property<FourColors, std::uint8_t>("palette_second_pixel", &FourColors::set_palette_second_pixel, &FourColors::get_palette_second_pixel, 0);
	register_property<FourColors, bool>("palette_0_first_pixel", &FourColors::set_palette_0_first_pixel, &FourColors::get_palette_0_first_pixel, false);
	register_property<FourColors, bool>("palette_1_first_pixel", &FourColors::set_palette_1_first_pixel, &FourColors::get_palette_1_first_pixel, false);
	register_property<FourColors, std::uint8_t>("palette_first_pixel", &FourColors::set_palette_first_pixel, &FourColors::get_palette_first_pixel, 0);
}

void FourColors::_init()
{
}

void FourColors::set_byte(std::uint8_t new_value)
{
	byte = new_value;
}

std::uint8_t FourColors::get_byte()
{
	return byte;
}

void FourColors::set_palette_0_fourth_pixel(bool new_value)
{
	bits.palette_0_fourth_pixel = new_value;
}

bool FourColors::get_palette_0_fourth_pixel()
{
	return bits.palette_0_fourth_pixel;
}

void FourColors::set_palette_1_fourth_pixel(bool new_value)
{
	bits.palette_1_fourth_pixel = new_value;
}

bool FourColors::get_palette_1_fourth_pixel()
{
	return bits.palette_1_fourth_pixel;
}

void FourColors::set_palette_fourth_pixel(std::uint8_t new_value)
{
	palette_index.fourth_pixel = new_value;
}

std::uint8_t FourColors::get_palette_fourth_pixel()
{
	return palette_index.fourth_pixel;
}

void FourColors::set_palette_0_third_pixel(bool new_value)
{
	bits.palette_0_third_pixel = new_value;
}

bool FourColors::get_palette_0_third_pixel()
{
	return bits.palette_0_third_pixel;
}

void FourColors::set_palette_1_third_pixel(bool new_value)
{
	bits.palette_1_third_pixel = new_value;
}

bool FourColors::get_palette_1_third_pixel()
{
	return bits.palette_1_third_pixel;
}

void FourColors::set_palette_third_pixel(std::uint8_t new_value)
{
	palette_index.third_pixel = new_value;
}

std::uint8_t FourColors::get_palette_third_pixel()
{
	return palette_index.third_pixel;
}

void FourColors::set_palette_0_second_pixel(bool new_value)
{
	bits.palette_0_second_pixel = new_value;
}

bool FourColors::get_palette_0_second_pixel()
{
	return bits.palette_0_second_pixel;
}

void FourColors::set_palette_1_second_pixel(bool new_value)
{
	bits.palette_1_second_pixel = new_value;
}

bool FourColors::get_palette_1_second_pixel()
{
	return bits.palette_1_second_pixel;
}

void FourColors::set_palette_second_pixel(std::uint8_t new_value)
{
	palette_index.second_pixel = new_value;
}

std::uint8_t FourColors::get_palette_second_pixel()
{
	return palette_index.second_pixel;
}

void FourColors::set_palette_0_first_pixel(bool new_value)
{
	bits.palette_0_first_pixel = new_value;
}

bool FourColors::get_palette_0_first_pixel()
{
	return bits.palette_0_first_pixel;
}

void FourColors::set_palette_1_first_pixel(bool new_value)
{
	bits.palette_1_first_pixel = new_value;
}

bool FourColors::get_palette_1_first_pixel()
{
	return bits.palette_1_first_pixel;
}

void FourColors::set_palette_first_pixel(std::uint8_t new_value)
{
	palette_index.first_pixel = new_value;
}

std::uint8_t FourColors::get_palette_first_pixel()
{
	return palette_index.first_pixel;
}
