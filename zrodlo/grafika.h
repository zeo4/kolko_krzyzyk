#pragma once

#include "obiekty.h"
#include "zasobyGraf.h"
#include <stdint.h>

template<class T>
void tworz_buf(ID3D11Device*const& karta, ID3D11Buffer*& bufor, UINT const il_el, UINT const laczenie) {
	if(bufor != 0) {
		bufor->Release();
	}
	
	D3D11_BUFFER_DESC opisBuf;
	ZeroMemory(&opisBuf, sizeof(opisBuf));
	opisBuf.ByteWidth = sizeof(T) * il_el;
	opisBuf.Usage = D3D11_USAGE_DEFAULT;
	opisBuf.BindFlags = laczenie;
	opisBuf.CPUAccessFlags = 0;
	opisBuf.MiscFlags = 0;

	karta->CreateBuffer(&opisBuf, NULL, &bufor);
}

class Grafika {
public:
									Grafika();
									~Grafika();
	void							przypisz(Obiekty3w const*);
	void							czysc() const;
	inline void						ustaw_kam_poz(
										float const&, float const&, float const&);
	inline void						ustaw_kam_cel(
										float const&, float const&, float const&);
	inline void						ustaw_kam_gora(
										float const&, float const&, float const&);
	inline void						ustaw_proj_kat(float const&);
	inline void						ustaw_proj_blizsza(float const&);
	inline void						ustaw_proj_dalsza(float const&);
	void							wyk_raz();
	void							wyk_co_klatka();
	void							wyk_co_ob(uint32_t const&);
private:
	void							tworz_karta_rend_lanc();
	void							tworz_ob_rend_wid();
	void							tworz_gleb_szab_buf_wid();
	void							tworz_wierz_szad();
	void							tworz_piks_szad();
	void							tworz_strukt_we();
	void							tworz_rzutnia();
	void							tworz_stan_prob();
	void							aktual_wierz_buf();
	void							aktual_wierz_teks_buf();
	void							aktual_ind_buf();
	void							wiaz_wierz_buf();
	Obiekty3w const*				_ob;
	ID3D11Device*					_karta;
	IDXGISwapChain*					_lanc_wym;
	ID3D11DeviceContext*			_rend;
	ID3D11RenderTargetView*			_ob_rend_wid;
	ID3D11Texture2D*				_gleb_szab_buf;
	ID3D11DepthStencilView*			_gleb_szab_wid;
	ID3D11InputLayout*				_strukt_we;
	D3D11_VIEWPORT					_rzutnia;
	ID3D11SamplerState*				_stan_prob;
	struct CoObDane {
								CoObDane();
		XMMATRIX				mac_swp;
	}		_co_ob_dane;
	ID3D11Buffer*					_co_ob_buf;
	ID3D11Buffer*					_wierz_buf;
	ID3D11Buffer*					_wierz_teks_buf;
	ID3D11VertexShader*				_wierz_szad;
	ID3D10Blob*						_wierz_szad_buf;
	ID3D11Buffer*					_ind_buf;
	ID3D11PixelShader*				_piks_szad;
	ID3D10Blob*						_piks_szad_buf;
	ID3D10Blob*						_blad_szad_buf;
	XMFLOAT3						_kam_poz;
	XMFLOAT3						_kam_cel;
	XMFLOAT3						_kam_gora;
	float							_proj_kat;
	float							_proj_blizsza;
	float							_proj_dalsza;
	XMFLOAT4X4						_mac_proj;
};



