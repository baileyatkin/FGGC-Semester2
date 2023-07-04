#include "Transform.h"

Transform::Transform()
{
	position = Vector(0.0f, 0.0f, 0.0f);
	rotation = Vector(0.0f, 0.0f, 0.0f);
	scale = Vector(1.0f, 1.0f, 1.0f);
}

Transform::Transform(Vector _position)
{
	position = _position;
	rotation = Vector(0.0f, 0.0f, 0.0f);
	scale = Vector(1.0f, 1.0f, 1.0f);
}

Transform::Transform(Vector _position, Vector _rotation)
{
	position = _position;
	rotation = _rotation;
	scale = Vector(1.0f, 1.0f, 1.0f);
}

Transform::Transform(Vector _position, Vector _rotation, Vector _scale)
{
	position = _position;
	rotation = _rotation;
	scale = _scale;
}