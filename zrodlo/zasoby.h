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

	HRESULT w = _karta->CreateBuffer(&_opis, NULL, &_bufor);
	if(w != S_OK) logi.pisz("", "nie stworzono buf");
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
	struct CoOb {
						CoOb();
		XMMATRIX		mac_swp;
	};
									ZGraf();
									~ZGraf();
	void							inic_mod_wierz();
	void							inic_mod_teks();
	void							inic_mod_ind();
	void							tworz_karta_rend_lanc();
	void							tworz_gleb_szab_wid();
	void							tworz_cel_rend_wid();
	void							tworz_stan_prob();
	void							tworz_rzutnia();
	void							tworz_strukt_we();
	void							tworz_szad_wierz();
	void							tworz_szad_piks();
	void							tworz_co_ob();
	void							aktual_mod_wierz(Wek2<XMFLOAT3>const&);
	void							aktual_mod_teks(Wek2<XMFLOAT2>const&);
	void							aktual_mod_ind(Wek2<DWORD>const&);
	void							aktual_co_ob();
	void							wiaz_cele_rend() const;
	void							wiaz_stan_prob() const;
	void							wiaz_rzutnia() const;
	void							wiaz_strukt_we() const;
	void							wiaz_topol_prym() const;
	void							wiaz_szad_wierz() const;
	void							wiaz_szad_piks() const;
	void							wiaz_mod_wierz(uint32_t const&) const;
	void							wiaz_mod_ind() const;
	void							wiaz_co_ob() const;
	void							wiaz_teks(ID3D11ShaderResourceView*const&) const;
	void							czysc_ekran() const;
	void							rys_model(uint32_t const&, uint32_t const&) const;
	ID3D11Device*					karta;
	IDXGISwapChain*					lanc;
	ID3D11DeviceContext*			rend;
	ID3D11Buffer*					mod_wierz_buf;
	ID3D11Buffer*					mod_teks_buf;
	ID3D11Buffer*					mod_ind_buf;
	ID3D11VertexShader*				szad_wierz;
	ID3D10Blob*						szad_wierz_buf;
	ID3D11PixelShader*				szad_piks;
	ID3D10Blob*						szad_piks_buf;
	ID3D10Blob*						szad_blad_buf;
	ID3D11Texture2D*				gleb_szab_teks2;
	ID3D11DepthStencilView*			gleb_szab_wid;
	ID3D11RenderTargetView*			cel_rend_wid;
	ID3D11SamplerState*				stan_prob;
	D3D11_VIEWPORT					rzutnia;
	ID3D11InputLayout*				strukt_we;
	CoOb							co_ob;
	ID3D11Buffer*					co_ob_buf;
};
struct ZasGraf {
	static ZGraf		zas;
};
// -------------------------------------------------------
struct Ob {
	UchPula				nr;
	Wek<uint32_t>		mod_uch;
	Wek<uint32_t>		teks_uch;
	UchLuz				mod_nr;
	UchLuz				mod_odn;
	Wek2<XMFLOAT3>		mod_wierz;
	Wek2<XMFLOAT2>		mod_teks;
	Wek2<DWORD>			mod_ind;
	typedef ID3D11ShaderResourceView* TeksWid_;
	UchLuz				teks_nr;
	UchLuz				teks_odn;
	Wek<TeksWid_>		teks_wid;
};
struct Obiekty {
	static Ob		ob;
};
// -------------------------------------------------------
struct PFiz {
	Wek<XMFLOAT3>		poz;
	Wek<XMFLOAT3>		v;
	Wek<XMFLOAT4X4>		mac_swiat;
};
struct ParFiz {
	static PFiz		par_fiz;
};
// -------------------------------------------------------
struct Kam {
									Kam();
	XMFLOAT4X4						mac_wid;
	XMFLOAT4X4						mac_proj;
	XMFLOAT3						poz;
	XMFLOAT3						cel;
	XMFLOAT3						gora;
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




