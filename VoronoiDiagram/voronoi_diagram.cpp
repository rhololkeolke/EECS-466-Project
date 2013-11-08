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
	void VoronoiDiagram::addArc(SitePtr site)
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
	void VoronoiDiagram::removeArc(BeachlineNodePtr arc)
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
	void VoronoiDiagram::checkCircleEvent(BeachlineNodePtr arc)
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

	// TODO: Implement me
	BeachlineNodePtr VoronoiDiagram::getArcUnderSite(SitePtr site)
	{
		// initialize the node to the root of the beachline
		std::shared_ptr<BeachlineNode> curr_edge(beachline_);
		
		// while we haven't hit an arc node
		while(curr_edge->type_ != BeachlineNodeType::ARC)
		{
			// get the edge's position
			Point edge_point = getEdgePoint(curr_edge, site->y);

			// if the x position of the current edge is left of the site
			if(edge_point.x  < site->x)
			{
				// make the current edge the parabola to the right
				curr_edge = curr_edge->getRightChild();
			}
			else
			{
				// make the current edge the parabola to the left
				curr_edge = curr_edge->getLeftChild();
			}

		}

		// when the while loop finishes the curr_edge is actually an arc (because its a leaf)
		return curr_edge;
	}

	// TODO: Implement me
	Point VoronoiDiagram::getEdgePoint(BeachlineNodePtr edge, float sweep_line_pos)
	{
		// solve for the intersection of the two parabola's defining this edge
		BeachlineNodePtr left_arc = BeachlineNode::getLeftArc(edge);
		BeachlineNodePtr right_arc = BeachlineNode::getRightArc(edge);

		// just a convenince variable so I don't have to type this over and over
		float denom1 = 2.0f*(left_arc->site_->y - sweep_line_pos);
		// calculate the coefficients of the left arc's equation
		float a1 = 1/denom1;
		float b1 = -2.0f*left_arc->site_->x / denom1;
		float c1 = (left_arc->site_->x*left_arc->site_->x + left_arc->site_->y*left_arc->site_->y + sweep_line_pos*sweep_line_pos) / denom1;

		// now do the same thing for the right parabola
		float denom2 = 2.0f*(right_arc->site_->y - sweep_line_pos);
		float a2 = 1/denom2;
		float b2 = -2.0f*right_arc->site_->x / denom2;
		float c2 = (right_arc->site_->x*right_arc->site_->x + right_arc->site_->y*right_arc->site_->y + sweep_line_pos*sweep_line_pos) / denom2;

		// now get the coefficients for the quadratic equation that we'll be solving
		float a = a1 - a2;
		float b = b1 - b2;
		float c = c1 - c2;

		float discriminant = b*b - 4.0f*a*c;
		float x1 = (-b + std::sqrt(discriminant))/(2.0f*a);
		float x2 = (-b - std::sqrt(discriminant))/(2.0f*a);

		Point edge_point(0.0f, 0.0f);
		if(left_arc->site_->y < right_arc->site_->y)
			edge_point.x = std::max(x1, x2);
		else
			edge_point.x = std::min(x1, x2);

		// finally plug the x back into one of the equations and get the y point
		edge_point.y = a1 * edge_point.x*edge_point.x + b1 * edge_point.x + c1;

		return edge_point;
	}
}