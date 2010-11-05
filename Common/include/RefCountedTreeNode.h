#pragma once
#include <irrlicht.h>

//A class that provides tree functionality (with a parent and children) with reference counting
//using Irrlicht's IReferenceCounted class.
class RefCountedTreeNode : virtual public irr::IReferenceCounted
{
public:
	RefCountedTreeNode(RefCountedTreeNode* nodeParent = nullptr);
	virtual ~RefCountedTreeNode();

	virtual void SetParent(RefCountedTreeNode* newParent);
	virtual RefCountedTreeNode* GetParent() { return parent; }
	virtual void RemoveFromParent(bool updateHierarchy = true);

	virtual void AddChild(RefCountedTreeNode* child);
	virtual void RemoveChild(RefCountedTreeNode* child);
	virtual void RemoveAllChildren();
	
	virtual irr::core::list<RefCountedTreeNode*>& GetChildren() { return children; }
	virtual const irr::core::list<RefCountedTreeNode*>& GetChildren() const { return children; }

	//Notify children of changes
	//Called recursively on all children
	virtual void OnHierarchyChange();

protected:
	RefCountedTreeNode* parent;
	irr::core::list<RefCountedTreeNode*> children;
};