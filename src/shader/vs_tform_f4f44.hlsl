float4 main(float4 _vert : VERT_COORD, float4x4 _wvp : WVP) : SV_Position {
	float4 _res = mul(_vert, _wvp);
	return _res;
	//return mul(_vert, _wvp);
}

