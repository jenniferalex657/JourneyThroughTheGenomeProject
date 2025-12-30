**CSCI 1300 Final Project – Journey Through the Genome**\

**Overview**\
 \
Journey Through the Genome is a two-player, turn-based, terminal-based board game written in C++. Players compete by progressing through a color-coded board, completing DNA-themed challenges, solving riddles, and encountering random scientific events. The goal is to reach the final tile while maximizing Discovery Points (DP).\
 \
The project demonstrates object-oriented programming principles, file I/O, dynamic memory management, and modular design in C++.\

⸻

**Game Objective**  
   
The objective of the game is to guide your player to the final tile (the Genome Conference) while earning as many Discovery Points (DP) as possible. DP are earned through events, DNA challenges, riddles, and final stat bonuses.  
⸻

**Gameplay Summary**\
	•	Two players take turns\
	•	Each player has their own board track\
	•	On each turn, a player may:\
	•	View their stats\
	•	Review their character or advisor\
	•	Display the board\
	•	Spin to move forward\
	•	Landing on different colored tiles triggers different challenges\
	•	The game ends when both players reach the final tile\

⸻

**Board Tile Types**\
Each tile color represents a different type of action:\
	•	Grey: Start tile\
	•	Green: Random scientific event (may increase or decrease DP)  
	•	Blue: DNA similarity challenge (equal-length strands)  
	•	Pink: DNA similarity challenge (unequal-length strands)  
	•	Red: DNA mutation identification task  
	•	Brown: DNA transcription (DNA → RNA)  
	•	Purple: Riddle challenge  
	•	Orange: Final tile (Genome Conference)  

⸻

**DNA Challenges**  
The game includes multiple DNA-based tasks:  
	•	Strand similarity comparison  
	•	Best alignment between unequal DNA strands  
	•	Identification of insertions, deletions, and substitutions  
	•	Transcription of DNA into RNA  

These challenges award Discovery Points upon completion.  

⸻

**Characters, Paths, and Advisors**  
	•	Players choose a character from a file containing predefined stats  
	•	Players choose one of two paths:  
	•	Training Fellowship  
	•	Lab Assignment  
	•	Certain paths allow the selection of an advisor  
	•	Advisors may protect players from negative events  

⸻

**Scoring System**  
Discovery Points (DP) are earned from:  
	•	Random events  
	•	DNA challenges  
	•	Riddles  
	•	End-game stat bonuses based on Accuracy, Efficiency, and Insight  

The player with the highest DP at the end of the game wins.  

⸻

**Project Structure**  

_main.cpp_  
Entry point of the program. Creates a Game object and starts gameplay.  

_Game.h / Game.cpp_  
Controls all game logic, including:  
	•	File loading  
	•	Character selection  
	•	Turn flow  
	•	Tile interactions  
	•	DNA challenges  
	•	Events and riddles  
	•	Final scoring and output  

_Board.h / Board.cpp_  
Manages the game board:  
	•	Tile generation  
	•	Player movement  
	•	Board display  
	•	Tile color logic  

_Player.h / Player.cpp_  
Represents each player:  
	•	Stores stats and progression  
	•	Handles Discovery Points and traits  
	•	Manages advisor protection  
	•	Formats player output  

Input Files  

_characters.txt_  
Defines playable characters and their starting stats.  
Format:  
  Name | Experience | Accuracy | Efficiency | Insight | DiscoveryPoints  

_random_events.txt_  
Defines random events triggered on green tiles.  
Format:  
  Description | PathType | Advisor | DPChange  

_riddles.txt_  
Contains riddle questions and answers.  
Format:  
  Question | Answer  

⸻

**Compilation Instructions**  
Use the following command to compile the project:  

g++ -std=c++11 main.cpp Game.cpp Board.cpp Player.cpp -o genome_game  

Run the program with:  

./genome_game  

⸻

**Key Concepts Demonstrated**  
	•	Object-oriented programming (classes, encapsulation)  
	•	File input/output  
	•	Dynamic memory allocation  
	•	Randomization  
	•	Modular program design  
	•	String manipulation  
	•	Console-based UI design  

⸻

**Notes**
	•	The game is fully terminal-based  
	•	ANSI color codes are used for visual board representation  
	•	No global variables or pointers are used for game logic  
	•	Designed to follow CSCI 1300 project constraints  

⸻

**Author**  
Jennifer Alex  
CSCI 1300 – University of Colorado Boulder  
