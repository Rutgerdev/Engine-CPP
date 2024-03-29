#include "DoorBehaviour.h"

#include <glm.hpp>
#include "mge/core/GameObject.hpp"
#include <SFML/Graphics.hpp>
#include "mge/behaviours/PressurePlateBehaviour.h"
#include "mge\core\collision/StaticGameObject.h"

#include "mge/StatsHolder.h"

#include "mge/core/SoundManager.h"

DoorBehaviour::DoorBehaviour()
{
}

DoorBehaviour::~DoorBehaviour()
{
    //dtor
}

void DoorBehaviour::update(float pStep){
	if (CheckPlates()) {
		if (glm::distance(_owner->getWorldPosition(), _openPos)>0.5f) {
			dynamic_cast<StaticGameObject*>(_owner)->moveStaticObject(glm::normalize(_openPos - _closedPos) * pStep * 2 );
			
			if (!SoundManager::getInstance().IsPlaying(_owner->getName())) {
				SoundManager::getInstance().PlaySound(_owner->getName());
			}
		}
		else
		{
			if (SoundManager::getInstance().IsPlaying(_owner->getName())) {
				SoundManager::getInstance().StopSound(_owner->getName());
			}
		}
	}
	else if (glm::distance(_owner->getWorldPosition(), _closedPos)>0.5f)
	{
		dynamic_cast<StaticGameObject*>(_owner)->moveStaticObject(-glm::normalize(_openPos - _closedPos) * pStep *2 );
	}
}

void DoorBehaviour::AddPressurePlate(GameObject * plate)
{
	plates.push_back(plate);
}

void DoorBehaviour::SetKeysNeeded(int amount)
{
	keysNeeded = amount;
}

void DoorBehaviour::InitializePositions()
{
	_closedPos = _owner->getLocalPosition();
	_openPos = _owner->getLocalPosition() + glm::vec3(0, 5, 0);
}

bool DoorBehaviour::CheckPlates()
{
	int count = 0;
	for (int i = 0; i<plates.size(); i++) {
		if (dynamic_cast<PressurePlateBehaviour*>( plates[i]->getBehaviour())->_activated) {
			count++;
		}
	}

	if (count == plates.size() && StatsHolder::getKeyCount()>= keysNeeded) {
		return true;
	}
	else
	{
		return false;
	}
}

void DoorBehaviour::SetOpenPos(glm::vec3 translateUp){
    _openPos = _owner->getLocalPosition() + translateUp;
}
