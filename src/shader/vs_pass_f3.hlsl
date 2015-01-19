float4 main(float3 _vert : VERT_COORD) : SV_Position {
	return float4(_vert.x, _vert.y, _vert.z, 1.0f);
}

