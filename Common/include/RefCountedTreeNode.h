#pragma once
#include <ErrorHandling.h>
#include <list>

namespace Core
{
	//A class that provides tree functionality (with a parent and children) with reference counting
	//using Irrlicht's IReferenceCounted class.
	class RefCountedTreeNode : virtual public irr::IReferenceCounted, public Error::CanErr
	{
	public:
		RefCountedTreeNode(RefCountedTreeNode* nodeParent = nullptr, bool updateOnChildrenChange = true);
		virtual ~RefCountedTreeNode();

		virtual Error::ECode SetParent(RefCountedTreeNode* newParent);
		virtual RefCountedTreeNode* GetParent() { return parent; }
		virtual void RemoveFromParent(bool updateHierarchy = true);

		virtual Error::ECode AddChild(RefCountedTreeNode* child);
		virtual Error::ECode RemoveChild(RefCountedTreeNode* child);
		virtual void RemoveAllChildren();
	
		virtual std::list<RefCountedTreeNode*>& GetChildren() { return children; }
		virtual const std::list<RefCountedTreeNode*>& GetChildren() const { return children; }

		void SetUpdateOnChildren(bool update) { caresAboutChildren = update; }
		bool GetUpdateOnChildren(bool update) { return caresAboutChildren; }

		//Used to update any necessary info by a derived class when the tree changes.
		//If caresAboutChildren is false, changes to children will not cause this to be called.
		//Otherwise, this will be called recursively up, then down the tree.
		virtual void OnHierarchyChange(bool goingUp);

	protected:
		RefCountedTreeNode* parent;
		std::list<RefCountedTreeNode*> children;
		//If this is true, changes to children will not cause OnHierarchyChange to be called
		bool caresAboutChildren;
	};
} //end namespace Core