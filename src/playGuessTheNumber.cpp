
#include "playGuessTheNumber.hpp"
#include "getFromPlayer.hpp"
#include "rand.hpp"

void playGuessTheNumber()
{
    std::cout << "========= GUESS THE NUMBER =========\n";

    int min = 0;
    int max = 100;

    int numberToGuess = rand(min, max);
    int numberPlayer;

    bool isNumberFound = false;

    std::cout << "I chose a number between " << min << " and " << max << ".\n";

    while (!isNumberFound) {
        numberPlayer = getFromPlayer<int>();

        // compare values & end game if it has to be
        if (numberPlayer == numberToGuess) {
            std::cout << "Good job, you won!\n";
            isNumberFound = true;
        }
        else if (numberPlayer < numberToGuess) {
            std::cout << "Nope, it's greater.\n";
        }
        else {
            std::cout << "Nope, it's smaller.\n";
        }
    }
}
