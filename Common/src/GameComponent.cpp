#include "GameComponent.h"
#include "ErrorHandling.h"

namespace GameCore
{
	GameComponent::GameComponent(GameObject* objOwner)
		: owner(objOwner)
	{
		if(objOwner == nullptr)
		{
			throw new Error::ArgumentException("A game component cannot have a null owner", __FUNCTION__);
		}
	}
} //end namespace GameCore