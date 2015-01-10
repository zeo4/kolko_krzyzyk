//Buffer<float> rect : register(c0);

float4 main(float4 _vert_loc : VERT_COORD, uint _vert_id : SV_VertexID) : SV_Position {
	//float4 a = rect[_vert_id];
	//float4 b = rect[_vert_id + 1];
	//float4 c = rect[_vert_id + 2];
	//float4 d = rect[_vert_id + 3];
	//return float4(a.z, b.z, c.z, d.z);
	//return float4(_vert_loc.x, _vert_loc.y, _vert_loc.z, 0.0f);
	return _vert_loc;
}


