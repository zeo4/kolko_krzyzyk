Buffer<float4> bbox : register(c0);
Buffer<float4> wvp : register(c1);
RWBuffer<float4> rect : register(u0);

[numthreads(1, 1, 1)]
void main(uint3 _gr : SV_GroupID) {
	uint _wvp_offset = _gr.x * 4;
	uint _bbox_offset = 0;

	// build wvp
	float4x4 _wvp = float4x4(
		wvp[_wvp_offset].x, wvp[_wvp_offset + 1].x, wvp[_wvp_offset + 2].x, wvp[_wvp_offset + 3].x,
		wvp[_wvp_offset].y, wvp[_wvp_offset + 1].y, wvp[_wvp_offset + 2].y, wvp[_wvp_offset + 3].y,
		wvp[_wvp_offset].z, wvp[_wvp_offset + 1].z, wvp[_wvp_offset + 2].z, wvp[_wvp_offset + 3].z,
		wvp[_wvp_offset].w, wvp[_wvp_offset + 1].w, wvp[_wvp_offset + 2].w, wvp[_wvp_offset + 3].w
	);

	// compute rectangle
	float4 _vert = mul(bbox[_bbox_offset], _wvp);
	_vert = _vert / _vert.w;
	float _left = _vert.x;
	float _right = _left;
	float _bottom = _vert.y;
	float _top = _bottom;
	float _near = _vert.z;
	for(uint _i = 1; _i < 8; ++_i) {
		_vert = mul(bbox[_bbox_offset + _i], _wvp);
		_vert = _vert / _vert.w;
		if(_vert.x < _left) _left = _vert.x;
		else if(_vert.x > _right) _right = _vert.x;
		if(_vert.y < _bottom) _bottom = _vert.y;
		else if(_vert.y > _top) _top = _vert.y;
		if(_vert.z < _near) _near = _vert.z;
	}
	rect[_gr.x * 5] = float4(_left, _bottom, _near, 1.0f);
	rect[_gr.x * 5 + 1] = float4(_left, _top, _near, 1.0f);
	rect[_gr.x * 5 + 2] = float4(_right, _top, _near, 1.0f);
	rect[_gr.x * 5 + 3] = float4(_right, _bottom, _near, 1.0f);
	rect[_gr.x * 5 + 4] = float4(_left, _bottom, _near, 1.0f);
}




