#pragma once
#include <global.h>
#include <debugs.h>
#include <world.h>
#include <tasks.h>
// -------------------------------------------------------
class Logika : protected Tasks {
public:
	void			exe();
	void			handle_input(MSG const&);
	void			get_result();
	void			init_scene();
protected:
	InputData		input_data;
	GameData		game_data;
	World			world;
};
// -------------------------------------------------------




