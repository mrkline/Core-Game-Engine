#include "TreeNode.h"

#include <queue>

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
			throw new ArgumentException("A tree node cannot set itself as its parent.",
				__FUNCTION__);
		}
		RemoveFromParent(false);
		if(newParent != nullptr)
		{
			newParent->AddChild(this);
		}
		else
		{
			UpdateHierarchy();
		}
	}

	void TreeNode::AddChild(TreeNode* newChild)
	{
		if(newChild == nullptr)
		{
			throw new ArgumentNullException("A tree node cannot add a null child.",
				__FUNCTION__);
		}
		if(newChild == this)
		{
			throw new ArgumentException("A tree node cannot add itself as a child.",
				__FUNCTION__);
		}
		for(list<TreeNode*>::iterator it = children.begin();
			it != children.end(); ++it)
		{
			// We're trying to add a duplicate child
			if(*it == newChild)
			{
				throw new ArgumentException("A tree node cannot have duplicate children.",
					__FUNCTION__);
			}
		}
		newChild->RemoveFromParent(false);
		children.push_back(newChild);
		newChild->parent = this;
		UpdateHierarchy();
	}

	void TreeNode::RemoveChild(TreeNode* toRemove)
	{
		if(toRemove == nullptr)
		{
			throw new ArgumentNullException("A tree node cannot remove a null child.",
				__FUNCTION__);
		}
		for(list<TreeNode*>::iterator it = children.begin();
			it != children.end(); ++it)
		{
			if(*it == toRemove)
			{
				toRemove->parent = nullptr;
				toRemove->UpdateHierarchy();
				children.erase(it);
				UpdateHierarchy();
				return;
			}
		}
		throw new ArgumentException("A tree node could not find the child that was to be removed.",
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
		UpdateHierarchy();
	}

	void TreeNode::RemoveFromParent(bool updateHD)
	{
		if (parent != nullptr)
				parent->RemoveChild(this);
		if(updateHD)
			UpdateHierarchy();
	}

	void TreeNode::UpdateHierarchy()
	{
		TreeNode* top = this;

		// Keep walking up the tree until it's time to stop and go back down
		while(top->parent != nullptr && top->parent->caresAboutChildren)
		{
			top = top->parent;
		}
		
		// Walk down the tree (level order), updating all children
		queue<TreeNode*> q;
		q.push(top);
		while(!q.empty())
		{
			TreeNode* curr = q.front();
			q.pop();

			curr->OnHierarchyChange();

			for(list<TreeNode*>::iterator it = curr->children.begin();
			it != curr->children.end(); ++it)
			{
				q.push(*it);
			}
		}
	}
} //end namespace Core