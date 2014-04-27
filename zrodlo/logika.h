#pragma once

#include "globalne.h"
#include "debug.h"
#include "wejscie.h"
#include "swiat.h"
#include "obiektyGra.h"

class Logika {
	Obiekt3w*		obiektKursor;
	IObiekt*		obiektWybrany;
	Swiat			swiat;
	Wejscie			wejscie;
	void			uwzglWejscie();
public:
					Logika(HINSTANCE const);
	void			inic3W();
	void			inicScena();
	void			tworzKolejnaKlatka();
};

