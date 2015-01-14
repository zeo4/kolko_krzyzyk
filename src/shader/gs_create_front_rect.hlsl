struct OUT {
	float4		pos : SV_Position;
};

[maxvertexcount(4)]
void main(line float4 _in[2] : SV_Position, inout TriangleStream<OUT> _out : SV_Position) {
	float _left = _in[0].x;
	float _right = _left;
	float _bottom = _in[0].y;
	float _top = _bottom;
	float _near = _in[0].z;
	if(_in[1].x < _left) _left = _in[1].x;
	else if(_in[1].x > _right) _right = _in[1].x;
	if(_in[1].y < _bottom) _bottom = _in[1].y;
	else if(_in[1].y > _top) _top = _in[1].y;
	if(_in[1].z < _near) _near = _in[1].z;

	OUT o;
	o.pos = float4(_left, _bottom, _near, 0.0f);
	_out.Append(o);
	o.pos = float4(_left, _top, _near, 0.0f);
	_out.Append(o);
	o.pos = float4(_right, _top, _near, 0.0f);
	_out.Append(o);
	o.pos = float4(_right, _bottom, _near, 0.0f);
	_out.Append(o);
}



