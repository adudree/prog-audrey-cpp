
#include "playHangman.hpp"
#include "rand.hpp"

std::string pickRandomWord() {
  std::vector<std::string> words{"prog", "dev", "algo", "code"};
  return words[rand(0, words.size() - 1)];
}

char getCharFromPlayer() {
  char charPlayer;

  std::cout << "\nEnter a char: ";

  while (!(std::cin >> charPlayer)) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max());
    std::cout << "Invalid input; please re-enter.\n";
  }

  return charPlayer;
}

bool isCharInString(char &myChar, std::string &myString) {
  for (size_t i = 0; i < myString.size(); i++) {
    if (myChar == myString[i]) {
      return true;
    }
  }
  return false;
}

char pickOneChar(std::string &pickedChars, char myChar) {

  std::cout << "selected chars : " << pickedChars << std::endl;

  if (!isCharInString(myChar, pickedChars)) {
    std::cout << "You chose " << myChar << std::endl;
    pickedChars.push_back(myChar);
    return myChar;

  } else {
    std::cout << "\nYou already proposed " << myChar << "! Pick another one\n";
    return pickOneChar(pickedChars, getCharFromPlayer());
  }

  return -1;
}

std::string createHiddenWord(std::string &word) {
  std::string hidden;

  for (size_t i = 0; i < word.size(); i++) {
    hidden.push_back('_');
    hidden.push_back(' ');
  }
  return hidden;
}

bool isCharInWord(char &myChar, std::string &word, int &position) {
  for (size_t i = 0; i < word.size(); i++) {
    if (word[i] == myChar) {
      position = i + 1;
      return true;
    }
  }
  std::cout << "\nOh no! " << myChar << " isn't in the word...\n\n";
  return false;
}

void replaceInWord(std::string &hidden, std::string &toReplaceWith,
                   int &position) {
  int posInWordHidden = position * 2 - 1;
  hidden.replace(posInWordHidden - 1, 1, toReplaceWith);
}

bool isWordFound(size_t letterCount, std::string &wordToGuess) {
  if (letterCount == wordToGuess.size()) {
    return true;
  }
  return false;
}

void endGame(int &life) {
  if (life == 0) {
    std::cout << "\nOh no, rip :(\n";
  } else {
    std::cout << "\nGood job, you found the word!\n";
  }
}

void playHangman() {
  std::cout << "========= HANGMAN =========\n";

  std::string wordToGuess = pickRandomWord();
  std::string wordHidden = createHiddenWord(wordToGuess);

  std::string pickedChars = ""; // all letters propositions
  size_t letterCount = 0;       // number of "good" letters found
  int life = 8;                 // player's life

  std::cout << wordHidden << std::endl;

  while (!isWordFound(letterCount, wordToGuess) && life != 0) {
    std::string playerChar; // char chosen by the player
                            // i need the replace() function later
                            // and it doesn't work with a char :()
    int posCharInWord;

    std::cout << "\nYou have " << life << " lives left.\n";

    playerChar = pickOneChar(pickedChars, getCharFromPlayer());

    if (isCharInWord(playerChar[0], wordToGuess, posCharInWord)) {
      replaceInWord(wordHidden, playerChar, posCharInWord);
      letterCount++;
    } else {
      life--;
    }
    std::cout << wordHidden << std::endl;
  }

  endGame(life);
}