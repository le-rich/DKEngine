#include <iostream>



class Render {

public:

	void initialize() {
		std::cout << "Render init" << std::endl;
	}

	void render() {
		std::cout << "Drawing UI" << std::endl;
	}


	void Update() {
		std::cout << "Render update" << std::endl;
	}
};

