#pragma once

#include <Utils/IDUtils.h>
#include <string>

class Entity;
class Transform;

class Component {

public:
	Component(Entity* mEntity);
	~Component();

	Entity* entity;

	UUIDv4::UUID componentID;
	std::string componentDisplayName;

	UUIDv4::UUID GetComponentID() {
		return componentID;
	}

	void SetComponentID(UUIDv4::UUID& newID) {
		this->componentID = newID;
	}

	const std::string& GetDisplayName()
	{
		return componentDisplayName;
	}

	// set display id of component
	void SetDisplayName(std::string newID)
	{
		componentDisplayName = newID;
	}

	bool operator==(const Component& other) const;

private:
	
};
