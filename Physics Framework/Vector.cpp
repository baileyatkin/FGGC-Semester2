#include "Vector.h"

Vector::Vector()
{

}

float Vector::DotProduct(const Vector vec)
{
	return (x * vec.x) + (y * vec.y) + (z * vec.z);
}

Vector Vector::CrossProduct(const Vector vec)
{
	Vector product;
	product.x = y * vec.z - z * vec.y;
	product.y = z * vec.x - x * vec.z;
	product.z = x * vec.y - y * vec.x;
	return product;
}

float Vector::Magnitude()
{
	return (x * x) + (y * y) + (z * z);
}

Vector Vector::Normalization()
{
	Vector normal;
	float length = sqrt(Magnitude());
	normal.x = x / length;
	normal.y = y / length;
	normal.z = z / length;
	return normal;
}

Vector Vector::operator+(const Vector vec)
{
	return Vector(x + vec.x, y + vec.y, z + vec.z);
}

Vector Vector::operator+=(const Vector vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	return *this;
}

Vector Vector::operator-(const Vector vec)
{
	return Vector(x - vec.x, y - vec.y, z - vec.z);
}

Vector Vector::operator-=(const Vector vec)
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return *this;
}

Vector Vector::operator*(float num)
{
	return Vector(x * num, y * num, z * num);
}

Vector Vector::operator*=(float num)
{
	x *= num;
	y *= num;
	z *= num;
	return *this;
}

Vector Vector::operator/(float num)
{
	return Vector(x / num, y / num, z / num);
}

Vector Vector::operator/=(float num)
{
	x /= num;
	y /= num;
	z /= num;
	return *this;
}

Vector Vector::operator=(const Vector vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	return *this;
}