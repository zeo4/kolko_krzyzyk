#pragma once
#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <vec.h>
#include <handles.h>
#include <debugs.h>
#include <vector>
#include <string>
using namespace DirectX;
// -------------------------------------------------------
template<class T>
void create_buf(ID3D11Device*const& _dev, ID3D11Buffer*& _bufor, UINT const _size, UINT const _bind_flg) {
	if(_bufor != 0) {
		_bufor->Release();
	}
	
	D3D11_BUFFER_DESC _opis;
	ZeroMemory(&_opis, sizeof(_opis));
	_opis.ByteWidth = sizeof(T) * _size;
	_opis.Usage = D3D11_USAGE_DEFAULT;
	_opis.BindFlags = _bind_flg;
	_opis.CPUAccessFlags = 0;
	_opis.MiscFlags = 0;

	HRESULT w = _dev->CreateBuffer(&_opis, NULL, &_bufor);
	if(w != S_OK) logi.pisz("", string("nie stworzono buf z wiazaniem ") + to_string(_bind_flg));
}
// -------------------------------------------------------
vector<byte> read_bytes(string);
// -------------------------------------------------------
enum {
	MESH_TRI,
	MESH_RECT,
	MESH_DIAMENT,
	TEX_TRI = 0,
	TEX_RECT,
	TEX_DIAMENT,
};
enum InputStructNr {
	IN_F4F2,
	IN_F4F2F44,
};
enum VertSzadNo {
	VS_PASS_ON,
	VS_WVP_TRANS,
};
enum PixSzadNo {
	PS_TEX_SAMPLE
};
// -------------------------------------------------------
struct GraphR {
	struct PerFrame {
						PerFrame();
		XMMATRIX		mtx_wvp[1024];
	};
									GraphR();
									~GraphR();
	void							init_vert();
	void							init_coord_tex();
	void							init_world();
	void							init_ind();
	void							create_dev_ctx_chain();
	void							create_depth_stencil();
	void							create_buf_back_rtv();
	void							create_occlu_map();
	void							create_per_frame();
	void							create_viewport();
	void							create_sampl_state();
	void							create_in_lay();
	void							create_szad_pix();
	void							create_szad_vert();
	void							update_vert(XMFLOAT3 const*const,
										uint32_t const);
	void							update_coord_tex(XMFLOAT2 const*const,
										uint32_t const);
	void							update_world(XMFLOAT4X4 const*const,
										uint32_t const);
	void							update_ind(DWORD const*const, uint32_t const);
	void							update_per_frame();
	void							bind_sampl_state() const;
	void							bind_viewport() const;
	void							bind_prim_topol() const;
	void							bind_vert(uint32_t const) const;
	void							bind_per_frame() const;
	ID3D11Device*					dev;
	IDXGISwapChain*					chain;
	ID3D11DeviceContext*			devctx;
	ID3D11Buffer*					buf_vert;
	ID3D11Buffer*					buf_coord_tex;
	ID3D11Buffer*					buf_world;
	ID3D11Buffer*					buf_ind;
	ID3D11Buffer*					buf_per_frame;
	ID3D10Blob*						buf_szad_err;
	ID3D11RenderTargetView*			buf_back_rtv;
	ID3D11Texture2D*				depth_stencil_tex2;
	ID3D11DepthStencilView*			depth_stencil_dsv;
	ID3D11ShaderResourceView*		depth_stencil_srv;
	Vec<ID3D11VertexShader*>		szad_vert;
	Vec<ID3D11PixelShader*>			szad_pix;
	ID3D11SamplerState*				sampl_state;
	D3D11_VIEWPORT					viewport;
	Vec<ID3D11InputLayout*>			in_lay;
	PerFrame						per_frame;
};
struct GraphRes {
	static GraphR		res;
};
// -------------------------------------------------------
class Meshes {
	typedef Pair<uint32_t> Pair_;
public:
									~Meshes();
	inline XMFLOAT3 const*const		get_vert() const;
	inline XMFLOAT3 const*const		get_vert(uint32_t const);
	inline uint32_t					get_vert_size() const;
	inline Pair_ const				get_vert_row(uint32_t const);
	inline XMFLOAT2 const*const		get_coord_tex() const;
	inline XMFLOAT2 const*const		get_coord_tex(uint32_t const);
	inline uint32_t					get_coord_tex_size() const;
	inline Pair_ const				get_coord_tex_row(uint32_t const);
	inline DWORD const*const		get_ind() const;
	inline DWORD const*const		get_ind(uint32_t const);
	inline uint32_t					get_ind_size() const;
	inline Pair_ const				get_ind_row(uint32_t const);
	inline uint32_t					get_mesh_size() const;
	void							sort_comp(uint32_t*&);
	void							sort_exe(uint32_t const*const);
	void							defrag(uint32_t const);
	void							create(uint32_t const);
	void							destroy(uint32_t const);
protected:
	VecSparse<uint32_t>				no;
	VecSparse<uint32_t>				ref;
	Vec2<XMFLOAT3>					vert;
	Vec2<XMFLOAT2>					coord_tex;
	Vec2<DWORD>						ind;
};
DWORD const*const Meshes::get_ind() const {
	return ind[0];
}
DWORD const*const Meshes::get_ind(uint32_t const _uch) {
	return ind[no[_uch]];
}
uint32_t Meshes::get_ind_size() const {
	return ind.get_size();
}
Meshes::Pair_ const Meshes::get_ind_row(uint32_t const _uch) {
	return ind.get_row(no[_uch]);
}
uint32_t Meshes::get_mesh_size() const {
	return vert.get_col_size();
}
XMFLOAT3 const*const Meshes::get_vert() const {
	return vert[0];
}
XMFLOAT3 const*const Meshes::get_vert(uint32_t const _uch) {
	return vert[no[_uch]];
}
uint32_t Meshes::get_vert_size() const {
	return vert.get_size();
}
Meshes::Pair_ const Meshes::get_vert_row(uint32_t const _uch) {
	return vert.get_row(no[_uch]);
}
XMFLOAT2 const*const Meshes::get_coord_tex() const {
	return coord_tex[0];
}
XMFLOAT2 const*const Meshes::get_coord_tex(uint32_t const _uch) {
	return coord_tex[no[_uch]];
}
uint32_t Meshes::get_coord_tex_size() const {
	return coord_tex.get_size();
}
Meshes::Pair_ const Meshes::get_coord_tex_row(uint32_t const _uch) {
	return coord_tex.get_row(no[_uch]);
}
// -------------------------------------------------------
class Textures : protected GraphRes {
	typedef ID3D11ShaderResourceView* TeksWid_;
public:
								~Textures();
	inline uint32_t				get_size() const;
	inline TeksWid_ const&		get(uint32_t const);
	void						sort_comp(uint32_t*&);
	inline void					sort_exe(uint32_t const*const);
	void						defrag(uint32_t const);
	void						create(uint32_t const);
	char const*const			get_path(uint32_t const) const;
	void						destroy(uint32_t const);
protected:
	VecSparse<uint32_t>			no;
	VecSparse<uint32_t>			ref;
	Vec<TeksWid_>				view;
};
void Textures::sort_exe(uint32_t const*const _map) {
	no.update(_map);
	view.sort_exe(_map);
}
uint32_t Textures::get_size() const {
	return view.get_size();
}
Textures::TeksWid_ const& Textures::get(uint32_t const _uch) {
	return view[no[_uch]];
}
// -------------------------------------------------------
struct GraphP {
							~GraphP();
	UchPula					no;
	Vec<uint32_t>			occluder_mesh_hnd;
	Vec<uint32_t>			occluder_tex_hnd;
	Meshes					occluder_mesh;
	Textures				occluder_tex;
	Vec<uint32_t>			occludee_mesh_hnd;
	Vec<uint32_t>			occludee_tex_hnd;
	Meshes					occludee_mesh;
	Textures				occludee_tex;
};
struct GraphPar {
	static GraphP		graph_par;
};
// -------------------------------------------------------
struct PhysP {
						~PhysP();
	UchPula				no;
	Vec<XMFLOAT3>		pos;
	Vec<XMFLOAT3>		v;
	Vec<XMFLOAT4X4>		mtx_world;
	Vec2<XMFLOAT3>		bound_box;
};
struct PhysPar {
	static PhysP		phys_par;
};
// -------------------------------------------------------
struct Cam {
									Cam();
	XMFLOAT4X4						mtx_view;
	XMFLOAT4X4						mtx_proj;
	XMFLOAT4						quat;
	XMFLOAT3						pos;
	XMFLOAT3						v;
	float							angle;
	float							near_z;
	float							far_z;
};
struct Camera {
	static Cam		cam;
};
// -------------------------------------------------------
struct GameData {
	uint32_t		hnd_picked;
};
// -------------------------------------------------------
struct InputData {
	bool		flg_mouse;
};
// -------------------------------------------------------




