#include "colors.h"
#include "d3dx12.h"
#include <dxgi1_4.h>
#include <wrl/client.h>

struct fVector {
	float x, y, z;

	fVector() {
		x, y, z = 0;
	}

	fVector(float i) {
		x, y, z = i;
	}

	fVector(float x, float y, float z) : x(x), y(y), z(z) {
		
	}
};

struct Vertex {
	fVector position;
	FcolorRGBA color;
};