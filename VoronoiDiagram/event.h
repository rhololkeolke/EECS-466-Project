#ifndef VORONOIDIAGRAM_EVENT_H_
#define VORONOIDIAGRAM_EVENT_H_

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

		Event_(float y, SitePtr site) :
			y_(y),
			type_(EventType::SITE),
			site_(site)
		{}

		Event_(float y, std::shared_ptr<BeachlineNode> arc) :
			y_(y),
			type_(EventType::CIRCLE),
			site_(nullptr),
			arc_(arc)
		{}
	} Event;

	typedef std::shared_ptr<Event> EventPtr;

}

#endif