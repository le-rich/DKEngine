#include <iostream>


class UI {

public:
	void initialize() {
		std::cout << "Physics init" << std::endl;
	}

	void draw() {
		std::cout << "Drawing UI" << std::endl;
	}

	void Update() {
		std::cout << "UI update" << std::endl;
	}

};