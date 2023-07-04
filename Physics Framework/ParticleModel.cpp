#include "ParticleModel.h"

ParticleModel::ParticleModel(string type, Transform* transform, bool useConstAccel, Vector initialVelocity, Vector initialAcceleration, float mass)
{
	_transform = transform;
	_parent = nullptr;
	_velocity = { 0.0f, 0.0f, 0.0f };
	_acceleration = { 0.0f, 0.0f, 0.0f };
	_objectMass = mass;
	_netForce = Vector(0.0f, 0.0f, 0.0f);
	_dragFactor = 0.5f;
	_drag = Vector{ 0.0f, 0.0f, 0.0f };
	_dragEnabled = true;
	_useGravity = true;
	_thrustEnabled = false;
	_thrust = (_objectMass * _gravity) * 1.5f;
	_gravity = 0.5f;
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
	_type = type;
}

ParticleModel::~ParticleModel()
{
	if (_transform == nullptr) delete(_transform);
}

void ParticleModel::Update(float deltaTime)
{
	_deltaTime = deltaTime;

	if (_type != "Floor")
	{
		if (_dragEnabled)
			MotionInFluid();


		UpdateNetForce();

		UpdateAcc();

		Move();
	}
}

void ParticleModel::Move()
{
	/*Vector oldPos = _transform->position;
	_transform->position.x = oldPos.x + _velocity.x * _deltaTime;
	_transform->position.y = oldPos.y + _velocity.y * _deltaTime;
	_transform->position.z = oldPos.z + _velocity.z * _deltaTime;*/

	_transform->position.x = _transform->position.x + _velocity.x * _deltaTime;
	_transform->position.y = _transform->position.y + _velocity.y * _deltaTime;
	_transform->position.z = _transform->position.z + _velocity.z * _deltaTime;

	if (_useGravity)
	{
		if (_transform->position.y < 0.5f)
		{
			_acceleration.y = 0.0f;
			_velocity.y = 0.0f;
			_netForce.y = 0.0f;
			_transform->position.y = 0.5f;
		}
	}

	_velocity.x = _velocity.x + _acceleration.x * _deltaTime;
	_velocity.y = _velocity.y + _acceleration.y * _deltaTime;
	_velocity.z = _velocity.z + _acceleration.z * _deltaTime;

}

void ParticleModel::AddVelocity(Vector velocity)
{
	_velocity.x += velocity.x;
	_velocity.y += velocity.y;
	_velocity.z += velocity.z;
}

void ParticleModel::AddAcceleration(Vector acceleration)
{
	//_acceleration.x += acceleration.x;
	float clampRangeAdd = 2;
	float clampRangeAcc = 15;
	_acceleration.x += clamp(acceleration.x, -clampRangeAdd, clampRangeAdd);
	_acceleration.y += clamp(acceleration.y, -clampRangeAdd * 10, clampRangeAdd);
	_acceleration.z += clamp(acceleration.z, -clampRangeAdd, clampRangeAdd);

	_acceleration.x = clamp(acceleration.x, -clampRangeAcc, clampRangeAcc);
	_acceleration.y = clamp(acceleration.y, -clampRangeAcc * 10, clampRangeAcc);
	_acceleration.z = clamp(acceleration.z, -clampRangeAcc, clampRangeAcc);
	
}

void ParticleModel::UpdateAcc()
{
	_acceleration.x = _netForce.x / _objectMass;
	_acceleration.y = _netForce.y / _objectMass;
	_acceleration.z = _netForce.z / _objectMass;
}

void ParticleModel::UpdateNetForce()
{
	for each (Vector force in _forces)
	{
		_netForce.x += force.x * _deltaTime;
		_netForce.y += force.y * _deltaTime;
		_netForce.z += force.z * _deltaTime;
		
	}


	if (_useGravity)
	{
		if(_velocity.y > 0)
			_netForce.y -= (_objectMass * _gravity);

		if (_thrustEnabled)
		{
			_thrust = clamp(_thrust, -10.0f, 10.0f);
			_netForce.y += -_thrust;
		}
	}

	if (_dragEnabled)
	{
		_velocity.x -= _drag.x * _deltaTime;
		_velocity.y -= _drag.y * _deltaTime;
		_velocity.z -= _drag.z * _deltaTime;

		_acceleration.x -= _drag.x * _deltaTime;
		_acceleration.y -= _drag.y * _deltaTime;
		_acceleration.z -= _drag.z * _deltaTime;
	}
}

void ParticleModel::AddVelOrAcc(Vector addFloat)
{
	if (_useConstAccel)
		AddAcceleration(addFloat);
	else
		AddVelocity(addFloat);
}

void ParticleModel::MotionInFluid()
{
	DragForce(_useLaminarDrag);
}

void ParticleModel::DragForce(bool laminar)
{
	if (laminar)
		DragLamFlow();
	else
		DragTurbFlow();
}

void ParticleModel::DragLamFlow()
{
	_drag.x = _dragFactor * _velocity.x;
	_drag.y = _dragFactor * _velocity.y;
	_drag.z = _dragFactor * _velocity.z;
}

