#ifndef VORONOIDIAGRAM_EDGE_H_
#define VORONOIDIAGRAM_EDGE_H_

#include "point.h"
#include "edge.h"
#include <memory>
#include <cmath>

namespace voronoi_diagram
{
	typedef struct Edge_
	{
	public:
		Edge_(PointPtr start, SitePtr left, SitePtr right) :
			start_(start),
			left_(left),
			right_(right)
		{
			// calculate the direction vector components
			// The direction is normal to the line from right to left site
			float x = right->y - left->y;
			float y = -(right->x - left->x);
			// calculate the direction vector's magnitude
			float magnitude = sqrt(x*x + y*y);

			// normalize the direction vector
			direction_.reset(new Point(x/magnitude, y/magnitude));



		}

		PointPtr start_;
		PointPtr end_;
		PointPtr direction_;

		SitePtr left_;
		SitePtr right_;

		//double f_, g_;

		std::shared_ptr<Edge_> neighbor_;
	} Edge;

	typedef std::shared_ptr<Edge> EdgePtr;
}

#endif
