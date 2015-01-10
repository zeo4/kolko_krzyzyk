#pragma once
#include <logic.h>
// -------------------------------------------------------
void Logika::exe() {
	//logi.pisz("", "------------");
	//logi.czas();
	world.do_tasks();
	//logi.czas();
	get_result();
	task.pop_back(task.get_col_size());
	uint32_t* _mapa_wier = 0,* _mapa_el = 0;
	result.defrag_comp(_mapa_wier, _mapa_el, 1);
	result.defrag_exe(_mapa_wier, _mapa_el);
	free(_mapa_wier);
	free(_mapa_el);
}
void Logika::get_result() {
	for(uint32_t _i = 0; _i < result.get_col_size(); ++_i) {
		if(result.get_row(_i) == result.empty) continue;

		switch(*(TaskCode*)result[_i]) {
		case TASK_PICK_OB: {
			ResultPickOb _w = *(ResultPickOb*)result[_i];
			data_game.hnd_picked = _w.ob_hnd;
			break;
		}
		}

		result.erase(_i);
	}
}
void Logika::handle_input(MSG const& _wiad) {
	switch(_wiad.message) {
	case WM_LBUTTONDOWN: {
		POINT _pkt;
		GetCursorPos(&_pkt);
		ScreenToClient(uch_okno, &_pkt);
		insert_task(TaskPickOb{TASK_PICK_OB, 0, _pkt.x, _pkt.y});
		break;
	}
	case WM_MOUSEMOVE: {
		if(!data_input.flg_mouse) {
			data_input.flg_mouse = true;
			break;
		}
		POINTS _pkt = MAKEPOINTS(_wiad.lParam);
		_pkt.x = float(_pkt.x) - float(GraphDev::scr_size.width)/2;
		_pkt.y = -float(_pkt.y) + float(GraphDev::scr_size.height)/2;
		if(_pkt.x > 0) {
			insert_task(TaskSetRotCam{TASK_SET_ROT_CAM, task.get_col_size(), XMFLOAT3(0.0f, 3.14f*1.5f/180, 0.0f)});
		} else if(_pkt.x < 0) {
			insert_task(TaskSetRotCam{TASK_SET_ROT_CAM, task.get_col_size(), XMFLOAT3(0.0f, -3.14f*1.5f/180, 0.0f)});
		}
		if(_pkt.y > 0) {
			insert_task(TaskSetRotCam{TASK_SET_ROT_CAM, task.get_col_size(), XMFLOAT3(-3.14f*1.5f/180, 0.0f, 0.0f)});
		}else if(_pkt.y < 0) {
			insert_task(TaskSetRotCam{TASK_SET_ROT_CAM, task.get_col_size(), XMFLOAT3(3.14f*1.5f/180, 0.0f, 0.0f)});
		}
		POINT _p = {GraphDev::scr_size.width/2, GraphDev::scr_size.height/2};
		ClientToScreen(uch_okno, &_p);
		SetCursorPos(_p.x, _p.y);
		data_input.flg_mouse = false;
		break;
	}
	case WM_KEYDOWN: {
		switch(_wiad.wParam) {
		case 0x4c: { // l
			POINT _pkt = {GraphDev::scr_size.width/2, GraphDev::scr_size.height/2};
			ClientToScreen(uch_okno, &_pkt);
			SetCursorPos(_pkt.x, _pkt.y);
			data_input.flg_mouse = false;
			break;
		}
		case 0x42: // b
			insert_task(TaskSetLocOb{TASK_SET_LOC_OB, 0, XMFLOAT3(-1.5f, 0.0f, 4.0f)});
			insert_task(TaskSetLocOb{TASK_SET_LOC_OB, 1, XMFLOAT3(-0.5f, 0.0f, 3.0f)});
			insert_task(TaskSetLocOb{TASK_SET_LOC_OB, 2, XMFLOAT3(0.0f, 0.0f, 2.5f)});
			insert_task(TaskSetLocOb{TASK_SET_LOC_OB, 3, XMFLOAT3(0.5f, 0.0f, 3.0f)});
			insert_task(TaskSetLocOb{TASK_SET_LOC_OB, 4, XMFLOAT3(1.5f, 0.0f, 4.0f)});
			insert_task(TaskSetLocOb{TASK_SET_LOC_OB, 5, XMFLOAT3(0.0f, 0.0f, 6.0f)});
			break;
		}
		if(data_game.hnd_picked != 0x80000000) {
			switch(_wiad.wParam) {
			case 0x57: // w
				insert_task(TaskSetVOb{TASK_SET_V_OB, data_game.hnd_picked, XMFLOAT3(0.0f, 0.05f, 0.0f)});
				break;
			case 0x53: // s
				insert_task(TaskSetVOb{TASK_SET_V_OB, data_game.hnd_picked, XMFLOAT3(0.0f, -0.05f, 0.0f)});
				break;
			case 0x41: // a
				insert_task(TaskSetVOb{TASK_SET_V_OB, data_game.hnd_picked, XMFLOAT3(-0.05f, 0.0f, 0.0f)});
				break;
			case 0x44: // d
				insert_task(TaskSetVOb{TASK_SET_V_OB, data_game.hnd_picked, XMFLOAT3(0.05f, 0.0f, 0.0f)});
				break;
			}
		} else {
			switch(_wiad.wParam) {
			case 0x57: // w
				insert_task(TaskSetVCam{TASK_SET_V_CAM, task.get_col_size(), XMFLOAT3(0.0f, 0.0f, 0.2f)});
				break;
			case 0x53: // s
				insert_task(TaskSetVCam{TASK_SET_V_CAM, task.get_col_size(), XMFLOAT3(0.0f, 0.0f, -0.2f)});
				break;
			case 0x41: // a
				insert_task(TaskSetVCam{TASK_SET_V_CAM, task.get_col_size(), XMFLOAT3(-0.2f, 0.0f, 0.0f)});
				break;
			case 0x44: // d
				insert_task(TaskSetVCam{TASK_SET_V_CAM, task.get_col_size(), XMFLOAT3(0.2f, 0.0f, 0.0f)});
				break;
			case 0x52: // r
				insert_task(TaskSetVCam{TASK_SET_V_CAM, task.get_col_size(), XMFLOAT3(0.0f, 0.2f, 0.0f)});
				break;
			case 0x46: // f
				insert_task(TaskSetVCam{TASK_SET_V_CAM, task.get_col_size(), XMFLOAT3(0.0f, -0.2f, 0.0f)});
				break;
			}
		}
		break;
	}
	case WM_KEYUP: {
		if(data_game.hnd_picked != 0x80000000) {
		} else {
			switch(_wiad.wParam) {
			case 0x57: // w
				insert_task(TaskSetVCam{TASK_SET_V_CAM, task.get_col_size(), XMFLOAT3(1.0f, 1.0f, 0.0f)});
				break;
			case 0x53: // s
				insert_task(TaskSetVCam{TASK_SET_V_CAM, task.get_col_size(), XMFLOAT3(1.0f, 1.0f, 0.0f)});
				break;
			case 0x41: // a
				insert_task(TaskSetVCam{TASK_SET_V_CAM, task.get_col_size(), XMFLOAT3(0.0f, 1.0f, 1.0f)});
				break;
			case 0x44: // d
				insert_task(TaskSetVCam{TASK_SET_V_CAM, task.get_col_size(), XMFLOAT3(0.0f, 1.0f, 1.0f)});
				break;
			case 0x52: // r
				insert_task(TaskSetVCam{TASK_SET_V_CAM, task.get_col_size(), XMFLOAT3(1.0f, 0.0f, 1.0f)});
				break;
			case 0x46: // f
				insert_task(TaskSetVCam{TASK_SET_V_CAM, task.get_col_size(), XMFLOAT3(1.0f, 0.0f, 1.0f)});
				break;
			}
		}
		break;
	}
	}
}
void Logika::init_scene() {
	for(uint32_t _i = 0; _i < 9; ++_i)
		insert_task(TaskCreateOb{TASK_CREATE_OB, _i, MESH_DIAMENT, TEX_DIAMENT});

	data_game.hnd_picked = 0x80000000;
}
// -------------------------------------------------------

//void Logika::inicScena() {
//	try{
//	obiektKursor = world.tworzObiektKursor();
//	obiektWybrany = world.tworzObiektRycerz();
//	world.tworzObiektSmok();
//	Tekst t;
//	//t.pisz("fps: 60, to i tak duzo.");
//	t.pisz("fpsabcd:dsfafadsfsdfdsf");
//	world.dodaj(t.wezObiektGraf());
//	Obiekt3w::test = t.wezObiektGraf();
//	}
//	catch(Wyjatek wyj){
//	ObslugaWyjatek(wyj);
//	}
//}



