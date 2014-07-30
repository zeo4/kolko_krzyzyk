#pragma once

#include <stdint.h>
#include <obiekty.h>
#include <string>

#include "fizyka.h"
#include "grafika.h"

using std::string;

Obiekty3w::Obiekty3w() 
	: _wierz_buf(0),
	_tekstury_wsp_buf(0),
	_tekstury_dane((TeksturaDane*)malloc(8*sizeof(TeksturaDane))),
	_ind_buf(0) {
}
void Obiekty3w::aktual_buf() const {
	D3D11_BUFFER_DESC wierz_opis, ind_opis;

	_wierz_buf->GetDesc(&wierz_opis);
	if(_wierz.wez_il()*sizeof(XMFLOAT3) > wierz_opis.ByteWidth) {
		tworz_bufor<XMFLOAT3>(_wierz_buf, _wierz.wez_il(), D3D11_BIND_VERTEX_BUFFER);
		tworz_bufor<XMFLOAT2>(
			_tekstury_wsp_buf, _tekstury_wsp.wez_il(), D3D11_BIND_VERTEX_BUFFER
		);
	}
	zasoby.render->UpdateSubresource(_wierz_buf, 0, 0, _wierz[0], 0, 0);
	zasoby.render->UpdateSubresource(_tekstury_wsp_buf, 0, 0, _tekstury_wsp[0], 0, 0);

	_ind_buf->GetDesc(&ind_opis);
	if(_ind.wez_il()*sizeof(XMFLOAT2) > ind_opis.ByteWidth) {
		tworz_bufor<DWORD>(_ind_buf, _ind.wez_il(), D3D11_BIND_INDEX_BUFFER);
	}
	zasoby.render->UpdateSubresource(_ind_buf, 0, 0, _ind[0], 0, 0);
}
void Obiekty3w::usun_wykonaj() {
	WektorWpis<uint32_t> el_do_usun = _wierz.wez_do_usun();
	for(uint32_t i = 0; i < el_do_usun.wez_il(); ++i) {
		_nr.usun(el_do_usun[i]); tutaj źle
		--_tekstury_dane[_tekstury_mapa[el_do_usun[i]]].il;
		if(_tekstury_dane[_tekstury_mapa[el_do_usun[i]]].il == 0) {
			_tekstury_dane[_tekstury_mapa[el_do_usun[i]]].widok->Release();
			_tekstury_dane[_tekstury_mapa[el_do_usun[i]]].widok = 0;
		}
	}
	_wierz.usun_wykonaj();
	_tekstury_wsp.usun_wykonaj();
	_tekstury_wid.usun_wykonaj();
	_ind.usun_wykonaj();
}
void Obiekty3w::usun_zbierz(uint32_t const& nr) {
	_wierz.usun_zbierz(nr);
	_tekstury_wsp.usun_zbierz(nr);
	_tekstury_wid.usun_zbierz(nr);
	_ind.usun_zbierz(nr);
}
void Obiekty3w::tworz_ob(XMFLOAT3* wierz, XMFLOAT2* tekstury, uint32_t const& il_wierz, DWORD* indeksy, uint32_t const& il_indeksy, uint32_t tekstura_nr) {
	_nr.wstaw_kon(_wierz.wstaw_kon(wierz, il_wierz));
	_tekstury_wsp.wstaw_kon(tekstury, il_wierz);

	_ind.wstaw_kon(indeksy, il_indeksy);

	tekstury_mapa.wstaw_kon(tekstura_nr);
	++_tekstury_dane[tekstura_nr].il;
	if(_tekstury_dane[tekstura_nr].widok != 0) return;
	D3DX11CreateShaderResourceViewFromFile(zasoby.karta, wez_sciezka_tekstura(tekstura_nr), 0, 0, &_tekstury_dane[tekstura_nr].widok, 0);
}
char const* Obiekty3w::wez_sciezka_tekstura(uint32_t const& nr) {
	switch(nr) {
	case 1: return "tekstura\\1.jpg";
	case 2: return "tekstura\\2.jpg";
	}
}

void Obiekt3w::wezKolizjePromien(MapaFloatObiekt_* const odlKolizje, XMVECTOR const pocz, XMVECTOR const kier) const {
	XMVECTOR p = pocz;
	XMVECTOR k = kier;
	fiz->usunSwiatPkt(&p);
	fiz->usunSwiatWektor(&k);
	fiz->wezKolizjePromien(odlKolizje, p, k);
}





