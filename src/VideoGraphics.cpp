#include <ImageTexture.hpp>
#include <Variant.hpp>

#include "VideoGraphics.hpp"
#include "TwoColors.hpp"
#include "FourColors.hpp"
#include "SixteenColors.hpp"
#include "Alphanumeric.hpp"

#include "FontInfo.hpp"

using namespace godot;

VideoGraphics::VideoGraphics()
{
}

VideoGraphics::~VideoGraphics()
{
}

void VideoGraphics::_register_methods()
{
	// Methods
	register_method("_on_video_resolution_register_set", &VideoGraphics::_on_video_resolution_register_set);
	register_method("_on_video_mode_register_set", &VideoGraphics::_on_video_mode_register_set);
	register_method("_on_vdg_control_register_set", &VideoGraphics::_on_vdg_control_register_set);
	register_method("_on_sam_control_registers_display_mode_control_set", &VideoGraphics::_on_sam_control_registers_display_mode_control_set);
	register_method("_on_initialization_register_0_coco_compatibility_enabled_toggled", &VideoGraphics::_on_initialization_register_0_coco_compatibility_enabled_toggled);

	register_method("get_memory", &VideoGraphics::get_memory);
	register_method("get_processor", &VideoGraphics::get_processor);
	register_method("get_initialization_register_0", &VideoGraphics::get_initialization_register_0);
	register_method("get_video_mode_register", &VideoGraphics::get_video_mode_register);
	register_method("get_video_resolution_register", &VideoGraphics::get_video_resolution_register);
	register_method("get_vertical_scroll_register", &VideoGraphics::get_vertical_scroll_register);
	register_method("get_vertical_offset_registers", &VideoGraphics::get_vertical_offset_registers);
	register_method("get_horizontal_offset_register", &VideoGraphics::get_horizontal_offset_register);
	register_method("get_monitor", &VideoGraphics::get_monitor);
	register_method("get_sam_control_registers", &VideoGraphics::get_sam_control_registers);
	register_method("get_vdg_control_register", &VideoGraphics::get_vdg_control_register);

	register_method("render_frame", &VideoGraphics::render_frame);
	register_method("get_image_data", &VideoGraphics::get_image_data);
	register_method("get_horizontal_pixels", &VideoGraphics::get_horizontal_pixels);
	register_method("get_vertical_pixels", &VideoGraphics::get_vertical_pixels);
	register_method("get_horizontal_border", &VideoGraphics::get_horizontal_border);
	register_method("get_vertical_border", &VideoGraphics::get_vertical_border);
}

void VideoGraphics::_init()
{
	color_palette_data = Ref<ColorPaletteData>(ColorPaletteData::_new());
	target_screen_buffer = Ref<StreamPeerBuffer>(StreamPeerBuffer::_new());
}

void VideoGraphics::_on_initialization_register_0_coco_compatibility_enabled_toggled(bool new_value)
{
	update_graphics_mode();
}

void VideoGraphics::_on_video_resolution_register_set(std::int64_t new_value)
{
	update_graphics_mode();
}

void VideoGraphics::_on_video_mode_register_set(bool new_value)
{
	update_graphics_mode();
}

void VideoGraphics::_on_vdg_control_register_set(std::int64_t new_value)
{
	update_graphics_mode();
}

void VideoGraphics::_on_sam_control_registers_display_mode_control_set(std::int64_t new_value)
{
	if (tcc1014->initialization_register_0->bits.coco_compatibility_enabled)
	{
		update_graphics_mode();
	}
}

Ref<Memory> VideoGraphics::get_memory()
{
	return memory;
}

Ref<Mc68b09e> VideoGraphics::get_processor()
{
	return processor;
}

Ref<InitializationRegister0> VideoGraphics::get_initialization_register_0()
{
	return tcc1014->initialization_register_0;
}

Ref<VideoModeRegister> VideoGraphics::get_video_mode_register()
{
	return tcc1014->video_mode_register;
}

Ref<VideoResolutionRegister> VideoGraphics::get_video_resolution_register()
{
	return tcc1014->video_resolution_register;
}

Ref<VerticalScrollRegister> VideoGraphics::get_vertical_scroll_register()
{
	return tcc1014->vertical_scroll_register;
}

Ref<VerticalOffset> VideoGraphics::get_vertical_offset_registers()
{
	return tcc1014->vertical_offset_registers;
}

Ref<HorizontalOffsetRegister> VideoGraphics::get_horizontal_offset_register()
{
	return tcc1014->horizontal_offset_register;
}

Ref<Monitor> VideoGraphics::get_monitor()
{
	return tcc1014->monitor;
}

