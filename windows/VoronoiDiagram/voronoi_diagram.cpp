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
				removeArc(curr_event);
			}
		}

		// walk through the beachline tree and finish any unfinished edges at the edges of the diagram
		finishEdges(beachline_, width_, height_);
		beachline_.reset(); // free up the memory used in the beachline

		// go through the edges and join edges with their neighbors. Remove joined edges
		for(Edges::iterator edge = edges_->begin(); edge != edges_->end(); edge++)
		{
			// if this edge has been deleted
			if(!(*edge))
				continue;

			// if this edge has a neighbor
			if((*edge)->neighbor_)
			{
				(*edge)->start_ = (*edge)->neighbor_->end_;
				(*edge)->neighbor_.reset();
			}
		}

		// finally copy all of the remaining finalized edges
		EdgesPtr finalized_edges(new Edges());
		for(Edges::iterator edge = edges_->begin(); edge != edges_->end(); edge++)
		{
			if(!(*edge))
				continue;

			finalized_edges->push_back(*edge);
		}

		return finalized_edges;
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
			split_arc->circle_event_.reset();
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

		// the left edge points to the right edge
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
		left_edge_node->edge_ = left_edge;
		// set the left child of the new edge node to arc0
		left_edge_node->setLeftChild(arc0);
		// set the right child of the new edge node to a new edge node
		BeachlineNodePtr right_edge_node(new BeachlineNode(right_edge));
		right_edge_node->setLeftChild(arc1);
		right_edge_node->setRightChild(arc2);
		// set this node as the right child of the left edge
		left_edge_node->setRightChild(right_edge_node);


		// check for circle event with a in the middle
		checkCircleEvent(arc0, event->y_);
		// check for circle event with c in the middle
		checkCircleEvent(arc2, event->y_);
		// NOTE: don't check for circle event with b in the middle because a and c have the same site (need 3 sites to make a circle event)
	}

	// TODO: implement me!
	void VoronoiDiagram::removeArc(EventPtr event)
	{
		// get pointer to arc that will be removed
		BeachlineNodePtr middle_arc = event->arc_.lock();

		// left_arc <- arc left of given arc
		BeachlineNodePtr left_arc = BeachlineNode::getLeftArc(middle_arc);
		// right_arc <- arc left of given arc
		BeachlineNodePtr right_arc = BeachlineNode::getRightArc(middle_arc);
		// if left_arc has a circle event
		if(left_arc->circle_event_)
		{
			// remove left_arc's circle event
			deleted_events_.insert(left_arc->circle_event_);
			left_arc->circle_event_.reset();
		}
		// if right_arc has a circle event
		if(right_arc->circle_event_)
		{
			// remove right_arc's circle event
			deleted_events_.insert(right_arc->circle_event_);
			right_arc->circle_event_.reset();
		}

		// s <- circumcenter between left_arc's site, given arc's site and right_arc's site
		PointPtr s = getArcPoint(middle_arc->site_, event->y_, event->site_->x);
		points_.push_back(s);
		
		// finish the left and right edges of the arc to be removed at the circumcircle site
		BeachlineNodePtr left_edge = BeachlineNode::getLeftEdge(middle_arc);
		BeachlineNodePtr right_edge = BeachlineNode::getRightEdge(middle_arc);
		left_edge->edge_->end_ = s;
		right_edge->edge_->end_ = s;

		// x <- new dangling edge at s
		EdgePtr new_edge(new Edge(s, left_arc->site_, right_arc->site_));
		edges_->push_back(new_edge);

		// if the left_edge is higher in the tree replace it with the new edge
		// otherwise replace the right edge
		BeachlineNodePtr highest_edge;
		BeachlineNodePtr curr_node = middle_arc->getParent();
		while(curr_node) // while we haven't reached the root
		{
			if(curr_node.get() == left_edge.get())
				highest_edge = left_edge;
			if(curr_node.get() == right_edge.get())
				highest_edge = right_edge;
			curr_node = curr_node->getParent();
		}
		highest_edge->edge_ = new_edge;

		// replace xl, given arc, xr by x
		BeachlineNodePtr parent_node = middle_arc->getParent();
		BeachlineNodePtr grandparent_node = parent_node->getParent();
		// if the arc to be replaced is left of its edge (we want to keep the right subtree of the edge)
		if(parent_node->getLeftChild().get() == middle_arc.get())
		{
			// if the arc is in the left subtree of its grandparent
			if(grandparent_node->getLeftChild().get() == parent_node.get())
			{
				// replace the grandparent's left subtree with the right subtree of the arc's edge
				grandparent_node->setLeftChild(parent_node->getRightChild());
			}
			else
			{
				// replace the grandparent's right subtree with the right subtree of the arc's edge
				grandparent_node->setRightChild(parent_node->getRightChild());
			}
		}
		else // the arc to be replaced is to the right of its edge (we want to keep the left subtree of the edge)
		{
			// if the arc is in the left subtree of its grandparent
			if(grandparent_node->getLeftChild().get() == parent_node.get())
			{
				// replace the grandparent's left subtree with the left subtree of the arc's edge
				grandparent_node->setLeftChild(parent_node->getLeftChild());
			}
			else
			{
				// replace the grandparent's left subtree with the right subtree of the arc's edge
				grandparent_node->setRightChild(parent_node->getLeftChild());
			}

		}

		// checkCircleEvent(left_arc)
		checkCircleEvent(left_arc, event->y_);
		// checkCircleEvent(right_arc)
		checkCircleEvent(right_arc, event->y_);
	}

	void VoronoiDiagram::checkCircleEvent(BeachlineNodePtr arc, float sweep_line)
	{
		// left_arc <- arc to the left of p
		BeachlineNodePtr left_arc = BeachlineNode::getLeftArc(arc);
		// right_arc <- arc to the right of p
		BeachlineNodePtr right_arc = BeachlineNode::getRightArc(arc);

		// if left_arc or right_arc does not exist
		if(!left_arc)
		{
			return; //( You need 3 points to have a circle event)
		}
		if(!right_arc)
		{
			return; // (You need 3 points to have a circle event)
		}
		// if right_arc's site == left arc's site
		if(right_arc->site_.get() == left_arc->site_.get())
		{
			return; // (You need 3 points to have a circle event)
		}

		// xl <- left edge of p
		BeachlineNodePtr left_edge_node = BeachlineNode::getLeftEdge(arc);
		EdgePtr left_edge = left_edge_node->edge_;
		// xr <- right edge of p
		BeachlineNodePtr right_edge_node = BeachlineNode::getRightEdge(arc);
		EdgePtr right_edge = right_edge_node->edge_;

		// s <- location where xl and xr cross each other
		PointPtr s = getEdgeIntersection(left_edge, right_edge);
		// if s does not exist
		if(!s)
			return;

		// r <- distance between s and p.site (circumcircle radius)
		float r = std::sqrt((s->x - arc->site_->x)*(s->x - arc->site_->x) + (s->y - arc->site_->y)*(s->y - arc->site_->y));
		// if s.y + r is under the sweepline
		if(s->y - r >= sweep_line)
			return;	

		// circle_event <- create new circle event with arc = p and y = s.y - r
		EventPtr circle_event(new Event(s, s->y-r, arc));
		arc->circle_event_ = circle_event; // TODO: Move this inside event constructor
		// add event to the queue
		event_queue_.push(circle_event);
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

		if(left_arc->site_->y == sweep_line_pos)
		{
			float x = left_arc->site_->x;
			float a = right_arc->site_->x;
			float b = right_arc->site_->y;
			float l = sweep_line_pos;
			float y = (a*a - 2.0f*a*x + b*b - l*l + x*x)/(2.0f*(b-l));

			Point edge_point(x,y);
			return edge_point;
		}

		if(right_arc->site_->y == sweep_line_pos)
		{
			float x = right_arc->site_->x;
			float a = left_arc->site_->x;
			float b = left_arc->site_->y;
			float l = sweep_line_pos;
			float y = (a*a - 2.0f*a*x + b*b - l*l + x*x)/(2.0f*(b-l));

			Point edge_point(x,y);
			return edge_point;
		}

		// just a convenince variable so I don't have to type this over and over
		float denom1 = 2.0f*(left_arc->site_->y - sweep_line_pos);
		// calculate the coefficients of the left arc's equation
		float a1 = 1/denom1;
		float b1 = -2.0f*left_arc->site_->x / denom1;
		float c1 = (left_arc->site_->x*left_arc->site_->x + left_arc->site_->y*left_arc->site_->y - sweep_line_pos*sweep_line_pos) / denom1;

		// now do the same thing for the right parabola
		float denom2 = 2.0f*(right_arc->site_->y - sweep_line_pos);
		float a2 = 1/denom2;
		float b2 = -2.0f*right_arc->site_->x / denom2;
		float c2 = (right_arc->site_->x*right_arc->site_->x + right_arc->site_->y*right_arc->site_->y - sweep_line_pos*sweep_line_pos) / denom2;

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
		double c = (focus->x*focus->x + focus->y*focus->y - directrix*directrix)/ denom;

		PointPtr arc_point(new Point(x, a*x*x + b*x + c));
		return arc_point;
	}

	PointPtr VoronoiDiagram::getEdgeIntersection(EdgePtr a, EdgePtr b)
	{
		// edge 1 has an equation 
		// y = m1*x + b1
		float m1 = a->direction_->y/a->direction_->x;
		float b1 = a->start_->y - m1*a->start_->x;

		// edge 2 has an equation
		// y = m2*x + b2
		float m2 = b->direction_->y/b->direction_->x;
		float b2 = b->start_->y - m2*b->start_->x;

		// setting the 2 equations equal and solving for the x-coordinate of the intersection
		float x = (b2-b1)/(m1-m2);
		// plug the x back into one of the edge equations to get y
		float y = m1*x + b1;

		// now check that this intersection is in the direction of both of the edges
		// otherwise they never actually intersect
		// do this by verifying the sign of the vector from each start point to the intersection matches
		// that edge's sign
		if((x - a->start_->x)/a->direction_->x < 0)
			return nullptr;
		if((y - a->start_->y)/a->direction_->y < 0)
			return nullptr;

		if((x - b->start_->x)/b->direction_->x < 0)
			return nullptr;
		if((y - b->start_->y)/b->direction_->y < 0)
			return nullptr;

		PointPtr intersection(new Point(x, y));
		return intersection;
	}

	void VoronoiDiagram::finishEdges(BeachlineNodePtr node, float width, float height)
	{
		// only finishing edges
		// we no longer care about the arcs themselves
		if(node->type_ == BeachlineNodeType::ARC)
			return;

		float x;
		// if the node is travelling towards the right of the screen (positive x direction)
		if(node->edge_->direction_->x > 0.0)
		{
			// cut off the node at the right edge
			x = width/2.0f;
		}
		else
		{
			// cut off the node at the left edge
			x = -width/2.0f;
		}
		// calculate the y position for the given x position
		float m = node->edge_->direction_->y/node->edge_->direction_->x;
		float b = node->edge_->start_->y - m*node->edge_->start_->x;
		float y = m*x + b;

		// set the edge's endpoint to the calculated position
		PointPtr end(new Point(x, y));
		node->edge_->end_ = end;

		// recursively call this on the left and right subtree
		finishEdges(node->getLeftChild(), width, height);
		finishEdges(node->getRightChild(), width, height);
	}
}