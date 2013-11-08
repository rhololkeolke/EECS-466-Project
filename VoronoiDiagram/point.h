#ifndef VORONOIDIAGRAM_POINT_H_
#define VORONOIDIAGRAM_POINT_H_

#include <memory>

namespace voronoi_diagram
{
	typedef struct Point_
	{
		float x, y;

		Point_(float x, float y) :
			x(x), y(y)
		{}
	} Point;

	typedef std::shared_ptr<Point> PointPtr;
}

#endif