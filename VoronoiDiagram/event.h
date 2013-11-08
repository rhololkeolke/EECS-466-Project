#ifndef VORONOIDIAGRAM_EVENT_H_
#define VORONOIDIAGRAM_EVENT_H_

#include "point.h"

namespace voronoi_diagram
{

	// TODO: Turn this into its own class
	class Arc{};
	typedef std::shared_ptr<Arc> ArcPtr;

	enum class EventType { SITE, CIRCLE };

	typedef struct Event_
	{
		float y_; // sorted by this
		EventType type_; // type of event. Either site or circle

		// stuff used when type is SITE
		SitePtr site_; // site for event

		// stuff used when type is CIRCLE
		ArcPtr arc_; // middle arc in triple defining the event

		Event(float y, SitePtr site) :
			y_(y),
			type_(EventType::SITE),
			site_(site),
			arc_(nullptr)
		{}

		Event(float y, ArcPtr arc) :
			y_(y),
			type_(EventType::CIRCLE),
			site_(nullptr),
			arc_(arc)
		{}
	} Event;

}

#endif