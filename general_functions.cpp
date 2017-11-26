#include "Piece_functions_header.h"
#include "General_functions_header.h"
#include "PGN_output_header.h"
#include<iostream>
#include <array>
#include<sstream>
#include <windows.h>
#include <stdio.h>
#include <ctype.h>

using namespace std;
//if input character is found in this array, then input is valid
array<char, 8> file_converter_array = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };

void print_board() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //acts as a 'pointer' to console window
	SetColorAndBackground(3, 0); //sets font color to blue and background to red
	for (int i = 8; i >= 0; i--) { //loops over all columns
		print_horizontal_line(); //prints horizontal line of dashes
		for (int j = 0; j < 9; j++) { //loops over all rows
			if (j == 1) { cout << "  "; }
			cout << " | ";
			if (is_black(square_occupancy(j, i)) && i >= 1) { SetColorAndBackground(10, 0); } //if black, font is green
			else if (is_white(square_occupancy(j, i)) && i >= 1) { SetColorAndBackground(15, 0); }//white font
			cout << square_occupancy(j, i); //print notation letter
			SetConsoleTextAttribute(hConsole, 3); //set console color back to blue
		}
		cout << " | " << endl;
	}
	cout << endl;
	SetColorAndBackground(0, 14); //set console color back to default yellow background
}

void SetColorAndBackground(int foregroundC, int backgroundC)
{
	//changes colour back ground to colours given by codes foregroundC and backgroundC
	WORD wColor = ((backgroundC & 0x0F) << 4) + (foregroundC & 0x0F);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
}

void print_centered_text(string String){
	//prints text in the center of the console window
	for (int i = 0; i < ((80 - static_cast<int>(String.length())) / 2); i++) cout << " ";
	cout << String << endl;
}

void new_line() {
	//new line in console
	cout << endl;
}
 
void print_intro_message() {
	//prints out centered intro message at the top of the screen
	new_line();
	print_centered_text("---------------------------------------------------");
	print_centered_text("RHYDIAN'S CHESS GAME");
	print_centered_text("---------------------------------------------------");
	new_line();
	print_centered_text("Welcome to my chess game!"); 
	print_centered_text("1) By entering the square location of a piece, one can then move it.");
	print_centered_text("2) If the move is legal, the board will print out the move");
	new_line();
}

bool is_valid_letter(char input_character) {
	for (auto& letter_value : file_converter_array) {
		if (input_character == letter_value) return true; //return true if input character found in file converter array
	}
	return false;
}

bool is_valid_digit(char input_character) {
	//returns true if input-character is a digit
	if (isdigit(input_character) && input_character != '0' && input_character != 9) {
		return true;
	}
	else return false;
}

bool is_valid_input(string input_code) {
	//input code must be 2 characters long
	if (input_code.length() != 2) {
		cout << "Input must be 2 characters long; a lower case letter and a number, e.g. e4" << endl;
		return false;
	}
	else {
		if (is_valid_letter(input_code[0])) {
			if (is_valid_digit(input_code[1])) {
				return true;
			}
			else {
				cout << "The second character must be a digit from 1 - 8" << endl;
				return false;
			}
		}
		else {
			cout << "First character must be a letter!" << endl;
			return false;
		}
	}
}

int get_file_no(string input_code) {
	for (int i = 0; i < 8; i++) {
		if (input_code[0] == file_converter_array[i]) return i + 1; //return true if input character found in file converter array
	}
	return 0;
}

int get_rank_no(string input_code) {
	input_code[1] - '0';
	return input_code[1] - '0'; // - '0' ensures character is not read out by ASCII code
}

//function used to get user to input a move and then make it
bool make_move(vector<Piece*> &vector_of_pieces, int number_of_moves_made, ofstream &file_to_write_to, bool record_game) {
	string piece_location, piece_new_location; //variables to store piece's new location in
	cout << "Which piece would you like to move?" << endl; //prompt message for user
	while (is_valid_input(piece_location) == false) { //checks input is of the right format using is_valid_input function
		getline(cin, piece_location); //if input is correct read in piece location
	}
	int file_no = get_file_no(piece_location); //gets file number from user input
	int rank_no = get_rank_no(piece_location); //gets rank number from user input
	if (find_piece(file_no, rank_no, vector_of_pieces) != nullptr) { //if there is a piece at piece location
		Piece* piece_being_moved = find_piece(file_no, rank_no, vector_of_pieces); //gets pointer to piece at location
		char piece_being_moved_notation_letter = get_notation_letter(piece_being_moved);
		if (check_colour(piece_being_moved, number_of_moves_made)) { //this line checks piece location is correct colour for the given move (white moves on odd move numbers)
			cout << "Where would you like to move the piece to?" << endl;
			while (is_valid_input(piece_new_location) == false) { //checks input is of the right format
				getline(cin, piece_new_location); //reads in location piece is to be moved to
			}
			int new_file = get_file_no(piece_new_location); //gets file number from user input
			int new_rank = get_rank_no(piece_new_location); //gets rank number from user input
			bool is_check = find_king_of_colour_making_move(vector_of_pieces, number_of_moves_made + 1)->is_in_check();
			if (piece_being_moved->move(new_file, new_rank, vector_of_pieces)) { //if the piece move is legal i.e. move() returns true and piece moves
				if (record_game) {
					filewrite::write_normal_move(piece_being_moved_notation_letter, number_of_moves_made, new_file, new_rank, file_to_write_to, is_check);
				}
				return true;
			} //return true to signify a legal move has been made
			  //comes here if standard move isn't legal, checks if a legal move or castle is possible with selected piece
			else if (piece_being_moved->take(new_file, new_rank, vector_of_pieces)) { 
				if (record_game) {
					filewrite::write_take_move(piece_being_moved_notation_letter, number_of_moves_made, new_file, new_rank, file_to_write_to, is_check);
				}
				return true; }
			else if (piece_being_moved->castle_left(new_file, new_rank, vector_of_pieces)) { 
				if (record_game) {
					void write_castle_left(int number_of_moves_made, std::ofstream &file, bool is_check);
				}
				return true;
			} //returns false always for all pieces except king
			else if (piece_being_moved->castle_right(new_file, new_rank, vector_of_pieces)) {
				if (record_game) {
					void write_castle_right(int number_of_moves_made, std::ofstream &file, bool is_check);
				}
				return true;
			} //returns false always for all pieces except king
			else {
				//if no legal move or take is possible between piece_location and piece_new_location squares, return false
				return false;
			}
		}
		else {
			//if piece of wrong colour is selected to move, return false
			cout << "Not this colour's turn to move!" << endl;
			return false;
		}
	}
	else {
		//if find_piece() returns nullptr, then there is no piece on selected square, return false
		cout << "There is no piece there!" << endl;
		return false;
	}
}

