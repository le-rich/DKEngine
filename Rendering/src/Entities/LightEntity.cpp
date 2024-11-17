#include "Entities/LightEntity.h"

#include "Components/LightComponent.h"

LightEntity::LightEntity() {
	LightComponent* lightComponent = new LightComponent(this);
	this->addComponent(*lightComponent);
}

LightEntity::~LightEntity() {

}