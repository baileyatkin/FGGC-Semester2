#pragma once

#include <iostream>
#include <math.h>
#include <assert.h>

using namespace std;


class Vector
{
public:
	float x, y, z;
	Vector();

	Vector(float x2, float y2, float z2)
	{
		x = x2;
		y = y2;
		z = z2;
	}

	float DotProduct(const Vector vec);
	Vector CrossProduct(const Vector vec);
	float Magnitude();
	Vector Normalization();

	Vector operator+(const Vector vec);
	Vector operator+=(const Vector vec);
	Vector operator-(const Vector vec);
	Vector operator-=(const Vector vec);
	Vector operator*(float num);
	Vector operator*=(float num);
	Vector operator/(float num);
	Vector operator/=(float num);
	Vector operator=(const Vector vec);
};

