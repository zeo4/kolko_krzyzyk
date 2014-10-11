#pragma once

#pragma comment (lib, "gdi32.lib")
#pragma comment (lib, "user32.lib")
#pragma comment (lib, "dxerr.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#include <windows.h>
#include <string>
#include <vector>
#include <list>
#include <forward_list>
#include <deque>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <fstream>
#include <ctime>
#include <dxerr.h>
#include <DirectXMath.h>
//#include <DirectXPackedVector.h>
#include <d3d11.h>
#include <d3dx11.h>
#define DIRECTINPUT_VERSION 0x0800 // bez tego DirectInput zgłasza ostrzeżenie
#include <dinput.h>

using namespace DirectX;
using std::string;
using std::to_string;
using std::vector;
using std::list;
using std::forward_list;
using std::deque;
using std::map;
using std::multimap;
using std::unordered_multimap;
using std::unordered_map;
using std::unordered_set;
using std::set;
using std::pair;
using std::tuple;
using std::get;

HWND extern uch_okno;
float extern szerRend;
float extern wysRend;

template<class TYP>
class Wsk {
	friend class Test_Wsk;
	TYP*			wskaznik;
public:
					Wsk();
					Wsk(TYP*);
	Wsk<TYP>&		operator=(TYP*);
	TYP&			operator*();
	TYP*			operator->();
	TYP const*		operator->() const;
	Wsk<TYP>&		operator++();
	Wsk<TYP>		operator++(int);
	Wsk<TYP>&		operator--();
	Wsk<TYP>		operator--(int);
};
template<class TYP>
Wsk<TYP>::Wsk() : wskaznik(NULL)
	{}
template<class TYP>
Wsk<TYP>::Wsk(
	TYP*		w
	) : wskaznik(w)
	{}
template<class TYP>
Wsk<TYP>& Wsk<TYP>::operator=(
	TYP*		wsk
	) {
	wskaznik = wsk;
	return *this;
}
template<class TYP>
TYP& Wsk<TYP>::operator*() {
	return *wskaznik;
}
template<class TYP>
TYP* Wsk<TYP>::operator->() {
	return wskaznik;
}
template<class TYP>
TYP const* Wsk<TYP>::operator->() const {
	return wskaznik;
}
template<class TYP>
Wsk<TYP>& Wsk<TYP>::operator++() {
	++wskaznik;
	return *this;
}
template<class TYP>
Wsk<TYP> Wsk<TYP>::operator++(
	int
	) {
	Wsk<TYP> w = *this;
	++wskaznik;
	return w;
}
template<class TYP>
Wsk<TYP>& Wsk<TYP>::operator--() {
	--wskaznik;
	return *this;
}
template<class TYP>
Wsk<TYP> Wsk<TYP>::operator--(
	int
	) {
	Wsk<TYP> w = *this;
	--wskaznik;
	return w;
}

struct RaportKolizja {
	bool		flgKolizjaModel;
};

