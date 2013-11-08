#ifndef VORONOIDIAGRAM_BEACHLINE_NODE_H_
#define VORONOIDIAGRAM_BEACHLINE_NODE_H_

#include <memory>
#include "point.h"

namespace voronoi_diagram
{
	struct Event_;

	enum class BeachlineNodeType { ARC, EDGE };

	class BeachlineNode
	{
	public:
		std::shared_ptr<BeachlineNode> left_child_;
		std::shared_ptr<BeachlineNode> right_child_;
		std::weak_ptr<BeachlineNode> parent_;
		BeachlineNodeType type_;

		// when an arc
		SitePtr site_; // site that is this arc's focal point
		std::shared_ptr<struct Event_> circle_event_; // event that is poised to kill this arc

		// when an edge
		// TODO: add data needed for when this acts as an edge


	};
}

#endif