#pragma once
#include "Primitive.h"
#include "Vector3.h"

//ÕË»è
class Collision
{
public:

	//Æ
	static bool testSphereSphere(const Sphere& sphere1, const Sphere& sphere2);

	//Æ¼ûÌ(AABB§Àt«)
	static bool testSphereBox(const Sphere& sphere, const Box& box);

	//ÆJvZ
	static bool testSphereCapsule(const Sphere& sphere, const Capsule& capsule);
};