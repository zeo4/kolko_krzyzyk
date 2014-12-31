struct Primitive {
	float4		vert_coord : SV_POSITION;
};

Primitive main(float4 _vert_coord : VERT_COORD, float4x4 _mtx_wvp : WVP) {
	Primitive _vert;
	_vert.vert_coord = mul(_vert_coord, _mtx_wvp);
	return _vert;
}

