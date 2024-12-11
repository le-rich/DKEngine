#pragma once

#include "Component.h"

#include <imgui.h>
#include <imgui_internal.h>

class UIComponent : public Component {
private:
	float posX;
	float posY;
	float width;
	float height;

	bool snapBottom;
	bool snapRight;

	bool isText;

	std::string imgPath;
	std::string text;
	
public:

	UIComponent(Entity* mEntity);

	float getPosX() { return posX; }
	float getPosY() { return posY; }
	float getWidth() { return width; }
	float getHeight() { return height; }
	std::string getImgPath() { return imgPath; }
	std::string getText() { return text; }
	bool getIsText() { return isText; }

	void setPosX(float newPos) { posX = newPos; }
	void setPosY(float newPos) { posY = newPos; }
	void setWidth(float newWidth) { width = newWidth; }
	void setHeight(float newHeight) { height = newHeight; }
	void setImgPath(std::string newImgPath) { imgPath = newImgPath; }
	void setText(std::string newText) { text = newText; }
	void setIsText(bool newIsText) { isText = newIsText; }


	void update();
};