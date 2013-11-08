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

	// TODO: implement me!
	void VoronoiDiagram::addArc(SitePtr site, EventQueue event_queue)
	{
		// split_arc <- get the arc under the site
		// if split_arc has a circle event
			// mark the circle event for deletion
		// a <- new arc with split_arc's site
		// b <- new arc with given site
		// c <- new arc with split_arc's site
		// xl <- new dangling edge starting from point under site
		// xr <- new dangling edge starting from point under site
		// xl.normal <- normal to (a.site, b.site)
		// xr.normal <- normal to (b.site, c.site)
		// replace split_arc with a, xl, b, xr, c
		// check for circle event with a in the middle
		// check for circle event with c in the middle
		// NOTE: don't check for circle event with b in the middle because a and c have the same site (need 3 sites to make a circle event)
	}

	// TODO: implement me!
	void VoronoiDiagram::removeArc(BeachlineNodePtr arc, EventQueue event_queue)
	{
		// left_arc <- arc left of given arc
		// right_arc <- arc left of given arc
		// if left_arc has a circle event
			// remove left_arc's circle event
		// if right_arc has a circle event
			// remove right_arc's circle event
		// s <- circumcenter between left_arc's site, given arc's site and right_arc's site
		// x <- new dangling edge at s
		// x.normal <- normal to (left_arc's site, right_arc's site)
		// xl <- left neighbor edge
		// finish xl at s
		// xr <- right neighbor edge
		// finish xr at s
		// replace xl, given arc, xr by x
		// checkCircleEvent(left_arc)
		// checkCircleEvent(right_arc)
	}

	// TODO: implement me
	void VoronoiDiagram::checkCircleEvent(BeachlineNodePtr arc, EventQueue event_queue)
	{
		// left_arc <- arc to the left of p
		// right_arc <- arc to the right of p
		// xl <- left edge of p
		// xr <- right edge of p
		// if left_arc or right_arc does not exist
			// return ( You need 3 points to have a circle event)
		// if right_arc's site == left arc's site
			// return (You need 3 points to have a circle event)
		// s <- location where xl and xr cross each other
		// if s does not exist
			// return
		// r <- distance between s and p.site (circumcircle radius)
		// if s.y + r is under the sweepline
			// return
		// circle_event <- create new circle event with arc = p and y = s.y + r
		// event_queue.push(circle_event)
	}
}