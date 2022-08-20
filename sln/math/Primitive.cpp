#include "Primitive.h"
#include "Vector3.h"

//球----------------------
Sphere::Sphere(const Vector3& position, float radius, unsigned color)
	:position(position), radius(radius), color(color)
{

}

void Sphere::draw()
{
/*	const unsigned white = GetColor(255, 255, 255);

	DrawSphere3D(position, radius, 32, color, white,true)*/;

}

void Sphere::drawInfo(int x, int y, unsigned color)
{
	/*DrawFormatString(x, y, color, "(%6.1f,%6.1f,%6.1f),%6.1f", position.x, position.y, position.z, radius);*/
}

//直方体-------------------------
Box::Box(const Vector3& minPosition, const Vector3& maxPosition, unsigned color)
	:minPosition(minPosition), maxPosition(maxPosition), color(color)
{

}

void Box::draw()
{
	//const unsigned white = GetColor(255, 255, 255);

	//DrawCube3D(minPosition, maxPosition, color, white, true);;
}

void Box::drawInfo(int x, int y, unsigned color)
{
	//DrawFormatString(x, y, color, "(%6.1f,%6.1f,%6.1f)-(%6.1f,%6.1f,%6.1f)",
	//	minPosition.x, minPosition.y, minPosition.z, maxPosition.x, maxPosition.y, maxPosition.z);
}

//カプセル
Capsule::Capsule(const Vector3& startPosition, const Vector3& endPosition, float radius, unsigned color)
	:startPosition(startPosition), endPosition(endPosition), color(color), radius(radius)
{

}

void Capsule::draw()
{
	//const unsigned white = GetColor(255, 255, 255);
	//
	//DrawCapsule3D(startPosition, endPosition, radius, 32, color, white, true);
}

void Capsule::drawInfo(int x, int y, unsigned color)
{
	//DrawFormatString(x, y, color, "Capsule(%6.1f,%6.1f,%6.1f)-(%6.1f,%6.1f,%6.1f),%6.1f",
	//	startPosition.x, startPosition.y, startPosition.z,
	//	endPosition.x, endPosition.y, endPosition.z, radius);
}