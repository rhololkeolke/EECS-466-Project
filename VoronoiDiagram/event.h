#ifndef VORONOIDIAGRAM_EVENT_H_
#define VORONOIDIAGRAM_EVENT_H_

#include "point.h"

namespace voronoi_diagram
{

	// TODO: Turn this into its own class
	class ArcPtr {};

	enum class EventType { SITE, CIRCLE };

	typedef struct Event_
	{
		float y_; // sorted by this
		EventType type_; // type of event. Either site or circle

		// stuff used when type is SITE
		SitePtr site_; // site for event

		// stuff used when type is CIRCLE
		ArcPtr arc; // middle arc in triple defining the event
	} Event;

}

#endif