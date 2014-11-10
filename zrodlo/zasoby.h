#pragma once
#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <wek.h>
#include <uch.h>
#include <debug.h>
using namespace DirectX;
// -------------------------------------------------------
template<class T>
void tworz_buf(ID3D11Device*const& _karta, ID3D11Buffer*& _bufor, UINT const _il_el, UINT const _wiazanie) {
	if(_bufor != 0) {
		_bufor->Release();
	}
	
	D3D11_BUFFER_DESC _opis;
	ZeroMemory(&_opis, sizeof(_opis));
	_opis.ByteWidth = sizeof(T) * _il_el;
	_opis.Usage = D3D11_USAGE_DEFAULT;
	_opis.BindFlags = _wiazanie;
	_opis.CPUAccessFlags = 0;
	_opis.MiscFlags = 0;

	HRESULT w = _karta->CreateBuffer(&_opis, NULL, &_bufor);
	if(w != S_OK) logi.pisz("", string("nie stworzono buf z wiazaniem ") + to_string(_wiazanie));
}
// -------------------------------------------------------
enum {
	MOD_TROJKAT,
	MOD_KWADRAT,
	MOD_DIAMENT,
	TEKS_TROJKAT = 0,
	TEKS_KWADRAT,
	TEKS_DIAMENT,
};
// -------------------------------------------------------
struct ZGraf {
	struct CoKlat {
						CoKlat();
		XMMATRIX		mac_swp[1024];
	};
									ZGraf();
									~ZGraf();
	void							inic_wierz();
	void							inic_wsp_teks();
	void							inic_swiat();
	void							inic_ind();
	void							tworz_karta_rend_lanc();
	void							tworz_gleb_szab_wid();
	void							tworz_cel_rend_wid();
	void							tworz_stan_prob();
	void							tworz_rzutnia();
	void							tworz_strukt_we();
	void							tworz_szad_wierz();
	void							tworz_co_klat();
	void							aktual_wierz(XMFLOAT3 const*const,
										uint32_t const);
	void							aktual_wsp_teks(XMFLOAT2 const*const,
										uint32_t const);
	void							aktual_swiat(XMFLOAT4X4 const*const,
										uint32_t const);
	void							aktual_ind(DWORD const*const, uint32_t const);
	void							aktual_co_klat();
	void							ustaw_szad_piks(char const*const);
	void							wiaz_cele_rend() const;
	void							wiaz_stan_prob() const;
	void							wiaz_rzutnia() const;
	void							wiaz_strukt_we() const;
	void							wiaz_topol_prym() const;
	void							wiaz_szad_wierz() const;
	void							wiaz_wierz() const;
	void							wiaz_ind() const;
	void							wiaz_co_klat() const;
	void							wiaz_teks(ID3D11ShaderResourceView*const&) const;
	void							czysc_gleb_szab() const;
	void							czysc_cel_rend() const;
	ID3D11Device*					karta;
	IDXGISwapChain*					lanc;
	ID3D11DeviceContext*			rend;
	ID3D11Buffer*					buf_wierz;
	ID3D11Buffer*					buf_wsp_teks;
	ID3D11Buffer*					buf_swiat;
	ID3D11Buffer*					buf_ind;
	ID3D11Buffer*					buf_co_klat;
	ID3D10Blob*						buf_szad_wierz;
	ID3D10Blob*						buf_szad_piks;
	ID3D10Blob*						buf_szad_blad;
	ID3D11DepthStencilView*			wid_gleb_szab;
	ID3D11RenderTargetView*			wid_cel_rend;
	ID3D11Texture2D*				teks2_gleb_szab;
	ID3D11VertexShader*				szad_wierz;
	ID3D11PixelShader*				szad_piks;
	ID3D11SamplerState*				stan_prob;
	D3D11_VIEWPORT					rzutnia;
	ID3D11InputLayout*				strukt_we;
	CoKlat							co_klat;
};
struct ZasGraf {
	static ZGraf		zas;
};
// -------------------------------------------------------
struct PGraf {
							~PGraf();
	UchPula					nr;
	Wek<uint32_t>			uch_mod;
	Wek<uint32_t>			uch_teks;
	WekLuz<uint32_t>		mod_nr;
	WekLuz<uint32_t>		mod_odn;
	Wek2<XMFLOAT3>			mod_wierz;
	Wek2<XMFLOAT2>			mod_wsp_teks;
	Wek2<DWORD>				mod_ind;
	typedef ID3D11ShaderResourceView* TeksWid_;
	WekLuz<uint32_t>		teks_nr;
	WekLuz<uint32_t>		teks_odn;
	Wek<TeksWid_>			teks_wid;
};
struct ParGraf {
	static PGraf		par_graf;
};
// -------------------------------------------------------
struct PFiz {
						~PFiz();
	UchPula				nr;
	Wek<XMFLOAT3>		poz;
	Wek<XMFLOAT3>		v;
	Wek<XMFLOAT4X4>		mac_swiat;
	Wek2<XMFLOAT3>		bryla_gran;
};
struct ParFiz {
	static PFiz		par_fiz;
};
// -------------------------------------------------------
struct Kam {
									Kam();
	XMFLOAT4X4						mac_wid;
	XMFLOAT4X4						mac_proj;
	XMFLOAT4						kwat;
	XMFLOAT3						poz;
	XMFLOAT3						v;
	float							kat;
	float							blizsza;
	float							dalsza;
};
struct Kamera {
	static Kam		kam;
};
// -------------------------------------------------------
struct DaneGra {
	uint32_t		uch_wybr;
};
// -------------------------------------------------------
struct DaneWej {
	bool		flg_mysz;
};




