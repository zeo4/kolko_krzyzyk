#pragma once
#include <globalne.h>
#include <debug.h>
#include <swiat.h>
#include <zadania.h>
// -------------------------------------------------------
class Logika : protected Zadania {
public:
	void			wykonaj();
	void			obsluz_wej(MSG const&);
	void			wez_wyn();
	void			inicScena();
protected:
	DaneGra			gra_dane;
	Swiat			swiat;
};
// -------------------------------------------------------




