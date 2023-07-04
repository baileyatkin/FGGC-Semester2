#pragma once

#include <directxmath.h>
#include <d3d11_1.h>
#include <string>
#include "Vector.h"
#include "Transform.h"
#include "ParticleModel.h"
#include "Appearance.h"
#include "RigidBody.h"

using namespace DirectX;
using namespace std;

class GameObject
{
public:
	GameObject(string type, Appearance* appearance, Transform* transform, float mass);
	~GameObject();

	ParticleModel* GetParticleModel() { return _particleModel; }

	string GetType() const { return _type; }

	XMMATRIX GetWorldMatrix() const { return XMLoadFloat4x4(&_world); }

	Appearance* GetAppearance() { return _appearance; }

	Transform* GetTransform() { return _transform; }

	void SetParent(GameObject * parent) { _parent = parent; }

	void Update(float t);
	void Draw(ID3D11DeviceContext * pImmediateContext);

	Rigidbody* GetRigidBody() { return _rigidBody; }

	XMMATRIX rotation;

private:
	string _type;

	XMFLOAT4X4 _world;

	float _mass;

	GameObject* _parent;

	Transform* _transform;

	ParticleModel* _particleModel;

	Appearance* _appearance;

	Rigidbody* _rigidBody;

	float deltaTime;
};

