#ifndef VORONOIDIAGRAM_BEACHLINE_NODE_H_
#define VORONOIDIAGRAM_BEACHLINE_NODE_H_

#include <memory>
#include "edge.h"
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
		EdgePtr edge_;

		// methods
		void addArc(SitePtr site);
		void removeArc(std::shared_ptr<BeachlineNode> arc);
	};

	typedef std::shared_ptr<BeachlineNode> BeachlineNodePtr;
}

#endif