Ref<SAMControlRegisters> VideoGraphics::get_sam_control_registers()
{
	return tcc1014->sam_control_registers;
}

Ref<VDGControl> VideoGraphics::get_vdg_control_register()
{
	return tcc1014->vdg_control_register;
}

Ref<ColorPaletteData> VideoGraphics::get_color_palette_data()
{
	return color_palette_data;
}

void VideoGraphics::set_tcc1014(Ref<Tcc1014> new_value)
{
	tcc1014 = new_value;
	tcc1014->initialization_register_0->connect("coco_compatibility_enabled_toggled", this, "_on_initialization_register_0_coco_compatibility_enabled_toggled");
	tcc1014->video_mode_register->connect("register_set", this, "_on_video_mode_register_set");
	tcc1014->video_resolution_register->connect("register_set", this, "_on_video_resolution_register_set");
	tcc1014->sam_control_registers->connect("display_mode_control_set", this, "_on_sam_control_registers_display_mode_control_set");
	tcc1014->vdg_control_register->connect("register_set", this, "_on_vdg_control_register_set");

	color_palette_data->monitor = tcc1014->monitor;
	color_palette_data->set_color_palette(tcc1014->color_palette);
}

int VideoGraphics::get_horizontal_pixels()
{
	if (tcc1014->initialization_register_0->bits.coco_compatibility_enabled)
	{
		// coco compatibility mode, lower resolutions available

		switch (tcc1014->sam_control_registers->display_mode_control.byte)
		{

		case SAMControlRegisters::Modes::ALPHANUMERIC:
			/**
			 * In the text mode there are thirty-two columns when HRES2 is clear and sixty-four
			 * when set. ~ Assembly Programming for the Color Computer 3, Laurence Tepolt
			 */
			if (tcc1014->video_resolution_register->bits.horizontal_resolution_2)
			{
				return 512;
			}

			return 256;

		case SAMControlRegisters::Modes::COLOR_RESOLUTION_GRAPHICS_1:
			if (tcc1014->vdg_control_register->bits.gm_0)
			{
				// resolution graphics one

				return 128;
			}
			else
			{
				// color graphics one

				return 64;
			}

		case SAMControlRegisters::Modes::COLOR_GRAPHICS_2:
		case SAMControlRegisters::Modes::RESOLUTION_GRAPHICS_2:
		case SAMControlRegisters::Modes::COLOR_GRAPHICS_3:
		case SAMControlRegisters::Modes::RESOLUTION_GRAPHICS_3:
		case SAMControlRegisters::Modes::COLOR_GRAPHICS_6:
			return 128;

		case SAMControlRegisters::Modes::RESOLUTION_GRAPHICS_6:
			return 256;
		}
	}
	else
	{
		// coco 3 mode, higher resolutions available

		if (!tcc1014->video_mode_register->bits.bit_plane)
		{
			// alphanumeric mode

			return get_character_columns() * 8;
		}
		else
		{
			// graphics mode

			switch (tcc1014->video_resolution_register->byte & 0x1F)
			{

			case VideoResolutionRegister::GraphicsResolution::ONE_SIXTY_16:
				return 160;

			case VideoResolutionRegister::GraphicsResolution::TWO_FIFTY_SIX_2:
			case VideoResolutionRegister::GraphicsResolution::TWO_FIFTY_SIX_4:
			case VideoResolutionRegister::GraphicsResolution::TWO_FIFTY_SIX_16:
				return 256;

			case VideoResolutionRegister::GraphicsResolution::THREE_TWENTY_4:
			case VideoResolutionRegister::GraphicsResolution::THREE_TWENTY_16:
				return 320;

			case VideoResolutionRegister::GraphicsResolution::FIVE_TWELVE_2:
			case VideoResolutionRegister::GraphicsResolution::FIVE_TWELVE_4:
				return 512;

			case VideoResolutionRegister::GraphicsResolution::SIX_FORTY_2:
			case VideoResolutionRegister::GraphicsResolution::SIX_FORTY_4:
				return 640;
			}
		}
	}

	return 0;
}

