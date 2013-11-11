#ifndef VORONOIDIAGRAM_EVENT_H_
#define VORONOIDIAGRAM_EVENT_H_

#include <functional>
#include <memory>
#include "beachline_node.h"
#include "point.h"

namespace voronoi_diagram
{

	enum class EventType { SITE, CIRCLE };

	typedef struct Event_ 
	{
		float y_; // sorted by this
		EventType type_; // type of event. Either site or circle

		// stuff used when type is SITE
		SitePtr site_; // site for event

		// stuff used when type is CIRCLE
		std::weak_ptr<BeachlineNode> arc_; // middle arc in triple defining the event

		Event_(SitePtr site) :
			y_(site->y),
			type_(EventType::SITE),
			site_(site)
		{}

		Event_(PointPtr intersection, float y, std::shared_ptr<BeachlineNode> arc) :
			site_(intersection),
			y_(y),
			type_(EventType::CIRCLE),
			arc_(arc)
		{}

		struct CompareEvent : public std::binary_function<std::shared_ptr<struct Event_>, std::shared_ptr<struct Event_>, bool>
		{
			bool operator()(const std::shared_ptr<struct Event_> l, const std::shared_ptr<struct Event_> r) const { return (l->y_ < r->y_); }
		};

		struct EqualEvent : public std::binary_function<std::shared_ptr<struct Event_>, std::shared_ptr<struct Event_>, bool>
		{
			bool operator()(const std::shared_ptr<struct Event_> l, const std::shared_ptr<struct Event_> r) const { return (l.get() == r.get()); }
		};
	} Event;



	typedef std::shared_ptr<Event> EventPtr;

}

#endif