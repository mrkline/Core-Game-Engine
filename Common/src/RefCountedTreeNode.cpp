#include "RefCountedTreeNode.h"
#include "ErrorHandling.h"

using namespace irr;
using namespace core;

RefCountedTreeNode::RefCountedTreeNode(RefCountedTreeNode* nodeParent, bool updateOnChildrenChange)
	: parent(nodeParent), caresAboutChildren(updateOnChildrenChange)
{
	if(parent != nullptr)
	{
		parent->AddChild(this);
	}
}

RefCountedTreeNode::~RefCountedTreeNode()
{
	RemoveAllChildren();
}

void RefCountedTreeNode::SetParent(RefCountedTreeNode* newParent)
{
	if(newParent == this)
	{
		throw new Error::ArgumentException("A tree node cannot be its own parent.", __FUNCTION__);
	}
	grab();
	RemoveFromParent(false);
	if(newParent != nullptr)
	{
		newParent->AddChild(this);
	}
	else
	{
		OnHierarchyChange(true);
	}
	drop();
}

void RefCountedTreeNode::AddChild(RefCountedTreeNode* newChild)
{
		if(newChild == nullptr || newChild == this)
		{
			throw new Error::ArgumentException("A child of an tree node cannot be null or itself.", __FUNCTION__);
		}
		for(list<RefCountedTreeNode*>::Iterator it = children.begin();
			it != children.end(); ++it)
		{
			if(*it == newChild)
			{
				throw new Error::ArgumentException("The provided tree node is already a child of this node.", __FUNCTION__);
			}
		}
		newChild->grab();
		newChild->RemoveFromParent(false);
		children.push_back(newChild);
		newChild->parent = this;
		OnHierarchyChange(true);
}

void RefCountedTreeNode::RemoveChild(RefCountedTreeNode* toRemove)
{
	if(toRemove == nullptr)
	{
		throw new Error::ArgumentException("You cannot remove a null tree node.", __FUNCTION__);
	}
	for(list<RefCountedTreeNode*>::Iterator it = children.begin();
		it != children.end(); ++it)
	{
		if(*it == toRemove)
		{
			toRemove->parent = nullptr;
			toRemove->OnHierarchyChange(false);
			toRemove->drop();
			children.erase(it);
			OnHierarchyChange(true);
			return;
		}
	}
	throw new Error::ArgumentException("The given tree node is not a child of this node.", __FUNCTION__);
}

void RefCountedTreeNode::RemoveAllChildren()
{
	for(list<RefCountedTreeNode*>::Iterator it = children.begin();
		it != children.end(); ++it)
	{
		RefCountedTreeNode* curr = *it;
		curr->parent = nullptr;
		curr->OnHierarchyChange(false);
		curr->drop();
	}

	children.clear();
	OnHierarchyChange(true);
}

void RefCountedTreeNode::RemoveFromParent(bool updateHD)
{
	if (parent != nullptr)
			parent->RemoveChild(this);
	if(updateHD)
		OnHierarchyChange(false);
}

void RefCountedTreeNode::OnHierarchyChange(bool goingUp)
{
	//Keep walking up the tree until it's time to stop and go back down
	if(goingUp && parent != nullptr && parent->caresAboutChildren)
	{
		parent->OnHierarchyChange(true);
	}
	//Don't forget to hit ourselves on the way back down
	else if(goingUp)
	{
			OnHierarchyChange(false);
	}
	//Walk down the tree, updating all children
	else
	{
		for(list<RefCountedTreeNode*>::Iterator it = children.begin();
		it != children.end(); ++it)
		{
			(*it)->OnHierarchyChange(false);
		}
	}
}