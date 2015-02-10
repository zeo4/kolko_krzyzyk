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
enum MeshNo{
	MESH_TRI,
	MESH_RECT,
	MESH_DIAMENT,
};
enum TexNo{
	TEX_TRI,
	TEX_RECT,
	TEX_DIAMENT,
};
enum InLayNo {
	IN_VOID,
	IN_F4,
	IN_F4F2,
	IN_F3F2,
	IN_F3F44,
	IN_F4F44,
	IN_F3F2F44,
	IN_F4F2F44,
};
enum CSNo {
	CS_CREATE_OCCL_SHAPE,
	CS_MARK_OCCLUDERS,
	CS_RESIZE_DS_POW2,
	CS_DOWNSAMPLE_OCCL_REP,
};
enum VSNo {
	VS_VOID_PASS_OCCL_RECT,
	VS_F4_PASS,
	VS_F4F2_PASS,
	VS_F4F44_TFORM,
	VS_F4F2F44_TFORM,
	VS_PASS_ON,
	VS_TFORM,
	TEST_VS_RECT_OCCL,
};
enum GSNo {
	GS_GEN_OCCL_RECT_FRAME,
};
enum PSNo {
	PS_SAMPLE_TEX,
	PS_DRAW_DEPTH,
	TEST_PS_RECT_OCCL,
};
enum ViewportNo {
	VIEWPORT_MAIN,
	VIEWPORT_MAP
};
// -------------------------------------------------------
class GraphDev {
public:
	struct Screen {
		uint32_t const				width;
		uint32_t const				height;
	};
	static void						create_dev_ctx_chain();
	static void						destroy_dev_ctx_chain();
	static Screen					screen;
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
		void							update_wvp(void*const*const,
											uint32_t const);
		void							update_bbox(XMFLOAT4 const*const,
											uint32_t const);
		void							update_occl_rect(XMFLOAT4 const*const,
											uint32_t const);
		void							update_occluders(bool const*const, uint32_t const);
		void							update_vert(XMFLOAT3 const*const, uint32_t const);
		void							update_vert(XMFLOAT4 const*const, uint32_t const);
		void							update_coord_tex(XMFLOAT2 const*const,
											uint32_t const);
		void							update_idx(DWORD const*const, uint32_t const);
		void							update_const_buf(void const*const, uint32_t const);
		void							update_so(uint32_t const);
		void							bind_vert(uint32_t const) const;
		Vec<XMFLOAT4X4>					wvp_tposed;
		ID3D11Buffer*					wvp_buf;
		ID3D11ShaderResourceView*		wvp_srv;
		ID3D11Buffer*					wvp_map_buf;
		ID3D11Buffer*					bbox_buf;
		ID3D11ShaderResourceView*		bbox_srv;
		ID3D11Buffer*					occl_rect_buf;
		ID3D11UnorderedAccessView*		occl_rect_uav;
		ID3D11ShaderResourceView*		occl_rect_srv;
		ID3D11Buffer*					occluders_buf;
		ID3D11UnorderedAccessView*		occluders_uav;
		ID3D11ShaderResourceView*		occluders_srv;
		ID3D11Buffer*					vert_buf;
		ID3D11UnorderedAccessView*		vert_uav;
		ID3D11Buffer*					coord_tex_buf;
		ID3D11Buffer*					idx_buf;
		ID3D11Buffer*					const_buf;
		ID3D11Buffer*					so_buf;
	};
										GraphR();
										~GraphR();
	void								create_scr_size();
	void								create_rtv();
	void								create_ds();
	void								create_map();
	void								create_viewport();
	void								create_cs();
	void								create_in_lay();
	void								create_vs();
	void								create_gs();
	void								create_ps();
	void								create_ss();
	void								bind_prim_topol() const;
	void								bind_ss() const;
	ID3D11Buffer*						scr_size_buf;
	ID3D11RenderTargetView*				rtv;
	ID3D11Texture2D*					ds_tex2;
	ID3D11DepthStencilView*				ds_dsv;
	ID3D11ShaderResourceView*			ds_srv;
	ID3D11Texture2D*					occl_rep_tex2;
	Vec<ID3D11UnorderedAccessView*>		occl_rep_mips_uav;
	Vec<ID3D11ShaderResourceView*>		occl_rep_mips_srv;
	ID3D11ShaderResourceView*			occl_rep_srv;
	ID3D11Texture2D*					map_tex2;
	ID3D11RenderTargetView*				map_rtv;
	ID3D11ShaderResourceView*			map_srv;
	Vec<D3D11_VIEWPORT>					viewport;
	Vec<ID3D11ComputeShader*>			cs;
	Vec<ID3D11InputLayout*>				in_lay;
	Vec<ID3D11VertexShader*>			vs;
	Vec<ID3D11GeometryShader*>			gs;
	Vec<ID3D11PixelShader*>				ps;
	ID3D11SamplerState*					ss;
	ObGroup								ob;
	ObGroup								test;
};
struct GraphRes {
	static GraphR		res;
};
// -------------------------------------------------------
class Meshes {
public:
							~Meshes();
	void					insert(uint32_t const, void*const*const, uint32_t const*const);
	void					erase(uint32_t const);
	VecSparse<uint32_t>		no;
	VecSparse<uint32_t>		ref_cnt;
	Vec2<XMFLOAT4>			bbox;
	Vec2<XMFLOAT3>			vert;
	Vec2<XMFLOAT2>			tex_coord;
	Vec2<DWORD>				vert_idx;
};
// -------------------------------------------------------
class Textures {
public:
										~Textures();
	void								insert(uint32_t const, void*const*const);
	void								erase(uint32_t const);
	VecSparse<uint32_t>					no;
	VecSparse<uint32_t>					ref_cnt;
	Vec<ID3D11Texture2D*>				tex;
	Vec<ID3D11ShaderResourceView*>		srv;
};
// -------------------------------------------------------
struct DataEngine {
	struct Data {
							~Data();
		UchPula				no;
		Vec<XMFLOAT3>		loc;
		Vec<XMFLOAT3>		v;
		Vec<XMFLOAT4X4>		world;
		Vec<XMFLOAT4X4>		wvp;
		Vec<XMFLOAT4X4>		wvp_map;
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
					Cam(XMFLOAT4, XMFLOAT3, float, float, float);
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
	static Cam		cam_map;
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




