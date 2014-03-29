#pragma once

class IObiekt;
class Obiekt3W;

typedef map<float const, IObiekt*>							MapaFloatObiekt_;
typedef pair<float const, IObiekt*>							ParaFloatObiekt_;

typedef map<IObiekt* const, set<IObiekt const* const>>		MapaKolizje_;

typedef map<char const, float *>							MapaLiteraPole_;
typedef pair<char const, float *>							ParaLiteraPole_;

typedef vector<Obiekt3W const* const>						WektObiekty3W_;


