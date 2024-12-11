#include "Components\UIComponent.h"

UIComponent::UIComponent(Entity* entity) : Component(entity) {
	this->componentType = ComponentType::UI;
}

void UIComponent::update() {

}