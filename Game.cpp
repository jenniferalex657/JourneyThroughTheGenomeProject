#include "Game.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

Game::Game() {
    srand((unsigned int)time(NULL));
    _events = NULL;
    _event_count = 0;
    _riddles = NULL;
    _riddle_count = 0;
}

void Game::start(string charactersFile, string eventsFile, string riddlesFile) {
    loadCharacters(charactersFile);
    loadEvents(eventsFile);
    loadRiddles(riddlesFile);
    showCharactersToChoose();
    pickCharacters();
    choosePathsAndAdvisors();
    mainGameLoop();
    finalizeAndWriteStats();
}


void Game::loadCharacters(string filename) {
    _charactersFileLines.clear();
    ifstream fin(filename.c_str());
    if (!fin.is_open()) {
        cout << "Failed to open " << filename << endl;
        return;
    }
    string line;
    getline(fin, line);
    while (getline(fin, line)) {
        if (line.size() > 0) _charactersFileLines.push_back(line);
    }
    fin.close();
}

void Game::loadEvents(string filename) {
    vector<Event> evs;
    ifstream fin(filename.c_str());
    if (!fin.is_open()) {
        cout << "Failed to open " << filename << endl;
        _event_count = 0;
        return;
    }
    string line;
    while (getline(fin, line)) {
        if (line.size() == 0) continue;
        if (line.size() >= 2 && line[0] == '/' && line[1] == '/') continue;
        vector<string> tokens;
        string temp;
        stringstream ss(line);
        while (getline(ss, temp, '|')) {
            while (temp.size()>0 && (temp[0] == ' ' || temp[0] == '\t')) temp.erase(0,1);
            while (temp.size()>0 && (temp[temp.size()-1] == ' ' || temp[temp.size()-1] == '\t')) temp.erase(temp.size()-1,1);
            tokens.push_back(temp);
        }
        if (tokens.size() >= 4) {
            Event e;
            e.description = tokens[0];
            e.pathType = atoi(tokens[1].c_str());
            e.advisor = atoi(tokens[2].c_str());
            e.dpChange = atoi(tokens[3].c_str());
            evs.push_back(e);
        }
    }
    fin.close();

    _event_count = (int)evs.size();
    if (_event_count > 0) {
        _events = new Event[_event_count];
        for (int i = 0; i < _event_count; i++) {
            _events[i] = evs[i];
        }
    } else {
        _events = NULL;
    }
}

void Game::loadRiddles(string filename) {
    vector<Riddle> rds;
    ifstream fin(filename.c_str());
    if (!fin.is_open()) {
        cout << "Failed to open " << filename << endl;
        _riddle_count = 0;
        return;
    }
    string line;
    getline(fin, line);
    if (line.find("Question") == string::npos) {
        fin.clear();
        fin.seekg(0);
    }
    while (getline(fin, line)) {
        if (line.size() == 0) continue;
        size_t sep = line.find('|');
        if (sep == string::npos) continue;
        Riddle r;
        r.question = line.substr(0, sep);
        r.answer = line.substr(sep + 1);
        while (r.question.size()>0 && (r.question[0]==' '||r.question[0]=='\t')) r.question.erase(0,1);
        while (r.question.size()>0 && (r.question[r.question.size()-1]==' '||r.question[r.question.size()-1]=='\t')) r.question.erase(r.question.size()-1,1);
        while (r.answer.size()>0 && (r.answer[0]==' '||r.answer[0]=='\t')) r.answer.erase(0,1);
        while (r.answer.size()>0 && (r.answer[r.answer.size()-1]==' '||r.answer[r.answer.size()-1]=='\t')) r.answer.erase(r.answer.size()-1,1);
        rds.push_back(r);
    }
    fin.close();

    _riddle_count = (int)rds.size();
    if (_riddle_count > 0) {
        _riddles = new Riddle[_riddle_count];
        for (int i = 0; i < _riddle_count; i++) _riddles[i] = rds[i];
    } else {
        _riddles = NULL;
    }
}


