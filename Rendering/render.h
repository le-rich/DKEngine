#include <iostream>



class Render {

public:

	Render() {

	}

	void initialize() {
		std::cout << "Render init" << std::endl;
	}

	void RunRender() {
		std::cout << "Drawing UI" << std::endl;
	}


	void Update() {
		std::cout << "Render update" << std::endl;
	}
};

