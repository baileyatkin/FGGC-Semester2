#pragma once

#include <DirectXMath.h>
#include <d3d11.h>
#include <vector>
#include <algorithm>
#include "Transform.h"
#include "Debug.h"
#include "Structures.h"
#include <string>

using namespace std;

class ParticleModel
{
public:
	ParticleModel(string type, Transform* transform, bool useConstAccel, Vector initialVelocity, Vector initialAcceleration, float mass);
	~ParticleModel();

	void Update(float deltaTime);

	void UpdateNetForce();

	Transform* GetTransform() { return _transform; }

	XMMATRIX GetWorldMatrix() const { return XMLoadFloat4x4(&_world); }

	void SetPosition(Vector position) { _transform->position = position; }
	void SetPosition(float x, float y, float z) { _transform->position.x = x; _transform->position.y = y; _transform->position.z = z; }
	Vector GetPosition() const { return _transform->position; }

	void SetScale(Vector scale) { _transform->scale = scale; }
	void SetScale(float x, float y, float z) { _transform->scale.x = x; _transform->scale.y = y; _transform->scale.z = z; }
	Vector GetScale() const { return _transform->scale; }

	void SetRotation(XMMATRIX rotation) { _rotation = rotation; }
	void SetRotation(float x, float y, float z) { _transform->rotation.x = x; _transform->rotation.y = y; _transform->rotation.z = z; }
	Vector GetRotation() const { return _transform->rotation; }

	void SetVelocity(Vector velocity) { _velocity = velocity; }
	void AddVelocity(Vector velocity);
	Vector GetVelocity() const { return _velocity; }

	void SetAcceleration(Vector acceleration) { _acceleration = acceleration; }
	void AddAcceleration(Vector acceleration);
	Vector GetAcceleration() const { return _acceleration; }

	void SetNetForce(Vector force) { _netForce = force; }
	void SetNetForce(float x, float y, float z) { _netForce.x = x; _netForce.y = y; _netForce.z = z; }
	Vector GetNetForce() const { return _netForce; }

	void SetObjectMass(float mass) { _objectMass = mass; }
	float GetObjectMass() const { return _objectMass; }

	void SetTransform(Transform* newTransform) { _transform = newTransform; }

	vector<Vector> GetForces() { return _forces; }
	void AddForce(Vector newForce) { _forces.push_back(newForce); }
	void AddForceX(float x) { _forces.push_back(Vector{ x, 0.0f, 0.0f }); }
	void AddForceY(float y) { _forces.push_back(Vector{ 0.0f, y, 0.0f }); }
	void AddForceZ(float z) { _forces.push_back(Vector{ 0.0f, 0.0f, z }); }
	void ClearForces();

	void SwapUsingConstAccel() { _useConstAccel = !_useConstAccel; }
	bool GetUsingConstAccel() { return _useConstAccel; }

	bool GetThrustEnabled() { return _thrustEnabled; }
	void SetThrustEnabled(bool isEnabled) { _thrustEnabled = isEnabled; }

	void AddVelOrAcc(Vector addFloat);

	void MotionInFluid();
	void DragForce(bool laminar);
	void DragLamFlow();
	void DragTurbFlow();

	void CreateAABB();

	void Move();
	void UpdateAcc();

	Vector GetDimensions() { return _dimensions; }

	float _objectMass;

	XMMATRIX _mRotation;

private:
	string _type;

	Transform* _transform;

	XMMATRIX _rotation;

	Vector _centreOfMass;

	Vector _velocity;

	Vector _acceleration;

	XMFLOAT4X4 _world;

	ParticleModel* _parent;

	float _deltaTime;

	Vector _netForce;
	vector<Vector> _forces;

	bool _useConstAccel;

	bool _useGravity;
	float _gravity;

	bool _thrustEnabled;
	float _thrust;

	bool _dragEnabled;
	bool _useLaminarDrag;
	float _dragFactor;
	Vector _drag;

	Vector _dimensions;

	bool _useCollision;

	vector<AABBFace> _AABBFaces;
};

