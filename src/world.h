#pragma once
#include "global.h"
#include "debugs.h"
#include "objectList.h"
#include "tree8.h"
#include <physics.h>
#include <graphics.h>
// -------------------------------------------------------
class World : protected GraphRes, protected Tasks {
public:
							World();
							~World();
	void					exe_tasks();
protected:
	void					exe_ob_pos(uint32_t const);
	void					exe_ob_v(uint32_t const);
	void					exe_cam_v(uint32_t const);
	void					exe_cam_rot(uint32_t const);
	void					exe_ob_create(uint32_t const);
	Physics*				phys;
	Graphics*				graph;
public:
	//void					dodaj(IObiekt* const);
	//void					wezObPromien(IObiekt** const, Obiekt3w const* const) const;
	//void					wykonajFizyka();
private:
	//MapaSasiedzi_			sasiedzi;
	//void					aktualizujSasiedzi();
	//void					niszczObiektSwiat(IObiekt* const);
	//void					niszczObiektySwiat();
	//void					usunProjekcjaZ1(
	//							XMVECTOR* const, float const, float const
	//						) const;
	//void					usunWidokPkt(XMVECTOR* const) const;
	//void					usunWidokWektor(XMVECTOR* const) const;
};
// -------------------------------------------------------


