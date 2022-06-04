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

int main()
{
    auto ctx = p6::Context{{800, 800, "Noughts & Crosses"}};

    drawBoard(3, ctx);

    ctx.mouse_pressed = [&](p6::MouseButton button) {
        switch (button.button) {
        case p6::Button::Left:
            std::cout << getActiveCell(ctx.mouse(), 3).posY << std::endl;
            break;
        case p6::Button::Right:
            break;
        case p6::Button::Middle: break;
        }
        return 0;
    };

    ctx.update = [&]() {
    };

    ctx.start();

    // bool quit = false;

    // while (!quit) {
    //     displayMenu();

    //     switch (getFromPlayer<char>()) {
    //     case '1':
    //         playGuessTheNumber();
    //         break;
    //     case '2':
    //         playHangman();
    //         break;
    //     case 'q':
    //         std::cout << "Goodbye!\n";
    //         quit = true;
    //         break;
    //     default: break;
    //     }
    // }
}