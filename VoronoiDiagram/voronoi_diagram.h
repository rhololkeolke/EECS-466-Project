#ifndef VORONOIDIAGRAM_VORONOIDIAGRAM_H_
#define VORONOIDIAGRAM_VORONOIDIAGRAM_H_

#include "beachline_node.h"
#include "event.h"
#include "edge.h"
#include "point.h"
#include <vector>
#include <queue>

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
		
		void addArc(SitePtr stie, EventQueue event_queue);
		void removeArc(BeachlineNodePtr arc, EventQueue event_queue);
		void checkCircleEvent(BeachlineNodePtr arc, EventQueue event_queue);

	private:
		SitesPtr sites_;

	};

}

#endif