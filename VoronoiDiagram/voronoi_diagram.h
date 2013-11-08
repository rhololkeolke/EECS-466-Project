#ifndef VORONOIDIAGRAM_VORONOIDIAGRAM_H_
#define VORONOIDIAGRAM_VORONOIDIAGRAM_H_

#include "beachline_node.h"
#include "event.h"
#include "edge.h"
#include "point.h"
#include <vector>
#include <queue>
#include <set>

namespace voronoi_diagram
{

	typedef std::vector<EdgePtr> Edges;
	typedef std::shared_ptr<Edges> EdgesPtr;

	typedef std::priority_queue<EventPtr, std::vector<EventPtr>, Event::CompareEvent> EventQueue;

	class VoronoiDiagram
	{
	public:

		VoronoiDiagram(SitesPtr sites)
			: sites_(sites)
		{}

		VoronoiDiagram() :
			sites_(new Sites())
		{}

		void addPoint(SitePtr site)
		{
			sites_->push_back(site);
		}

		SitesConstPtr getSites()
		{
			SitesConstPtr const_sites(sites_);
			return const_sites;
		}

		EdgesPtr getEdges();

	protected:
		
		void addArc(SitePtr site);
		void removeArc(BeachlineNodePtr arc);
		void checkCircleEvent(BeachlineNodePtr arc);

		BeachlineNodePtr getArcUnderSite(SitePtr site);
		Point getEdgePoint(BeachlineNodePtr edge, float sweep_line_pos);

	private:
		SitesPtr sites_;
		BeachlineNodePtr beachline_;
		EventQueue event_queue_;
		std::set<EventPtr, Event::EqualEvent> deleted_events_;
	};

}

#endif