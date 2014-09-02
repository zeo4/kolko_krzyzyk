#pragma once

#include <globalne.h>
#include <stdint.h>
#include <wektor.h>
#include <fizyka.h>

template<class T>
void tworz_buf(ID3D11Device*const& _karta, ID3D11Buffer*& _bufor, UINT const _il_el, UINT const _laczenie) {
	if(_bufor != 0) {
		_bufor->Release();
	}
	
	D3D11_BUFFER_DESC _opis;
	ZeroMemory(&_opis, sizeof(_opis));
	_opis.ByteWidth = sizeof(T) * _il_el;
	_opis.Usage = D3D11_USAGE_DEFAULT;
	_opis.BindFlags = _laczenie;
	_opis.CPUAccessFlags = 0;
	_opis.MiscFlags = 0;

	_karta->CreateBuffer(&_opis, NULL, &_bufor);
}

struct CoOb {
					CoOb();
	XMMATRIX		mac_swp;
};

class ZasobyGraf {
public:
									ZasobyGraf();
									~ZasobyGraf();
	void							tworz_karta_rend_lanc();
	void							tworz_gleb_szab_buf_wid();
	void							tworz_cel_rend_wid();
	void							wiaz_cele_rend() const;
	void							tworz_stan_prob();
	void							wiaz_stan_prob() const;
	void							tworz_rzutnia();
	void							wiaz_rzutnia() const;
	void							tworz_strukt_we();
	void							wiaz_strukt_we() const;
	void							wiaz_topol_prym() const;
	void							tworz_wierz_szad();
	void							wiaz_wierz_szad() const;
	void							tworz_piks_szad();
	void							wiaz_piks_szad() const;
	void							tworz_ob_wierz();
	void							aktual_ob_wierz(WekSegPula<XMFLOAT3>const&);
	void							tworz_ob_wierz_teks();
	void							aktual_ob_wierz_teks(
										WekSegPula<XMFLOAT2>const&);
	void							wiaz_ob_wierz(WekSegPula<XMFLOAT3>const&,
										WekSegPula<XMFLOAT2>const&)const;
	void							tworz_ob_ind();
	void							aktual_ob_ind(WekSegPula<DWORD>const&);
	void							wiaz_ob_ind(WekSegPula<DWORD>const&)const;
	void							tworz_co_ob();
	void							aktual_co_ob(CoOb const&);
	void							wiaz_co_ob() const;
	void							czysc_ekran() const;
	ID3D11Device*					karta;
	IDXGISwapChain*					lanc;
	ID3D11DeviceContext*			rend;
	ID3D11Buffer*					ob_wierz_buf;
	ID3D11Buffer*					ob_wierz_teks_buf;
	ID3D11Buffer*					ob_ind_buf;
	ID3D11VertexShader*				wierz_szad;
	ID3D10Blob*						wierz_szad_buf;
	ID3D11PixelShader*				piks_szad;
	ID3D10Blob*						piks_szad_buf;
	ID3D10Blob*						blad_szad_buf;
	ID3D11Texture2D*				gleb_szab_teks2;
	ID3D11DepthStencilView*			gleb_szab_wid;
	ID3D11RenderTargetView*			cel_rend_wid;
	ID3D11SamplerState*				stan_prob;
	D3D11_VIEWPORT					rzutnia;
	ID3D11InputLayout*				strukt_we;
	ID3D11Buffer*					co_ob_buf;
};

enum {
	MOD_KURSOR,
	MOD_KWADRAT,
	MOD_DIAMENT,
	TEKS_KURSOR = 0,
	TEKS_KWADRAT,
	TEKS_DIAMENT,
};

class Grafika {
public:
	void						lacz_fiz(Fizyka*const&);
	void						inic();
	void						wyk_co_klatka();
	void						wyk_co_ob(uint32_t const&);
	uint32_t					wpisz_ob(uint32_t const&, uint32_t const&);
	void						maz_ob(uint32_t const&);
	void						pakuj();
private:
	uint32_t					wpisz_model(uint32_t const&);
	void						wpisz_teks(uint32_t const&);
	char const*const			wez_teks_sciezka(uint32_t const&) const;
	void						aktual_co_ob(uint32_t const& _nr);
	XMFLOAT3					kam_poz;
	XMFLOAT3					kam_cel;
	XMFLOAT3					kam_gora;
	float						proj_kat;
	float						proj_blizsza;
	float						proj_dalsza;
	WekSegPula<XMFLOAT3>		ob_wierz;
	WekSegPula<XMFLOAT2>		ob_wierz_teks;
	WekSegPula<DWORD>			ob_ind;
	WekLuz<uint32_t>			ob_teks;
	ZasobyGraf					zas;
	Fizyka*						fiz;
	CoOb						co_ob;
	typedef ID3D11ShaderResourceView* TeksWid;
	WekLuz<TeksWid>					teks_wid;
	WekLuz<int32_t>					teks_ref_il;
};



