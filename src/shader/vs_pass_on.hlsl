struct Prim {
	float4		vert_coord : SV_POSITION;
	float2		tex_coord : TTTTT;
};

Prim main(float4 _vert_coord : VERT_COORD, float2 _tex_coord : TEX_COORD) {
	Prim _vert = {_vert_coord, _tex_coord};
	return _vert;
}
