#pragma once
#include "Entity.h"
#include "Components/Transform.h"

class Component {

public:
	Component(Entity* mEntity);
	~Component();

	Entity* entity;

private:

};
