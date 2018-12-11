#pragma once
#include "GameObject.h"

class PhysicalGameObject : public GameObject
{
public:
	//Physics
	Vector2 velocity;
	Vector2 acceleration;

	float angularVelocity;
	float angularAcceleration;

	float mass;

	//Collision
	float radius;

	//Advanced
	float bounciness;

	//Gravity
	Vector2 gravityFactor;

	PhysicalGameObject();
	virtual ~PhysicalGameObject();

	virtual void Start();
	virtual void Update(float deltaTime);

	virtual void UpdateCollision(float deltaTime, PhysicalGameObject& other);

	virtual void OnCollide(PhysicalGameObject& other, Vector2 normal, float distance);
};