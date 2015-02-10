struct Prim {
	float4		vert_coord : SV_POSITION;
	float2		tex_coord : TEX;
};

Prim main(float4 _vert_coord : VERT_COORD, float2 _tex_coord : TEX_COORD, float4x4 _mtx_wvp : WVP) {
	Prim _vert;
	_vert.vert_coord = mul(_vert_coord, _mtx_wvp);
	_vert.tex_coord = _tex_coord;
	return _vert;
}