void Game::showCharactersToChoose() {
    cout << "Available characters (from characters.txt):\n";
    for (int i = 0; i < (int)_charactersFileLines.size(); i++) {
        string line = _charactersFileLines[i];
        vector<string> parts;
        string token;
        stringstream ss(line);
        while (getline(ss, token, '|')) parts.push_back(token);
        if ((int)parts.size() >= 6) {
            cout << (i+1) << ") " << parts[0] << " | Exp: " << parts[1]
                 << " | Acc: " << parts[2] << " | Eff: " << parts[3]
                 << " | Ins: " << parts[4] << " | DP: " << parts[5] << "\n";
        }
    }
}

void Game::pickCharacters() {
    int choice1 = 0;
    while (true) {
        cout << "Player 1, enter your name: ";
        string pname;
        getline(cin, pname);
        if (pname.size() > 0) {
            cout << "Player 1, choose a scientist by number from the list: ";
            string sel;
            getline(cin, sel);
            choice1 = atoi(sel.c_str());
            if (choice1 >= 1 && choice1 <= (int)_charactersFileLines.size()) {
                string line = _charactersFileLines[choice1-1];
                vector<string> parts;
                string token;
                stringstream ss(line);
                while (getline(ss, token, '|')) parts.push_back(token);
                if ((int)parts.size() >= 6) {
                    string charName = parts[0];
                    int exp = atoi(parts[1].c_str());
                    int acc = atoi(parts[2].c_str());
                    int eff = atoi(parts[3].c_str());
                    int ins = atoi(parts[4].c_str());
                    int dp = atoi(parts[5].c_str());

                    _players[0] = Player(pname, exp, acc, eff, ins, dp);
                    break;
                }
            }
        }
        cout << "Invalid selection. Try again.\n";
    }

    int choice2 = 0;
    while (true) {
        cout << "Player 2, enter your name: ";
        string pname;
        getline(cin, pname);
        if (pname.size() > 0) {
            cout << "Player 2, choose a scientist by number from the list (cannot pick same): ";
            string sel;
            getline(cin, sel);
            choice2 = atoi(sel.c_str());
            if (choice2 >= 1 && choice2 <= (int)_charactersFileLines.size() && choice2 != choice1) {
                string line = _charactersFileLines[choice2-1];
                vector<string> parts;
                string token;
                stringstream ss(line);
                while (getline(ss, token, '|')) parts.push_back(token);
                if ((int)parts.size() >= 6) {
                    string charName = parts[0];
                    int exp = atoi(parts[1].c_str());
                    int acc = atoi(parts[2].c_str());
                    int eff = atoi(parts[3].c_str());
                    int ins = atoi(parts[4].c_str());
                    int dp = atoi(parts[5].c_str());
                    _players[1] = Player(pname, exp, acc, eff, ins, dp);
                    break;
                }
            }
        }
        cout << "Invalid selection. Try again.\n";
    }

}

