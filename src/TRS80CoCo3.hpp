#ifndef TRS80COCO3_H
#define TRS80COCO3_H

#include <cstdint>
#include <vector>

#include <Godot.hpp>
#include <Node.hpp>
#include <Ref.hpp>

#include "Memory.hpp"
#include "Tcc1014.hpp"
#include "Mc68b09e.hpp"
#include "VideoGraphics.hpp"

namespace godot
{
	class TRS80CoCo3 : public Node
	{
		GODOT_CLASS(TRS80CoCo3, Node)

	private:
	public:
		Ref<Memory> memory;
		Ref<Tcc1014> tcc1014;
		Ref<Mc68b09e> processor;
		Ref<VideoGraphics> video_graphics;

		// C++ Specific Definitions

		TRS80CoCo3();

		~TRS80CoCo3();

		// Godot Specific Definitions

		static void _register_methods();

		void _init(); // our initializer called by Godot

		// Class Specific Definitions

		Ref<Mc68b09e> get_processor();

		Ref<Memory> get_memory();

		Ref<VideoGraphics> get_video_graphics();

		Ref<Tcc1014> get_tcc1014();
	};
}

#endif