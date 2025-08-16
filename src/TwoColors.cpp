#include "TwoColors.hpp"

using namespace godot;

TwoColors::TwoColors() : byte{0}
{
}

TwoColors::~TwoColors()
{
}

void TwoColors::_register_methods()
{
	// Properties
	register_property<TwoColors, std::uint8_t>("byte", &TwoColors::set_byte, &TwoColors::get_byte, 0);
	register_property<TwoColors, bool>("palette_0_eighth_pixel", &TwoColors::set_palette_0_eighth_pixel, &TwoColors::get_palette_0_eighth_pixel, false);
	register_property<TwoColors, std::uint8_t>("palette_eighth_pixel", &TwoColors::set_palette_eighth_pixel, &TwoColors::get_palette_eighth_pixel, 0);
	register_property<TwoColors, bool>("palette_0_seventh_pixel", &TwoColors::set_palette_0_seventh_pixel, &TwoColors::get_palette_0_seventh_pixel, false);
	register_property<TwoColors, std::uint8_t>("palette_seventh_pixel", &TwoColors::set_palette_seventh_pixel, &TwoColors::get_palette_seventh_pixel, 0);
	register_property<TwoColors, bool>("palette_0_sixth_pixel", &TwoColors::set_palette_0_sixth_pixel, &TwoColors::get_palette_0_sixth_pixel, false);
	register_property<TwoColors, std::uint8_t>("palette_sixth_pixel", &TwoColors::set_palette_sixth_pixel, &TwoColors::get_palette_sixth_pixel, 0);
	register_property<TwoColors, bool>("palette_0_fifth_pixel", &TwoColors::set_palette_0_fifth_pixel, &TwoColors::get_palette_0_fifth_pixel, false);
	register_property<TwoColors, std::uint8_t>("palette_fifth_pixel", &TwoColors::set_palette_fifth_pixel, &TwoColors::get_palette_fifth_pixel, 0);
	register_property<TwoColors, bool>("palette_0_fourth_pixel", &TwoColors::set_palette_0_fourth_pixel, &TwoColors::get_palette_0_fourth_pixel, false);
	register_property<TwoColors, std::uint8_t>("palette_fourth_pixel", &TwoColors::set_palette_fourth_pixel, &TwoColors::get_palette_fourth_pixel, 0);
	register_property<TwoColors, bool>("palette_0_third_pixel", &TwoColors::set_palette_0_third_pixel, &TwoColors::get_palette_0_third_pixel, false);
	register_property<TwoColors, std::uint8_t>("palette_third_pixel", &TwoColors::set_palette_third_pixel, &TwoColors::get_palette_third_pixel, 0);
	register_property<TwoColors, bool>("palette_0_second_pixel", &TwoColors::set_palette_0_second_pixel, &TwoColors::get_palette_0_second_pixel, false);
	register_property<TwoColors, std::uint8_t>("palette_second_pixel", &TwoColors::set_palette_second_pixel, &TwoColors::get_palette_second_pixel, 0);
	register_property<TwoColors, bool>("palette_0_first_pixel", &TwoColors::set_palette_0_first_pixel, &TwoColors::get_palette_0_first_pixel, false);
	register_property<TwoColors, std::uint8_t>("palette_first_pixel", &TwoColors::set_palette_first_pixel, &TwoColors::get_palette_first_pixel, 0);
}

void TwoColors::_init()
{
}

void TwoColors::set_byte(std::uint8_t new_value)
{
	byte = new_value;
}

std::uint8_t TwoColors::get_byte()
{
	return byte;
}

void TwoColors::set_palette_0_eighth_pixel(bool new_value)
{
	bits.palette_0_eighth_pixel = new_value;
}

bool TwoColors::get_palette_0_eighth_pixel()
{
	return bits.palette_0_eighth_pixel;
}

void TwoColors::set_palette_eighth_pixel(std::uint8_t new_value)
{
	palette_index.eighth_pixel = new_value;
}

std::uint8_t TwoColors::get_palette_eighth_pixel()
{
	return palette_index.eighth_pixel;
}

void TwoColors::set_palette_0_seventh_pixel(bool new_value)
{
	bits.palette_0_seventh_pixel = new_value;
}

bool TwoColors::get_palette_0_seventh_pixel()
{
	return bits.palette_0_seventh_pixel;
}

void TwoColors::set_palette_seventh_pixel(std::uint8_t new_value)
{
	palette_index.seventh_pixel = new_value;
}

std::uint8_t TwoColors::get_palette_seventh_pixel()
{
	return palette_index.seventh_pixel;
}

void TwoColors::set_palette_0_sixth_pixel(bool new_value)
{
	bits.palette_0_sixth_pixel = new_value;
}

bool TwoColors::get_palette_0_sixth_pixel()
{
	return bits.palette_0_sixth_pixel;
}

void TwoColors::set_palette_sixth_pixel(std::uint8_t new_value)
{
	palette_index.sixth_pixel = new_value;
}

std::uint8_t TwoColors::get_palette_sixth_pixel()
{
	return palette_index.sixth_pixel;
}

void TwoColors::set_palette_0_fifth_pixel(bool new_value)
{
	bits.palette_0_fifth_pixel = new_value;
}

bool TwoColors::get_palette_0_fifth_pixel()
{
	return bits.palette_0_fifth_pixel;
}

void TwoColors::set_palette_fifth_pixel(std::uint8_t new_value)
{
	palette_index.fifth_pixel = new_value;
}

std::uint8_t TwoColors::get_palette_fifth_pixel()
{
	return palette_index.fifth_pixel;
}

void TwoColors::set_palette_0_fourth_pixel(bool new_value)
{
	bits.palette_0_fourth_pixel = new_value;
}

bool TwoColors::get_palette_0_fourth_pixel()
{
	return bits.palette_0_fourth_pixel;
}

void TwoColors::set_palette_fourth_pixel(std::uint8_t new_value)
{
	palette_index.fourth_pixel = new_value;
}

std::uint8_t TwoColors::get_palette_fourth_pixel()
{
	return palette_index.fourth_pixel;
}

void TwoColors::set_palette_0_third_pixel(bool new_value)
{
	bits.palette_0_third_pixel = new_value;
}

bool TwoColors::get_palette_0_third_pixel()
{
	return bits.palette_0_third_pixel;
}

void TwoColors::set_palette_third_pixel(std::uint8_t new_value)
{
	palette_index.third_pixel = new_value;
}

std::uint8_t TwoColors::get_palette_third_pixel()
{
	return palette_index.third_pixel;
}

void TwoColors::set_palette_0_second_pixel(bool new_value)
{
	bits.palette_0_second_pixel = new_value;
}

bool TwoColors::get_palette_0_second_pixel()
{
	return bits.palette_0_second_pixel;
}

void TwoColors::set_palette_second_pixel(std::uint8_t new_value)
{
	palette_index.second_pixel = new_value;
}

std::uint8_t TwoColors::get_palette_second_pixel()
{
	return palette_index.second_pixel;
}

void TwoColors::set_palette_0_first_pixel(bool new_value)
{
	bits.palette_0_first_pixel = new_value;
}

bool TwoColors::get_palette_0_first_pixel()
{
	return bits.palette_0_first_pixel;
}

void TwoColors::set_palette_first_pixel(std::uint8_t new_value)
{
	palette_index.first_pixel = new_value;
}

std::uint8_t TwoColors::get_palette_first_pixel()
{
	return palette_index.first_pixel;
}
