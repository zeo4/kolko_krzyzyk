#pragma once
#include <globalne.h>
#include <debug.h>
#include <swiat.h>
#include <zadania.h>
// -------------------------------------------------------
class Logika : protected Zadania {
public:
	void			rys_klatka();
	void			obsluz_wej(MSG const&);
	void			inicScena();
protected:
	DaneGra			dane_gra;
	Swiat			swiat;
};
// -------------------------------------------------------




