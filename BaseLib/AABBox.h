
#ifndef _BASELIB_AABBOX_H_
#define _BASELIB_AABBOX_H_

#include "BaseLib/Common.h"
#include "BaseLib/Math/vec3.h"


class BASELIB_API AABBox
{
public:
	AABBox() {}
	AABBox(const math3d::vec3f& min_pt, const math3d::vec3f& max_pt);

	math3d::vec3f minPt;
	math3d::vec3f maxPt;
};


#endif // _BASELIB_AABBOX_H_
