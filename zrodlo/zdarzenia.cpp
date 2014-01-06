#ifndef _DEBUG_
#define _DEBUG_
#include "debug.cpp"
#endif

#ifndef _GRAFIKA_
#define _GRAFIKA_
#include "grafika.cpp"
#endif

class Zdarzenie {
public:
};

class Ruch : public Zdarzenie {
	ObiektScena* const		obiekt;
public:
							Ruch(ObiektScena* const);
};
Ruch::Ruch(
	ObiektScena* const		ob
	) : obiekt(ob)
	{}
