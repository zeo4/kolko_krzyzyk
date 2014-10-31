#pragma once
#include <logika.h>
// -------------------------------------------------------
void Logika::inicScena() {
	for(uint32_t _i = 0; _i < 500; ++_i)
	wstaw_zad(ZadTworzOb{TWORZ_OB, _i, MOD_KWADRAT, TEKS_TROJKAT});
	//for(uint32_t _i = 500; _i < 1000; ++_i)
	//wstaw_zad(ZadTworzOb{TWORZ_OB, _i, MOD_KWADRAT, TEKS_TROJKAT});
	//for(uint32_t _i = 1000; _i < 1500; ++_i)
	//wstaw_zad(ZadTworzOb{TWORZ_OB, _i, MOD_KWADRAT, TEKS_TROJKAT});
	//for(uint32_t _i = 1500; _i < 2000; ++_i)
	//wstaw_zad(ZadTworzOb{TWORZ_OB, _i, MOD_KWADRAT, TEKS_TROJKAT});
	//for(uint32_t _i = 2000; _i < 2500; ++_i)
	//wstaw_zad(ZadTworzOb{TWORZ_OB, _i, MOD_KWADRAT, TEKS_TROJKAT});

	//for(uint32_t _i = 0; _i < 2500; ++_i)
	//	wstaw_zad(ZadTworzOb{TWORZ_OB, _i, MOD_DIAMENT, TEKS_DIAMENT});

	gra_dane.uch_wybr = 0x80000000;
}
void Logika::obsluz_wej(MSG const& _wiad) {
	if(_wiad.message == WM_LBUTTONDOWN) {
		POINT _pkt;
		GetCursorPos(&_pkt);
		ScreenToClient(uch_okno, &_pkt);
		wstaw_zad(ZadWybOb{WYB_OB, 0, _pkt.x, _pkt.y});
	}
	if(_wiad.message == WM_KEYDOWN) {
		if(gra_dane.uch_wybr != 0x80000000) {
			if(_wiad.wParam == 0x41) {
				// lewy
				wstaw_zad(ZadUstawOb{USTAW_OB, gra_dane.uch_wybr, XMFLOAT3(-0.5f, 0.0f, 0.5f)});
			}
			if(_wiad.wParam == 0x44) {
				// prawy
				wstaw_zad(ZadUstawOb{USTAW_OB, gra_dane.uch_wybr, XMFLOAT3(0.5f, 0.0f, 0.5f)});
			}
			if(_wiad.wParam == 0x53) {
				// dół
				wstaw_zad(ZadUstawOb{USTAW_OB, gra_dane.uch_wybr, XMFLOAT3(0.0f, -0.5f, 0.5f)});
			}
			if(_wiad.wParam == 0x57) {
				// góra
				wstaw_zad(ZadUstawOb{USTAW_OB, gra_dane.uch_wybr, XMFLOAT3(0.0f, 0.5f, 0.5f)});
			}
			if(_wiad.wParam == 0x42) {
				// b
				wstaw_zad(ZadUstawOb{USTAW_OB, 0, XMFLOAT3(0.0f, 0.0f, 0.49f)});
			}
		}
	}
}
void Logika::wykonaj() {
	//logi.pisz("", "------------");
	//logi.czas();
	wstaw_zad(Zad{RYSUJ, 0});
	swiat.wyk_zad();
	//logi.czas();
	wez_wyn();
	zad.usun_kon(zad.wez_il_wier());
	uint32_t* _mapa_wier = 0,* _mapa_el = 0;
	wyn.defrag_licz(_mapa_wier, _mapa_el, 1);
	wyn.defrag_wyk(_mapa_wier, _mapa_el);
	free(_mapa_wier);
	free(_mapa_el);
}
void Logika::wez_wyn() {
	for(uint32_t _i = 0; _i < wyn.wez_il_wier(); ++_i) {
		if(wyn.wez_wier(_i) == wyn.pusty) continue;

		switch(*(KodZad*)wyn[_i]) {
		case WYB_OB: {
			WynWybOb w = *(WynWybOb*)wyn[_i];
			gra_dane.uch_wybr = w.uch_ob;
			break;
		}
		}

		wyn.usun(_i);
	}
}
// -------------------------------------------------------

//void Logika::inicScena() {
//	try{
//	obiektKursor = swiat.tworzObiektKursor();
//	obiektWybrany = swiat.tworzObiektRycerz();
//	swiat.tworzObiektSmok();
//	Tekst t;
//	//t.pisz("fps: 60, to i tak duzo.");
//	t.pisz("fpsabcd:dsfafadsfsdfdsf");
//	swiat.dodaj(t.wezObiektGraf());
//	Obiekt3w::test = t.wezObiektGraf();
//	}
//	catch(Wyjatek wyj){
//	ObslugaWyjatek(wyj);
//	}
//}



