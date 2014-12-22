#pragma once

#include "global.h"

class IObiekt;
class Obiekt3w;

struct Hasz {
	uint64_t		operator()(Obiekt3w const* const);
};

typedef map<float const, IObiekt*>						MapaFloatObiekt_;
typedef pair<float const, IObiekt*>						ParaFloatObiekt_;

typedef set<Obiekt3w* const>							ZbiorOb3w_;
typedef unordered_set<Obiekt3w const* const, Hasz>		Sasiedzi_;
//typedef set<Obiekt3w const* const>						Sasiedzi_;

typedef unordered_map<Obiekt3w const* const, Sasiedzi_, Hasz>		MapaSasiedzi_;
//typedef map<Obiekt3w const* const, Sasiedzi_>						MapaSasiedzi_;
typedef pair<Obiekt3w const* const, Sasiedzi_>						ParaSasiedzi_;

typedef map<char const, float *>						MapaLiteraPole_;
typedef pair<char const, float *>						ParaLiteraPole_;


