#include <iostream>


class UI {

public:

	UI() {

	}

	void initialize() {
		std::cout << "Physics init" << std::endl;
	}

	void Draw() {
		std::cout << "Drawing UI" << std::endl;
	}

	void Update() {
		std::cout << "UI update" << std::endl;
	}

};