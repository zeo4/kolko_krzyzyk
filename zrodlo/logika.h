#ifndef _LOGIKA_H_
#define _LOGIKA_H_

#include "globalne.h"
#include "debug.h"
#include "wejscie.h"
#include "swiat.h"

class Logika {
	IObiekt3W*		obiektKursor;
	IObiekt3W*		obiektWybrany;
	Swiat			swiat;
	Wejscie			wejscie;
	void			uwzglWejscie();
public:
					Logika(HINSTANCE const);
	void			inic3W();
	void			inicScena();
	void			tworzKolejnaKlatka();
	void			wyswietlScena();
};

#endif