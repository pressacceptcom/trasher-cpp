#include "Alphanumeric.hpp"

using namespace godot;

Alphanumeric::Alphanumeric()
{
}

Alphanumeric::~Alphanumeric()
{
}

void Alphanumeric::_register_methods()
{
}

void Alphanumeric::_init()
{
	character = Ref<Character>(Character::_new());
	attribute = Ref<Attribute>(Attribute::_new());
}

void Alphanumeric::set_bytes(std::uint16_t bytes)
{
	character->byte = bytes >> 8;
	attribute->byte = bytes & 0xFF;
}

std::uint16_t Alphanumeric::get_bytes()
{
	return (character->byte << 8) | attribute->byte;
}

void Alphanumeric::set_character(Ref<Character> new_character)
{
	character = new_character;
}

Ref<Character> Alphanumeric::get_character()
{
	return character;
}

void Alphanumeric::set_attribute(Ref<Attribute> new_attribute)
{
	attribute = new_attribute;
}

Ref<Attribute> Alphanumeric::get_attribute()
{
	return attribute;
}
