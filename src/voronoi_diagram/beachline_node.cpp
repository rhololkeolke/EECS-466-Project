#include "beachline_node.h"
#include "event.h"

namespace voronoi_diagram
{
	std::shared_ptr<BeachlineNode> BeachlineNode::getLeftArc(std::shared_ptr<BeachlineNode> node)
	{
		std::shared_ptr<BeachlineNode> curr_node;
		if(node->type_ == BeachlineNodeType::ARC) // if this is an arc
		{
			// initialize last_node to the input 
			std::shared_ptr<BeachlineNode> last_node = node;
			// initialize curr_node to the input's parent
			curr_node = node->getParent();

			while(curr_node && curr_node->getLeftChild().get() == last_node.get())
			{
				last_node = curr_node;
				curr_node = curr_node->getParent();
			}

			// if we traversed all the way to the root without finding a new left branch
			// then there is no arc to the left of the one specified
			if(!curr_node)
				return nullptr;

			// go down the new left branch
			curr_node = curr_node->getLeftChild();
		}
		else
			curr_node = node->getLeftChild(); // no need to go up if we're starting on an edge

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
			// initialize last_node to the input
			std::shared_ptr<BeachlineNode> last_node = node;
			// initialize curr_node to the input's parent
			curr_node = node->getParent();

			while(curr_node && curr_node->getRightChild().get() == last_node.get())
			{
				last_node = curr_node;
				curr_node = curr_node->getParent();
			}

			// if we traversed all the way to the root without finding a new right branch
			// then there is no arc to the right of the one specified
			if(!curr_node)
				return nullptr;

			// go down the new right branch
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

	// to be a left edge of an arc the arc must have come from the right subtree
	std::shared_ptr<BeachlineNode> BeachlineNode::getLeftEdge(std::shared_ptr<BeachlineNode> node)
	{
		std::shared_ptr<BeachlineNode> last_node = node;
		std::shared_ptr<BeachlineNode> curr_node = node->getParent();
		// keep going until we come from a right subtree or we run out of nodes
		while(curr_node && curr_node->getLeftChild().get() == last_node.get())
		{
			last_node = curr_node;
			curr_node = last_node->getParent();
		}

		return curr_node;
	}

	// to be a right edge of an arc the arc must have come from the edge's left subtree
	std::shared_ptr<BeachlineNode> BeachlineNode::getRightEdge(std::shared_ptr<BeachlineNode> node)
	{
		std::shared_ptr<BeachlineNode> last_node = node;
		std::shared_ptr<BeachlineNode> curr_node = node->getParent();
		// keep going until we come from a left subtree or we run out of nodes
		while(curr_node && curr_node->getRightChild().get() == last_node.get())
		{
			last_node = curr_node;
			curr_node = last_node->getParent();
		}
		return curr_node;
	}

}