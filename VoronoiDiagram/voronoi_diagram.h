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

		VoronoiDiagram(SitesPtr sites, float width, float height)
			: sites_(sites),
			width_(width),
			height_(height)
		{}

		VoronoiDiagram() :
			sites_(new Sites())
		{}

		virtual ~VoronoiDiagram()
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

		void setDimensions(float width, float height)
		{
			width_ = width;
			height_ = height;
		}

		float getWidth()
		{
			return width_;
		}

		float getHeight()
		{
			return height_;
		}

	protected:
		
		void addArc(EventPtr event);
		void removeArc(EventPtr event);
		void checkCircleEvent(BeachlineNodePtr arc, float sweep_line);

		BeachlineNodePtr getArcUnderSite(SitePtr site);
		Point getEdgePoint(BeachlineNodePtr edge, float sweep_line_pos);
		PointPtr getArcPoint(PointPtr focus, float directrix, float x);
		PointPtr getEdgeIntersection(EdgePtr a, EdgePtr b);
		void finishEdges(BeachlineNodePtr node);

		SitesPtr sites_; // the sites that this diagram is being generated for
		BeachlineNodePtr beachline_; // Holds the representation of the beachline used during calculation

		EventQueue event_queue_; // Holds the 2 types of events sorted by y coordinate
		std::set<EventPtr, Event::CompareEvent> deleted_events_; // holds events that should be deleted when popped instead of executed

		Points points_; // points created during the algorithm's execution
		EdgesPtr edges_; // edges created during the algorithm's execution

		float width_, height_; // dimensions of the diagram

	};

}

#endif