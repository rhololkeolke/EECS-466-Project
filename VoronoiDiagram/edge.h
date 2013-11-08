#ifndef VORONOIDIAGRAM_EDGE_H_
#define VORONOIDIAGRAM_EDGE_H_

#include "point.h"
#include "edge.h"
#include <memory>

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
			direction_.reset(new Point(right->y - left->y, -(right->x - left->y)));
		}

		PointPtr start_;
		PointPtr end_;
		PointPtr direction_;

		SitePtr left_;
		SitePtr right_;

		std::shared_ptr<Edge_> neighbor_;
	} Edge;

	typedef std::shared_ptr<Edge> EdgePtr;
}

#endif