#ifndef VORONOIDIAGRAM_VORONOIDIAGRAM_H_
#define VORONOIDIAGRAM_VORONOIDIAGRAM_H_

#include "point.h"
#include <vector>

namespace voronoi_diagram
{
	typedef Point Site;
	typedef PointPtr SitePtr;
	typedef std::vector<SitePtr> Sites;
	typedef std::shared_ptr<Sites> SitesPtr;

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

	private:
		SitesPtr sites_;

	};

}

#endif