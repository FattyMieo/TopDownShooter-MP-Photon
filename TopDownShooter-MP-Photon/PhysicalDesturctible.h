#pragma once
#include "PhysicalGameObject.h"

class PhysicalDestructible : public PhysicalGameObject
{
private:
	float timer;
public:
	bool isToBeDestroyed;

	PhysicalDestructible();
	virtual ~PhysicalDestructible();

	virtual void Update(float deltaTime);
	virtual void UpdateCollision(float deltaTime, PhysicalGameObject& other);
	virtual void OnCollide(PhysicalGameObject& other, Vector2 normal, float distance);
};
