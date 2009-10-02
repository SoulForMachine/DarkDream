
#include "Bounds.h"

using namespace math3d;


math3d::vec3f AABBox::GetCenterPoint() const
{
	return (minPt + maxPt) * 0.5f;
}

void AABBox::GetOrientedBBox(OBBox& bbox, const math3d::mat4f& mat) const
{
	transform(bbox.points[0], vec3f(minPt.x, minPt.y, minPt.z), mat);
	transform(bbox.points[1], vec3f(maxPt.x, minPt.y, minPt.z), mat);
	transform(bbox.points[2], vec3f(minPt.x, maxPt.y, minPt.z), mat);
	transform(bbox.points[3], vec3f(maxPt.x, maxPt.y, minPt.z), mat);
	transform(bbox.points[4], vec3f(minPt.x, minPt.y, maxPt.z), mat);
	transform(bbox.points[5], vec3f(maxPt.x, minPt.y, maxPt.z), mat);
	transform(bbox.points[6], vec3f(minPt.x, maxPt.y, maxPt.z), mat);
	transform(bbox.points[7], vec3f(maxPt.x, maxPt.y, maxPt.z), mat);
}

void AABBox::GetPoints(math3d::vec3f points[8]) const
{
	points[0].set(minPt.x, minPt.y, minPt.z);
	points[1].set(maxPt.x, minPt.y, minPt.z);
	points[2].set(minPt.x, maxPt.y, minPt.z);
	points[3].set(maxPt.x, maxPt.y, minPt.z);
	points[4].set(minPt.x, minPt.y, maxPt.z);
	points[5].set(maxPt.x, minPt.y, maxPt.z);
	points[6].set(minPt.x, maxPt.y, maxPt.z);
	points[7].set(maxPt.x, maxPt.y, maxPt.z);
}



math3d::vec3f OBBox::GetCenterPoint() const
{
	vec3f center(vec3f::null);
	for(int i = 0; i < 8; ++i)
		center += points[i];

	return center / 8.0f;
}
