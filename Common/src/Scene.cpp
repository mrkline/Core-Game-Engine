#include "Scene.h"

#include <stack>
#include <queue>

#include "GameObject.h"

using namespace std;

namespace Core
{
	Scene::Scene(PhysicsManager* physicsMan)
		: physMan(physicsMan)
	{
		rootObject = std::auto_ptr<GameObject>(new GameObject(nullptr, Transform(), 0, "root"));
	}

	GameObject* Scene::GetObject(const std::string& name, SearchType st)
	{
		switch(st)
		{
		case E_ST_BFS:
			{
				queue<GameObject*> q;

				q.push(rootObject.get());

				while(!q.empty())
				{
					GameObject* curr = q.front();
					q.pop();

					if(curr->GetName() == name)
					{
						return curr;
					}
					else
					{
						const list<GameObject*>& children = curr->GetChildren();
						for(auto it = children.begin(); it != children.end(); ++it)
							q.push(*it);
					}
				}
			}
			break;

		case E_ST_DFS:
			{
				stack<GameObject*> s;

				s.push(rootObject.get());

				while(!s.empty())
				{
					GameObject* curr = s.top();
					s.pop();

					if(curr->GetName() == name)
					{
						return curr;
					}
					else
					{
						const list<GameObject*>& children = curr->GetChildren();
						for(auto it = children.begin(); it != children.end(); ++it)
							s.push(*it);
					}
				}
			}
			break;
		}

		return nullptr;
	}

	GameObject* Scene::GetObject(int id, SearchType st)
	{
		switch(st)
		{
		case E_ST_BFS:
			{
				queue<GameObject*> q;

				q.push(rootObject.get());

				while(!q.empty())
				{
					GameObject* curr = q.front();
					q.pop();

					if(curr->GetID() == id)
					{
						return curr;
					}
					else
					{
						const list<GameObject*>& children = curr->GetChildren();
						for(auto it = children.begin(); it != children.end(); ++it)
							q.push(*it);
					}
				}
			}
			break;

		case E_ST_DFS:
			{
				stack<GameObject*> s;

				s.push(rootObject.get());

				while(!s.empty())
				{
					GameObject* curr = s.top();
					s.pop();

					if(curr->GetID() == id)
					{
						return curr;
					}
					else
					{
						const list<GameObject*>& children = curr->GetChildren();
						for(auto it = children.begin(); it != children.end(); ++it)
							s.push(*it);
					}
				}
			}
			break;
		}

		return nullptr;
	}
} // end namespace Core