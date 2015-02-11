struct Prim {
	float4		pos : SV_Position;
	uint		inst_id : InstanceID;
};
Prim main(float4 _pos : VERT_COORD, float4x4 _wvp : WVP, uint _inst_id : SV_InstanceID) {
	Prim _prim = {_pos, _inst_id};
	_prim.pos = mul(_prim.pos, _wvp);
	return _prim;
}

