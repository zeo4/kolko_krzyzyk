#pragma once

class IObiekt;
class Obiekt3w;

typedef map<float const, IObiekt*>						MapaFloatObiekt_;
typedef pair<float const, IObiekt*>						ParaFloatObiekt_;

typedef set<Obiekt3w* const>								ZbiorOb3w_;
typedef set<Obiekt3w const* const>							ZbiorOb3wStale_;

typedef map<Obiekt3w const* const, ZbiorOb3wStale_>			MapaOb3wObiekty3w_;
typedef pair<Obiekt3w const* const, ZbiorOb3wStale_>		ParaOb3wObiekty3w_;

typedef map<char const, float *>						MapaLiteraPole_;
typedef pair<char const, float *>						ParaLiteraPole_;


