struct Prim {
	float4		vert : SV_Position;
	float2		tex_coord : TEX_COORD;
};

Prim main(float4 _vert : VERT_COORD, float2 _tex_coord : TEX_COORD) {
	Prim _prim = {_vert, _tex_coord};
	return _prim;
}

