#include "playNoughtAndCrosses.hpp"
#include <p6/p6.h>
#include "Board.hpp"

// ================== COORDINATES ================== //

cell setCell(int x, int y)
{
    cell myCell;
    myCell.posX = x;
    myCell.posY = y;
    return myCell;
}

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

// décalage pour affichage des ronds
glm::vec2 bottomLeftToCenter(glm::vec2 posBottomLeft, int totalSize)
{
    return posBottomLeft + cellSize(totalSize);
}

// ==================== DISPLAY ==================== //

void drawOneCell(p6::Context& ctx, cell myCell, int totalSize)
{
    ctx.square(p6::BottomLeftCorner{coordAbsToRel(myCell, totalSize)});
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

void drawNought(p6::Context& ctx, glm::vec2 position, int totalSize)
{
    ctx.fill = {0.8f, 0.2f, 0.3f}; // pink
    ctx.circle(p6::Center{bottomLeftToCenter(position, totalSize)}, p6::Radius{cellSize(totalSize)});

    ctx.fill = {0.4f, 0.2f, 0.8f}; // purple
    ctx.circle(p6::Center{bottomLeftToCenter(position, totalSize)}, p6::Radius{cellSize(totalSize) * 2 / 3});
}

void drawCross(p6::Context& ctx, glm::vec2 position, int totalSize)
{
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

// ==================== CELLS ======================== //

cell getActiveCell(glm::vec2 position, int totalSize)
{
    // map mouse position to absolute position
    glm::vec2 idxPosition = coordRelToAbs(position, totalSize);
    return setCell(idxPosition.x, idxPosition.y);
}

// ==================== PLAYER ======================== //

int switchPlayer(int currentPlayer)
{
    if (currentPlayer == 1)
        return 2;
    else
        return 1;
}

void playNoughtAndCrosses()
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