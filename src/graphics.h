#pragma once
#include <global.h>
#include <stdint.h>
#include <vec.h>
#include <physics.h>
#include <handles.h>
// -------------------------------------------------------
class Graphics : protected Tasks, protected GraphRes, protected Camera, protected GraphPar, protected PhysPar {
public:
	void					exe_tasks();
protected:
	void					exe_cam_v(uint32_t const);
	void					exe_cam_rot(uint32_t const);
	void					exe_ob_create(uint32_t const);
	void					exe_cam_curr_pos(uint32_t const);
	void					exe_cam_curr(uint32_t const);
	void					exe_ob_sort(uint32_t const);
	void					exe_world_curr(uint32_t const);
	void					exe_occlu_erase(uint32_t const);
	void					exe_draw(uint32_t const);
	void					exe_graph_defrag(uint32_t const);
	void					draw_ob(Vec<uint32_t>const&, Vec<uint32_t>const&,
								Meshes*const, Textures*const) const;
	void					create_occlu_map();
	void					erase_occlu();
	char const*const		get_tex_path(uint32_t const&) const;
public:
	void					erase_ob(uint32_t const&);
};
// -------------------------------------------------------


