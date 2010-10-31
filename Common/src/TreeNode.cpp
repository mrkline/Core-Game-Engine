#include "TreeNode.h"
#include "ErrorHandling.h"

using namespace irr;
using namespace core;

TreeNode::~TreeNode()
{
	RemoveFromParent(false);
	RemoveAllChildren();
}

void TreeNode::SetParent(TreeNode* newParent)
{
	if(newParent == this)
	{
		throw new Error::ArgumentException("A tree node cannot be its own parent.", __FUNCTION__);
	}
	RemoveFromParent(false);
	if(newParent != nullptr)
	{
		newParent->AddChild(this);
	}
	else
	{
		OnHierarchyChange();
	}
}

void TreeNode::AddChild(TreeNode* newChild)
{
		if(newChild == nullptr || newChild == this)
		{
			throw new Error::ArgumentException("A child of an tree node cannot be null or itself.", __FUNCTION__);
		}
		for(list<TreeNode*>::Iterator it = children.begin();
			it != children.end(); ++it)
		{
			if(*it == newChild)
			{
				throw new Error::ArgumentException("The provided tree node is already a child of this node.", __FUNCTION__);
			}
		}
		newChild->RemoveFromParent(false);
		children.push_back(newChild);
		newChild->parent = this;
		OnHierarchyChange();
}

void TreeNode::RemoveChild(TreeNode* toRemove)
{
	if(toRemove == nullptr)
	{
		throw new Error::ArgumentNullException("You cannot remove a null tree node.", __FUNCTION__);
	}
	for(list<TreeNode*>::Iterator it = children.begin();
		it != children.end(); ++it)
	{
		if(*it == toRemove)
		{
			toRemove->parent = nullptr;
			toRemove->OnHierarchyChange();
			children.erase(it);
			OnHierarchyChange();
			return;
		}
	}
	throw new Error::ArgumentException("The given tree node is not a child of this node.", __FUNCTION__);
}

void TreeNode::RemoveAllChildren()
{
	for(list<TreeNode*>::Iterator it = children.begin();
		it != children.end(); ++it)
	{
		(*it)->parent = nullptr;
		(*it)->OnHierarchyChange();
	}

	children.clear();
	OnHierarchyChange();
}

void TreeNode::RemoveFromParent(bool updateHD)
{
	if (parent != nullptr)
			parent->RemoveChild(this);
	if(updateHD)
		OnHierarchyChange();
}

void TreeNode::OnHierarchyChange()
{
	for(list<TreeNode*>::Iterator it = children.begin();
		it != children.end(); ++it)
	{
		(*it)->OnHierarchyChange();
	}
}