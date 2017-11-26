//Rhydian Windsor 9437658 
//May 2017
//this is the main program file for an object orientated chess board. 


//includes
#include <iostream>
#include "General_functions_header.h"
#include "PGN_output_header.h"
#include <vector>
#include <string>
#include "Piece_functions_header.h"
#include <windows.h>

using namespace std;
char Piece::board[9][9] = {}; //initialise board array static data. This is used to print out the board
vector<Piece*> pieces_vector; //vector used to store all the pointers to the pieces


int number_of_moves_made = 1;
int main(){
	bool record_game; //set to true if game is recorded, false if not
	ofstream gamefile; //file to write game to
	//set name of console window
	SetConsoleTitle( TEXT("Rhydian's Chess Game")); 
	//sets dimensions of console window
	HWND console = GetConsoleWindow(); //get pointer to console window
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, 600,500, TRUE);
	// this set's console background colour to yellow and default text to black
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	system("COLOR e0");
	SetColorAndBackground(0, 14);
	//print intro message to chess board progam
	print_intro_message();
	//offer user option to record their game as a .PGN file
	print_centered_text("Would you like to record a .PGN file of your game?");
	print_centered_text("1) Yes 2) No ");
	switch (get_menu_entry(2)) {
	case 1: //comes here if game if to be recorded
		record_game = true;
		gamefile = filewrite::make_PGN_file();
		system("CLS"); //clear console screen
		break;
	case 2: //comes here if game is not to be recorded
		record_game = false;
		system("CLS");
	}
	setup_board(pieces_vector); //this appends the pieces in their starting location to pieces_vecto
	while (true) { 
		//error_in_move = false;
		bool good_move = true;
		do {
			print_intro_message(); print_board(); //prints out information relating to the current state of the board
			//if there was an error in the last move, prints out a warning message to user
			if (!good_move) {
				cout << "Error: Invalid Move. Please re-enter your move!" << endl;
			}
			cout << "Move Number " << number_of_moves_made << ". " << endl;
			cout << "Would you like to 1) make a move, 2) quit this game or 3) start a new game?" << endl;
			//if checkmate, end game for colour about to move
			if (check_if_checkmate(pieces_vector, number_of_moves_made) == true) {
				cout << "Checkmate! Thanks for playing!" << endl;
				if (record_game) { gamefile.close(); }
				return 0;
			}
			switch (get_menu_entry(3)) { //get user to input an integer from 1 to 3, and choose action accordinly
			case(1): //make move case
				good_move = make_move(pieces_vector, number_of_moves_made, gamefile, record_game); //make a move. if legal sets good_move to true
				if (good_move) { number_of_moves_made++; } //if the move is legal, increase the number of moves made by 1
				system("CLS"); //clear console window
				break;
			case(2): //exit game case
				cout << "Thanks for playing!" << endl;
				if (record_game) { gamefile.close(); }
				return 0; break;
			case (3): //new game case
				number_of_moves_made = 1;
				pieces_vector.clear();
				setup_board(pieces_vector);
				//if restart game, closes gamefile and overwrites it
				if (record_game) {
					gamefile.close();
					gamefile = filewrite::create_PGN_file("myGame", "Player_1", "Player_2"); 
				}
				system("CLS"); //clear console window
				break;
			}
			


		} while (!good_move);

	}
	return 0;
}