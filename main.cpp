#include <iostream>


#include "cards.cpp"

int main() {
    std::string filename;
    std::cin >> filename;
    std::cout << (PatienceSolver::solution(filename) ? "Solvable" : "Non-Solvable") << std::endl;
    return 0;
}
