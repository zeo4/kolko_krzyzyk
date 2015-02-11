#pragma once
#include <global.h>
#include <stdint.h>
#include <vec.h>
#include <physics.h>
#include <handles.h>
// -------------------------------------------------------
class Graphics : protected Tasks, protected GraphRes, protected Camera, protected DataEngine, protected GraphDev {
protected:
public:
	void			do_tasks();
protected:
	void			update_pos_ob(uint32_t const);
	void			draw(uint32_t const);
	struct OcclCuller {
		void		execute();
		void		create_occl_shape();
		void		draw_depth_occl_shape();
		void		downsample_ds();
		void		mark_occluders();
		void		draw_occl_map();
		void		draw_occl_rep();
	};
	struct Debugger {
		void		draw_bbox();
		void		draw_rect_front_frame();
		void		draw_rect_front_face();
	};
	uint32_t		get_gr_cnt(uint32_t) const;

	void			do_cam_update_pos(uint32_t const);
	void			draw_previous();
	void			draw_ob(Vec<uint32_t>const&, Vec<uint32_t>const&,
						Meshes*const, Textures*const) const;
	void			do_ob_sort(uint32_t const);
};
// -------------------------------------------------------


