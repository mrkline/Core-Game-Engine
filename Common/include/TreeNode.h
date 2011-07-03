#pragma once

#include <list>

namespace Core
{
	/*!
	\brief A class that provides tree functionality (with a parent and children)
	\todo What should be done to restrict subclasses from adding any old TreeNode as a child, etc.?
			Restrict their type, Possibly via making TreeNode a template.
	*/
	class TreeNode
	{
	public:
		/*!
		\brief Construct a TreeNode
		\param nodeParent The parent of the node, or null if it doesn't have one
		\param updateOnChildrenChange Set to true to give the object notifications of hierarchy changes
		*/
		TreeNode(TreeNode* nodeParent = nullptr, bool updateOnChildrenChange = true);
		virtual ~TreeNode() {}

		/*!
		\brief Sets a parent for this tree node.
		\param newParent The new parent, or null if the tree node should have no parent
		*/
		void SetParent(TreeNode* newParent);

		//! Gets the tree node's parent, or null if it has none
		TreeNode* GetParent() { return parent; }

		/*!
		\brief Removes the tree node from its parent
		\param updateHierarchy Set to true to notify the surrounding tree of the change.
		*/
		void RemoveFromParent(bool updateHierarchy = true);

		/*!
		\brief Adds a child to this node
		\param child Child to add
		*/
		void AddChild(TreeNode* child);

		/*!
		\brief Remove a child from this node
		\param child Child to remove
		*/
		void RemoveChild(TreeNode* child);

		//! Deletes all children
		void DeleteAllChildren();
	
		//! Gets a list of the node's children
		std::list<TreeNode*>& GetChildren() { return children; }

		//! Gets a list of the node's children
		const std::list<TreeNode*>& GetChildren() const { return children; }

		/*!
		\brief Sets whether or not the node cares about hierarchy changes that occur to its children
		\see OnHierarchyChange
		*/
		void SetUpdateOnChildren(bool update) { caresAboutChildren = update; }

		/*!
		\brief Gets whether or not the node cares about hierarchy changes that occur to its children
		\see OnHierarchyChange
		*/
		bool GetUpdateOnChildren() { return caresAboutChildren; }

		/*!
		\brief Used to update any necessary info by a derived class when the tree changes.

		If caresAboutChildren is false, changes to children will not cause this to be called.
		*/
		virtual void OnHierarchyChange() {}

	protected:
		//! Node's parent, or null if it has none
		TreeNode* parent; 
		//! Linked list of children of this node
		std::list<TreeNode*> children; 
		//! If this is true, changes to children will not cause OnHierarchyChange to be called
		bool caresAboutChildren;

	private:
		//! Called internally to call OnHierarchyChange on all those in the hierarchy who care
		void UpdateHierarchy();
	};
} // end namespace Core