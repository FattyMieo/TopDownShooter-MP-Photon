#pragma once

#include "MathExtension.h"

namespace PhysicsExtension
{
	static const float gravity = 9.81f;
	static const float airDensity = 1.225f;  // Density of air.
	static const float waterDensity = 997.0f;  // Density of water.
	static const float dragCoeff_Sphere = 0.47f; // Drag coefficient for a sphere

	static float GetSphericalFrontalArea(float radius)
	{
		return MathExtension::PI * radius * radius / 10000.0f; // Divided by 10000 to compensate for the 1px = 1cm relation;
	}
}