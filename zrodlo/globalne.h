#ifndef _GLOBALNE_H_
#define _GLOBALNE_H_

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
#include <deque>
#include <map>
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
using std::deque;
using std::map;
using std::set;
using std::pair;

// ilość tików przy starcie programu
UINT		tikProgramStart;

HWND		uchOknoGl = 0;
float		szerRend = 640;
float		wysRend = 480;

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

struct RaportKolizja {
	bool		flgKolizjaModel;
};

#endif