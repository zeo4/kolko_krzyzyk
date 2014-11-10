#pragma once
#include <logika.h>
// -------------------------------------------------------
void Logika::inicScena() {
	for(uint32_t _i = 0; _i < 5; ++_i)
		wstaw_zad(ZadTworzOb{TWORZ_OB, _i, MOD_DIAMENT, TEKS_DIAMENT});

	dane_gra.uch_wybr = 0x80000000;
}
void Logika::obsluz_wej(MSG const& _wiad) {
	switch(_wiad.message) {
	case WM_LBUTTONDOWN: {
		POINT _pkt;
		GetCursorPos(&_pkt);
		ScreenToClient(uch_okno, &_pkt);
		wstaw_zad(ZadWybOb{WYB_OB, 0, _pkt.x, _pkt.y});
		break;
	}
	case WM_MOUSEMOVE: {
		if(!dane_wej.flg_mysz) {
			dane_wej.flg_mysz = true;
			break;
		}
		POINTS _pkt = MAKEPOINTS(_wiad.lParam);
		_pkt.x = float(_pkt.x) - float(szerRend)/2;
		_pkt.y = -float(_pkt.y) + float(wysRend)/2;
		if(_pkt.x > 0) {
			wstaw_zad(ZadKamObrot{KAM_OBROT, zad.wez_il_wier(), XMFLOAT3(0.0f, 3.14f*1/180, 0.0f)});
		} else if(_pkt.x < 0) {
			wstaw_zad(ZadKamObrot{KAM_OBROT, zad.wez_il_wier(), XMFLOAT3(0.0f, -3.14f*1/180, 0.0f)});
		}
		if(_pkt.y > 0) {
			wstaw_zad(ZadKamObrot{KAM_OBROT, zad.wez_il_wier(), XMFLOAT3(-3.14f*1/180, 0.0f, 0.0f)});
		}else if(_pkt.y < 0) {
			wstaw_zad(ZadKamObrot{KAM_OBROT, zad.wez_il_wier(), XMFLOAT3(3.14f*1/180, 0.0f, 0.0f)});
		}
		POINT _p = {szerRend/2, wysRend/2};
		ClientToScreen(uch_okno, &_p);
		SetCursorPos(_p.x, _p.y);
		dane_wej.flg_mysz = false;
		break;
	}
	case WM_KEYDOWN: {
		switch(_wiad.wParam) {
		case 0x4c: { // l
			POINT _pkt = {szerRend/2, wysRend/2};
			ClientToScreen(uch_okno, &_pkt);
			SetCursorPos(_pkt.x, _pkt.y);
			dane_wej.flg_mysz = false;
			break;
		}
		case 0x42: // b
			wstaw_zad(ZadObPoz{OB_POZ, 0, XMFLOAT3(-1.5f, 0.0f, 4.0f)});
			wstaw_zad(ZadObPoz{OB_POZ, 1, XMFLOAT3(-0.5f, 0.0f, 3.0f)});
			wstaw_zad(ZadObPoz{OB_POZ, 2, XMFLOAT3(0.0f, 0.0f, 2.5f)});
			wstaw_zad(ZadObPoz{OB_POZ, 3, XMFLOAT3(0.5f, 0.0f, 3.0f)});
			wstaw_zad(ZadObPoz{OB_POZ, 4, XMFLOAT3(1.5f, 0.0f, 4.0f)});
			break;
		}
		if(dane_gra.uch_wybr != 0x80000000) {
			switch(_wiad.wParam) {
			case 0x57: // w
				wstaw_zad(ZadObV{OB_V, dane_gra.uch_wybr, XMFLOAT3(0.0f, 0.05f, 0.0f)});
				break;
			case 0x53: // s
				wstaw_zad(ZadObV{OB_V, dane_gra.uch_wybr, XMFLOAT3(0.0f, -0.05f, 0.0f)});
				break;
			case 0x41: // a
				wstaw_zad(ZadObV{OB_V, dane_gra.uch_wybr, XMFLOAT3(-0.05f, 0.0f, 0.0f)});
				break;
			case 0x44: // d
				wstaw_zad(ZadObV{OB_V, dane_gra.uch_wybr, XMFLOAT3(0.05f, 0.0f, 0.0f)});
				break;
			}
		} else {
			switch(_wiad.wParam) {
			case 0x57: // w
				wstaw_zad(ZadKamV{KAM_V, zad.wez_il_wier(), XMFLOAT3(0.0f, 0.0f, 0.1f)});
				break;
			case 0x53: // s
				wstaw_zad(ZadKamV{KAM_V, zad.wez_il_wier(), XMFLOAT3(0.0f, 0.0f, -0.1f)});
				break;
			case 0x41: // a
				wstaw_zad(ZadKamV{KAM_V, zad.wez_il_wier(), XMFLOAT3(-0.1f, 0.0f, 0.0f)});
				break;
			case 0x44: // d
				wstaw_zad(ZadKamV{KAM_V, zad.wez_il_wier(), XMFLOAT3(0.1f, 0.0f, 0.0f)});
				break;
			}
		}
		break;
	}
	case WM_KEYUP: {
		if(dane_gra.uch_wybr != 0x80000000) {
		} else {
			switch(_wiad.wParam) {
			case 0x57: // w
				wstaw_zad(ZadKamV{KAM_V, zad.wez_il_wier(), XMFLOAT3(1.0f, 1.0f, 0.0f)});
				break;
			case 0x53: // s
				wstaw_zad(ZadKamV{KAM_V, zad.wez_il_wier(), XMFLOAT3(1.0f, 1.0f, 0.0f)});
				break;
			case 0x41: // a
				wstaw_zad(ZadKamV{KAM_V, zad.wez_il_wier(), XMFLOAT3(0.0f, 1.0f, 1.0f)});
				break;
			case 0x44: // d
				wstaw_zad(ZadKamV{KAM_V, zad.wez_il_wier(), XMFLOAT3(0.0f, 1.0f, 1.0f)});
				break;
			}
		}
		break;
	}
	}
}
void Logika::wykonaj() {
	//logi.pisz("", "------------");
	//logi.czas();
	//wstaw_zad(Zad{RYSUJ, 0});
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
			dane_gra.uch_wybr = w.uch_ob;
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



