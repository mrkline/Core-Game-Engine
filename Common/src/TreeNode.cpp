#include "TreeNode.h"


using namespace std;

namespace Core
{
	using namespace Error;

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

	ECode TreeNode::SetParent(TreeNode* newParent)
	{
		if(newParent == this)
		{
			lastError = Error::E_CEK_BAD_ARG;
			lastErrorFunction = __FUNCTION__;
			customLastErrorMessage = "A tree node cannot set itself as its parent.";
			return lastError;
		}
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
		return Error::E_CEK_SUCCESS;
	}

	ECode TreeNode::AddChild(TreeNode* newChild)
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
		for(list<TreeNode*>::iterator it = children.begin();
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
		newChild->RemoveFromParent(false);
		children.push_back(newChild);
		newChild->parent = this;
		OnHierarchyChange(true);
		return Error::E_CEK_SUCCESS;
	}

	ECode TreeNode::RemoveChild(TreeNode* toRemove)
	{
		if(toRemove == nullptr)
		{
			lastError = Error::E_CEK_NULL_ARG;
			lastErrorFunction = __FUNCTION__;
			customLastErrorMessage = "A tree node cannot remove a null child.";
			return lastError;
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
				return Error::E_CEK_SUCCESS;
			}
		}
		lastError = Error::E_CEK_BAD_ARG;
		lastErrorFunction = __FUNCTION__;
		customLastErrorMessage = "A tree node could not find the child that was to be removed.";
		return lastError;
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
			for(list<TreeNode*>::iterator it = children.begin();
			it != children.end(); ++it)
			{
				(*it)->OnHierarchyChange(false);
			}
		}
	}
} //end namespace Core