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
	void			do_tasks();
protected:
	void			set_loc_ob(uint32_t const);
	void			set_v_ob(uint32_t const);
	void			set_rot_cam(uint32_t const);
	void			set_loc_cam(uint32_t const);
	void			set_v_cam(uint32_t const);
	void			create_ob(uint32_t const);
	void			cull_occl(uint32_t const);
	Physics*		phys;
	Graphics*		graph;
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


