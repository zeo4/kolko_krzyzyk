#pragma once

#include "obiekty.h"
#include "zasobyGraf.h"

class IGrafika {
public:
	virtual				~IGrafika();
	void virtual		rysuj() const = 0;
};

class Grafika3WPodstawa : public IGrafika {
	void				aktualizujCoObiekt() const;
	void				wiaz() const;
	void				wiazIndeksy() const;
	void				wiazTekstura() const;
	void				wiazWierzcholki() const;
public:
	Obiekt3W* const		obiekt;
						Grafika3WPodstawa(Obiekt3W* const);
	virtual				~Grafika3WPodstawa();
	void virtual		rysuj() const;
};

class GrafikaZbiorPodstawa : public IGrafika {
public:
	ObiektZbior* const		obiekt;
							GrafikaZbiorPodstawa(ObiektZbior* const);
	virtual					~GrafikaZbiorPodstawa();
	void virtual			rysuj() const;
};

