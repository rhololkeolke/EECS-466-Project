#ifndef VORONOIDIAGRAM_POINT_H_
#define VORONOIDIAGRAM_POINT_H_

#include <memory>
#include <vector>

namespace voronoi_diagram
{
	typedef struct Point_
	{
		float x, y, z;

		Point_(float x, float y, float z=0) :
			x(x), y(y), z(z)
		{}

	} Point;

	typedef std::shared_ptr<Point> PointPtr;
	typedef Point Site;
	typedef PointPtr SitePtr;
	typedef std::vector<PointPtr> Points;
	typedef std::shared_ptr<Points> PointsPtr;
	typedef std::vector<SitePtr> Sites;
	typedef std::shared_ptr<Sites> SitesPtr;
	typedef std::shared_ptr<const Sites> SitesConstPtr;
}

#endif
