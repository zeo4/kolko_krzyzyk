float4 main(float4 _vert : VERT_COORD, float4x4 _wvp : WVP) : SV_Position {
	return mul(_vert, _wvp);
}

