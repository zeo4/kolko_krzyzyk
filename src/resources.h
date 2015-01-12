#pragma once
#include <global.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <vec.h>
#include <handles.h>
#include <debugs.h>
#include <vector>
#include <string>
using namespace DirectX;
// -------------------------------------------------------
interface DECLSPEC_UUID("9f251514-9d4d-4902-9d60-18988ab7d4b5") DECLSPEC_NOVTABLE
IDXGraphicsAnalysis : public IUnknown
{
    STDMETHOD_(void, BeginCapture)() PURE;
    STDMETHOD_(void, EndCapture)() PURE;
};
// -------------------------------------------------------
vector<byte> read_bytes(string);
// -------------------------------------------------------
enum InLayNo {
	IN_F3,
	IN_F3F2,
	IN_F3F44,
	IN_F3F2F44,
};
enum CSNo {
	CS_OCCL_CULL,
	TEST_CS_RECT_OCCL,
};
enum VSNo {
	VS_PASS_ON,
	VS_TFORM,
	VS_TFORM_TEX,
	TEST_VS_RECT_OCCL,
};
enum PSNo {
	PS_SAMPLE_TEX,
	PS_WRITE_DEPTH,
	TEST_PS_RECT_OCCL,
};
// -------------------------------------------------------
class GraphDev {
public:
	struct ScrSize {
		uint32_t const				width;
		uint32_t const				height;
	};
	static void						create_dev_ctx_chain();
	static void						destroy_dev_ctx_chain();
	static ScrSize					scr_size;
protected:
	static ID3D11Device*			dev;
	static IDXGISwapChain*			chain;
	static ID3D11DeviceContext*		devctx;
	static ID3D11Debug*				debug;
	static ID3D11InfoQueue*			debug_info;
	static IDXGraphicsAnalysis*		g_analysis;
};
// -------------------------------------------------------
struct GraphR : public GraphDev {
	struct ObGroup : public GraphDev {
										ObGroup();
										~ObGroup();
		void							update_wvp(XMFLOAT4X4 const*const,
											uint32_t const);
		void							update_bbox(float const*const, uint32_t const);
		void							update_is_occluder(bool const*const,
											uint32_t const);
		void							update_vert(XMFLOAT3 const*const, uint32_t const);
		void							update_coord_tex(XMFLOAT2 const*const,
											uint32_t const);
		void							update_ind(DWORD const*const, uint32_t const);
		void							bind_vert(uint32_t const) const;
		XMFLOAT4X4*						wvp_tposed;
		ID3D11Buffer*					wvp_buf;
		ID3D11ShaderResourceView*		wvp_srv;
		ID3D11Buffer*					bbox_buf;
		ID3D11ShaderResourceView*		bbox_srv;
		ID3D11Buffer*					occluder_buf;
		ID3D11UnorderedAccessView*		occluder_uav;
		ID3D11ShaderResourceView*		occluder_srv;
		ID3D11Buffer*					vert_buf;
		ID3D11UnorderedAccessView*		vert_uav;
		ID3D11Buffer*					coord_tex_buf;
		ID3D11Buffer*					ind_buf;
		//void							test_vsga_update(float const*const, uint32_t const,
		//									float const*const);
	};
										GraphR();
										~GraphR();
	void								create_ds();
	void								create_rtv();
	void								create_buf_struct();
	void								create_scr_size();
	void								create_viewport();
	void								create_ss();
	void								create_in_lay();
	void								create_cs();
	void								create_ps();
	void								create_vs();
	void								bind_ss() const;
	void								bind_viewport() const;
	void								bind_prim_topol() const;
	ID3D11Buffer*						buf_struct;
	ID3D11Buffer*						scr_size_buf;
	ID3D11RenderTargetView*				rtv;
	ID3D11Texture2D*					ds_tex2;
	ID3D11DepthStencilView*				ds_dsv;
	ID3D11ShaderResourceView*			ds_srv;
	Vec<ID3D11ComputeShader*>			cs;
	Vec<ID3D11VertexShader*>			vs;
	Vec<ID3D11PixelShader*>				ps;
	ID3D11SamplerState*					ss;
	D3D11_VIEWPORT						viewport;
	Vec<ID3D11InputLayout*>				in_lay;
	ObGroup								ob;
	ObGroup								test;
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
XMFLOAT2 const*const Meshes::get_coord_tex() const {
	return coord_tex[0];
}
XMFLOAT2 const*const Meshes::get_coord_tex(uint32_t const _uch) {
	return coord_tex[no[_uch]];
}
Meshes::Pair_ const Meshes::get_coord_tex_row(uint32_t const _uch) {
	return coord_tex.get_row(no[_uch]);
}
uint32_t Meshes::get_coord_tex_size() const {
	return coord_tex.get_size();
}
DWORD const*const Meshes::get_ind() const {
	return ind[0];
}
DWORD const*const Meshes::get_ind(uint32_t const _uch) {
	return ind[no[_uch]];
}
Meshes::Pair_ const Meshes::get_ind_row(uint32_t const _uch) {
	return ind.get_row(no[_uch]);
}
uint32_t Meshes::get_ind_size() const {
	return ind.get_size();
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
Meshes::Pair_ const Meshes::get_vert_row(uint32_t const _uch) {
	return vert.get_row(no[_uch]);
}
uint32_t Meshes::get_vert_size() const {
	return vert.get_size();
}
// -------------------------------------------------------
class Textures : protected GraphDev {
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
Textures::TeksWid_ const& Textures::get(uint32_t const _uch) {
	return view[no[_uch]];
}
uint32_t Textures::get_size() const {
	return view.get_size();
}
void Textures::sort_exe(uint32_t const*const _map) {
	no.update(_map);
	view.sort_exe(_map);
}
// -------------------------------------------------------
struct DataEngine {
	struct Data {
							~Data();
		UchPula				no;
		Vec<XMFLOAT3>		loc;
		Vec<XMFLOAT3>		v;
		Vec<XMFLOAT4X4>		mtx_world;
		Vec<XMFLOAT4X4>		mtx_wvp;
		Vec2<XMFLOAT4>		bbox_local;
		Vec2<XMFLOAT4>		bbox_scr;
		Vec<float>			t_coll;
		Vec<bool>			occluder;
		Vec<uint32_t>		mesh_hnd;
		Vec<uint32_t>		tex_hnd;
		Meshes				mesh;
		Textures			tex;
	};
	static Data				data_e;
};
// -------------------------------------------------------
struct Cam {
					Cam();
	XMFLOAT4X4		mtx_view;
	XMFLOAT4X4		mtx_proj;
	XMFLOAT4		q;
	XMFLOAT3		loc;
	XMFLOAT3		v;
	float			fov;
	float			near_z;
	float			far_z;
};
struct Camera {
	static Cam		cam;
};
// -------------------------------------------------------
struct DataGame {
	uint32_t		hnd_picked;
};
// -------------------------------------------------------
struct DataInput {
	bool		flg_mouse;
};
// -------------------------------------------------------




