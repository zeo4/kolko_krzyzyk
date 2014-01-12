#ifndef _GLOBALNE_
#define _GLOBALNE_
#include "globalne.cpp"
#endif

#ifndef _DEBUG_
#define _DEBUG_
#include "debug.cpp"
#endif

#ifndef _GRAFZASOBY_
#define _GRAFZASOBY_
#include "grafZasoby.cpp"
#endif

#ifndef _GRAFZDARZENIA_
#define _GRAFZDARZENIA_
#include "grafZdarzenia.cpp"
#endif

#ifndef _GRAF_
#define _GRAF_
#include "graf.cpp"
#endif

#ifndef _FIZZDARZENIA_
#define _FIZZDARZENIA_
#include "fizZdarzenia.cpp"
#endif

#ifndef _FIZ_
#define _FIZ_
#include "fiz.cpp"
#endif

#ifndef _KONTROLERY_
#define _KONTROLERY_
#include "kontrolery.cpp"
#endif

class Gra {
	Fizyk				fizyk;
	Grafik				grafik;
	Kontrolery			kontrolery;
	HINSTANCE const		uchAplikacji;
	ListaOb_			obiektyScena;
	ListaZdarzFiz_		zdarzeniaFiz;
	ListaZdarzGraf_		zdarzeniaGraf;
public:
						Gra(HINSTANCE const);
	void				inic3W();
	void				inicScena();
	void				wyswietlScena();
};
Gra::Gra(
	HINSTANCE const		uchwyt
	) : kontrolery(uchwyt), uchAplikacji(uchwyt)
	{}
void Gra::inic3W() {
	try{
		// twórz opis bufora tylnego
		DXGI_MODE_DESC opisBufTylny;
		ZeroMemory(&opisBufTylny, sizeof(opisBufTylny));
		opisBufTylny.Width = szerRend;
		opisBufTylny.Height = wysRend;
		opisBufTylny.RefreshRate.Numerator = 60;
		opisBufTylny.RefreshRate.Denominator = 1;
		opisBufTylny.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		opisBufTylny.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		opisBufTylny.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// twórz opis łańcucha wymiany
		DXGI_SWAP_CHAIN_DESC opisLancWym;
		ZeroMemory(&opisLancWym, sizeof(opisLancWym));
		opisLancWym.BufferDesc = opisBufTylny;
		opisLancWym.SampleDesc.Count = 1;
		opisLancWym.SampleDesc.Quality = 0;
		opisLancWym.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		opisLancWym.BufferCount = 1;
		opisLancWym.OutputWindow = uchOknoGl;
		opisLancWym.Windowed = true;
		opisLancWym.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		//twórz interfejsy karty graficznej, renderowania i łańcucha wymiany
		wynik = D3D11CreateDeviceAndSwapChain(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			D3D11_CREATE_DEVICE_BGRA_SUPPORT,
			NULL,
			NULL,
			D3D11_SDK_VERSION,
			&opisLancWym,
			&zasoby.lancWym,
			&zasoby.karta,
			NULL,
			&zasoby.render);
		SprawdzWynik(wynik, "Tworz interfejsy: karty, renderowania i lancucha.");

		//przypisz obiekt bufora tylnego do interfejsu
		ID3D11Texture2D* bufTyl;
		wynik = zasoby.lancWym->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&bufTyl);
		SprawdzWynik(wynik, "Przypisz bufor tylny do interfejsu.");
			
		//twórz widok bufora tylnego
		wynik = zasoby.karta->CreateRenderTargetView(bufTyl, NULL, &zasoby.widBufTyl);
		SprawdzWynik(wynik, "Tworz widok bufora tylnego.");
			
		//zwolnij obiekt bufora tylnego
		bufTyl->Release();

		zasoby.tworzGlebiaSzablon();

		zasoby.render->OMSetRenderTargets(1, &zasoby.widBufTyl, zasoby.widokGlebiaSzablon);
	}
	catch(Wyjatek wyj){
		ObslugaWyjatek(wyj);
	}
}
void Gra::inicScena() {
	try{
		Wierzcholek w1[] = {
			Wierzcholek(-0.5f, -0.5f, +0.5f, +0.0f, +1.0f),
			Wierzcholek(+0.0f, +0.5f, +0.5f, +0.5f, +0.0f),
			Wierzcholek(+0.5f, -0.5f, +0.5f, +1.0f, +1.0f),
		};
		Wierzcholek w2[] = {
			Wierzcholek(-1.0f, -1.0f, +1.0f, +0.0f, +1.0f),
			Wierzcholek(-1.0f, +0.0f, +1.0f, +0.0f, +0.5f),
			Wierzcholek(+0.0f, -1.0f, +1.0f, +0.5f, +1.0f),
		};

		DWORD indeksyModel[] = {
			0, 1, 2
		};

		Obiekt3W* figura1 = new Obiekt3W;
		figura1->wgrajWierzcholki(w1, 3);
		figura1->wgrajIndeksy(indeksyModel, 3);
		figura1->tworz();
		obiektyScena.push_back(figura1);
		Obiekt3W* figura2 = new Obiekt3W;
		figura2->wgrajWierzcholki(w2, 3);
		figura2->wgrajIndeksy(indeksyModel, 3);
		figura2->tworz();
		obiektyScena.push_back(figura2);

		zasoby.wgrajSzadWierz("szader\\efekty.fx", "SW");
		zasoby.tworzSzadWierz();
		zasoby.wgrajSzadPiks("szader\\efekty.fx", "SP");
		zasoby.tworzSzadPiks();

		zasoby.tworzStruktWe();
		zasoby.wiazStruktWe();

		//ustaw topologię podstaw
		zasoby.render->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		logi.pisz("OK", "Ustaw topologie elementów podstawowych.");

		zasoby.tworzRzutnie();
		zasoby.wiazRzutnie();

		zasoby.tworzCoObiekt();

		zasoby.tworzStanProbkowania();
		zasoby.wiazStanProbkowania();

		figura1->wgrajTeksture("tekstura\\t1.jpg");
		figura1->wiazTeksture();
		// czwarty parametr wektora nie istotny
		figura1->ustawPrzesun(XMVectorSet(+0.0f, +0.0f, +1.0f, 0.0f));
		figura2->ustawPrzesun(XMVectorSet(+0.0f, +0.0f, +2.0f, 0.0f));

		kontrolery.przypnijListaFiz(&zdarzeniaFiz);
		fizyk.przypnijListaFiz(&zdarzeniaFiz);
		fizyk.przypnijObiektyScena(&obiektyScena);
		fizyk.przypnijListaGraf(&zdarzeniaGraf);
		grafik.przypnijListaGraf(&zdarzeniaGraf);
		kontrolery.ustawObiekt(*obiektyScena.begin());
		//kontrolery.ustawObiekt(*(++obiektyScena.begin()));
	}
	catch(Wyjatek wyj){
		ObslugaWyjatek(wyj);
	}
}
void Gra::wyswietlScena() {
	// czyść widok bufora tylnego
	const FLOAT kolor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
	zasoby.render->ClearRenderTargetView(zasoby.widBufTyl, kolor);
	// czyść widok bufora glebi i szablonu
	zasoby.render->ClearDepthStencilView(zasoby.widokGlebiaSzablon, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

	zasoby.wiazSzadWierz();
	zasoby.wiazSzadPiks();
	kontrolery.tworzZdarzenia();
	fizyk.wykonajZdarzenia();
	fizyk.tworzZdarzenia();
	grafik.wykonajZdarzenia();
	
	//przestaw bufory
	zasoby.lancWym->Present(0, 0);
}


