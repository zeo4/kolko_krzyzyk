struct A {float val;};

RWStructuredBuffer<A> test1 : register(u0);
RWStructuredBuffer<A> test2 : register(u1);

[numthreads(1, 1, 1)]
void main(uint3 _gr : SV_GroupID) {
	float a = test1[0].val;
	test2[0].val = a;
}




