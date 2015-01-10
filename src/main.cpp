#pragma once
#pragma comment (lib, "gdi32.lib")
#pragma comment (lib, "user32.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#include <windows.h>
#include <DirectXMath.h>
#include <d3d11.h>
#include <d3dx11.h>
using namespace DirectX;
//#include <global.h>
//#include <debugs.h>
//#include <logic.h>
//#include <DXGIDebug.h>
// -------------------------------------------------------
HINSTANCE uchAp;
HWND uch_okno;
float color;
// -------------------------------------------------------
void test() {
	IDXGISwapChain* _chain;
	ID3D11Device* _dev;
	ID3D11DeviceContext* _devctx;
	ID3D11RenderTargetView* _rtv;
	ID3D11Texture2D* _back_buf;
	ID3D10Blob* _vs_buf;
	ID3D10Blob* _ps_buf;
	ID3D11VertexShader* _vs;
	ID3D11PixelShader* _ps;
	ID3D11InputLayout* _in_lay;
	ID3D11Buffer* _vert_buf;

	DXGI_MODE_DESC _desc;
	ZeroMemory(&_desc, sizeof(DXGI_MODE_DESC));
	_desc.Width = 640;
	_desc.Height = 480;
	_desc.RefreshRate.Numerator = 60;
	_desc.RefreshRate.Denominator = 1;
	_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	_desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	_desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	DXGI_SWAP_CHAIN_DESC _chain_desc;
	ZeroMemory(&_chain_desc, sizeof(DXGI_SWAP_CHAIN_DESC));
	_chain_desc.BufferDesc = _desc;
	_chain_desc.SampleDesc.Count = 1;
	_chain_desc.SampleDesc.Quality = 0;
	_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	_chain_desc.BufferCount = 1;
	_chain_desc.OutputWindow = uch_okno;
	_chain_desc.Windowed = TRUE; 
	_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	HRESULT _r = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &_chain_desc, &_chain, &_dev, NULL, &_devctx);
	_r = _chain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (void**)&_back_buf);
	_r = _dev->CreateRenderTargetView(_back_buf, NULL, &_rtv);
	_back_buf->Release();
	_devctx->OMSetRenderTargets(1, &_rtv, NULL);
	_r = D3DX11CompileFromFile("shader\\effects.fx", 0, 0, "VS", "vs_4_0", 0, 0, 0, &_vs_buf, 0, 0);
	_r = D3DX11CompileFromFile("shader\\effects.fx", 0, 0, "PS", "ps_4_0", 0, 0, 0, &_ps_buf, 0, 0);
	_r = _dev->CreateVertexShader(_vs_buf->GetBufferPointer(), _vs_buf->GetBufferSize(), NULL, &_vs);
	_r = _dev->CreatePixelShader(_ps_buf->GetBufferPointer(), _ps_buf->GetBufferSize(), NULL, &_ps);
	_devctx->VSSetShader(_vs, 0, 0);
	_devctx->PSSetShader(_ps, 0, 0);
	XMFLOAT3 _v[] = {
		XMFLOAT3(-1.0f, -1.0f, 4.0f),
		XMFLOAT3(-1.0f, 1.0f, 4.0f),
		XMFLOAT3(1.0f, 1.0f, 4.0f),
	};
	D3D11_BUFFER_DESC _vert_buf_desc;
	ZeroMemory(&_vert_buf_desc, sizeof(_vert_buf_desc));
	_vert_buf_desc.Usage = D3D11_USAGE_DEFAULT;
	_vert_buf_desc.ByteWidth = sizeof(XMFLOAT3) * 3;
	_vert_buf_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	_vert_buf_desc.CPUAccessFlags = 0;
	_vert_buf_desc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA _vert_init_data; 
	ZeroMemory(&_vert_init_data, sizeof(_vert_init_data));
	_vert_init_data.pSysMem = _v;
	_r = _dev->CreateBuffer(&_vert_buf_desc, &_vert_init_data, &_vert_buf);
	uint32_t _stride = sizeof(XMFLOAT3);
	uint32_t _offset = 0;
	_devctx->IASetVertexBuffers(0, 1, &_vert_buf, &_stride, &_offset);
	D3D11_INPUT_ELEMENT_DESC _lay[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	_dev->CreateInputLayout(_lay, 1, _vs_buf->GetBufferPointer(), _vs_buf->GetBufferSize(), &_in_lay);
	_devctx->IASetInputLayout(_in_lay);
	_devctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D11_VIEWPORT _viewport;
	ZeroMemory(&_viewport, sizeof(D3D11_VIEWPORT));
	_viewport.TopLeftX = 0;
	_viewport.TopLeftY = 0;
	_viewport.Width = 640;
	_viewport.Height = 480;
	_devctx->RSSetViewports(1, &_viewport);
	float _color[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	_devctx->ClearRenderTargetView(_rtv, _color);
	_devctx->Draw(3, 0);
	_chain->Present(0, 0);
	_rtv->Release();
	_vs_buf->Release();
	_ps_buf->Release();
	_vs->Release();
	_ps->Release();
	_in_lay->Release();
	_vert_buf->Release();
	_dev->Release();
	_devctx->Release();
	_chain->Release();
}
// -------------------------------------------------------
LRESULT CALLBACK ProcOknoGl(HWND _uch_okno, UINT wiad, WPARAM paramW, LPARAM paramL) {
	// indywidualna obsługa wiadomości
	switch(wiad){
		// gdy wciśnięty klawisz
		case WM_KEYDOWN:
			// gdy escape
			if(paramW == VK_ESCAPE){
				// niszcz okno
				DestroyWindow(uch_okno);
			}
			return 0;
		// gdy okno zniszczone
		case WM_DESTROY:
			// zakończ aplikację
			PostQuitMessage(0);
			return 0;
	}

	// domyślna obsługa wiadomości
	return DefWindowProc(_uch_okno, wiad, paramW, paramL);
}
// -------------------------------------------------------
void PetlaWiad() {
	// struktura wiadomości
	MSG wiad;

	//Logika logika;
	//logika.init_scene();

	while(1){
		// gdy wiadomość w kolejce
		if(PeekMessage(&wiad, 0, 0, 0, PM_REMOVE)){
			// wyjscie z programu
			if(wiad.message == WM_QUIT) {
				break;
			}
			//logika.handle_input(wiad);
			// przerób na wiadomość tekstową
			TranslateMessage(&wiad);
			// ślij wiadomość
			DispatchMessage(&wiad);
		// gdy brak wiadomości w kolejce
		}else{
			// rysuj scenę
			test();
			//logika.exe();
		}
	}
}
// -------------------------------------------------------
int WINAPI WinMain(HINSTANCE uchAplikacji, HINSTANCE uchPoprzAplikacji, PSTR liniaKomend, int opcjaWysw) {
	// test
	//IDXGIDebug::ReportLiveObjects();

	uchAp = uchAplikacji;

	// twórz klasę okna głównego
	WNDCLASSEX klasaOkno;
	klasaOkno.cbSize = sizeof(WNDCLASSEX);
	klasaOkno.style = CS_HREDRAW|CS_VREDRAW;
	klasaOkno.lpfnWndProc = ProcOknoGl;
	klasaOkno.cbClsExtra = 0;
	klasaOkno.cbWndExtra = 0;
	klasaOkno.hInstance = uchAplikacji;
	klasaOkno.hIcon = ::LoadIcon(0, IDI_APPLICATION);
	klasaOkno.hCursor = ::LoadCursor(0, IDC_ARROW);
	klasaOkno.hbrBackground = static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));
	klasaOkno.lpszMenuName = NULL;
	klasaOkno.lpszClassName = "KolkoKrzyzyk";
	klasaOkno.hIconSm = NULL;

	// rejestruj klasę okna głównego
	if(!RegisterClassEx(&klasaOkno)){
		MessageBox(0, "Rejestracja klasy okna glownego", 0, MB_OK|MB_ICONERROR);
		return 0;
	}

	// stwórz okno główne
	uch_okno = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		"KolkoKrzyzyk",
		"Kolko Krzyzyk",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		//GraphDev::scr_size.width,
		//GraphDev::scr_size.height,
		640,
		480,
		0,
		0,
		uchAplikacji,
		0);
	if(uch_okno == 0){
		MessageBox(0, "Tworzenie okna glownego", 0, MB_OK|MB_ICONERROR);
		return 0;
	}

	// pokaż i odśwież okno
	ShowWindow(uch_okno, opcjaWysw);
	UpdateWindow(uch_okno);

	// pętla wiadomości
	PetlaWiad();

	return 1;
}
// -------------------------------------------------------

