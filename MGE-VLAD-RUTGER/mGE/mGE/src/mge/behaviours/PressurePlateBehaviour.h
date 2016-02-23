#pragma once

#include <glm.hpp>
#include "mge/behaviours/AbstractBehaviour.hpp"
#include <SFML/Graphics.hpp>

class PressurePlateBehaviour : public AbstractBehaviour
{
public:
	PressurePlateBehaviour();
	~PressurePlateBehaviour();
	virtual void update(float step);

	bool _activated = false;

	void SetStatue(GameObject * pStatue);
	
private:
	GameObject * _statue;
};