void Game::choosePathsAndAdvisors() {
    for (int i = 0; i < 2; i++) {
        cout << "\n" << _players[i].getName() << ", choose your path:\n";
        cout << "0) Training Fellowship (-5000 DP, +500 Acc, +500 Eff, +1000 Ins, includes advisor choice)\n";
        cout << "1) Direct Lab Assignment (+5000 DP, +200 Acc, +200 Eff, +200 Ins, no advisor initially)\n";
        cout << "Enter 0 or 1: ";
        string in;
        getline(cin, in);
        int path = atoi(in.c_str());
        if (path != 0 && path != 1) path = 1;
        _players[i].setPathType(path);
        if (path == 0) {
            _players[i].addDiscoveryPoints(-5000);
            _players[i].addAccuracy(500);
            _players[i].addEfficiency(500);
            _players[i].addInsight(1000);

            cout << "Choose an advisor for " << _players[i].getName() << " (advisor protects against negative events):\n";
            cout << "1) Dr. Aliquot (avoid contamination)\n";
            cout << "2) Dr. Assembler (improve efficiency)\n";
            cout << "3) Dr. Pop-Gen (identify rare variants)\n";
            cout << "4) Dr. Bio-Script (debugging/data)\n";
            cout << "5) Dr. Loci (equipment help)\n";
            cout << "Enter 1-5: ";
            string a;
            getline(cin, a);
            int aid = atoi(a.c_str());
            if (aid < 1 || aid > 5) aid = 1;
            _players[i].setAdvisor(aid);
        } else {
            _players[i].addDiscoveryPoints(5000);
            _players[i].addAccuracy(200);
            _players[i].addEfficiency(200);
            _players[i].addInsight(200);
            _players[i].setAdvisor(0);
        }
        _players[i].enforceMinimumTraits();
    }
}


void Game::mainGameLoop() {
    bool finished[2] = {false, false};
    int currentPlayer = 0;
    cout << "\nGame start! Board layout:\n";
    _board.displayBoard();

    while (!(finished[0] && finished[1])) {
        cout << "\n--- " << _players[currentPlayer].getName() << "'s Turn ---\n";
        _board.displayBoard();
        displayMainMenu(currentPlayer);

        if (_board.getPlayerPosition(currentPlayer) >= 51) {
            finished[currentPlayer] = true;
            currentPlayer = 1 - currentPlayer;
            continue;
        }

        bool didMoveThisTurn = false;
        while (!didMoveThisTurn) {
            cout << "Enter menu choice number: ";
            string chs;
            getline(cin, chs);
            int ch = atoi(chs.c_str());
            handleMenuChoice(currentPlayer, ch);
            if (ch == 5) { 
                didMoveThisTurn = true;
            }
            if (_board.getPlayerPosition(currentPlayer) >= 51) {
                finished[currentPlayer] = true;
                didMoveThisTurn = true;
            }
        }

        currentPlayer = 1 - currentPlayer;
    }

    cout << "\nBoth players reached the Genome Conference (finish). Calculating final scores...\n";
}


void Game::displayMainMenu(int playerIndex) {
    cout << "Main Menu Options:\n";
    cout << "1) Check Player Progress\n";
    cout << "2) Review Character\n";
    cout << "3) Check Position (Display Board)\n";
    cout << "4) Review your Advisor\n";
    cout << "5) Move Forward (Spin)\n";
    cout << "(Choose the number for option; some options will request suboptions)\n";
}

void Game::handleMenuChoice(int playerIndex, int choice) {
    if (choice == 1) {
        cout << "1) Review Discover Points or 2) Review Trait stats. Enter 1 or 2: ";
        string s; getline(cin, s);
        int sub = atoi(s.c_str());
        if (sub == 1) {
            cout << _players[playerIndex].getName() << " DP: " << _players[playerIndex].getDiscoveryPoints() << "\n";
        } else {
            cout << _players[playerIndex].toString() << "\n";
        }
    } else if (choice == 2) {
        cout << "Character info:\n";
        cout << _players[playerIndex].toString() << "\n";
    } else if (choice == 3) {
        cout << "Board:\n";
        _board.displayBoard();
        cout << "You are on tile: " << _board.getPlayerPosition(playerIndex) << "\n";
    } else if (choice == 4) {
        cout << "Advisor: " << _players[playerIndex].advisorName() << "\n";
        cout << "1) Display abilities or 2) Use ability for current challenge (if available). Enter 1 or 2: ";
        string s; getline(cin, s);
        int sub = atoi(s.c_str());
        if (sub == 1) {
            cout << "Advisor abilities:\n";
            cout << "1) Dr. Aliquot – avoids contamination events.\n";
            cout << "2) Dr. Assembler – assists in efficiency; may block some negative events.\n";
            cout << "3) Dr. Pop-Gen – insight for identifying rare variants.\n";
            cout << "4) Dr. Bio-Script – helps debug code; blocks code-related penalties.\n";
            cout << "5) Dr. Loci – helps with equipment issues.\n";
        } else {
            _players[playerIndex].enableAdvisorProtection();
            cout << "Your advisor protection is active for the next negative event.\n";
        }
    } else if (choice == 5) {
        doMove(playerIndex);
    } else {
        cout << "Invalid menu choice.\n";
    }
}


