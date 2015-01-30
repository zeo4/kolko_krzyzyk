#pragma once
#include <world.h>
#include <physics.h>
#include <graphics.h>
#include <tree8.h>
// -------------------------------------------------------
World::World()
	: phys(new Physics),
	graph(new Graphics) {
	res.create_dev_ctx_chain();
	res.create_scr_size();
	res.create_rtv();
	res.create_ds();
	res.create_viewport();
	res.create_cs();
	res.create_in_lay();
	res.create_vs();
	res.create_gs();
	res.create_ps();
	res.create_ss();
	res.bind_viewport();
	res.bind_prim_topol();
	res.bind_ss();
}
World::~World() {
	res.destroy_dev_ctx_chain();
	delete phys;
	delete graph;
}
void World::do_tasks() {
	if(task.get_col_size() > 1) {
		uint32_t* _mapa = 0;
		task.sort_comp(_mapa);
		task.sort_exe(_mapa);
		task.erase_dupl_comp(_mapa, FunHasz<uint8_t>(), FunHasz2<uint8_t>());
		task.sort_exe(_mapa);
		free(_mapa);
	}

	for(uint32_t _i = 0; _i < task.get_col_size(); ++_i) {
		if(task.get_row(_i) == task.empty) continue;

		switch(((Task*)task[_i])->code) {
		case TASK_SET_LOC_OB: set_loc_ob(_i); break;
		case TASK_SET_V_OB: set_v_ob(_i); break;
		case TASK_SET_ROT_CAM: set_rot_cam(_i); break;
		case TASK_SET_LOC_CAM: set_loc_cam(_i); break;
		case TASK_SET_V_CAM: set_v_cam(_i); break;
		case TASK_CREATE_OB: create_ob(_i); break;
		}
	}

	phys->do_tasks();
	graph->do_tasks();
	flg_task.clear();
}
void World::set_loc_ob(uint32_t const _i_task)  {
	if(!flg_task[TASK_DETECT_COLL]) insert_task(Task{TASK_DETECT_COLL, 0});
	if(!flg_task[TASK_UPDATE_POS_OB]) insert_task(Task{TASK_UPDATE_POS_OB, 0});
	if(!flg_task[TASK_DRAW]) insert_task(Task{TASK_DRAW, 0});
}
void World::set_v_ob(uint32_t const _i_task)  {
	if(!flg_task[TASK_DETECT_COLL]) insert_task(Task{TASK_DETECT_COLL, 0});
	if(!flg_task[TASK_UPDATE_POS_OB]) insert_task(Task{TASK_UPDATE_POS_OB, 0});
	if(!flg_task[TASK_DRAW]) insert_task(Task{TASK_DRAW, 0});
}
void World::set_rot_cam(uint32_t const _i_task)  {
	if(!flg_task[TASK_UPDATE_POS_OB]) insert_task(Task{TASK_UPDATE_POS_OB, 0});
	if(!flg_task[TASK_DRAW]) insert_task(Task{TASK_DRAW, 0});
}
void World::set_loc_cam(uint32_t const _i_task)  {
	if(!flg_task[TASK_UPDATE_POS_OB]) insert_task(Task{TASK_UPDATE_POS_OB, 0});
	if(!flg_task[TASK_DRAW]) insert_task(Task{TASK_DRAW, 0});
}
void World::set_v_cam(uint32_t const _i_task)  {
	if(!flg_task[TASK_UPDATE_POS_OB]) insert_task(Task{TASK_UPDATE_POS_OB, 0});
	if(!flg_task[TASK_DRAW]) insert_task(Task{TASK_DRAW, 0});
}
void World::create_ob(uint32_t const _i_task) {
	if(!flg_task[TASK_DETECT_COLL]) insert_task(Task{TASK_DETECT_COLL, 0});
	if(!flg_task[TASK_UPDATE_POS_OB]) insert_task(Task{TASK_UPDATE_POS_OB, 0});
	if(!flg_task[TASK_DRAW]) insert_task(Task{TASK_DRAW, 0});
}
// -------------------------------------------------------







