#include "RefCountedTreeNode.h"
#include "ErrorHandling.h"

using namespace irr;
using namespace core;

RefCountedTreeNode::RefCountedTreeNode(RefCountedTreeNode* nodeParent)
	: parent(nodeParent)
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
		OnHierarchyChange();
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
		OnHierarchyChange();
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
			toRemove->OnHierarchyChange();
			toRemove->drop();
			children.erase(it);
			OnHierarchyChange();
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
		curr->OnHierarchyChange();
		curr->drop();
	}

	children.clear();
	OnHierarchyChange();
}

void RefCountedTreeNode::RemoveFromParent(bool updateHD)
{
	if (parent != nullptr)
			parent->RemoveChild(this);
	if(updateHD)
		OnHierarchyChange();
}

void RefCountedTreeNode::OnHierarchyChange()
{
	for(list<RefCountedTreeNode*>::Iterator it = children.begin();
		it != children.end(); ++it)
	{
		(*it)->OnHierarchyChange();
	}
}