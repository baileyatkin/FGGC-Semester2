#pragma once

#include <DirectXMath.h>
#include "ParticleModel.h"
#include "Quaternion.h"
#include "Structures.h"
#include "Vector.h"
using namespace DirectX;

class Rigidbody : public ParticleModel
{
public:
	Rigidbody(string type, Transform* transform, bool useConstAccel, Vector initialVelocity, Vector initialAcceleration, float mass) : ParticleModel(type, transform, useConstAccel, initialVelocity, initialAcceleration, mass)
	{
		if (type == "Cube")
		{
			_centreOfMass = { 0, 0, 0 };
			_dimensions = { 2, 2, 2 };
		}
		if (type == "Floor")
		{
			_centreOfMass = { 0, 0, 0 };
			_dimensions = { 2, 2, 0 };
		}
		_inertiaTensor._11 = 0.08333333333 * mass * ((_dimensions.y * _dimensions.y) + (_dimensions.z * _dimensions.z));
		_inertiaTensor._12 = 0;
		_inertiaTensor._13 = 0;
		_inertiaTensor._21 = 0;
		_inertiaTensor._22 = 0.08333333333 * mass * ((_dimensions.x * _dimensions.x) + (_dimensions.z * _dimensions.z));
		_inertiaTensor._23 = 0;
		_inertiaTensor._31 = 0;
		_inertiaTensor._32 = 0;
		_inertiaTensor._33 = 0.08333333333 * mass * ((_dimensions.x * _dimensions.x) + (_dimensions.y * _dimensions.y));

		_angularAcceleration = { 0.0f, 0.0f, 0.0f, 0.0f };
		_angularVelocity = { 0.0f, 0.0f, 0.0f, 0 };
		_orientation = XMMatrixRotationRollPitchYaw(transform->rotation.x, transform->rotation.y, transform->rotation.z);
		_qOrientation = Quaternion();
		_rotating = false;
		_torquePoint = XMFLOAT3(0.0f, 0.0f, 0.0f);
		_torqueForce = XMFLOAT3(1.0f, 1.0f, 1.0f);
	}

	XMVECTOR Torque(XMVECTOR point, XMVECTOR force) { return XMVector3Cross(point, force); }

	//XMFLOAT3X3 CalculateInertiaTensor();
	void SetInertiaTensor(XMFLOAT3X3 newInertiaTensor) { _inertiaTensor = newInertiaTensor; }
	XMFLOAT3X3 GetInertiaTensor() { return _inertiaTensor; }

	XMVECTOR CalcAngularAcceleration();
	XMVECTOR CalcAngularAcceleration(XMVECTOR torque);
	XMVECTOR CalcAngularVelocity(float deltaTime);

	void DampAngularVelocity(float deltaTime);

	void Rotate(float deltaTime);
	XMMATRIX CalcOrientation(float deltaTime);
	Quaternion _qOrientation;
	XMMATRIX _orientation;

	bool GetRotating() { return _rotating; }
	void SetRotating(bool newRot) { _rotating = newRot; }

	float* GetAngularDamp() { return &_angularDamp; }
	void SetAngularDamp(float newAngDamp) { _angularDamp = newAngDamp; }

	XMFLOAT3 GetTorquePoint() { return _torquePoint; }
	void SetTorquePoint(XMFLOAT3 newTP) { _torquePoint = newTP; }
	XMFLOAT3 GetTorqueForce() { return _torqueForce; }
	void SetTorqueForce(XMFLOAT3 newTF) { _torqueForce = newTF; }

private:
	float _angularDamp = 1.0f;
	XMFLOAT3X3 _inertiaTensor;

	XMVECTOR _angularAcceleration;
	XMVECTOR _angularVelocity;

	Vector _centreOfMass;
	Vector _dimensions;

	bool _rotating;

	XMFLOAT3 _torquePoint;
	XMFLOAT3 _torqueForce;
};