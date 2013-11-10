#ifndef VORONOI_DIAGRAM_ANIMATED_VORONOI_DIAGRAM_H_
#define VORONOI_DIAGRAM_ANIMATED_VORONOI_DIAGRAM_H_

#include "voronoi_diagram.h"

namespace voronoi_diagram
{
	class AnimatedVoronoiDiagram : public VoronoiDiagram
	{
	public:
		AnimatedVoronoiDiagram(SitesPtr sites, float width, float height): 
			VoronoiDiagram(sites, width, height)
		{}

		virtual ~AnimatedVoronoiDiagram()
		{}

		void display(void);

		void nextEvent();

		void restartAnimation();

	private:
		EventPtr curr_event_;
	};
}

#endif