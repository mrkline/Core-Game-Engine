#pragma once
#include <irrlicht.h>

//A class that provides tree functionality (with a parent and children) with reference counting
//using Irrlicht's IReferenceCounted class.
class RefCountedTreeNode : virtual public irr::IReferenceCounted
{
public:
	RefCountedTreeNode(RefCountedTreeNode* nodeParent = nullptr, bool updateOnChildrenChange = true);
	virtual ~RefCountedTreeNode();

	virtual void SetParent(RefCountedTreeNode* newParent);
	virtual RefCountedTreeNode* GetParent() { return parent; }
	virtual void RemoveFromParent(bool updateHierarchy = true);

	virtual void AddChild(RefCountedTreeNode* child);
	virtual void RemoveChild(RefCountedTreeNode* child);
	virtual void RemoveAllChildren();
	
	virtual irr::core::list<RefCountedTreeNode*>& GetChildren() { return children; }
	virtual const irr::core::list<RefCountedTreeNode*>& GetChildren() const { return children; }

	//Used to update any necessary info by a derived class when the tree changes.
	//If caresAboutChildren is false, changes to children will not cause this to be called.
	//Otherwise, this will be called recursively up, then down the tree.
	virtual void OnHierarchyChange(bool goingUp);

protected:
	RefCountedTreeNode* parent;
	irr::core::list<RefCountedTreeNode*> children;
	//If this is true, changes to children will not cause OnHierarchyChange to be called
	bool caresAboutChildren;
};