#ifndef VORONOI_DIAGRAM_ANIMATED_VORONOI_DIAGRAM_H_
#define VORONOI_DIAGRAM_ANIMATED_VORONOI_DIAGRAM_H_

#include "voronoi_diagram.h"

namespace voronoi_diagram
{
	class AnimatedVoronoiDiagram : public VoronoiDiagram
	{
	public:
		AnimatedVoronoiDiagram(SitesPtr sites, float width, float height): 
			VoronoiDiagram(sites, width, height),
			view_width_(4*width),
			view_height_(4*height),
			finished_animation_(false)
		{}

		virtual ~AnimatedVoronoiDiagram()
		{}

		void display(void);

		void nextEvent();

		void restartAnimation();

		float getViewWidth()
		{
			return view_width_;
		}
		void setViewWidth(float width);

		float getViewHeight()
		{
			return view_height_;
		}
		void setViewHeight(float height);

		bool finished()
		{
			return finished_animation_;
		}

	private:
		EventPtr curr_event_;

		void drawBeachline(BeachlineNodePtr arc, float line_position);

		float view_width_, view_height_;

		bool finished_animation_;

	};
}

#endif
