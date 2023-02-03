// main.cpp

#include <iostream>
#include "Solution.hpp"

// all point set use the vector to be a container 

using namespace std;
int main() {
	try {
		Tools::solutionTools.readFile("test_in.txt");
		Tools::solutionTools.runSolution("test_out.txt");
	}
	catch (const std::exception& e) {
		cerr << e.what() << '\n';
		return -1;
	}

	return 0;
}