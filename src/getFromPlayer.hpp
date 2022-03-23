#include <iostream>
#include <random>
#include <string>

template<typename T>
T getFromPlayer()
{
    T input;

    std::cout << "\nEnter something: ";

    while (!(std::cin >> input)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max());
        std::cout << "Invalid input; please re-enter.\n";
    }
    return input;
}
