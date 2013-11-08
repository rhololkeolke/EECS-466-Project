#ifndef VORONOIDIAGRAM_VORONOIDIAGRAM_H_
#define VORONOIDIAGRAM_VORONOIDIAGRAM_H_

#include "point.h"
#include <vector>

namespace voronoi_diagram
{

	class VoronoiDiagram
	{
	public:

		VoronoiDiagram(SitesPtr sites)
			: sites_(sites)
		{}

		VoronoiDiagram() :
			sites_(new Sites())
		{}

		void addPoint(SitePtr site)
		{
			sites_->push_back(site);
		}

		SitesConstPtr getSites()
		{
			SitesConstPtr const_sites(sites_);
			return const_sites;
		}



	private:
		SitesPtr sites_;

	};

}

#endif