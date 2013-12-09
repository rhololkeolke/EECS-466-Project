#ifndef VORONOIDIAGRAM_BEACHLINE_NODE_H_
#define VORONOIDIAGRAM_BEACHLINE_NODE_H_

#include <memory>
#include "edge.h"
#include "point.h"

namespace voronoi_diagram
{
	struct Event_;

	enum class BeachlineNodeType { ARC, EDGE };

	class BeachlineNode : public std::enable_shared_from_this<BeachlineNode>
	{
	public:

		BeachlineNode(SitePtr site) :
			type_(BeachlineNodeType::ARC),
			site_(site),
			circle_event_(nullptr),
			edge_(nullptr)
		{}

		// TODO: Consider making this a factory method to support
		// constructing an edge with a specified left and right edeg
		// can't do it in a normal constructor, because enabled_shared_from_this
		// doesn't work in a constructor
		BeachlineNode(EdgePtr edge) :
			type_(BeachlineNodeType::EDGE),
			site_(nullptr),
			circle_event_(nullptr),
			edge_(edge)
		{}

		BeachlineNodeType type_;

		// when an arc
		SitePtr site_; // site that is this arc's focal point
		std::shared_ptr<struct Event_> circle_event_; // event that is poised to kill this arc

		// when an edge
		EdgePtr edge_;

		// methods
		std::shared_ptr<BeachlineNode> getArcUnderSite(SitePtr site);

		std::shared_ptr<BeachlineNode> getLeftChild()
		{
			return left_child_;
		}

		void setLeftChild(std::shared_ptr<BeachlineNode> child)
		{
			left_child_ = child;
			left_child_->parent_ = shared_from_this();
		}

		std::shared_ptr<BeachlineNode> getRightChild()
		{
			return right_child_;
		}

		void setRightChild(std::shared_ptr<BeachlineNode> child)
		{
			right_child_ = child;
			right_child_->parent_ = shared_from_this();
		}

		std::shared_ptr<BeachlineNode> getParent()
		{
			return parent_.lock();
		}

		void setParent(std::shared_ptr<BeachlineNode> parent)
		{
			parent_ = parent;
		}

		static std::shared_ptr<BeachlineNode> getLeftArc(std::shared_ptr<BeachlineNode> node);
		static std::shared_ptr<BeachlineNode> getRightArc(std::shared_ptr<BeachlineNode> node);
		static std::shared_ptr<BeachlineNode> getLeftEdge(std::shared_ptr<BeachlineNode> node);
		static std::shared_ptr<BeachlineNode> getRightEdge(std::shared_ptr<BeachlineNode> node);

	private:
		std::shared_ptr<BeachlineNode> left_child_;
		std::shared_ptr<BeachlineNode> right_child_;
		std::weak_ptr<BeachlineNode> parent_;
	};

	typedef std::shared_ptr<BeachlineNode> BeachlineNodePtr;
}

#endif