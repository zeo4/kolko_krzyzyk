#pragma once

#include "obiekty.h"
#include "zasobyGraf.h"

class IGrafika {
public:
	virtual				~IGrafika();
	virtual void		rysuj() = 0;
};

class Grafika3w : public IGrafika {
	void				aktualizujCoObiekt() const;
	void				wiaz() const;
	void				wiazIndeksy() const;
	void				wiazTekstura() const;
	void				wiazWierzcholki() const;
public:
	Obiekt3w* const		obiekt;
						Grafika3w(Obiekt3w* const);
	virtual				~Grafika3w();
	virtual void		rysuj();
};

class GrafikaZbior : public IGrafika {
public:
	ObiektZbior* const		obiekt;
							GrafikaZbior(ObiektZbior* const);
	virtual					~GrafikaZbior();
	virtual void			rysuj();
};

