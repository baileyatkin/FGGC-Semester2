#include "Rigidbody.h"

XMVECTOR Rigidbody::CalcAngularAcceleration()
{
	XMMATRIX matTensor = XMLoadFloat3x3(&_inertiaTensor);
	XMMATRIX invTensor = XMMatrixInverse(nullptr, matTensor);
	XMVECTOR torque = Torque(XMLoadFloat3(&_torquePoint), XMLoadFloat3(&_torqueForce));
	_angularAcceleration = XMVector3Transform(torque, invTensor);
	return _angularAcceleration;
}
XMVECTOR Rigidbody::CalcAngularAcceleration(XMVECTOR torque)
{
	XMMATRIX matTensor = XMLoadFloat3x3(&_inertiaTensor);
	XMMATRIX invTensor = XMMatrixInverse(nullptr, matTensor);
	_angularAcceleration = XMVector3Transform(torque, invTensor);
	return _angularAcceleration;
}

XMVECTOR Rigidbody::CalcAngularVelocity(float deltaTime)
{
	_angularVelocity = _angularVelocity + _angularAcceleration * deltaTime;
	return _angularVelocity;
}

void Rigidbody::DampAngularVelocity(float deltaTime)
{
	_angularVelocity *= pow(_angularDamp, deltaTime);
}

XMMATRIX Rigidbody::CalcOrientation(float deltaTime)
{
	XMFLOAT3 angVelV3;
	XMStoreFloat3(&angVelV3, _angularVelocity);
	Vector angVelV3_2 = { angVelV3.x, angVelV3.y, angVelV3.z };
	_qOrientation.addScaledVector(angVelV3_2, deltaTime);
	_qOrientation.normalise();
	CalculateTransformMatrixRowMajor(_orientation, GetPosition(), _qOrientation);
	return _orientation;
}

void Rigidbody::Rotate(float deltaTime)
{
	CalcAngularAcceleration();
	CalcAngularVelocity(deltaTime);
	DampAngularVelocity(deltaTime);
}

