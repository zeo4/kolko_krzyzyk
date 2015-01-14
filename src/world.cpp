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
void World::set_loc_ob(uint32_t const _i_task) {
	if(!flg_task[TASK_DETECT_COLL]) insert_task(Task{TASK_DETECT_COLL, 0});
	if(!flg_task[TASK_UPDATE_POS_OB]) insert_task(Task{TASK_UPDATE_POS_OB, 0});
	if(!flg_task[TASK_CULL_OCCL]) insert_task(Task{TASK_CULL_OCCL, 0});
	if(!flg_task[TASK_DRAW]) insert_task(Task{TASK_DRAW, 0});
}
void World::set_v_ob(uint32_t const _i_task) {
	if(!flg_task[TASK_DETECT_COLL]) insert_task(Task{TASK_DETECT_COLL, 0});
	if(!flg_task[TASK_UPDATE_POS_OB]) insert_task(Task{TASK_UPDATE_POS_OB, 0});
	if(!flg_task[TASK_CULL_OCCL]) insert_task(Task{TASK_CULL_OCCL, 0});
	if(!flg_task[TASK_DRAW]) insert_task(Task{TASK_DRAW, 0});
}
void World::set_rot_cam(uint32_t const _i_task) {
	if(!flg_task[TASK_UPDATE_POS_OB]) insert_task(Task{TASK_UPDATE_POS_OB, 0});
	if(!flg_task[TASK_CULL_OCCL]) insert_task(Task{TASK_CULL_OCCL, 0});
	if(!flg_task[TASK_DRAW]) insert_task(Task{TASK_DRAW, 0});
}
void World::set_loc_cam(uint32_t const _i_task) {
	if(!flg_task[TASK_UPDATE_POS_OB]) insert_task(Task{TASK_UPDATE_POS_OB, 0});
	if(!flg_task[TASK_CULL_OCCL]) insert_task(Task{TASK_CULL_OCCL, 0});
	if(!flg_task[TASK_DRAW]) insert_task(Task{TASK_DRAW, 0});
}
void World::set_v_cam(uint32_t const _i_task) {
	if(!flg_task[TASK_UPDATE_POS_OB]) insert_task(Task{TASK_UPDATE_POS_OB, 0});
	if(!flg_task[TASK_CULL_OCCL]) insert_task(Task{TASK_CULL_OCCL, 0});
	if(!flg_task[TASK_DRAW]) insert_task(Task{TASK_DRAW, 0});
}
void World::create_ob(uint32_t const _i_task) {
	if(!flg_task[TASK_DETECT_COLL]) insert_task(Task{TASK_DETECT_COLL, 0});
	if(!flg_task[TASK_UPDATE_POS_OB]) insert_task(Task{TASK_UPDATE_POS_OB, 0});
	if(!flg_task[TASK_CULL_OCCL]) insert_task(Task{TASK_CULL_OCCL, 0});
	if(!flg_task[TASK_DRAW]) insert_task(Task{TASK_DRAW, 0});
}
// -------------------------------------------------------

/*
World::~World() {
	niszczObiektySwiat();
}
void World::aktualizujSasiedzi() {
	//logi.pisz("  #", "");
	Drzewo8 drzewo8(XMFLOAT3(0,0,0));
	ListaObiekty::const_iterator itA;
	ZbiorOb3w_::const_iterator itB;
	ZbiorOb3w_ obiektyRob;
	for(itA = obiektySwiat.begin(); itA != obiektySwiat.end(); ++itA) {
		(*itA)->wezFiz()->wezObiekty3W(&obiektyRob);
		for(itB = obiektyRob.begin(); itB != obiektyRob.end(); ++itB) {
			drzewo8.dodaj(*itB);
		}
	}
	//logi.pisz(" ##", "");
	sasiedzi.czysc();
	drzewo8.wezSasiedzi(&sasiedzi);
	//logi.pisz("###", "");
}
void World::dodaj(IObiekt* const ob) {
	obiektySwiat.insert(ob);
	ob->ustawSasiedzi(&sasiedzi);
}
void World::niszczObiektSwiat(IObiekt* const ob) {
	if(obiektySwiat.count(ob) != 0) {
		obiektySwiat.erase(ob);
		delete ob;
	}
}
void World::niszczObiektySwiat() {
	ListaObiekty::const_iterator it;
	for(it = obiektySwiat.begin(); it != obiektySwiat.end(); ) {
		niszczObiektSwiat(*it++);
	}
}*/

