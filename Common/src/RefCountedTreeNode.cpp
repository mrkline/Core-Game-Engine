#include <RefCountedTreeNode.h>

using namespace irr;
using namespace core;

namespace Core
{
	using namespace Error;

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

	ECode RefCountedTreeNode::SetParent(RefCountedTreeNode* newParent)
	{
		if(newParent == this)
		{
			lastError = Error::E_CEK_BAD_ARG;
			lastErrorFunction = __FUNCTION__;
			customLastErrorMessage = "A tree node cannot set itself as its parent.";
			return lastError;
		}
		grab();
		RemoveFromParent(false);
		if(newParent != nullptr)
		{
			if(Failed(newParent->AddChild(this)))
			{
				lastErrorFunction = __FUNCTION__;
				customLastErrorMessage = "A tree node could not add itself as a child of its parent.";
				return newParent->lastError;
			}
		}
		else
		{
			OnHierarchyChange(true);
		}
		drop();
		return Error::E_CEK_SUCCESS;
	}

	ECode RefCountedTreeNode::AddChild(RefCountedTreeNode* newChild)
	{
		if(newChild == nullptr)
		{
			lastError = Error::E_CEK_NULL_ARG;
			lastErrorFunction = __FUNCTION__;
			customLastErrorMessage = "A tree node cannot add a null child.";
			return lastError;
		}
		if(newChild == this)
		{
			lastError = Error::E_CEK_BAD_ARG;
			lastErrorFunction = __FUNCTION__;
			customLastErrorMessage = "A tree node cannot add itself as a child.";
			return lastError;
		}
		for(list<RefCountedTreeNode*>::Iterator it = children.begin();
			it != children.end(); ++it)
		{
			//We're trying to add a duplicate child
			if(*it == newChild)
			{
				lastError = Error::E_CEK_BAD_ARG;
				lastErrorFunction =  __FUNCTION__;
				customLastErrorMessage = "A tree node cannot have duplicate children.";
				return lastError;
			}
		}
		newChild->grab();
		newChild->RemoveFromParent(false);
		children.push_back(newChild);
		newChild->parent = this;
		OnHierarchyChange(true);
		return Error::E_CEK_SUCCESS;
	}

	ECode RefCountedTreeNode::RemoveChild(RefCountedTreeNode* toRemove)
	{
		if(toRemove == nullptr)
		{
			lastError = Error::E_CEK_NULL_ARG;
			lastErrorFunction = __FUNCTION__;
			customLastErrorMessage = "A tree node cannot remove a null child.";
			return lastError;
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
				return Error::E_CEK_SUCCESS;
			}
		}
		lastError = Error::E_CEK_BAD_ARG;
		lastErrorFunction = __FUNCTION__;
		customLastErrorMessage = "A tree node could not find the child that was to be removed.";
		return lastError;
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
} //end namespace Core