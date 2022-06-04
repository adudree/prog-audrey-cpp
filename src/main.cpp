#include <p6/p6.h>
#include <array>
#include <iostream>
#include <string>
#include "getFromPlayer.hpp"
#include "playGuessTheNumber.hpp"
#include "playHangman.hpp"
#include "rand.hpp"

void displayMenu()
{
    std::cout << "\nWhat game do you wanna play?" << std::endl;
    std::cout << "1 - Guess The Number \n2 - Hangman \nq - Quit";
}

struct cell {
    int posX;
    int posY;
};

// ==================== DISPLAY ======================== //

float cellSize(int totalSize)
{
    return 1.f / totalSize;
}

// map absolute coordinates in grid to relative coord.
glm::vec2 coordAbsToRel(cell myCell, int totalSize)
{
    glm::vec2 idCell = glm::vec2{static_cast<float>(myCell.posX), static_cast<float>(myCell.posY)};
    return p6::map(idCell,
                   glm::vec2{0.f}, glm::vec2{static_cast<float>(totalSize)},
                   glm::vec2{-1.f}, glm::vec2{1.f});
}

// map relative coord. to absolute position in grid
glm::vec2 coordRelToAbs(glm::vec2 position, int totalSize)
{
    return p6::map(position, glm::vec2{-1.f}, glm::vec2{1.f}, glm::vec2{0.f}, glm::vec2{static_cast<float>(totalSize)});
}

void drawOneCell(p6::Context& ctx, cell myCell, int totalSize)
{
    ctx.square(p6::BottomLeftCorner{
        coordAbsToRel(myCell, totalSize)});
}

cell setCell(int x, int y)
{
    cell myCell;
    myCell.posX = static_cast<float>(x);
    myCell.posY = static_cast<float>(y);
    return myCell;
}

void drawBoard(int size, p6::Context& ctx)
{
    ctx.fill   = {0.4f, 0.2f, 0.8f};
    ctx.stroke = {1, 1, 1};

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cell newCell = setCell(i, j);
            drawOneCell(ctx, newCell, size);
        }
    }
}

// décalage pour affichage des ronds
glm::vec2 bottomLeftToCenter(glm::vec2 posBottomLeft, int totalSize)
{
    return posBottomLeft + cellSize(totalSize);
}

void drawNought(p6::Context& ctx, glm::vec2 position, int totalSize)
{
    ctx.fill = {0.8f, 0.2f, 0.3f}; // pink

    ctx.circle(p6::Center{bottomLeftToCenter(position, totalSize)}, p6::Radius{cellSize(totalSize)});

    ctx.fill = {0.4f, 0.2f, 0.8f}; // purple

    ctx.circle(p6::Center{bottomLeftToCenter(position, totalSize)}, p6::Radius{cellSize(totalSize) * 2 / 3});
}

void drawCross(p6::Context& ctx, glm::vec2 position, int totalSize)
{
    // je voulais changer le code pour dessiner une autre forme
    // si tu lis ces lignes c'est que j'ai pas eu le temps

    ctx.fill = {0.2f, 0.8f, 0.3f}; // green

    ctx.circle(p6::Center{bottomLeftToCenter(position, totalSize)}, p6::Radius{cellSize(totalSize)});
}

void addNoughtOrCross(p6::Context& ctx, cell selectedCell, int totalSize, int currentPlayer)
{
    glm::vec2 relPosition = coordAbsToRel(selectedCell, totalSize);

    if (currentPlayer == 1)
        drawNought(ctx, relPosition, totalSize);
    else
        drawCross(ctx, relPosition, totalSize);
}

class Board {
    int _size;
    int _values[3][3];

public:
    Board(int totalSize = 0)
    {
        _size = totalSize;
        for (int i = 0; i < _size; i++) {
            for (int j = 0; j < _size; j++) {
                _values[i][j] = 0;
            }
        }
    }

    int readValue(cell index)
    {
        return _values[index.posX][index.posY];
    }

    void addValue(cell index, int player)
    {
        if (_values[index.posX][index.posY] == 0) {
            _values[index.posX][index.posY] = player;
        }
        if (testWinner() != 0) {
            endGame(player);
        }
    }
    void endGame(int winner)
    {
        for (int i = 0; i < _size; i++) {
            for (int j = 0; j < _size; j++) {
                _values[i][j] = -1;
            }
        }
        std::cout << "Player " << winner << " won!" << std::endl;
    }

    int testWinner()
    {
        for (int i = 0; i < _size; i++) {
            // test colonnes
            if (_values[i][0] != 0 &&
                _values[i][0] == _values[i][1] &&
                _values[i][0] == _values[i][2]) {
                return _values[i][0];
            }

            // test lignes
            else if (_values[0][i] != 0 &&
                     _values[0][i] == _values[1][i] &&
                     _values[0][i] == _values[2][i]) {
                return _values[0][i];
            }
        }

        // diagonales
        if (_values[0][0] != 0 &&
            _values[0][0] == _values[1][1] &&
            _values[0][0] == _values[2][2]) {
            std::cout << "DIAGONALE 1\n";
            return _values[0][0];
        }

        else if (_values[2][0] != 0 &&
                 _values[2][0] == _values[1][1] &&
                 _values[2][0] == _values[0][2]) {
            std::cout << "DIAGONALE 2\n";
            return _values[2][0];
        }

        else
            return 0;
    }
};

// ==================== CELLS ======================== //

cell getActiveCell(glm::vec2 position, int totalSize)
{
    // map mouse position to absolute position
    glm::vec2 idxPosition = coordRelToAbs(position, totalSize);

    cell activeCell;
    activeCell.posX = static_cast<int>(idxPosition.x);
    activeCell.posY = static_cast<int>(idxPosition.y);
    return activeCell;
}

// ==================== PLAYER ======================== //

int switchPlayer(int currentPlayer)
{
    if (currentPlayer == 1)
        return 2;
    else
        return 1;
}

void playNoughtCrosses()
{
    auto ctx = p6::Context{{800, 800, "Noughts & Crosses"}};

    int player = 1;
    int size   = 3;

    Board board(size);

    drawBoard(size, ctx);

    ctx.mouse_pressed = [&](p6::MouseButton button) {
        switch (button.button) {
        case p6::Button::Left:

            if (board.readValue(getActiveCell(ctx.mouse(), size)) == 0) {
                addNoughtOrCross(ctx, getActiveCell(ctx.mouse(), size), size, player);
                board.addValue(getActiveCell(ctx.mouse(), size), player);
                player = switchPlayer(player);
            }
            break;
        default: break;
        }
        return 0;
    };

    ctx.update = [&]() {
    };

    ctx.start();
}

int main()
{
    bool quit = false;

    while (!quit) {
        displayMenu();

        switch (getFromPlayer<char>()) {
        case '1':
            playGuessTheNumber();
            break;
        case '2':
            playHangman();
            break;
        case '3':
            playNoughtCrosses();
            break;
        case 'q':
            std::cout << "Goodbye!\n";
            quit = true;
            break;
        default: break;
        }
    }
}