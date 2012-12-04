#ifndef _FLOAT3_H_
#define _FLOAT3_H_

class Float3 {
	public:
		Float3() { x = y = z = 0; }

	Float3(float x, float y, float z) : x(x), y(y), z(z) {};

	~Float3() {};
	float x, y, z;
};
#endif //  _FLOAT3_H_
