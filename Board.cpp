#include "Board.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

#define ORANGE "\033[48;2;230;115;0m"
#define GREY "\033[48;2;128;128;128m"
#define GREEN "\033[48;2;34;139;34m"
#define BLUE "\033[48;2;10;10;230m"
#define PINK "\033[48;2;255;105;180m"
#define BROWN "\033[48;2;139;69;19m"
#define RED "\033[48;2;230;10;10m"
#define PURPLE "\033[48;2;128;0;128m"
#define RESET "\033[0m"

using namespace std;

Board::Board() {
    _player_count = _MAX_PLAYERS;
    for (int i = 0; i < _player_count; i++) {
        _player_position[i] = 0;
    }
    initializeBoard();
}

void Board::initializeTiles(int player_index) {
    Tile tile;
    int green_count = 0;
    int total_tiles = _BOARD_SIZE;

    for (int i = 0; i < total_tiles; i++) {
        if (i == total_tiles - 1) {
            tile.color = 'O';
        } 
        else if (i == 0) {
            tile.color = 'Y';
        } 
        else if (green_count < 30 && (rand() % (total_tiles - i) < 30 - green_count)) {
            tile.color = 'G';
            green_count++;
        }
        else {
            int color_choice = rand() % 5;
            switch (color_choice) {
                case 0:
                    tile.color = 'B';
                    break;
                case 1:
                    tile.color = 'P';
                    break;
                case 2:
                    tile.color = 'T';
                    break;
                case 3:
                    tile.color = 'R';
                    break;
                case 4:
                    tile.color = 'U';
                    break;
            }
        }
        _tiles[player_index][i] = tile;
    }
}

bool Board::isPlayerOnTile(int player_index, int pos) {
    if (player_index >= 0 && player_index < _player_count && _player_position[player_index] == pos) {
        return true;
    }
    return false;
}

void Board::displayTile(int player_index, int pos) {
    string color = "";
    bool player = isPlayerOnTile(player_index, pos);

    switch(_tiles[player_index][pos].color) {
        case 'O': color = ORANGE; break;
        case 'Y': color = GREY; break;
        case 'G': color = GREEN; break;
        case 'B': color = BLUE; break;
        case 'P': color = PINK; break;
        case 'T': color = BROWN; break;
        case 'R': color = RED; break;
        case 'U': color = PURPLE; break;
        default: color = GREEN; break;
    }

    if (player == true) {
        cout << color << "|" << (player_index + 1) << "|" << RESET;
    }
    else {
        cout << color << "| |" << RESET;
    }
}

void Board::initializeBoard() {
    for (int i = 0; i < 2; i++) {
        initializeTiles(i);
    }
}

void Board::displayTrack(int player_index) {
    for (int i = 0; i < _BOARD_SIZE; i++) {
        displayTile(player_index, i);
    }
    cout << endl;
}

void Board::displayBoard() {
    for (int i = 0; i < 2; i++) {
        displayTrack(i);
        if (i == 0) {
            cout << endl;
        }
    }
}

bool Board::movePlayer(int player_index, int steps) {
    if (player_index < 0 || player_index >= _player_count) return false;
    _player_position[player_index] += steps;
    if (_player_position[player_index] >= _BOARD_SIZE - 1) {
        _player_position[player_index] = _BOARD_SIZE - 1;
        return true;
    }
    return false;
}

int Board::getPlayerPosition(int player_index) const {
    if (player_index >= 0 && player_index < _player_count) {
        return _player_position[player_index];
    }
    return -1;
}

char Board::getTileColor(int lane, int pos) const {
    if (lane < 0 || lane >= 2) return 'G';
    if (pos < 0 || pos >= _BOARD_SIZE) return 'G';
    return _tiles[lane][pos].color;
}
