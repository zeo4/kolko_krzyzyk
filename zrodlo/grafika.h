#ifndef _GRAFIKA_H_
#define _GRAFIKA_H_

#include "obiekty.h"
#include "zasobyGraf.h"

class IGrafika {
public:
	virtual				~IGrafika();
	void virtual		rysuj() const = 0;
};

class GrafikaObiekt3WPodstawa : public IGrafika {
	void				aktualizujCoObiekt() const;
	void				wiaz() const;
	void				wiazIndeksy() const;
	void				wiazTekstura() const;
	void				wiazWierzcholki() const;
public:
	Obiekt3W* const		obiekt;
						GrafikaObiekt3WPodstawa(Obiekt3W* const);
	virtual				~GrafikaObiekt3WPodstawa();
	void virtual		rysuj() const;
};

class GrafikaObiektZbiorPodstawa : public IGrafika {
public:
	ObiektZbior* const		obiekt;
							GrafikaObiektZbiorPodstawa(ObiektZbior* const);
	virtual					~GrafikaObiektZbiorPodstawa();
	void virtual			rysuj() const;
};

#endif