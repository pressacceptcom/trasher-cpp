/**
 * * ---------------------------
 * | FF20 - FF23 | PIA | IC4 |
 * ---------------------------
 *
 * FF20:
 *       BIT 0 = CASSETTE DATA INPUT
 *       BIT 1 = RS-232C DATA OUTPUT
 *       BIT 2 = 6 BIT D/A LSB
 *       BIT 3 = 6 BIT D/A
 *       BIT 4 = 6 BIT D/A
 *       BIT 5 = 6 BIT D/A
 *       BIT 6 = 6 BIT D/A
 *       BIT 7 = 6 BIT D/A MSB
 *
 * FF21:
 *       BIT 0   Control of the CD     / 0 = FIRQ* to CPU Disabled
 *               (RS-232C status)      \ 1 = FIRQ* to CPU Enabled
 *               Interrupt
 *       BIT 1   Control of Interrupt  / 0 = sets flag on falling edge CD
 *               Polarity              \ 1 = sets flag on rising edge CD
 *       BIT 2 = Normally 1:    0 = changes FF20 to the data direction register
 *       BIT 3 = Cassette Motor Control:    0 = OFF  1 = ON
 *       BIT 4 = 1 Always
 *       BIT 5 = 1 Always
 *       BIT 6 = Not used
 *       BIT 7 = CD Interrupt Flag
 *
 * FF22:
 *       BIT 0 = RS-232C DATA INPUT
 *       BIT 1 = SINGLE BIT SOUND OUTPUT
 *       BIT 2 = RAM SIZE INPUT
 *       BIT 3 = RGB Monitor Sensing INPUT    CSS
 *       BIT 4 = VDG CONTROL OUTPUT           GM0 & UPPER/LOWER CASE*
 *       BIT 5 = VDG CONTROL OUTPUT           GM1 & INVERT
 *       BIT 6 = VDG CONTROL OUTPUT           GM2
 *       BIT 7 = VDG CONTROL OUTPUT           A* / G
 *
 * FF23:
 *       BIT 0   Control of the cartridge  / 0 = FIRQ* to CPU Disabled
 *               Interrupt                 \ 1 = FIRQ* to CPU Enabled
 *       BIT 1   Control of Interrupt      / 0 = sets flag on falling edge CART*
 *               Polarity                  \ 1 = sets flag on rising edge CART*
 *       BIT 2 = Normally 1:    0 = changes FF22 to the data direction register
 *       BIT 3 = Sound Enable
 *       BIT 4 = 1 Always
 *       BIT 5 = 1 Always
 *       BIT 6 = Not used
 *       BIT 7 = Cartridge Interrupt Flag
 *
 * FF40 - FFBF: Not used
 *
 * Note: FF22, FF23 are duplicated in tcc1014 (VC2645QC), and V.D.G Control Bit
 * (Bit 3 through Bit 7) affects this IC (TCC1014) only.
 */

#ifndef MC6821_PIA2_H
#define MC6821_PIA2_H

#include <cstdint>

#include <Godot.hpp>
#include <Reference.hpp>
#include <Ref.hpp>

#include "PinsState.hpp"
#include "VDGControl.hpp"

namespace godot
{
	class Mc6821_PIA2 : public Reference
	{
		GODOT_CLASS(Mc6821_PIA2, Reference)

	private:
	public:
		union DataRegisterA
		{
			std::uint8_t byte;
			struct
			{
				bool cassette_data_input : 1;
				bool rs_232c_data_output : 1;
				bool dac_0 : 1;
				bool dac_1 : 1;
				bool dac_2 : 1;
				bool dac_3 : 1;
				bool dac_4 : 1;
				bool dac_5 : 1;
			} bits;
			struct
			{
				unsigned int ignore : 2;
				std::uint8_t value : 6;
			} dac;
		} data_register_a;

		union ControlRegisterA
		{
			std::uint8_t byte;
			struct
			{
				bool carrier_detect_interrupt_enable : 1;
				bool carrier_detect_interrupt_polarity : 1;
				bool accessing_data_register : 1;
				bool cassette_motor_control : 1;
				bool bit_5 : 1; // 1 always
				bool bit_4 : 1; // 1 always
				bool not_used : 1;
				bool carrier_detect_interrupt_flag : 1;
			} bits;
		} control_register_a;

		union DataDirectionRegisterA
		{
			std::uint8_t byte;
			struct
			{
				bool ddra_0 : 1;
				bool ddra_1 : 1;
				bool ddra_2 : 1;
				bool ddra_3 : 1;
				bool ddra_4 : 1;
				bool ddra_5 : 1;
				bool ddra_6 : 1;
				bool ddra_7 : 1;
			} bits;
		} data_direction_register_a;

		union DataRegisterB
		{
			std::uint8_t byte;
			struct
			{
				bool rs_232c_data_input : 1;
				bool single_bit_sound_output : 1;
				bool ram_size_input : 1;
				bool color_set_select : 1;
				bool vdg_control_output_gm0 : 1;
				bool vdg_control_output_gm1 : 1;
				bool vdg_control_output_gm2 : 1;
				bool vdg_control_output_ag : 1;
			} bits;
			struct
			{
				unsigned int ignore : 3;
				bool rgb_monitor_sensing_input : 1;
			} input;
		} data_register_b;

		union ControlRegisterB
		{
			std::uint8_t byte;
			struct
			{
				bool cartridge_interrupt_enable : 1;
				bool cartridge_interrupt_polarity : 1;
				bool accessing_data_register : 1;
				bool sound_enable : 1;
				bool bit_5 : 1; // 1 always
				bool bit_4 : 1; // 1 always
				bool not_used : 1;
				bool cartridge_interrupt_flag : 1;
			} bits;
		} control_register_b;

