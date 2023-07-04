#include "GameObject.h"

GameObject::GameObject(string type, Appearance* appearance, Transform* transform, float mass) :  _type(type)
{
	_parent = nullptr;
	_mass = mass;
	_transform = transform;
	_appearance = appearance;
	_rigidBody = new Rigidbody(_type, _transform, true, Vector{ 0.0f, 0.0f, 0.0f }, Vector{ 0.0f, 0.0f, 0.0f }, _mass);
	rotation = XMMatrixRotationRollPitchYaw(transform->rotation.x, transform->rotation.y, transform->rotation.z);
	_rigidBody->_orientation = rotation;
	
}

GameObject::~GameObject()
{
	if (_transform)
	{
		delete _transform;
		_transform = nullptr;
	}
}

void GameObject::Update(float t)
{
	XMMATRIX scale = XMMatrixScaling(_transform->scale.x, _transform->scale.y, _transform->scale.z);

	if (!(_type.find("Floor") != string::npos))
	{
		if (_rigidBody->GetRotating())
		{
			_rigidBody->Rotate(deltaTime);
		}
		rotation = _rigidBody->CalcOrientation(deltaTime);

	}
	else
	{
		rotation = XMMatrixRotationRollPitchYaw(_transform->rotation.x, _transform->rotation.y, _transform->rotation.z);
		XMFLOAT4 fRotation;
		XMStoreFloat4(&fRotation, XMQuaternionRotationMatrix(rotation));
		Quaternion quatRotation = Quaternion(fRotation.w, fRotation.x, fRotation.y, fRotation.z);
		quatRotation.normalise();
		CalculateTransformMatrixRowMajor(rotation, _transform->position, quatRotation);
	}
	XMMATRIX translation = XMMatrixTranslation(_transform->position.x, _transform->position.y, _transform->position.z);

	XMStoreFloat4x4(&_world, scale * rotation * translation);

	_rigidBody->Update(t);
}

void GameObject::Draw(ID3D11DeviceContext * pImmediateContext)
{
	// NOTE: We are assuming that the constant buffers and all other draw setup has already taken place

	// Set vertex and index buffers
	pImmediateContext->IASetVertexBuffers(0, 1, &_appearance->_geometry.vertexBuffer, &_appearance->_geometry.vertexBufferStride, &_appearance->_geometry.vertexBufferOffset);
	pImmediateContext->IASetIndexBuffer(_appearance->_geometry.indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	pImmediateContext->DrawIndexed(_appearance->_geometry.numberOfIndices, 0, 0);
}
