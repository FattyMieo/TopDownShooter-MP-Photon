#include "PhysicalDesturctible.h"

PhysicalDestructible::PhysicalDestructible() : PhysicalGameObject()
{
	timer = 0.0f;
	isToBeDestroyed = false;
}

PhysicalDestructible::~PhysicalDestructible() { }

void PhysicalDestructible::Update(float deltaTime)
{
	PhysicalGameObject::Update(deltaTime);

	if (timer < 1.0f)
	{
		timer += deltaTime;
	}
	else
	{
		isToBeDestroyed = true;
	}
}

void PhysicalDestructible::UpdateCollision(float deltaTime, PhysicalGameObject & other)
{
	PhysicalGameObject::UpdateCollision(deltaTime, other);
}

void PhysicalDestructible::OnCollide(PhysicalGameObject& other, Vector2 normal, float distance)
{
	PhysicalGameObject::OnCollide(other, normal, distance);

	Color c = this->GetSprite().GetColor();
	if (!c.Equal(other.GetSprite().GetColor()))
	{
		isToBeDestroyed = true;

		PhysicalDestructible* derived = dynamic_cast<PhysicalDestructible*>(&other);
		if (derived)
		{
			derived->isToBeDestroyed = true;
		}
		else //Enemy
		{
			other.GetTransform().scale = Vector2(2.0f);
		}
	}
}
