#include "voronoi_diagram.h"

namespace voronoi_diagram
{
	// TODO: implement me
	EdgesPtr VoronoiDiagram::getEdges()
	{
		edges_.reset(new Edges());

		// reinitialize the event queue
		while(!event_queue_.empty())
			event_queue_.pop();
		// reinitialize the beachline
		beachline_.reset();

		// for each site
		for(Sites::iterator site = sites_->begin();
			site != sites_->end();
			site++)
		{

			// event <- create site event
			EventPtr site_event(new Event(*site));
			// push event into event queue
			event_queue_.push(site_event);
		}

		// while event queue is not empty
		while(!event_queue_.empty())
		{
			// curr_event <- pop from event queue
			EventPtr curr_event = event_queue_.top();
			event_queue_.pop();

			// verify that this event has not been marked for deletion
			// priority queues don't support random deletions so the algorithm
			// marks removed events for deletion
			if(deleted_events_.find(curr_event) != deleted_events_.end())
			{
				// remove from the set of deleted events
				deleted_events_.erase(curr_event);
				continue;
			}

			// if curr_event is site event
			if(curr_event->type_ == EventType::SITE)
			{
				// add arc to beachline with curr_event's site
				addArc(curr_event);
			}
			else
			{
				// remove Arc specified in curr_event from beachline
				removeArc(curr_event->arc_.lock());
			}
		}

		return edges_;
	}

	// TODO: implement me!
	void VoronoiDiagram::addArc(EventPtr event)
	{
		// if this is the first arc
		if(!beachline_)
		{
			// initialize the beachline with an arc focused at this site
			beachline_.reset(new BeachlineNode(event->site_));
			return;
		}

		// split_arc <- get the arc under the site
		BeachlineNodePtr split_arc = getArcUnderSite(event->site_);
		// if split_arc has a circle event
		if(split_arc->circle_event_)
		{
			// mark the circle event for deletion
			deleted_events_.insert(split_arc->circle_event_);
		}

		// start a new edge at this point
		// TODO: Fix the y-coordinate
		PointPtr start_point(getArcPoint(split_arc->site_, event->site_->y, event->site_->x));
		points_.push_back(start_point);

		// A new edge will grow from this parabola
		// for programmatica reasons this is modeled as 2 edges
		// one grows to the left out of the intersection point between the vertical ray and the split parabola
		// one grows to the right out of the intersection point between the verticle ray and the split parabola
		EdgePtr left_edge(new Edge(start_point, split_arc->site_, event->site_));
		EdgePtr right_edge(new Edge(start_point, event->site_, split_arc->site_));

		// the left edge points to the right edeg
		// this allows for the different edge segments to be joined after the algorithm is complete
		left_edge->neighbor_ = right_edge;

		// add the left edge to the list of edges
		edges_->push_back(left_edge);

		// a <- new arc with split_arc's site
		BeachlineNodePtr arc0(new BeachlineNode(split_arc->site_));
		// b <- new arc with given site
		BeachlineNodePtr arc1(new BeachlineNode(event->site_));
		// c <- new arc with split_arc's site
		BeachlineNodePtr arc2(new BeachlineNode(split_arc->site_));
		
		// replace split_arc with a, xl, b, xr, c
		// ----------------------------------------
		// convert the split_arc to an edge node
		BeachlineNodePtr left_edge_node = split_arc;
		left_edge_node->type_ = BeachlineNodeType::EDGE;
		// set the left child of the new edge node to arc0
		left_edge_node->setLeftChild(arc0);
		// set the right child of the new edge node to a new edge node
		BeachlineNodePtr right_edge_node(new BeachlineNode(right_edge));
		right_edge_node->setLeftChild(arc1);
		right_edge_node->setRightChild(arc2);
		// set this node as the right child of the left edge
		left_edge_node->setRightChild(right_edge_node);


		// check for circle event with a in the middle
		checkCircleEvent(arc0);
		// check for circle event with c in the middle
		checkCircleEvent(arc2);
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

	// get the point on a parabola given a focus, directrix and x coordinate
	PointPtr VoronoiDiagram::getArcPoint(PointPtr focus, float directrix, float x)
	{
		double denom = 2 * (focus->y - directrix);
		double a = 1.0f / denom;
		double b = -2.0f * focus->x / denom;
		double c = (focus->x*focus->x + focus->y*focus->y + directrix*directrix)/ denom;

		PointPtr arc_point(new Point(x, a*x*x + b*x + c));
		return arc_point;
	}
}