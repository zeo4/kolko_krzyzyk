#pragma once

#include "obiekty.h"
#include "zasobyGraf.h"

class IGrafika {
public:
	virtual				~IGrafika();
	virtual void		rysuj() = 0;
};

class Grafika3WPodstawa : public IGrafika {
	void				aktualizujCoObiekt() const;
	void				aktualizujPoz();
	void				wiaz() const;
	void				wiazIndeksy() const;
	void				wiazTekstura() const;
	void				wiazWierzcholki() const;
public:
	Obiekt3W* const		obiekt;
						Grafika3WPodstawa(Obiekt3W* const);
	virtual				~Grafika3WPodstawa();
	virtual void		rysuj();
};

class GrafikaZbiorPodstawa : public IGrafika {
public:
	ObiektZbior* const		obiekt;
							GrafikaZbiorPodstawa(ObiektZbior* const);
	virtual					~GrafikaZbiorPodstawa();
	virtual void			rysuj();
};

