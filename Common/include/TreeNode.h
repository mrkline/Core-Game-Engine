#pragma once
#include <ErrorHandling.h>
#include <list>

namespace Core
{
	//A class that provides tree functionality (with a parent and children) with reference counting
	//using Irrlicht's IReferenceCounted class.
	class TreeNode : public Error::CanErr
	{
	public:
		TreeNode(TreeNode* nodeParent = nullptr, bool updateOnChildrenChange = true);
		virtual ~TreeNode();

		virtual Error::ECode SetParent(TreeNode* newParent);
		virtual TreeNode* GetParent() { return parent; }
		virtual void RemoveFromParent(bool updateHierarchy = true);

		virtual Error::ECode AddChild(TreeNode* child);
		virtual Error::ECode RemoveChild(TreeNode* child);
		virtual void RemoveAllChildren();
	
		virtual std::list<TreeNode*>& GetChildren() { return children; }
		virtual const std::list<TreeNode*>& GetChildren() const { return children; }

		void SetUpdateOnChildren(bool update) { caresAboutChildren = update; }
		bool GetUpdateOnChildren(bool update) { return caresAboutChildren; }

		//Used to update any necessary info by a derived class when the tree changes.
		//If caresAboutChildren is false, changes to children will not cause this to be called.
		//Otherwise, this will be called recursively up, then down the tree.
		virtual void OnHierarchyChange(bool goingUp);

	protected:
		TreeNode* parent;
		std::list<TreeNode*> children;
		//If this is true, changes to children will not cause OnHierarchyChange to be called
		bool caresAboutChildren;
	};
} //end namespace Core