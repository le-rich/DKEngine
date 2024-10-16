#include "Entities/CameraEntity.h"

#include "Components/CameraComponent.h"

CameraEntity::CameraEntity() {
	CameraComponent* cameraComponent = new CameraComponent(this);
	this->addComponent(*cameraComponent);

}

CameraEntity::~CameraEntity() {

}
