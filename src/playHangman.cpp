#include "playHangman.hpp"
#include "getFromPlayer.hpp"
#include "rand.hpp"

std::string pickRandomWord()
{
    std::vector<std::string> words{"prog", "dev", "algo", "code"};
    return words[rand(0, words.size() - 1)];
}

bool isCharInString(const char& myChar, const std::string& myString)
{
    return (myString.find(myChar) != std::string::npos);
}

char pickOneChar(std::string& pickedChars, const char& myChar)
{
    std::cout << "selected chars : " << pickedChars << std::endl;

    if (!isCharInString(myChar, pickedChars)) {
        std::cout << "You chose " << myChar << ".\n\n";
        pickedChars.push_back(myChar);
        return myChar;
    }
    else {
        std::cout << "\nYou already proposed " << myChar << "! Pick another one\n";
        return pickOneChar(pickedChars, getFromPlayer<char>());
    }

    return -1;
}

std::string createHiddenWord(const std::string& word)
{
    std::string hidden;

    for (size_t i = 0; i < word.size(); i++) {
        hidden.push_back('_');
    }
    return hidden;
}

void printHiddenWord(const std::string& word)
{
    for (size_t i = 0; i < word.size(); i++) {
        std::cout << word[i] << " ";
    }
    std::cout << std::endl;
}

void replaceInWord(std::string& hidden, const char& toReplaceWith,
                   const std::string& word)
{
    for (size_t i = 0; i < hidden.size(); i++) {
        if (toReplaceWith == word[i]) {
            hidden.replace(i, 1, 1, toReplaceWith);
        }
    }
}

bool isWordFound(std::string& hiddenWord, const std::string& wordToGuess)
{
    return (hiddenWord == wordToGuess);
}

void endGame(int& life)
{
    if (life == 0) {
        std::cout << "\nOh no, rip :(\n";
    }
    else {
        std::cout << "\nGood job, you found the word!\n";
    }
}

void playHangman()
{
    std::cout << "\n========= HANGMAN =========\n";

    const std::string wordToGuess = pickRandomWord();
    std::string       wordHidden  = createHiddenWord(wordToGuess);
    std::string       pickedChars = ""; // all letters propositions
    int               life        = 8;  // player's life

    printHiddenWord(wordHidden);

    while (!isWordFound(wordHidden, wordToGuess) && life != 0) {
        std::cout << "\nYou have " << life << " lives left.\n";

        const char playerChar = pickOneChar(pickedChars, getFromPlayer<char>());

        if (isCharInString(playerChar, wordToGuess)) {
            replaceInWord(wordHidden, playerChar, wordToGuess);
        }
        else {
            std::cout << playerChar << " isn't in the word.\n\n";
            life--;
        }
        printHiddenWord(wordHidden);
    }
    endGame(life);
}