int VideoGraphics::get_vertical_pixels()
{
	if (tcc1014->initialization_register_0->bits.coco_compatibility_enabled)
	{
		// coco compatibility mode, lower resolutions available

		switch (tcc1014->sam_control_registers->display_mode_control.byte)
		{
		case SAMControlRegisters::Modes::ALPHANUMERIC:
			/**
			 * VRES1-0 control the vertical resolution. The valid values that may be put in VRES1-0
			 * and the number of text or graphics rows they select are show in Table 5-4.
			 *
			 * --------------------------------------------------
			 * | VRES1 | VRES0 | Text  | G1x/G2C | G2R/G3 | G6x |
			 * --------------------------------------------------
			 * |   0   |   0   | 16    |   64    |   96   | 192 |
			 * --------------------------------------------------
			 * |   0   |   1   | 16.75 |   67    |  100   | 200 |
			 * --------------------------------------------------
			 * |   1   |   1   | 19    |   75    |  112   | 225 |
			 * --------------------------------------------------
			 */

			if (tcc1014->video_resolution_register->bits.lines_per_field_0)
			{
				if (tcc1014->video_resolution_register->bits.lines_per_field_1)
				{
					return 228;
				}

				return 201;
			}

			return 192; // 12 * 16

		case SAMControlRegisters::Modes::COLOR_RESOLUTION_GRAPHICS_1:
		case SAMControlRegisters::Modes::COLOR_GRAPHICS_2:
			if (tcc1014->video_resolution_register->bits.lines_per_field_0)
			{
				if (tcc1014->video_resolution_register->bits.lines_per_field_1)
				{
					return 75;
				}

				return 67;
			}

			return 64;

		case SAMControlRegisters::Modes::RESOLUTION_GRAPHICS_2:
		case SAMControlRegisters::Modes::COLOR_GRAPHICS_3:
			if (tcc1014->video_resolution_register->bits.lines_per_field_0)
			{
				if (tcc1014->video_resolution_register->bits.lines_per_field_1)
				{
					return 112;
				}

				return 100;
			}

			return 96;

		case SAMControlRegisters::Modes::RESOLUTION_GRAPHICS_3:
		case SAMControlRegisters::Modes::COLOR_GRAPHICS_6:
		case SAMControlRegisters::Modes::RESOLUTION_GRAPHICS_6:
			if (tcc1014->video_resolution_register->bits.lines_per_field_0)
			{
				if (tcc1014->video_resolution_register->bits.lines_per_field_1)
				{
					return 225;
				}

				return 200;
			}

			return 192;
		}
	}
	else
	{
		// coco 3 mode, higher resolutions available

		// graphics and text mode

		// 192 / 8 = 24 rows, 200 / 8 = 25 rows, 225 / 8 ~ 28 rows
		// 8 is the lowest "lines per row"

		switch (tcc1014->video_resolution_register->get_lines_per_field())
		{

		case VideoResolutionRegister::LinesPerField::ONE_NINETY_TWO:
			return 192;

		case VideoResolutionRegister::LinesPerField::TWO_HUNDERD:
			return 200;

		case VideoResolutionRegister::LinesPerField::TWO_TWENTY_FIVE:
			return 225;
		}
	}

	return 0;
}

int VideoGraphics::get_colors()
{
	if (tcc1014->initialization_register_0->bits.coco_compatibility_enabled)
	{
		// coco compatibility mode, lower resolutions available

		switch (tcc1014->sam_control_registers->display_mode_control.byte)
		{

		case SAMControlRegisters::Modes::ALPHANUMERIC:
			return 8;

		case SAMControlRegisters::Modes::COLOR_RESOLUTION_GRAPHICS_1:
			if (tcc1014->vdg_control_register->bits.gm_0)
			{
				// resolution graphics one

				return 2;
			}
			else
			{
				// color graphics one

				return 4;
			}

		case SAMControlRegisters::Modes::COLOR_GRAPHICS_2:
		case SAMControlRegisters::Modes::COLOR_GRAPHICS_3:
		case SAMControlRegisters::Modes::COLOR_GRAPHICS_6:
			return 4;

		case SAMControlRegisters::Modes::RESOLUTION_GRAPHICS_2:
		case SAMControlRegisters::Modes::RESOLUTION_GRAPHICS_3:
		case SAMControlRegisters::Modes::RESOLUTION_GRAPHICS_6:
			return 2;
		}
	}
	else
	{
		switch (tcc1014->video_resolution_register->get_color_resolution())
		{

		case VideoResolutionRegister::ColorResolution::TWO:
			return 2;

		case VideoResolutionRegister::ColorResolution::FOUR:
			return 4;

		case VideoResolutionRegister::ColorResolution::SIXTEEN:
			return 16;
		}
	}

	return 2;
}

