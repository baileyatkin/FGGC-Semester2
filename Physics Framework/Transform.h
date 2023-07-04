#pragma once

#include <DirectXMath.h>
#include "Vector.h"
using namespace DirectX;

class Transform
{
public:
	Transform();
	Transform(Vector _position);
	Transform(Vector _position, Vector _rotation);
	Transform(Vector _position, Vector _rotation, Vector _scale);
	
	Vector position;
	Vector rotation;
	Vector scale;
};

