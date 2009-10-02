
#ifndef _BASELIB_BOUNDS_H_
#define _BASELIB_BOUNDS_H_

#include "BaseLib/Common.h"
#include "BaseLib/Math/math3d.h"

class OBBox;


/*
	Axis-aligned bounding box
*/
class BASELIB_API AABBox
{
public:
	AABBox() {}
	AABBox(const math3d::vec3f& min_pt, const math3d::vec3f& max_pt);

	math3d::vec3f GetCenterPoint() const;
	void GetOrientedBBox(OBBox& bbox, const math3d::mat4f& mat) const;
	void GetPoints(math3d::vec3f points[8]) const;

	math3d::vec3f minPt;
	math3d::vec3f maxPt;
};


/*
	Object oriented bounding box

	indices of box points:
	lower 4 points:
		0	1

		4	5

	higher 4 points:
		2	3

		6	7

*/
class BASELIB_API OBBox
{
public:
	OBBox() {}

	math3d::vec3f GetCenterPoint() const;

	math3d::vec3f points[8];
};


#endif // _BASELIB_BOUNDS_H_
