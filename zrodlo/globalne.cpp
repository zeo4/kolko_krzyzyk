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
#include <fstream>
#include <ctime>
#include <dxerr.h>
#include <xnamath.h>
#include <d3d11.h>
#include <d3dx11.h>
#define DIRECTINPUT_VERSION 0x0800 // bez tego DirectInput zgłasza ostrzeżenie
#include <dinput.h>

using std::string;
using std::to_string;
using std::vector;
using std::list;

// ilość tików przy starcie programu
UINT		tikProgramStart;

HWND		uchOknoGl = 0;
float		szerRend = 640;
float		wysRend = 480;