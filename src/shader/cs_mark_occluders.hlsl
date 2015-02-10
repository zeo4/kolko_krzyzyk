Buffer<float4> rect : register(t0);
Texture2D<float> occl_rep : register(t1);
RWBuffer<bool> occludees : register(u0);

[numthreads(1, 1, 1)]
void main(uint3 _gr : SV_GroupID) {
	uint _dummy, _width, _height, _mip_count, _u_from, _u_to, _v_from, _v_to;
	occl_rep.GetDimensions(0, _dummy, _dummy, _mip_count);
	uint _rect_offset = _gr.x * 4;

	for(int _mip_lvl = _mip_count-1; _mip_lvl >= 0; --_mip_lvl) {
		occl_rep.GetDimensions(_mip_lvl, _width, _height, _dummy);
		_u_from = (rect[_rect_offset].x+1) / 2 * _width;
		_v_from = (rect[_rect_offset + 2].y-1) / -2 * _height;
		_u_to = (rect[_rect_offset + 2].x+1) / 2 * _width;
		_v_to = (rect[_rect_offset].y-1) / -2 * _height;

		occludees[_gr.x] = true;
		for(uint _u = _u_from; _u <= _u_to; ++_u) {
			for(uint _v = _v_from; _v <= _v_to; ++_v) {
				float _f1 = rect[_rect_offset].z;
				float _f2 = occl_rep.Load(uint3(_u, _v, _mip_lvl));
				if(_f1 <= _f2) {
					occludees[_gr.x] = false;
					break;
				}
			}
			if(occludees[_gr.x] == false)
				break;
		}
		if(occludees[_gr.x] == true)
			return;
	}
}




