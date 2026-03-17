#include <iostream>
#include <stdlib.h>

int main() {
	int a;
	std::cout << "Enter a number to execute a corresponding file: \
		\n\t1. snowflakes.cpp \
		\n\t2. trafficLight.cpp\n";
	
	std::cin >> a;

	switch (a) {
	case 1:
		system("g++ -o snowflakes.cpp a.out && a.out");
		break;
	case 2:
		system("g++ -o trafficLight.cpp a.out && a.out");
		break;
	default:
		std::cout << "bro are we deadass\n";
		break;
	}

	return 0;
}