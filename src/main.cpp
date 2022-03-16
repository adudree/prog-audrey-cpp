#include <array>
#include <iostream>
#include <string>
#include "getInputFromUser.hpp"
#include "playGuessTheNumber.hpp"
#include "playHangman.hpp"
#include "rand.hpp"

void displayMenu()
{
    std::cout << "\nWhat game do you wanna play?" << std::endl;
    std::cout << "1 - Guess The Number \n2 - Hangman \nq - Quit";
}

void mainMenu()
{
}

int main()
{
    bool quit = false;

    while (!quit) {
        displayMenu();
        char input = getInputFromPlayer<char>();

        switch (input) {
        case '1':
            playGuessTheNumber();
            break;
        case '2':
            playHangman();
            break;
        case 'q':
            std::cout << "Goodbye!\n";
            quit = true;
            break;
        default: break;
        }
    }
}