void Game::doMove(int playerIndex) {
    int steps = spinSpinner();
    cout << _players[playerIndex].getName() << " spun a " << steps << " and moves forward " << steps << " tiles.\n";
    bool finished = _board.movePlayer(playerIndex, steps);
    int newPos = _board.getPlayerPosition(playerIndex);
    cout << "Now on tile " << newPos << ".\n";
    int lane = _players[playerIndex].getPathType(); 
    char tileColor = _board.getTileColor(lane, newPos);
    handleTileEvent(playerIndex, tileColor);
    if (finished) {
        cout << _players[playerIndex].getName() << " reached the Genome Conference!\n";
    }
}

void Game::handleTileEvent(int playerIndex, char tileColor) {
    if (tileColor == 'G') {
        int chance = rand() % 100;
        if (chance < 50) {
            triggerRegularTile(playerIndex);
        } else {
            cout << "Nothing happens on this green tile.\n";
        }
    } 
    else if (tileColor == 'B') triggerBlueTile(playerIndex);
    else if (tileColor == 'P') triggerPinkTile(playerIndex);
    else if (tileColor == 'R') triggerRedTile(playerIndex);
    else if (tileColor == 'T') triggerBrownTile(playerIndex);
    else if (tileColor == 'U') triggerPurpleTile(playerIndex);
    else if (tileColor == 'O') {
        cout << "Genome Conference tile reached!\n";
    } else {
        cout << "Unrecognized tile. Nothing happens.\n";
    }
}


void Game::triggerRegularTile(int playerIndex) {
    cout << "Regular tile event triggered! Picking a random event relevant to your path...\n";
    Event e = pickRandomEventForPlayer(playerIndex);
    cout << "Event: " << e.description << " (DP change: " << e.dpChange << ")\n";
    if (e.dpChange < 0) {
        if (_players[playerIndex].getAdvisor() == e.advisor && e.advisor != 0) {
            cout << "Your advisor protects you from this negative event. No DP lost.\n";
            return;
        }
        if (_players[playerIndex].hasAdvisorProtection()) {
            cout << "Your active advisor protection avoids this negative event. No DP lost.\n";
            _players[playerIndex].disableAdvisorProtection();
            return;
        }
    }
    _players[playerIndex].addDiscoveryPoints(e.dpChange);
    cout << "Your Discovery Points changed by " << e.dpChange << ". New DP: " << _players[playerIndex].getDiscoveryPoints() << "\n";
}

void Game::triggerBlueTile(int playerIndex) {
    cout << "Blue Tile: DNA Task 1 - Similarity (Equal-Length)\n";
    cout << "Enter first DNA strand (uppercase A/C/G/T), equal length: ";
    string s1; getline(cin, s1);
    cout << "Enter second DNA strand (same length): ";
    string s2; getline(cin, s2);
    if (s1.size() != s2.size()) {
        cout << "Strands are not equal length. Task failed.\n";
        return;
    }
    double score = strandSimilarity(s1, s2);
    cout << "Similarity score: " << score << "\n";
    int bonus = (int)(score * 1000.0); 
    _players[playerIndex].addDiscoveryPoints(bonus);
    cout << "You earned " << bonus << " DP. Total DP: " << _players[playerIndex].getDiscoveryPoints() << "\n";
}

