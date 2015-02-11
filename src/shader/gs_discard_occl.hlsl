struct Prim {
	float4		pos : SV_Position;
	uint		inst_id : InstanceID;
};
Buffer<bool> occludees : register(t0);

[maxvertexcount(1)]
void main(point Prim _in[1] : SV_Position, inout PointStream<Prim> _out) {
	if(occludees[_in[0].inst_id] == false) {
		_out.Append(_in[0]);
	}


	//_prim.pos = float4(_in[0].x, _in[0].y, _in[0].z, 1.0f);
	//_out.Append(_prim);
	//_prim.pos = float4(_in[0].x, _in[1].y, _in[0].z, 1.0f);
	//_out.Append(_prim);
	//_prim.pos = float4(_in[1].x, _in[1].y, _in[0].z, 1.0f);
	//_out.Append(_prim);
	//_prim.pos = float4(_in[1].x, _in[0].y, _in[0].z, 1.0f);
	//_out.Append(_prim);
	//_prim.pos = float4(_in[0].x, _in[0].y, _in[0].z, 1.0f);
	//_out.Append(_prim);
}



