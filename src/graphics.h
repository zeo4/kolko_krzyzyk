﻿#pragma once
#include <global.h>
#include <stdint.h>
#include <vec.h>
#include <physics.h>
#include <handles.h>
// -------------------------------------------------------
class Graphics : protected Tasks, protected GraphRes, protected Camera, protected DataEngine, protected GraphDev {
public:
	void			do_tasks();
protected:
	void			update_pos_ob(uint32_t const);
	void			cull_occl(uint32_t const);
	uint32_t		get_gr_cnt(uint32_t) const;
	void			debug_draw_rect_front(uint32_t const);
	void			draw(uint32_t const);

	void			do_cam_update_pos(uint32_t const);
	void			draw_previous();
	void			draw_ob(Vec<uint32_t>const&, Vec<uint32_t>const&,
						Meshes*const, Textures*const) const;
	void			do_ob_sort(uint32_t const);
};
// -------------------------------------------------------


