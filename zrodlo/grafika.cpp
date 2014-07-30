#pragma once

#include "grafika.h"

Grafika::CoObDane::CoObDane()
	: mac_swp(XMMatrixIdentity()) {
}
void Grafika::aktual_co_ob(uint32_t const& nr) {
	// macierz SWP musi być transponowana przed wysłaniem do szadera
	_co_ob_dane.mac_swp = XMMatrixTranspose(
		XMLoadFloat4x4(_ob->_mac_swiat[nr]) * XMLoadFloat4x4(_ob->_mac_widok[nr]) * XMLoadFloat4x4(_ob->_mac_proj[nr])
	);
	zasoby.render->UpdateSubresource(_co_ob_buf, 0, 0, &_co_ob_dane, 0, 0);
}
void Grafika::przypisz(Obiekty3w const* obiekty) {
	_ob = obiekty;
}
void Grafika::rysuj() const {
	uint32_t nr;
	for(uint32_t i = 0; i < _ob->_nr.wez_il(); ++i) {
		nr = _ob->_nr[i];
		aktualCoOb(nr);
		zasoby.render->VSSetConstantBuffers(0, 1, &_co_ob_buf);
		zasoby.render->IASetVertexBuffers(
			0, 2,
			{_wierz_buf, _tekstury_wsp_buf},
			{_wierz.wez_rozm_el(), _wierz.wez_rozm_el()},
			{_wierz.wez_il()*_wierz.wez_rozm_el(), _tekstury_wsp.wez_il()*_tekstury_wsp.wez_rozm_el()}
		);
		zasoby.render->IASetIndexBuffer(_ob->_ind_buf[nr], DXGI_FORMAT_R32_UINT, 0);
		zasoby.render->PSSetShaderResources(
			0, 1, &_ob->_tekstury_dane[_ob->_tekstury_mapa[nr]].widok
		);

		zasoby.render->DrawIndexed(_ob->_ind.wez_seg().drug, _ob->_ind.wez_seg().pierw, 0);
	}
}
void Grafika::tworz_co_ob() {
	tworz_bufor<CoObDane>(_co_ob_buf, 1, D3D11_BIND_CONSTANT_BUFFER);
}