void ParticleModel::DragTurbFlow()
{
	float _velMag = _velocity.Magnitude();
	Vector _unitVel = _velocity.Normalization();

	float _dragMag = _dragFactor * _velMag * _velMag;

	_drag.x = _dragMag * _unitVel.x;
	_drag.y = _dragMag * _unitVel.y;
}

void ParticleModel::ClearForces()
{
	_netForce.x = 0;
	_netForce.y = 0;
	_netForce.z = 0;

	_forces.clear();

	_acceleration.x = clamp(_acceleration.x, -15.0f, 15.0f);
	_acceleration.y = clamp(_acceleration.y, -15.0f, 15.0f);
	_acceleration.z = clamp(_acceleration.z, -15.0f, 15.0f);
}

void ParticleModel::CreateAABB()
{
	AABBFace face;

	// front face
	face.minX = _centreOfMass.x - ((_dimensions.x / 2) * (_transform->scale.x / 2));
	face.maxX = _centreOfMass.x + ((_dimensions.x / 2) * (_transform->scale.x / 2));

	face.minY = _centreOfMass.y - ((_dimensions.y / 2) * (_transform->scale.y / 2));
	face.maxY = _centreOfMass.y + ((_dimensions.y / 2) * (_transform->scale.y / 2));

	face.minZ = _centreOfMass.z - ((_dimensions.z / 2) * (_transform->scale.z / 2));
	face.maxZ = _centreOfMass.z - ((_dimensions.z / 2) * (_transform->scale.z / 2));

	_AABBFaces.push_back(face);

	// back face
	face.minX = _centreOfMass.x - ((_dimensions.x / 2) * (_transform->scale.x / 2));
	face.maxX = _centreOfMass.x + ((_dimensions.x / 2) * (_transform->scale.x / 2));

	face.minY = _centreOfMass.y - ((_dimensions.y / 2) * (_transform->scale.y / 2));
	face.maxY = _centreOfMass.y + ((_dimensions.y / 2) * (_transform->scale.y / 2));

	face.minZ = _centreOfMass.z + ((_dimensions.z / 2) * (_transform->scale.z / 2));
	face.maxZ = _centreOfMass.z + ((_dimensions.z / 2) * (_transform->scale.z / 2));

	_AABBFaces.push_back(face);

	// left face
	face.minX = _centreOfMass.x - ((_dimensions.x / 2) * (_transform->scale.x / 2));
	face.maxX = _centreOfMass.x - ((_dimensions.x / 2) * (_transform->scale.x / 2));

	face.minY = _centreOfMass.y - ((_dimensions.y / 2) * (_transform->scale.y / 2));
	face.maxY = _centreOfMass.y + ((_dimensions.y / 2) * (_transform->scale.y / 2));

	face.minZ = _centreOfMass.z - ((_dimensions.z / 2) * (_transform->scale.z / 2));
	face.maxZ = _centreOfMass.z + ((_dimensions.z / 2) * (_transform->scale.z / 2));

	_AABBFaces.push_back(face);

	// right face
	face.minX = _centreOfMass.x + ((_dimensions.x / 2) * (_transform->scale.x / 2));
	face.maxX = _centreOfMass.x + ((_dimensions.x / 2) * (_transform->scale.x / 2));

	face.minY = _centreOfMass.y - ((_dimensions.y / 2) * (_transform->scale.y / 2));
	face.maxY = _centreOfMass.y + ((_dimensions.y / 2) * (_transform->scale.y / 2));

	face.minZ = _centreOfMass.z - ((_dimensions.z / 2) * (_transform->scale.z / 2));
	face.maxZ = _centreOfMass.z + ((_dimensions.z / 2) * (_transform->scale.z / 2));

	_AABBFaces.push_back(face);

	// top face
	face.minX = _centreOfMass.x - ((_dimensions.x / 2) * (_transform->scale.x / 2));
	face.maxX = _centreOfMass.x + ((_dimensions.x / 2) * (_transform->scale.x / 2));

	face.minY = _centreOfMass.y + ((_dimensions.y / 2) * (_transform->scale.y / 2));
	face.maxY = _centreOfMass.y + ((_dimensions.y / 2) * (_transform->scale.y / 2));

	face.minZ = _centreOfMass.z - ((_dimensions.z / 2) * (_transform->scale.z / 2));
	face.maxZ = _centreOfMass.z + ((_dimensions.z / 2) * (_transform->scale.z / 2));

	_AABBFaces.push_back(face);

	// top face
	face.minX = _centreOfMass.x - ((_dimensions.x / 2) * (_transform->scale.x / 2));
	face.maxX = _centreOfMass.x + ((_dimensions.x / 2) * (_transform->scale.x / 2));

	face.minY = _centreOfMass.y - ((_dimensions.y / 2) * (_transform->scale.y / 2));
	face.maxY = _centreOfMass.y - ((_dimensions.y / 2) * (_transform->scale.y / 2));

	face.minZ = _centreOfMass.z - ((_dimensions.z / 2) * (_transform->scale.z / 2));
	face.maxZ = _centreOfMass.z + ((_dimensions.z / 2) * (_transform->scale.z / 2));

	_AABBFaces.push_back(face);
}