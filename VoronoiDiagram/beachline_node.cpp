#include "beachline_node.h"
#include "event.h"

namespace voronoi_diagram
{
	std::shared_ptr<BeachlineNode> BeachlineNode::getLeftArc(std::shared_ptr<BeachlineNode> node)
	{
		std::shared_ptr<BeachlineNode> curr_node;
		if(node->type_ == BeachlineNodeType::ARC) // if this is an arc
		{
			// first go up to the left edge
			curr_node = node->getParent();
			if(!curr_node) // if this is the root 
				return nullptr; // there is nothing to the left of this
			curr_node = curr_node->getLeftChild(); // otherwise start going down the edges to the left of this node's left edge
		}
		else
			curr_node = node->getLeftChild(); // no need to go up if we're starting on an edge
		if(!curr_node)
			return nullptr;
		while(curr_node->type_ != BeachlineNodeType::ARC)
		{
			curr_node = curr_node->getRightChild();
		}
		return curr_node;
	}

	// opposite of getLeftArc method
	std::shared_ptr<BeachlineNode> BeachlineNode::getRightArc(std::shared_ptr<BeachlineNode> node)
	{
		std::shared_ptr<BeachlineNode> curr_node;
		if(node->type_ == BeachlineNodeType::ARC)
		{
			curr_node = node->getParent();
			if(!curr_node)
				return nullptr;
			curr_node = curr_node->getRightChild();
		}
		else
			curr_node = node->getRightChild();

		if(!curr_node)
			return nullptr;
		while(curr_node->type_ != BeachlineNodeType::ARC)
		{
			curr_node = curr_node->getLeftChild();
		}
		return curr_node;
	}

}