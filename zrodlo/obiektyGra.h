#pragma once

#include "globalne.h"
#include "obiekty.h"
#include "typedefy.h"

class IObiektGra {
public:
	virtual IObiekt* const		wezObiektGraf() const = 0;
};

class Tekst : public IObiektGra {
	MapaLiteraPole_			mapaTekstura;
	ObiektZbior* const		literyGraf;
	float static			rozmPiks;
	UINT static				szerTekstura;
	Obiekt3W*				tworzLitera(char const);
public:
							Tekst();
	void					pisz(string const);
	IObiekt* const			wezObiektGraf() const;
};