bool check_colour(Piece* piece_being_moved, int number_of_moves_made) {
	if (number_of_moves_made % 2 == 1) {
		if (is_white(get_notation_letter(piece_being_moved))) {
			//comes here if odd move number and the piece is white, and returns true
			return true;
		}
		else {
			//comes here if odd move number and piece is black and returns false
			return false;
		}
	}
	else {
		if (is_black(get_notation_letter(piece_being_moved))) {
			//comes here if even move number and the piece is black, and returns true
			return true;
		}
		else {
			//comes here if even move number and piece is white, and returns false
			return false;
		}
	}
}

bool king_is_in_check(std::vector<Piece*> vector_of_pieces, int number_of_moves_made) {
	if (find_king_of_colour_making_move(vector_of_pieces, number_of_moves_made)->is_in_check()) {
		return true;
	}
	else {
		return false;
	}
}

void print_horizontal_line() {
	cout << " ----------------------------------------" << endl;
}

bool check_if_checkmate(vector<Piece*> vector_of_pieces, int number_of_moves_made) {
	if (find_king_of_colour_making_move(vector_of_pieces, number_of_moves_made)->is_in_check()) {
		for (int i = 0; i < vector_of_pieces.size(); i++) {
			if (vector_of_pieces[i]->is_taken() == false && vector_of_pieces[i]->is_of_colour_to_move(number_of_moves_made)) {//checks to see piece hasn't already been taken
				int piece_initial_rank_no = vector_of_pieces[i]->get_rank_number();
				int piece_initial_file_no = vector_of_pieces[i]->get_file_number();
				//loops through every possible move for each piece on the board. If any are legal, returns false, as must not be checkmate.
				for (int rank_iterator = 1; rank_iterator <= 8; rank_iterator++) {
					for (int file_iterator = 1; file_iterator <= 8; file_iterator++) {
						Piece* potential_taken_piece_pointer = find_piece(file_iterator, rank_iterator, vector_of_pieces);
						if (vector_of_pieces[i]->move(file_iterator, rank_iterator, vector_of_pieces)) { //comes here if this move puts king out of check
							vector_of_pieces[i]->change_board_position(piece_initial_file_no, piece_initial_rank_no);
							cout << " Piece on " << file_converter_array[piece_initial_file_no -1] << piece_initial_rank_no << endl;
							cout << "Move to " << file_converter_array[file_iterator - 1 ] << rank_iterator << endl;
							return false;
						}
						else if (vector_of_pieces[i]->take(file_iterator, rank_iterator, vector_of_pieces)) { //comes here if this take puts king out of check
							cout << "Can make taking move!" << endl;
							vector_of_pieces[i]->change_board_position(piece_initial_file_no, piece_initial_rank_no);
							potential_taken_piece_pointer->set_piece_state_to_not_captured(); //set taken piece to not captured
							cout << " Piece on " << file_converter_array[piece_initial_file_no - 1] << piece_initial_rank_no << endl;
							cout << "Move to " << file_converter_array[file_iterator - 1] << rank_iterator << endl;
							return false;
						}
					}
				}
			}
		}
		return true; //comes here if all moves for every piece is illegal. Hence it is checkmate and returns true.
	}
	else { 
		return false; }
}

int get_menu_entry(int i) {
	int ret_integer;
	string str_number;

	while (true) {

		cin.clear();
		getline(cin, str_number); // Get string input and store in str_number
		stringstream convert(str_number); // Turn the string into a stream

										  // Check for complete conversion into an integer and check greater than min value allowed
		if (convert >> ret_integer && !(convert >> str_number) && ret_integer >= 1 && ret_integer <= i) return ret_integer;

		cin.clear(); // in case error occurs in cin
		cout << "Input must be an integer between 1 & " << i <<". Please try again: ";
	}
}
