#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include "Board.h"
#include "Player.h"

struct Event {
    std::string description;
    int pathType;  
    int advisor;  
    int dpChange;
};

struct Riddle {
    std::string question;
    std::string answer;
};

class Game {
    private:
        Board _board;
        Player _players[2];
        Event* _events; 
        int _event_count;
        Riddle* _riddles;
        int _riddle_count;
        std::vector<std::string> _charactersFileLines;

        void loadCharacters(std::string filename);
        void loadEvents(std::string filename);
        void loadRiddles(std::string filename);

        void showCharactersToChoose();
        void pickCharacters();
        void choosePathsAndAdvisors();
        void mainGameLoop();

        void displayMainMenu(int playerIndex);
        void handleMenuChoice(int playerIndex, int choice);

        void doMove(int playerIndex);
        void handleTileEvent(int playerIndex, char tileColor);

        void triggerRegularTile(int playerIndex);
        void triggerBlueTile(int playerIndex);
        void triggerPinkTile(int playerIndex);
        void triggerRedTile(int playerIndex);
        void triggerBrownTile(int playerIndex);
        void triggerPurpleTile(int playerIndex);

        double strandSimilarity(std::string s1, std::string s2);
        int bestStrandMatch(std::string input_strand, std::string target_strand);
        void identifyMutations(std::string input_strand, std::string target_strand);
        void transcribeDNAtoRNA(std::string strand);

        int spinSpinner();
        Event pickRandomEventForPlayer(int playerIndex);

        void finalizeAndWriteStats();
    public:
        Game();
        void start(std::string charactersFile, std::string eventsFile, std::string riddlesFile);
};

#endif
