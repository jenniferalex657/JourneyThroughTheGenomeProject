#ifndef BOARD_H
#define BOARD_H

#include "Tile.h"

class Board {
    private:
        static const int _BOARD_SIZE = 52;
        static const int _MAX_PLAYERS = 2;

        Tile _tiles[2][_BOARD_SIZE];

        int _player_count;
        int _player_position[_MAX_PLAYERS];

        void initializeTiles(int player_index);
        bool isPlayerOnTile(int player_index, int pos);
        void displayTile(int player_index, int pos);

    public:
        Board();

        void initializeBoard();
        void displayTrack(int player_index);
        void displayBoard();
        bool movePlayer(int player_index, int steps); 
        int getPlayerPosition(int player_index) const;
        char getTileColor(int lane, int pos) const;
};

#endif
