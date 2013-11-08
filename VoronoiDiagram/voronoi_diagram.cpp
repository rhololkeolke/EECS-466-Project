#include "voronoi_diagram.h"

namespace voronoi_diagram
{
	// TODO: implement me
	EdgesPtr VoronoiDiagram::getEdges()
	{
		EdgesPtr edges(new Edges());
		BeachlineNodePtr beachline;

		// for each site
			// event <- create site event
			// push event into event queue

		// while event queue is not empty
			// curr_event <- pop from event queue
			// if curr_event is site event
				// add arc to beachline with curr_event's site
			// else
				// remove Arc specified in curr_event from beachline

		return edges;
	}

	// TODO: implement me
	void VoronoiDiagram::checkCircleEvent(BeachlineNodePtr arc, EventQueue event_queue)
	{

	}
}