void Game::triggerPinkTile(int playerIndex) {
    cout << "Pink Tile: DNA Task 2 - Similarity (Unequal-Length)\n";
    cout << "Enter input strand (longer or shorter): ";
    string input; getline(cin, input);
    cout << "Enter target strand: ";
    string target; getline(cin, target);
    int idx = bestStrandMatch(input, target);
    if (idx < 0) {
        cout << "Invalid strands or no match possible.\n";
        return;
    }
    cout << "Best alignment starting index in input strand: " << idx << "\n";
    int matches = 0;
    for (int i = 0; i < (int)target.size(); i++) {
        if (idx + i < (int)input.size() && input[idx + i] == target[i]) matches++;
    }
    double score = (double)matches / (double)target.size();
    int bonus = (int)(score * 1000.0);
    cout << "Best similarity score: " << score << ", earned " << bonus << " DP.\n";
    _players[playerIndex].addDiscoveryPoints(bonus);
}

void Game::triggerRedTile(int playerIndex) {
    cout << "Red Tile: DNA Task 3 - Mutation Identification\n";
    cout << "Enter input strand: ";
    string input; getline(cin, input);
    cout << "Enter target strand: ";
    string target; getline(cin, target);
    identifyMutations(input, target);
    _players[playerIndex].addDiscoveryPoints(300);
    cout << "You earned 300 DP for performing mutation identification.\n";
}

void Game::triggerBrownTile(int playerIndex) {
    cout << "Brown Tile: DNA Task 4 - Transcribe DNA to RNA\n";
    cout << "Enter DNA strand: ";
    string s; getline(cin, s);
    transcribeDNAtoRNA(s);
    _players[playerIndex].addDiscoveryPoints(200);
    cout << "You earned 200 DP for successful transcription.\n";
}

void Game::triggerPurpleTile(int playerIndex) {
    cout << "Purple Tile: Riddle! Answer correctly to earn Insight points.\n";
    if (_riddle_count == 0) {
        cout << "No riddles loaded.\n";
        return;
    }
    int idx = rand() % _riddle_count;
    cout << "Riddle: " << _riddles[idx].question << "\n";
    cout << "Enter your answer (exact as in riddles.txt required formatting): ";
    string ans; getline(cin, ans);
    string a1 = ans;
    string a2 = _riddles[idx].answer;
    for (int i = 0; i < (int)a1.size(); i++) a1[i] = (char)tolower(a1[i]);
    for (int i = 0; i < (int)a2.size(); i++) a2[i] = (char)tolower(a2[i]);
    if (a1 == a2) {
        cout << "Correct! +500 Insight Points.\n";
        _players[playerIndex].addInsight(500);
    } else {
        cout << "Incorrect. The correct answer was: " << _riddles[idx].answer << "\n";
    }
}


double Game::strandSimilarity(string s1, string s2) {
    if (s1.size() == 0 || s1.size() != s2.size()) return 0.0;
    int matches = 0;
    for (int i = 0; i < (int)s1.size(); i++) {
        if (s1[i] == s2[i]) matches++;
    }
    return (double)matches / (double)s1.size();
}

int Game::bestStrandMatch(string input_strand, string target_strand) {
    if (input_strand.size() == 0 || target_strand.size() == 0) return -1;
    if (target_strand.size() > input_strand.size()) {
        string tmp = input_strand;
        input_strand = target_strand;
        target_strand = tmp;
    }
    int bestMatches = -1;
    int bestIndex = -1;
    int maxStart = (int)input_strand.size() - (int)target_strand.size();
    for (int start = 0; start <= maxStart; start++) {
        int matches = 0;
        for (int j = 0; j < (int)target_strand.size(); j++) {
            if (input_strand[start + j] == target_strand[j]) matches++;
        }
        if (matches > bestMatches) {
            bestMatches = matches;
            bestIndex = start;
        }
    }
    return bestIndex;
}