/*
void World::usunProjekcjaZ1(XMVECTOR* const pkt3W, float const x, float const y) const {
	// współrzędne 3W przy założeniu, że z = 1 (w ten sposób usuwamy projekcję)
	*pkt3W = XMVectorSetX(*pkt3W, x / macProjekcja._11);
	*pkt3W = XMVectorSetY(*pkt3W, y / macProjekcja._22);
	*pkt3W = XMVectorSetZ(*pkt3W, 1.0f);
}
void World::usunWidokPkt(XMVECTOR* const pkt3W) const {
	XMMATRIX mac = XMLoadFloat4x4(&macWidok);
	// wartość nie ważna, konieczny dla XMMatrixInverse()
	XMVECTOR w;

	XMMATRIX macOdwrot = XMMatrixInverse(&w, mac);
	// czwarty parametr wektora nie istotny, XMVector3TransformCoord() załatwia obliczenia z nim związane
	*pkt3W = XMVector3TransformCoord(*pkt3W, macOdwrot);
}
void World::usunWidokWektor(XMVECTOR* const wekt3W) const {
	XMMATRIX mac = XMLoadFloat4x4(&macWidok);
	// wartość nie ważna, konieczny dla XMMatrixInverse()
	XMVECTOR w;

	XMMATRIX macOdwrot = XMMatrixInverse(&w, mac);
	// czwarty parametr wektora nie istotny, XMVector3TransformNormal() załatwia obliczenia z nim związane
	*wekt3W = XMVector3TransformNormal(*wekt3W, macOdwrot);
}
void World::wezObPromien(IObiekt** const ob, Obiekt3w const* const obWybierajacy) const {
	*ob = NULL;
	if(obiektySwiat.size() == 0) {
		return;
	}

	// początek i kierunek promienia wyboru
	XMVECTOR pocz1 = XMLoadFloat3(&pozKamera);
	XMVECTOR kier1 = obWybierajacy->wezFiz()->wezPrzes();
	kier1 = kier1 - pocz1;

	// obiekty kolidujące z promieniem wyboru, ułożone od najbliższych początku promienia do najdalszych
	ListaObiekty::const_iterator it;
	MapaFloatObiekt_ odlKolizje;
	MapaFloatObiekt_::const_iterator itKol;
	float odlMin = -1;
	for(it = obiektySwiat.begin(); it != obiektySwiat.end(); ++it) {
		if(*it == obWybierajacy) {
			continue;
		}

		(*it)->wezKolizjePromien(&odlKolizje, pocz1, kier1);
		if(odlKolizje.size() > 0) {
			itKol = odlKolizje.upper_bound(0);
			if(odlMin == -1 || itKol->first < odlMin) {
				odlMin = itKol->first;
				*ob = itKol->second;
			}
		}
	}
}
void World::wykonajFizyka() {
	//logi.pisz("test", "przed");
	aktualizujSasiedzi();
	//logi.pisz("test", "po");

	ListaObiekty::const_iterator it;
	for(it = obiektySwiat.begin(); it != obiektySwiat.end(); ++it) {
		(*it)->wezFiz()->liczSwiatyBezkol();
		(*it)->wezFiz()->liczCzasKolizja();
		(*it)->wezFiz()->liczSwiatyParam();
	}
}*/





