#pragma once
#include <irrlicht.h>

class TreeNode
{
public:
	virtual ~TreeNode();

	virtual void SetParent(TreeNode* newParent);
	virtual TreeNode* GetParent() { return parent; }
	virtual void RemoveFromParent(bool updateHierarchy = true);

	virtual void AddChild(TreeNode* child);
	virtual void RemoveChild(TreeNode* child);
	virtual void RemoveAllChildren();
	
	virtual irr::core::list<TreeNode*>& GetChildren() { return children; }
	virtual const irr::core::list<TreeNode*>& GetChildren() const { return children; }

	//Notify children of changes
	//Called recursively on all children
	virtual void OnHierarchyChange();

protected:

	TreeNode* parent;
	irr::core::list<TreeNode*> children;
};