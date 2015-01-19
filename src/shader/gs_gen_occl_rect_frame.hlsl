struct Prim {
	float4		pos : SV_Position;
};

// _in[0] - most left, bottom, near bbox coord
// _in[1] - most right, top, far bbox coord
[maxvertexcount(5)]
void main(line float4 _in[2] : SV_Position, inout LineStream<Prim> _out) {
	Prim _prim;
	_prim.pos = float4(_in[0].x, _in[0].y, _in[0].z, 1.0f);
	_out.Append(_prim);
	_prim.pos = float4(_in[0].x, _in[1].y, _in[0].z, 1.0f);
	_out.Append(_prim);
	_prim.pos = float4(_in[1].x, _in[1].y, _in[0].z, 1.0f);
	_out.Append(_prim);
	_prim.pos = float4(_in[1].x, _in[0].y, _in[0].z, 1.0f);
	_out.Append(_prim);
	_prim.pos = float4(_in[0].x, _in[0].y, _in[0].z, 1.0f);
	_out.Append(_prim);
}



