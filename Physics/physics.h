#include <iostream>


class Physics {

public:

	Physics() {

	}

	void initialize() {
		std::cout << "Physics init" << std::endl;
	}


	void Simulate() {
		std::cout << "Running Simulation" << std::endl;
	}


	void Update() {
		std::cout << "Updating Physics" << std::endl;
	}

};