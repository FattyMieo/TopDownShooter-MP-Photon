#include "PhysicalGameObject.h"

PhysicalGameObject::PhysicalGameObject() : GameObject()
{
	velocity = Vector2(0.0f);
	acceleration = Vector2(0.0f);

	mass = 0.1f;

	radius = 10.0f;

	bounciness = -0.5f;

	gravityFactor = Vector2(0.0f);
}

PhysicalGameObject::PhysicalGameObject(Sprite sprite) : GameObject(sprite)
{
	PhysicalGameObject();
}

PhysicalGameObject::~PhysicalGameObject()
{

}

void PhysicalGameObject::Start()
{
	GameObject::Start();
}

void PhysicalGameObject::Update(float deltaTime)
{
	GameObject::Update(deltaTime);

	Vector2 force;

	//Gravity weight force
	force += mass * gravityFactor * gravity;

	// Air resistance force
	force += -0.5f * airDensity * dragCoeff * frontalArea * velocity;

	//Velocity Verlet Method
	Vector2 lastAccel = acceleration;
	m_transform.position += (velocity * deltaTime + (0.5f * lastAccel * deltaTime * deltaTime)) * 100.0f;
	Vector2 newAccel = force / mass;
	acceleration = (lastAccel + newAccel) / 2.0f;
	velocity += acceleration * deltaTime;

	//acceleration = constForce / mass;
	//velocity += acceleration * deltaTime;
	//m_transform.position += velocity * deltaTime;

	angularVelocity += angularAcceleration * deltaTime;
	m_transform.rotation += angularVelocity * deltaTime;
}
