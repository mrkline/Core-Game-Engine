#include "TreeNode.h"

#include "ErrorHandling.h"

using namespace std;

namespace Core
{

	TreeNode::TreeNode(TreeNode* nodeParent, bool updateOnChildrenChange)
		: parent(nodeParent), caresAboutChildren(updateOnChildrenChange)
	{
		if(parent != nullptr)
		{
			parent->AddChild(this);
		}
	}

	TreeNode::~TreeNode()
	{
		DeleteAllChildren();
	}

	void TreeNode::SetParent(TreeNode* newParent)
	{
		if(newParent == this)
		{
			throw new Error::ArgumentException("A tree node cannot set itself as its parent.",
				__FUNCTION__);
		}
		RemoveFromParent(false);
		if(newParent != nullptr)
		{
			newParent->AddChild(this);
		}
		else
		{
			OnHierarchyChange(true);
		}
	}

	void TreeNode::AddChild(TreeNode* newChild)
	{
		if(newChild == nullptr)
		{
			throw new Error::ArgumentNullException("A tree node cannot add a null child.",
				__FUNCTION__);
		}
		if(newChild == this)
		{
			throw new Error::ArgumentException("A tree node cannot add itself as a child.",
				__FUNCTION__);
		}
		for(list<TreeNode*>::iterator it = children.begin();
			it != children.end(); ++it)
		{
			// We're trying to add a duplicate child
			if(*it == newChild)
			{
				throw new Error::ArgumentException("A tree node cannot have duplicate children.",
					__FUNCTION__);
			}
		}
		newChild->RemoveFromParent(false);
		children.push_back(newChild);
		newChild->parent = this;
		OnHierarchyChange(true);
	}

	void TreeNode::RemoveChild(TreeNode* toRemove)
	{
		if(toRemove == nullptr)
		{
			throw new Error::ArgumentNullException("A tree node cannot remove a null child.",
				__FUNCTION__);
		}
		for(list<TreeNode*>::iterator it = children.begin();
			it != children.end(); ++it)
		{
			if(*it == toRemove)
			{
				toRemove->parent = nullptr;
				toRemove->OnHierarchyChange(false);
				children.erase(it);
				OnHierarchyChange(true);
				return;
			}
		}
		throw new Error::ArgumentException("A tree node could not find the child that was to be removed.",
			__FUNCTION__);
	}

	void TreeNode::DeleteAllChildren()
	{
		for(list<TreeNode*>::iterator it = children.begin();
			it != children.end(); ++it)
		{
			delete *it;
		}

		children.clear();
		OnHierarchyChange(true);
	}

	void TreeNode::RemoveFromParent(bool updateHD)
	{
		if (parent != nullptr)
				parent->RemoveChild(this);
		if(updateHD)
			OnHierarchyChange(false);
	}

	void TreeNode::OnHierarchyChange(bool goingUp)
	{
		// Keep walking up the tree until it's time to stop and go back down
		if(goingUp && parent != nullptr && parent->caresAboutChildren)
		{
			parent->OnHierarchyChange(true);
		}
		// Don't forget to hit ourselves on the way back down
		else if(goingUp)
		{
				OnHierarchyChange(false);
		}
		// Walk down the tree, updating all children
		else
		{
			for(list<TreeNode*>::iterator it = children.begin();
			it != children.end(); ++it)
			{
				(*it)->OnHierarchyChange(false);
			}
		}
	}
} //end namespace Core