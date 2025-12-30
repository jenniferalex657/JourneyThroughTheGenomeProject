
#include "Game.h"
#include <iostream>

int main() {
    Game game;
    std::string charactersFile = "characters.txt";
    std::string eventsFile = "random_events.txt";
    std::string riddlesFile = "riddles.txt";

    game.start(charactersFile, eventsFile, riddlesFile);

    return 0;
}