		union DataDirectionRegisterB
		{
			std::uint8_t byte;
			struct
			{
				bool ddrb_0 : 1;
				bool ddrb_1 : 1;
				bool ddrb_2 : 1;
				bool ddrb_3 : 1;
				bool ddrb_4 : 1;
				bool ddrb_5 : 1;
				bool ddrb_6 : 1;
				bool ddrb_7 : 1;
			} bits;
		} data_direction_register_b;

		bool debug = false;

		Ref<PinsState> mpu_pins_state;
		Ref<VDGControl> vdg_control_register;

		// C++ Specific Definitions

		Mc6821_PIA2();

		~Mc6821_PIA2();

		// Godot Specific Definitions

		static void _register_methods();

		void _init(); // our initializer called by Godot

		// Class Definitions

		std::uint8_t read(std::uint8_t address);
		void write(std::uint8_t address, std::uint8_t data);

		void carrier_detect_transition(bool sync_value);
		void cartridge_transition(bool sync_value);

		void set_data_register_a(std::uint8_t new_value);
		std::uint8_t get_data_register_a();

		void set_cassette_data_input(bool new_value);
		bool get_cassette_data_input();

		void set_rs_232c_data_output(bool new_value);
		bool get_rs_232c_data_output();

		void set_dac_0(bool new_value);
		bool get_dac_0();

		void set_dac_1(bool new_value);
		bool get_dac_1();

		void set_dac_2(bool new_value);
		bool get_dac_2();

		void set_dac_3(bool new_value);
		bool get_dac_3();

		void set_dac_4(bool new_value);
		bool get_dac_4();

		void set_dac_5(bool new_value);
		bool get_dac_5();

		void set_dac(std::uint8_t new_value);
		std::uint8_t get_dac();

		void set_control_register_a(std::uint8_t new_value);
		std::uint8_t get_control_register_a();

		void set_carrier_detect_interrupt_enable(bool new_value);
		bool get_carrier_detect_interrupt_enable();

		void set_carrier_detect_interrupt_polarity(bool new_value);
		bool get_carrier_detect_interrupt_polarity();

		void set_data_direction_a(bool new_value);
		bool get_data_direction_a();

		void set_cassette_motor_control(bool new_value);
		bool get_cassette_motor_control();

		void set_carrier_detect_interrupt_flag(bool new_value);
		bool get_carrier_detect_interrupt_flag();

		void set_data_direction_register_a(std::uint8_t new_value);
		std::uint8_t get_data_direction_register_a();

		void set_ddra_0(bool new_value);
		bool get_ddra_0();

		void set_ddra_1(bool new_value);
		bool get_ddra_1();

		void set_ddra_2(bool new_value);
		bool get_ddra_2();

		void set_ddra_3(bool new_value);
		bool get_ddra_3();

		void set_ddra_4(bool new_value);
		bool get_ddra_4();

		void set_ddra_5(bool new_value);
		bool get_ddra_5();

		void set_ddra_6(bool new_value);
		bool get_ddra_6();

		void set_ddra_7(bool new_value);
		bool get_ddra_7();

		void set_data_register_b(std::uint8_t new_value);
		std::uint8_t get_data_register_b();

		void set_rs_232c_data_input(bool new_value);
		bool get_rs_232c_data_input();

		void set_single_bit_sound_output(bool new_value);
		bool get_single_bit_sound_output();

		void set_ram_size_input(bool new_value);
		bool get_ram_size_input();

		void set_color_set_select(bool new_value);
		bool get_color_set_select();

		void set_vdg_control_output_gm0(bool new_value);
		bool get_vdg_control_output_gm0();

		void set_vdg_control_output_gm1(bool new_value);
		bool get_vdg_control_output_gm1();

		void set_vdg_control_output_gm2(bool new_value);
		bool get_vdg_control_output_gm2();

		void set_vdg_control_output_ag(bool new_value);
		bool get_vdg_control_output_ag();

		void set_rgb_monitor_sensing_input(bool new_value);
		bool get_rgb_monitor_sensing_input();

		void set_control_register_b(std::uint8_t new_value);
		std::uint8_t get_control_register_b();

		void set_cartridge_interrupt_enable(bool new_value);
		bool get_cartridge_interrupt_enable();

		void set_cartridge_interrupt_polarity(bool new_value);
		bool get_cartridge_interrupt_polarity();

		void set_data_direction_b(bool new_value);
		bool get_data_direction_b();

		void set_sound_enable(bool new_value);
		bool get_sound_enable();

		void set_cartridge_interrupt_flag(bool new_value);
		bool get_cartridge_interrupt_flag();

		void set_data_direction_register_b(std::uint8_t new_value);
		std::uint8_t get_data_direction_register_b();

		void set_ddrb_0(bool new_value);
		bool get_ddrb_0();

		void set_ddrb_1(bool new_value);
		bool get_ddrb_1();

		void set_ddrb_2(bool new_value);
		bool get_ddrb_2();

		void set_ddrb_3(bool new_value);
		bool get_ddrb_3();

		void set_ddrb_4(bool new_value);
		bool get_ddrb_4();

		void set_ddrb_5(bool new_value);
		bool get_ddrb_5();

		void set_ddrb_6(bool new_value);
		bool get_ddrb_6();

		void set_ddrb_7(bool new_value);
		bool get_ddrb_7();
	};
}

#endif