int VideoGraphics::get_character_columns()
{
	if (tcc1014->initialization_register_0->bits.coco_compatibility_enabled)
	{
		// coco compatibility mode, lower resolutions available

		/**
		 * In the text mode there are thirty-two columns when HRES2 is clear and sixty-four when
		 * set. ~ Assembly Programming for the Color Computer 3, Laurence Tepolt
		 */
		if (tcc1014->video_resolution_register->bits.horizontal_resolution_2)
		{
			return 64;
		}

		return 32;
	}
	else
	{
		// coco 3 mode

		if (!tcc1014->video_mode_register->bits.bit_plane)
		{
			switch (tcc1014->video_resolution_register->byte & 0x1F)
			{

			case VideoResolutionRegister::AlphanumericsWidth::THIRTY_TWO_CHARACTER:
				return 32;

			case VideoResolutionRegister::AlphanumericsWidth::FORTY_CHARACTER:
				return 40;

			/**
			 * Laurence Tepolt's Assembly Language Programming for the Color Computer 3 claims
			 * there is a sixty-four character column mode when HRES2 is 1 and HRES0 is 0.
			 */
			case VideoResolutionRegister::AlphanumericsWidth::SIXTY_FOUR_CHARACTER:
				return 64;

			case VideoResolutionRegister::AlphanumericsWidth::EIGHTY_CHARACTER:
				return 80;
			}
		}
	}

	return 0;
}

int VideoGraphics::get_lines_per_row()
{
	switch (tcc1014->video_mode_register->get_lines_per_row())
	{
	case VideoModeRegister::LinesPerRow::ONE:
		return 1;

	case VideoModeRegister::LinesPerRow::TWO:
		return 2;

	case VideoModeRegister::LinesPerRow::THREE:
		return 3;

	case VideoModeRegister::LinesPerRow::EIGHT:
		return 8;

	case VideoModeRegister::LinesPerRow::NINE:
		return 9;

	case VideoModeRegister::LinesPerRow::TWELVE:
		return 12;
	}

	return 0;
}

int VideoGraphics::get_vertical_border()
{
	switch (get_vertical_pixels())
	{

	case 64:
	case 96:
	case 192:
		return 24; // ((480 / 2) - 192) / 2

	case 67:
	case 100:
	case 200:
	case 201:
		return 20;

	case 75:
	case 112:
	case 225:
	case 228:
		return 7;
	}

	return 0;
}

int VideoGraphics::get_horizontal_border()
{
	switch (get_horizontal_pixels())
	{

	case 512:
	case 256:
	case 128:
	case 64:
		return 32; // (640 - 512) / 2

	case 640:
	case 320:
	case 160:
		return 0;
	}

	return 0;
}

int VideoGraphics::get_buffer_width()
{
	if (tcc1014->initialization_register_0->bits.coco_compatibility_enabled)
	{
		// coco compatibility mode, lower resolutions available

		switch (tcc1014->sam_control_registers->display_mode_control.byte)
		{

		case SAMControlRegisters::Modes::ALPHANUMERIC:
			return get_character_columns();

		case SAMControlRegisters::Modes::COLOR_RESOLUTION_GRAPHICS_1:
			return 16; // 64 / 4 = 16, 128 / 8 = 16 bytes

		case SAMControlRegisters::Modes::COLOR_GRAPHICS_2:
		case SAMControlRegisters::Modes::COLOR_GRAPHICS_3:
		case SAMControlRegisters::Modes::COLOR_GRAPHICS_6:
			return get_horizontal_pixels() / 4;

		case SAMControlRegisters::Modes::RESOLUTION_GRAPHICS_2:
		case SAMControlRegisters::Modes::RESOLUTION_GRAPHICS_3:
		case SAMControlRegisters::Modes::RESOLUTION_GRAPHICS_6:
			return get_horizontal_pixels() / 8;
		}
	}
	else
	{
		// coco 3 mode, higher resolutions available

		if (tcc1014->video_mode_register->bits.bit_plane)
		{
			// graphics mode

			switch (get_colors())
			{

			case 2:
				return get_horizontal_pixels() / 8;

			case 4:
				return get_horizontal_pixels() / 4;

			case 16:
				return get_horizontal_pixels() / 2;
			}
		}
		else
		{
			return get_character_columns() * 2;
		}
	}

	return 0;
}

void VideoGraphics::update_graphics_mode()
{
	int vertical_pixels = get_vertical_pixels();
	int horizontal_pixels = get_horizontal_pixels();

	if (vertical_pixels && horizontal_pixels)
	{
		std::int64_t new_size = vertical_pixels * horizontal_pixels * TARGET_BYTE_PER_PIXEL;

		target_screen_buffer->resize(new_size);

		target_screen_buffer->seek(0);

		for (int i = 0; i < new_size; i++)
		{
			target_screen_buffer->put_u8(0);
		}
	}
}

