#include "PhysicalGameObject.h"
#include "PhysicsExtension.h"
#include "ResDefinition.h"
#include <iostream>

PhysicalGameObject::PhysicalGameObject() : GameObject()
{
	velocity = Vector2(0.0f);
	acceleration = Vector2(0.0f);

	mass = 0.1f;

	radius = 10.0f;

	bounciness = 0.5f;

	gravityFactor = Vector2(0.0f);
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
	force += mass * gravityFactor * PhysicsExtension::gravity;

	// Air resistance force
	force.x += (velocity.x > 0 ? -1.0f : 1.0f) * 0.5f * PhysicsExtension::airDensity * PhysicsExtension::dragCoeff_Sphere * PhysicsExtension::GetSphericalFrontalArea(radius) * velocity.x * velocity.x;
	force.y += (velocity.y > 0 ? -1.0f : 1.0f) * 0.5f * PhysicsExtension::airDensity * PhysicsExtension::dragCoeff_Sphere * PhysicsExtension::GetSphericalFrontalArea(radius) * velocity.y * velocity.y;

	//Velocity Verlet Method
	Vector2 lastAccel = acceleration;
	m_transform.position += (velocity * deltaTime + (0.5f * lastAccel * deltaTime * deltaTime)) * 100.0f;
	Vector2 newAccel = force / mass;
	acceleration = (lastAccel + newAccel) / 2.0f;
	velocity += acceleration * deltaTime;

	//Collision Check
	Vector2 bounceFactor = Vector2(1.0f);

	//Border Checks
	if (m_transform.position.x + radius >= RESOLUTION_X && velocity.x > 0.0f)
	{
		bounceFactor.x = -bounciness;
		/* Move the ball back a little bit so it's not still "stuck" in the wall. */
		m_transform.position.x = RESOLUTION_X - radius;
	}
	else if (m_transform.position.x - radius < 0.0f && velocity.x < 0.0f)
	{
		bounceFactor.x = -bounciness;
		/* Move the ball back a little bit so it's not still "stuck" in the wall. */
		m_transform.position.x = radius;
	}

	if (m_transform.position.y + radius >= RESOLUTION_Y && velocity.y > 0.0f)
	{
		bounceFactor.y = -bounciness;
		/* Move the ball back a little bit so it's not still "stuck" in the wall. */
		m_transform.position.y = RESOLUTION_Y - radius;
	}
	else if (m_transform.position.y - radius < 0.0f && velocity.y < 0.0f)
	{
		bounceFactor.y = -bounciness;
		/* Move the ball back a little bit so it's not still "stuck" in the wall. */
		m_transform.position.y = radius;
	}

	/* This is a simplification of impulse-momentum collision response. it should be a negative number, which will change the velocity's direction. */
	velocity *= bounceFactor;

	//std::cout << velocity.x << " " << velocity.y << std::endl;
	//std::cout << acceleration.x << " " << acceleration.y << std::endl;
	//system("PAUSE");

	//acceleration = constForce / mass;
	//velocity += acceleration * deltaTime;
	//m_transform.position += velocity * deltaTime;

	//angularVelocity += angularAcceleration * deltaTime;
	//m_transform.rotation += angularVelocity * deltaTime;
}

void PhysicalGameObject::UpdateCollision(float deltaTime, PhysicalGameObject& other)
{
	//Collision Check against others
	float totalRadius = this->radius + other.radius;
	float totalRadiusSquared = totalRadius * totalRadius;

	float distanceSquared = this->GetTransform().position.GetSquaredDistanceTo(other.GetTransform().position);

	//If both objects collided
	if (distanceSquared <= totalRadiusSquared)
	{
		Vector2 dir = other.GetTransform().position - this->GetTransform().position;
		dir *= sqrt(dir.GetSquaredMagnitude());
		float distance = sqrt(distanceSquared);
		float penetration = totalRadius - distance;

		this->GetTransform().position += dir * penetration * -1.0f * deltaTime;
		this->velocity *= bounciness;
		this->velocity += dir * -1.0f * deltaTime;
		//float penetration = (this->radius / 100.0f) - distance;
		//this->GetTransform().position += dir * penetration * 1.0f;
		//this->velocity += dir * penetration * 1.0f;
		//this->velocity *= bounciness;

		other.GetTransform().position += dir * penetration * 1.0f * deltaTime;
		other.velocity *= bounciness;
		other.velocity += dir * 1.0f * deltaTime;
		//float otherPenetration = (other.radius / 100.0f) - distance;
		//other.GetTransform().position += dir * otherPenetration * -1.0f;
		//other.velocity += dir * otherPenetration * -1.0f;
		//other.velocity *= bounciness;

		OnCollide(other, dir, distance);
	}
}

void PhysicalGameObject::OnCollide(PhysicalGameObject& other, Vector2 normal, float distance)
{

}
