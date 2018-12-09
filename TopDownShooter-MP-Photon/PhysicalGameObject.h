#pragma once
#include "GameObject.h"

class PhysicalGameObject : public GameObject
{
private:
	const float gravity = 9.81f;
	const float airDensity = 1.225f;  // Density of air. Try 997 for water.
	const float dragCoeff = 0.47f; // Drag coefficient for a ball
	const float frontalArea = MathExtension::PI * radius * radius / 10000.0f; // Frontal area of the ball; divided by 10000 to compensate for the 1px = 1cm relation

protected:
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


public:
	Vector2 gravityFactor;

	PhysicalGameObject();
	PhysicalGameObject(Sprite sprite);
	virtual ~PhysicalGameObject();

	virtual void Start();
	virtual void Update(float deltaTime);
};