#ifndef VORONOIDIAGRAM_POINT_H_
#define VORONOIDIAGRAM_POINT_H_

namespace voronoi_diagram
{
	typedef struct Point_
	{
		float x, y;

		Point(float x, float y) :
			x(x), y(y)
		{}
	} Point;
}

#endif