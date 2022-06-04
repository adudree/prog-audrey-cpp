#include <p6/p6.h>
#include "playNoughtAndCrosses.hpp"

struct cell {
    int posX;
    int posY;
};

// used for check what cells belong to each player
class Board {
    int _size;
    int _values[3][3];

public:
    Board(int totalSize = 0)
    {
        _size = totalSize;
        initTab(0);
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

private:
    void initTab(int newValue)
    {
        for (int i = 0; i < _size; i++) {
            for (int j = 0; j < _size; j++) {
                _values[i][j] = newValue;
            }
        }
    }

    int testWinner()
    {
        // diagonales
        if (_values[0][0] != 0 &&
            _values[0][0] == _values[1][1] &&
            _values[0][0] == _values[2][2]) {
            return _values[0][0];
        }

        else if (_values[2][0] != 0 &&
                 _values[2][0] == _values[1][1] &&
                 _values[2][0] == _values[0][2]) {
            return _values[2][0];
        }

        // lignes & colonnes
        else {
            for (int i = 0; i < _size; i++) {
                // colonnes
                if (_values[i][0] != 0 &&
                    _values[i][0] == _values[i][1] &&
                    _values[i][0] == _values[i][2]) {
                    return _values[i][0];
                }

                // lignes
                else if (_values[0][i] != 0 &&
                         _values[0][i] == _values[1][i] &&
                         _values[0][i] == _values[2][i]) {
                    return _values[0][i];
                }
            }
        }
        return 0;
    }

    void endGame(int winner)
    {
        initTab(-1);
        std::cout << "Player " << winner << " won!" << std::endl;
    }
};