// input layout zgodny z parametrami wejściowymi szadera wierzchołków - sprawdzić
// template tworzBufor bez danych inicjalizujących
// wypelnijBufWierz(): potestować wypełnianie bufora (skąd UpdateSubresource ma wiedzieć ile skopiować z danych źródłowych)
// Tworzyć kontroler solidnie, szczególnie z uwzględnieniem uchwytu aplikacji. Teraz jest porozrzucany w głównych funkcjach.
// Obslużyć sytuację: Wskaźnik do wskaźnika do wskaźnika ... Tak aby nie można było zmieniać ob2 w: const Wsk<ob1> -> zwykły wsk_ob2 -> ob2 <- to już można zmieniać.
// Obiekty 3W bazują na wierzchołkach utworzonych poprzez tablicę o stałej ilości elementów. Usuwanie takiej tablicy nie wymaga uwagi. Ale co gdy będzie używana tablica wierzchołków stworzona przez new?
// Przy dodaniu macierzy obrotu do klasy obiektu 3W, uwzględnić to w: pobieraniu bryły granicznej, pobieraniu obszaru siatki.
// Kiedy już nowa fizyka zacznie działać pousuwać niepotrzebne funkcje.
// Po dopisaniu zakańczającej klasy FizykiZbioru i FizykiObiektu, sprawdzić czy dodając tylko ją jako przyjaciela klas obiektów, wystarczy.
// Poprawić tworzenie obiektów tak aby to nie świat tylko logika gry tworzyła poszczególne obiekty, które potem sa "podpinane" do świata.
// Napisać dekstruktory do wszystkich klas fizyk i grafik.
// Dopracować wybieranie promieniem tak aby zawsze nie wybierał po modelach i to dodatkowo wszystkich obiektów.
// Kolizje oparte na założeniu że prędkość obiektu (obiekt->v) to jest prędkość per klatka. Nie koniecznie tak musi być później. Gdyby, to dzielić prędkość przez ilosc klatek i potem używać takiej prędkości.
// Przy kolizji zapobiegać "przestrzeleniu" obiektów.
// Cała inicjalizacja do logów.
// Napisać kolizje modeli tak aby się po sobie zsuwały. Wykorzystać siły (F).
// Dorobić przy kolizjach wpływ od kolidujących obiektów i ich rodziców. Nie uwzględnia parametrów rodzica przy sprawdzaniu kolizjii.
// Nie sprawdzać kolizji obiektów z tego samego zbioru, lub przynajmniej rozróżniać czy sprawdzać czy nie w zależności od rodzaju zbioru (kolizyjny, niekolizyjny) albo rodzaju elementów w zbiorze (kolizyjne, niekolizyjne)
// Kolizja liczona na podstawie prostej przesunięcia brył (nie jest liczona po łuku przesunięcia). Dorobić.
// Dopisać kolizje dla szybko poruszających się obiektów.
// W procesie tworzenia klatki najpierw usuwać obiekty, potem wykonywać na nich operacje (tak że gdy wykonywane są na usuniętych, to te wykonają odpowiednią czynność związaną z usunięciem danego obiektu), a dopiero potem dodawać nowe obiekty.
// Tworzyć tekstury w jednej tablicy tekstur a nie każdą osobno.
// Wektory: Za każdym razem kiedy jest zmiana, trzeba jawnie aktualizować nagłówek obszaru pamięci. Wykombinować jak nie aktualizować tego jawnie.
// Usunąć pobieranie wielkości bufora prosto z karty graficznej przy aktualizacji bufora. Transfer GPU -> CPU jest wolny. Niech rozmiar będzie trzymany po stronie CPU również.
// Fizyka: zmienić miejsce pobierania rozmiaru obszaru klienta okna, tak aby pobierany był rzadziej.
// ZasobyGraf: ustawić wszystkie możliwe metody na inline.

// Przetestować Wek2::wstaw_kon(Wek2) czy działa dobrze.
// Ukladac co jakis czas (np. 60 klatek) obiekty wedlug rozmieszczenia w drzewie kolizji, lub ukladac co klatke czesc obiektow wedlug drzewa, w kolejnej klatce kolejna czesc obiektow, itd.. Pomoze to zachowac lokalnosc obiektow przy wykrywaniu kolizji. ALE ROWNIEZ SORTOWANIE WEDLUG MESH/TEX PRZED WYRYSOWANIEM.