void Game::identifyMutations(string input_strand, string target_strand) {
    cout << "Identifying mutations between input and target.\n";
    int start = bestStrandMatch(input_strand, target_strand);
    if (start < 0) {
        cout << "Unable to align sequences.\n";
        return;
    }
    cout << "Best alignment start index (input): " << start << "\n";
    int i = start;
    int j = 0;
    int inLen = (int)input_strand.size();
    int tarLen = (int)target_strand.size();

    while (i < inLen && j < tarLen) {
        if (input_strand[i] == target_strand[j]) {
            i++; j++;
        } else {
            if ( (i+1 < inLen) && (input_strand[i+1] == target_strand[j]) ) {
                cout << "Deletion at input pos " << i << ": '" << input_strand[i] << "' was deleted in target.\n";
                i++; 
            }
            else if ( (j+1 < tarLen) && (input_strand[i] == target_strand[j+1]) ) {
                cout << "Insertion at target pos " << j << ": '" << target_strand[j] << "' inserted in target.\n";
                j++;
            }
            else {
                cout << "Substitution at input pos " << i << ": '" << input_strand[i] << "' -> '" << target_strand[j] << "'\n";
                i++; j++;
            }
        }
    }
    while (i < inLen) {
        cout << "Deletion at input pos " << i << ": '" << input_strand[i] << "' missing in target.\n";
        i++;
    }
    while (j < tarLen) {
        cout << "Insertion at target pos " << j << ": '" << target_strand[j] << "' extra in target.\n";
        j++;
    }
}

void Game::transcribeDNAtoRNA(string strand) {
    string rna = "";
    for (int i = 0; i < (int)strand.size(); i++) {
        char c = strand[i];
        if (c == 'T') rna.push_back('U');
        else if (c == 't') rna.push_back('u');
        else rna.push_back(c);
    }
    cout << "RNA: " << rna << "\n";
}


int Game::spinSpinner() {
    return (rand() % 6) + 1;
}

Event Game::pickRandomEventForPlayer(int playerIndex) {
    Event fallback;
    fallback.description = "No event found";
    fallback.pathType = 0;
    fallback.advisor = 0;
    fallback.dpChange = 0;
    if (_event_count == 0) return fallback;

    vector<int> candidates;
    for (int i = 0; i < _event_count; i++) {
        if (_events[i].pathType == _players[playerIndex].getPathType()) {
            candidates.push_back(i);
        }
    }
    if (candidates.size() == 0) {
        int idx = rand() % _event_count;
        return _events[idx];
    }
    int pickIndex = candidates[rand() % (int)candidates.size()];
    return _events[pickIndex];
}


void Game::finalizeAndWriteStats() {
    for (int i = 0; i < 2; i++) {
        int addDP = 0;
        addDP += (_players[i].getAccuracy() / 100) * 1000;
        addDP += (_players[i].getEfficiency() / 100) * 1000;
        addDP += (_players[i].getInsight() / 100) * 1000;
        _players[i].addDiscoveryPoints(addDP);
        cout << _players[i].getName() << " received " << addDP << " DP from traits.\n";
    }

    cout << "Final Results:\n";
    for (int i = 0; i < 2; i++) {
        cout << _players[i].toString() << "\n";
    }
    int winner = 0;
    if (_players[1].getDiscoveryPoints() > _players[0].getDiscoveryPoints()) winner = 1;
    cout << "Winner: " << _players[winner].getName() << " with " << _players[winner].getDiscoveryPoints() << " DP.\n";

    ofstream fout("game_stats.txt");
    if (fout.is_open()) {
        fout << "Final Game Results\n";
        for (int i = 0; i < 2; i++) {
            fout << _players[i].toString() << "\n";
        }
        cout << "Winner: " << _players[winner].getName() << " | DP: " << _players[winner].getDiscoveryPoints() << "\n";
        fout.close();
        cout << "Game stats written to game_stats.txt\n";
    } else {
        cout << "Failed to write game_stats.txt\n";
    }
}
