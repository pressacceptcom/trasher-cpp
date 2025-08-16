#include "Character.hpp"

using namespace godot;

Character::Character() : byte{0}
{
}

Character::~Character()
{
}

void Character::_register_methods()
{
	// Properties
	register_property<Character, std::uint8_t>("byte", &Character::set_byte, &Character::get_byte, 0);
	register_property<Character, bool>("bit_0", &Character::set_bit_0, &Character::get_bit_0, false);
	register_property<Character, bool>("bit_1", &Character::set_bit_1, &Character::get_bit_1, false);
	register_property<Character, bool>("bit_2", &Character::set_bit_2, &Character::get_bit_2, false);
	register_property<Character, bool>("bit_3", &Character::set_bit_3, &Character::get_bit_3, false);
	register_property<Character, bool>("bit_4", &Character::set_bit_4, &Character::get_bit_4, false);
	register_property<Character, bool>("bit_5", &Character::set_bit_5, &Character::get_bit_5, false);
	register_property<Character, bool>("bit_6", &Character::set_bit_6, &Character::get_bit_6, false);
}

void Character::_init()
{
}

void Character::set_byte(std::uint8_t new_value)
{
	byte = new_value;
}

std::uint8_t Character::get_byte()
{
	return byte;
}

void Character::set_bit_0(bool new_value)
{
	bits.bit_0 = new_value;
}

bool Character::get_bit_0()
{
	return bits.bit_0;
}

void Character::set_bit_1(bool new_value)
{
	bits.bit_1 = new_value;
}

bool Character::get_bit_1()
{
	return bits.bit_1;
}

void Character::set_bit_2(bool new_value)
{
	bits.bit_2 = new_value;
}

bool Character::get_bit_2()
{
	return bits.bit_2;
}

void Character::set_bit_3(bool new_value)
{
	bits.bit_3 = new_value;
}

bool Character::get_bit_3()
{
	return bits.bit_3;
}

void Character::set_bit_4(bool new_value)
{
	bits.bit_4 = new_value;
}

bool Character::get_bit_4()
{
	return bits.bit_4;
}

void Character::set_bit_5(bool new_value)
{
	bits.bit_5 = new_value;
}

bool Character::get_bit_5()
{
	return bits.bit_5;
}

void Character::set_bit_6(bool new_value)
{
	bits.bit_6 = new_value;
}

bool Character::get_bit_6()
{
	return bits.bit_6;
}