void VideoGraphics::render_frame()
{
	int vertical_border = get_vertical_border();
	std::uint32_t vram_start_address;

	if (tcc1014->initialization_register_0->bits.coco_compatibility_enabled)
	{
		/* From Assembly Language Programming for the CoCo 3 by Laurence Tepolt
		 *
		 *                         Y18                  Y0
		 * Physical Buffer Addr    xxx xxxx xxx x xxxx x000
		 *                         --- -------- --------
		 *                          |      |        |
		 * Vert Offset 1          b7-b5    |        |
		 * Vert Offset 0                   |      b5-b0
		 * SAM Vert Offset               F6-F0
		 *
		 * Fig. 5-1. Buffer Address Apportionment
		 *
		 */

		// 7    01         F8
		// 0111 0000 0001  1111 1000
		vram_start_address = (tcc1014->vertical_offset_registers->get_offset() & 0x701F8) | (tcc1014->sam_control_registers->display_offset.byte << 9);
	}
	else
	{
		vram_start_address = tcc1014->vertical_offset_registers->get_offset();
	}

	// set blink
	// assert vertical sync

	// vertical blanking for 13 lines
	// 4 unrendered top border lines

	for (int i = 0; i < 17; i++)
	{
		scanline();
	}

	// vertical border (drawn by BorderRegister)
	for (int i = 0; i < vertical_border; i++) // -4 ?
	{
		scanline();
	}

	// We're now in the active display area
	target_screen_buffer->seek(0);

	int lines_per_field = get_vertical_pixels();
	int buffer_width = get_buffer_width();

	if (tcc1014->initialization_register_0->bits.coco_compatibility_enabled)
	{
		// coco compatibility mode, low resolution available

		if (tcc1014->sam_control_registers->display_mode_control.byte == SAMControlRegisters::Modes::ALPHANUMERIC)
		{
			// alphanumeric / semigraphics 4 mode

			std::vector<std::uint8_t> buffer_data;
			buffer_data.resize(buffer_width);

			for (int y = 0; y < lines_per_field; y++)
			{
				if (y % 12 == 0)
				{
					buffer_data = memory->get_video_memory(vram_start_address, vram_start_address + buffer_width);
				}

				for (int x = 0; x < buffer_width; x++)
				{
					std::uint32_t text_palette[2] = {0x00, 0x00};
					std::uint8_t character = buffer_data[x];
					std::uint8_t row;

					/**
					 * -------------------------------------------------------------------
					 * | VDG Bit | State |                    Result                     |
					 * -------------------------------------------------------------------
					 * |    7    |   0   | Text Display Mode                             |
					 * -------------------------------------------------------------------
					 * |    6    |   0   | Black Border                                  |
					 * |         |   1   | Border is green (bit 3=0) or orange (bit 3=1) |
					 * -------------------------------------------------------------------
					 * |    5    |   0   | Palette regs used per bit 3                   |
					 * |         |   1   | Fore/back pal regs reversed & border = black  |
					 * -------------------------------------------------------------------
					 * |    4    |   0   | Lower case displayed with reverse video       |
					 * |         |   1   | True lower case displayed                     |
					 * -------------------------------------------------------------------
					 * |    3    |   0   | Char. color= pal reg 12, backgnd = pal reg 13 |
					 * |         |   1   | Char. color= pal reg 14, backgnd = pal reg 15 |
					 * -------------------------------------------------------------------
					 *
					 * Fig 5-2. VDG Control ~ Assemblly Language Programming for the Color Computer
					 *                        by Laurence Tepolt
					 */
					if (tcc1014->vdg_control_register->bits.gm_1)
					{
						if (tcc1014->vdg_control_register->bits.color_set_select)
						{
							text_palette[1] = color_palette_data->data[14];
							text_palette[0] = color_palette_data->data[15];
						}
						else
						{
							text_palette[1] = color_palette_data->data[12];
							text_palette[0] = color_palette_data->data[13];
						}
					}
					else
					{
						if (tcc1014->vdg_control_register->bits.color_set_select)
						{
							text_palette[0] = color_palette_data->data[14];
							text_palette[1] = color_palette_data->data[15];
						}
						else
						{
							text_palette[0] = color_palette_data->data[12];
							text_palette[1] = color_palette_data->data[13];
						}
					}

					switch (character & 0xC0)
					{

					case 0x00:
						// normal (inverse to our eyes) character
						character = character & 0x3F;

						if (character < 32 && tcc1014->vdg_control_register->bits.gm_0) // lowercase
						{
							row = ~coco2font[character + 80][y % 12];
						}
						else
						{
							row = coco2font[character][y % 12];
						}
						break;

					case 0x40:
						// normal (inverse to computer) character
						row = ~coco2font[character & 0x3F][y % 12];
						break;

					case 0x80:
					case 0xC0:
						// semigraphics character
						text_palette[0] = color_palette_data->data[8];
						text_palette[1] = color_palette_data->data[(character & 0x70) >> 4];
						row = coco2font[0x40 + (character & 0x0F)][y % 12];
					}

					target_screen_buffer->put_u64((static_cast<std::uint64_t>(text_palette[(row >> 6) & 0x01]) << 32) | text_palette[(row >> 7) & 0x01]);
					target_screen_buffer->put_u64((static_cast<std::uint64_t>(text_palette[(row >> 4) & 0x01]) << 32) | text_palette[(row >> 5) & 0x01]);
					target_screen_buffer->put_u64((static_cast<std::uint64_t>(text_palette[(row >> 2) & 0x01]) << 32) | text_palette[(row >> 3) & 0x01]);
					target_screen_buffer->put_u64((static_cast<std::uint64_t>(text_palette[row & 0x01]) << 32) | text_palette[(row >> 1) & 0x01]);
				}

				if (y % 12 == 11)
				{
					vram_start_address += buffer_width;
				}

				scanline();
			}
		}
		else
		{
			// graphics modes

			switch (get_colors())
			{
			case 2:
			{
				TwoColors two_colors;
				std::uint8_t color_offset;

				if (tcc1014->vdg_control_register->bits.color_set_select)
				{
					color_offset = 10;
				}
				else
				{
					color_offset = 8;
				}

				for (int y = 0; y < lines_per_field; y++)
				{
					std::vector<std::uint8_t> buffer_data = memory->get_video_memory(vram_start_address, vram_start_address + buffer_width);

					for (int x = 0; x < buffer_width; x++)
					{
						two_colors.byte = buffer_data[x];

						// 8 pixels, 2 at a time
						target_screen_buffer->put_u64((static_cast<std::uint64_t>(color_palette_data->data[two_colors.palette_index.second_pixel + color_offset]) << 32) | color_palette_data->data[two_colors.palette_index.first_pixel + color_offset]);
						target_screen_buffer->put_u64((static_cast<std::uint64_t>(color_palette_data->data[two_colors.palette_index.fourth_pixel + color_offset]) << 32) | color_palette_data->data[two_colors.palette_index.third_pixel + color_offset]);
						target_screen_buffer->put_u64((static_cast<std::uint64_t>(color_palette_data->data[two_colors.palette_index.sixth_pixel + color_offset]) << 32) | color_palette_data->data[two_colors.palette_index.fifth_pixel + color_offset]);
						target_screen_buffer->put_u64((static_cast<std::uint64_t>(color_palette_data->data[two_colors.palette_index.eighth_pixel + color_offset]) << 32) | color_palette_data->data[two_colors.palette_index.seventh_pixel + color_offset]);
					}

					vram_start_address += buffer_width;
					scanline();
				}
			}
			break;

			case 4:
			{
				FourColors four_colors;
				std::uint8_t color_offset;

				if (tcc1014->vdg_control_register->bits.color_set_select)
				{
					color_offset = 4;
				}
				else
				{
					color_offset = 0;
				}

				for (int y = 0; y < lines_per_field; y++)
				{
					std::vector<std::uint8_t> buffer_data = memory->get_video_memory(vram_start_address, vram_start_address + buffer_width);

					for (int x = 0; x < buffer_width; x++)
					{
						four_colors.byte = buffer_data[x];

						// 4 pixels, 2 at a time
						target_screen_buffer->put_u64((static_cast<std::uint64_t>(color_palette_data->data[four_colors.palette_index.second_pixel + color_offset]) << 32) | color_palette_data->data[four_colors.palette_index.first_pixel + color_offset]);
						target_screen_buffer->put_u64((static_cast<std::uint64_t>(color_palette_data->data[four_colors.palette_index.fourth_pixel + color_offset]) << 32) | color_palette_data->data[four_colors.palette_index.third_pixel + color_offset]);
					}

					vram_start_address += buffer_width;
					scanline();
				}
			}
			break;
			}
		}
	}
	else
	{
		// coco 3 mode, high resolution available

		if (!tcc1014->video_mode_register->bits.bit_plane)
		{
			// alphanumeric mode

			Ref<Alphanumeric> alphanumeric = Ref<Alphanumeric>(Alphanumeric::_new());

			int lines_per_row = get_lines_per_row();
			int vertical_scroll = 0;

			std::vector<std::uint8_t> buffer_data;

			if (tcc1014->horizontal_offset_register->bits.horizontal_extended)
			{
				buffer_data.resize(256);
			}
			else
			{
				buffer_data.resize(buffer_width);
			}

			if (!tcc1014->vertical_scroll_register->bits.vertical_scroll_3)
			{
				// vertical scrolling is enabled

				vertical_scroll = lines_per_row * (static_cast<float>(tcc1014->vertical_scroll_register->byte & 0x07) / 8);
			}

			if (vertical_scroll)
			{
				// preload the line

				if (tcc1014->horizontal_offset_register->bits.horizontal_extended)
				{
					buffer_data = memory->get_video_memory(vram_start_address, vram_start_address + 256);
				}
				else
				{
					buffer_data = memory->get_video_memory(vram_start_address, vram_start_address + buffer_width);
				}
			}

			int horizontal_offset = 0;

			if (tcc1014->horizontal_offset_register->bits.horizontal_extended)
			{
				horizontal_offset = tcc1014->horizontal_offset_register->get_horizontal_offset() * 2;
			}

			for (int y = vertical_scroll; y < lines_per_field + vertical_scroll; y++)
			{

				if (y % lines_per_row == 0)
				{
					if (tcc1014->horizontal_offset_register->bits.horizontal_extended)
					{
						buffer_data = memory->get_video_memory(vram_start_address, vram_start_address + 256);
					}
					else
					{
						buffer_data = memory->get_video_memory(vram_start_address, vram_start_address + buffer_width);
					}
				}

				for (
					int x = (horizontal_offset ? horizontal_offset : 0);
					x < buffer_width + (horizontal_offset ? horizontal_offset : 0);
					x += 2)
				{
					std::uint32_t text_palette[2] = {0x00, 0x00};
					std::uint8_t row;

					alphanumeric->set_bytes(static_cast<std::int16_t>(buffer_data[x % 256] << 8) | buffer_data[(x + 1) % 256]);

					row = coco3font[coco3translation[alphanumeric->character->byte]][y % lines_per_row];

					text_palette[0] = color_palette_data->data[alphanumeric->attribute->get_background()];
					text_palette[1] = color_palette_data->data[8 + (alphanumeric->attribute->get_foreground() >> 3)];

					if (y % lines_per_row == lines_per_row - 1 && alphanumeric->attribute->bits.underline)
					{
						row = 0xFF;
					}

					// blink state

					target_screen_buffer->put_u64((static_cast<std::uint64_t>(text_palette[(row >> 6) & 0x01]) << 32) | text_palette[(row >> 7) & 0x01]);
					target_screen_buffer->put_u64((static_cast<std::uint64_t>(text_palette[(row >> 4) & 0x01]) << 32) | text_palette[(row >> 5) & 0x01]);
					target_screen_buffer->put_u64((static_cast<std::uint64_t>(text_palette[(row >> 2) & 0x01]) << 32) | text_palette[(row >> 3) & 0x01]);
					target_screen_buffer->put_u64((static_cast<std::uint64_t>(text_palette[row & 0x01]) << 32) | text_palette[(row >> 1) & 0x01]);
				}

				if (y % lines_per_row == lines_per_row - 1)
				{
					if (tcc1014->horizontal_offset_register->bits.horizontal_extended)
					{
						vram_start_address += 256;
					}
					else
					{
						vram_start_address += buffer_width;
					}
				}

				scanline();
			}
		}
		else
		{
			// graphics mode

			switch (get_colors())
			{

			case 2:
			{
				TwoColors two_colors;

				std::vector<std::uint8_t> buffer_data;

				if (tcc1014->horizontal_offset_register->bits.horizontal_extended)
				{
					buffer_data.resize(256);
				}
				else
				{
					buffer_data.resize(buffer_width);
				}

				int horizontal_offset = 0;

				if (tcc1014->horizontal_offset_register->bits.horizontal_extended)
				{
					horizontal_offset = tcc1014->horizontal_offset_register->get_horizontal_offset() * 2;
				}

				for (int y = 0; y < lines_per_field; y++)
				{
					if (tcc1014->horizontal_offset_register->bits.horizontal_extended)
					{
						buffer_data = memory->get_video_memory(vram_start_address, vram_start_address + 256);
					}
					else
					{
						buffer_data = memory->get_video_memory(vram_start_address, vram_start_address + buffer_width);
					}

					for (
						int x = (horizontal_offset ? horizontal_offset : 0);
						x < buffer_width + (horizontal_offset ? horizontal_offset : 0);
						x++)
					{
						two_colors.byte = buffer_data[x % 256];

						// 8 pixels, 2 at a time
						target_screen_buffer->put_u64((static_cast<std::uint64_t>(color_palette_data->data[two_colors.palette_index.second_pixel]) << 32) | color_palette_data->data[two_colors.palette_index.first_pixel]);
						target_screen_buffer->put_u64((static_cast<std::uint64_t>(color_palette_data->data[two_colors.palette_index.fourth_pixel]) << 32) | color_palette_data->data[two_colors.palette_index.third_pixel]);
						target_screen_buffer->put_u64((static_cast<std::uint64_t>(color_palette_data->data[two_colors.palette_index.sixth_pixel]) << 32) | color_palette_data->data[two_colors.palette_index.fifth_pixel]);
						target_screen_buffer->put_u64((static_cast<std::uint64_t>(color_palette_data->data[two_colors.palette_index.eighth_pixel]) << 32) | color_palette_data->data[two_colors.palette_index.seventh_pixel]);
					}

					if (tcc1014->horizontal_offset_register->bits.horizontal_extended)
					{
						vram_start_address += 256;
					}
					else
					{
						vram_start_address += buffer_width;
					}

					scanline();
				}
			}
			break;

			case 4:
			{
				FourColors four_colors;

				std::vector<std::uint8_t> buffer_data;

				if (tcc1014->horizontal_offset_register->bits.horizontal_extended)
				{
					buffer_data.resize(256);
				}
				else
				{
					buffer_data.resize(buffer_width);
				}

				int horizontal_offset = 0;

				if (tcc1014->horizontal_offset_register->bits.horizontal_extended)
				{
					horizontal_offset = tcc1014->horizontal_offset_register->get_horizontal_offset() * 2;
				}

				for (int y = 0; y < lines_per_field; y++)
				{
					if (tcc1014->horizontal_offset_register->bits.horizontal_extended)
					{
						buffer_data = memory->get_video_memory(vram_start_address, vram_start_address + 256);
					}
					else
					{
						buffer_data = memory->get_video_memory(vram_start_address, vram_start_address + buffer_width);
					}

					for (
						int x = (horizontal_offset ? horizontal_offset : 0);
						x < buffer_width + (horizontal_offset ? horizontal_offset : 0);
						x++)
					{
						four_colors.byte = buffer_data[x % 256];

						// 8 pixels, 2 at a time
						target_screen_buffer->put_u64((static_cast<std::uint64_t>(color_palette_data->data[four_colors.palette_index.second_pixel]) << 32) | color_palette_data->data[four_colors.palette_index.first_pixel]);
						target_screen_buffer->put_u64((static_cast<std::uint64_t>(color_palette_data->data[four_colors.palette_index.fourth_pixel]) << 32) | color_palette_data->data[four_colors.palette_index.third_pixel]);
					}

					if (tcc1014->horizontal_offset_register->bits.horizontal_extended)
					{
						vram_start_address += 256;
					}
					else
					{
						vram_start_address += buffer_width;
					}

					scanline();
				}
			}
			break;

			case 16:
			{
				SixteenColors sixteen_colors;

				std::vector<std::uint8_t> buffer_data;

				if (tcc1014->horizontal_offset_register->bits.horizontal_extended)
				{
					buffer_data.resize(256);
				}
				else
				{
					buffer_data.resize(buffer_width);
				}

				int horizontal_offset = 0;

				if (tcc1014->horizontal_offset_register->bits.horizontal_extended)
				{
					horizontal_offset = tcc1014->horizontal_offset_register->get_horizontal_offset() * 2;
				}

				for (int y = 0; y < lines_per_field; y++)
				{
					if (tcc1014->horizontal_offset_register->bits.horizontal_extended)
					{
						buffer_data = memory->get_video_memory(vram_start_address, vram_start_address + 256);
					}
					else
					{
						buffer_data = memory->get_video_memory(vram_start_address, vram_start_address + buffer_width);
					}

					for (
						int x = (horizontal_offset ? horizontal_offset : 0);
						x < buffer_width + (horizontal_offset ? horizontal_offset : 0);
						x++)
					{
						sixteen_colors.byte = buffer_data[x % 256];

						// 8 pixels, 2 at a time
						target_screen_buffer->put_u64((static_cast<std::uint64_t>(color_palette_data->data[sixteen_colors.palette_index.second_pixel]) << 32) | color_palette_data->data[sixteen_colors.palette_index.first_pixel]);
					}

					if (tcc1014->horizontal_offset_register->bits.horizontal_extended)
					{
						vram_start_address += 256;
					}
					else
					{
						vram_start_address += buffer_width;
					}

					scanline();
				}
			}
			break;
			}
		}
	}
	// assert vertical sync switch

	// bottom border (rendered by border_register) + 6 retracing lines
	for (int i = 0; i < vertical_border + 6; i++)
	{
		scanline();
	}
}

void VideoGraphics::scanline()
{
	if (!processor.is_valid())
	{
		return;
	}

	// signal horizontal interrupt from the ACVC

	cpu_drift = processor->execute(cpu_drift + (tcc1014->sam_control_registers->r1_cpu_rate ? CYCLES_PER_LINE_178 : CYCLES_PER_LINE_089), 0);
}

PoolByteArray VideoGraphics::get_image_data()
{
	return target_screen_buffer->get_data_array();
}
