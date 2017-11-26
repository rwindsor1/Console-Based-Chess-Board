#include<iostream>
#include "Piece_functions_header.h"
#include "General_functions_header.h"
#include <stdio.h>
#include <windows.h>
#include <ctype.h>
#include <cmath> //needed for 'abs()' absolute value function
using namespace std;

//Each piece type inherits from Piece abstract class. Need to overload move(int rank, int file), take(int rank, int file)

char file_converter_array[9] = { 'z','a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
//---------------------------------------------------------------------GENERAL FUNCTIONS-----------------------------------------------

bool is_white(char notation_letter) {
	//returns true if char is upper case
	if (isupper(notation_letter)) { return true; }
	else return false;
}

bool is_black(char notation_letter) {
	//returns true if char is upper case
	if (islower(notation_letter)) { return true; }
	else return false;
}

void setup_board(vector<Piece*>& piecesvector) {
	//put numbers and letters on board to mark squares
	Piece::board[1][0] = 'a';Piece::board[2][0] = 'b';Piece::board[3][0] = 'c';Piece::board[4][0] = 'd';
	Piece::board[5][0] = 'e';Piece::board[6][0] = 'f';Piece::board[7][0] = 'g';Piece::board[8][0] = 'h';
	for (int i = 1; i < 9; i++) {
		Piece::board[0][i] = '0' + i;
	}
	//place pawns
	for (int i = 1; i <= 8; i++) {
		piecesvector.push_back(new White_pawn(i, 2));
		piecesvector.push_back(new Black_pawn(i, 7));
		for (int j = 3; j <= 6; j++) {
			Piece::board[i][j] = ' ';
		}
	}
	piecesvector.push_back(new Black_rook(1, 8)); piecesvector.push_back(new Black_rook(8, 8)); //place black rooks
	piecesvector.push_back(new White_rook(1, 1)); piecesvector.push_back(new White_rook(8, 1)); //place white rooks
	piecesvector.push_back(new White_knight(2, 1)); piecesvector.push_back(new White_knight(7, 1)); //place white knights
	piecesvector.push_back(new Black_knight(2, 8)); piecesvector.push_back(new Black_knight(7, 8)); //place black knights
	piecesvector.push_back(new Black_bishop(3, 8)); piecesvector.push_back(new Black_bishop(6, 8)); //place black bishops
	piecesvector.push_back(new White_bishop(3, 1)); piecesvector.push_back(new White_bishop(6, 1)); //place white bishops
	piecesvector.push_back(new White_queen(4, 1)); piecesvector.push_back(new Black_queen(4, 8)); //place queens
	piecesvector.push_back(new White_king(5, 1)); piecesvector.push_back(new Black_king(5, 8)); //place kings
}

char square_occupancy(int fileno, int rankno) {
	//returns char of piece occupying certain square
	return Piece::board[fileno][rankno];
}



Piece::Piece() {
	// default constructor for any piece. sets has been captured to false
	has_been_captured = false;
}

Piece* find_piece(int file_no, int rank_no, std::vector<Piece*> vector_of_pieces) {
	for (unsigned i = 0; i < vector_of_pieces.size(); i++) { //loop over vector of piece pointers
		//if file number and rank number match and piece hasn't been captured
		if (file_no == vector_of_pieces[i]->file && rank_no == vector_of_pieces[i]->rank){
			if (vector_of_pieces[i]->has_been_captured == false) {
				return vector_of_pieces[i];
			}
		}
	}
	return nullptr; //if no piece found, return null pointer
}

void Piece::capture_this_piece() {
	//set piece captured boolean to true
	has_been_captured = true;
}

bool Piece::is_taken(){
	return has_been_captured; //returns true is piece has already been taken
}

Piece* find_king_of_colour_making_move(vector<Piece*> vector_of_pieces, int number_of_moves_made) {
	if (number_of_moves_made % 2) {
		for (unsigned i = 0; i < vector_of_pieces.size(); i++) {
			if (get_notation_letter(vector_of_pieces[i]) =='K') {
				return vector_of_pieces[i];
			}
		}
	}
	else {
		for (unsigned i = 0; i < vector_of_pieces.size(); i++) {
			if (get_notation_letter(vector_of_pieces[i]) == 'k') {
				return vector_of_pieces[i];
			}
		}
	}
	return nullptr; //returns a null pointer if there is no king on the board (should never be called)
}


void remove_piece_from_board(int file_no, int rank_no, vector<Piece*> vector_of_pieces) {
	//removes piece on given file and rank from board by setting captured boolean to true
	Piece* piece_to_be_removed = find_piece(file_no, rank_no, vector_of_pieces);
	for (unsigned i = 0; i < vector_of_pieces.size(); i++) {
		if (vector_of_pieces[i] == piece_to_be_removed) { 
			vector_of_pieces[i]->capture_this_piece();
		}
	}

}

bool is_legal_bishop_move(int file, int rank, int new_file, int new_rank) {
	if ((abs(new_file - file) == abs(new_rank - rank)) && !(new_file == file && new_rank == rank)) {
		if (new_rank > rank && new_file > file) { // if bishop is moving NORTH EAST
			for (int i = 1; i < new_rank - rank; i++) {
				if (Piece::board[file + i][rank + i] != ' ') { return false; }
			}
			return true;
		}
		if (new_rank < rank && new_file < file) { // if bishop is moving SOUTH WEST
			for (int i = 1; i < rank - new_rank; i++) {
				if (Piece::board[file - i][rank - i] != ' ') { return false; }
			}
			return true;
		}
		if (new_rank < rank && new_file > file) { // if bishop is moving SOUTH EAST
			for (int i = 1; i < new_file - file; i++) {
				if (Piece::board[file + i][rank - i] != ' ') { return false; }
			}
			return true;
		}
		if (new_rank > rank && new_file < file) { // if bishop is moving NORTH WEST
			for (int i = 1; i < new_rank - rank; i++) {
				if (Piece::board[file - i][rank + i] != ' ') { return false; }
			}
			return true;
		}
		else return false;
	}
	else return false;
}

bool is_legal_rook_move(int file, int rank, int new_file, int new_rank) {
	if ((file == new_file || rank == new_rank) && (file != new_file || rank != new_rank)) {
		//comes here if rook move can be made if there are no pieces in the way
		if (file == new_file) {
			if (rank < new_rank) { //does not check final square rook moves to!
				for (int i = rank + 1; i < new_rank; i++) {
					if (Piece::board[file][i] != ' ') {
						return false;
					}
				}
				return true;
			}
			else {
				for (int i = rank - 1; i > new_rank; i--) {
					if (Piece::board[file][i] != ' ') {
						return false;
					}
				}
				return true;
			}
		}
		else {
			if (file < new_file) { //does not check final square rook moves to!
				for (int i = file + 1; i < new_file; i++) {
					if (Piece::board[i][rank] != ' ') {
						return false;
					}
				}
				return true;
			}
			else {
				for (int i = file - 1; i > new_file; i--) {
					if (Piece::board[i][rank] != ' ') {
						return false;
					}
				}
				return true;
			}
		}
	}
	else return false;
}

bool is_legal_knight_move(int file, int rank, int new_file, int new_rank) {
	//returns true if legal knight move ie two squares in one direction and one in the other
	if ((abs(new_file - file) == 1 && abs(new_rank - rank) == 2) || (abs(new_file - file) == 2 && abs(new_rank - rank) == 1)) {
		return true;
	}
	else return false;
}

bool is_legal_queen_move(int file, int rank, int new_file, int new_rank) {
	//returns true if a legal bishop move or a legal queen move
	if (is_legal_bishop_move(file, rank, new_file, new_rank) || is_legal_rook_move(file, rank, new_file, new_rank)) {
		return true;
	}
	else return false;
}


bool is_legal_king_move(int file, int rank, int new_file, int new_rank) {
	//returns true if a legal queen move that only moves one square away
	if (is_legal_queen_move(file, rank, new_file, new_rank) && abs(rank - new_rank) <= 1 && abs(file - new_file) <= 1) {
		return true;
	}
	else return false;
}
bool is_legal_black_pawn_take(int file, int rank, int new_file, int new_rank) {
	//returns true if a legal taking pattern for a pawn
	if (abs(file - new_file) == 1 && rank - new_rank == 1) {
		return true;
	}
	else { return false; }
}

bool is_legal_white_pawn_take(int file, int rank, int new_file, int new_rank) {
	//returns true if a legal taking pattern for a white pawn
	if (abs(new_file - file) == 1 && new_rank - rank == 1) {
		return true;
	}
	else { return false; }
}

char get_notation_letter(Piece* chosen_piece) {
	//returns notation letter of piece pointer
	return chosen_piece->notation_letter;
}



//---------------------------------------------------------------------- GENERAL  PIECE FUNCTIONS ----------------------------------------------
void Piece::print_position() {
	//prints out the location of a given piece
	cout << "Piece location is " << file_converter_array[file] << rank << "." << endl;
}
int Piece::return_number_of_moves() {
	//returns the total number of moves made by a piece
	return number_of_moves_made;
}
//standard destructor for a piece
Piece::~Piece() { board[file][rank] = ' '; }
int Piece::get_file_number() { return file; }
int Piece::get_rank_number() { return rank; }
bool Piece::move_if_white_not_in_check( int new_file, int new_rank, vector<Piece*> &vector_of_pieces) {
	board[file][rank] = ' ';
	board[new_file][new_rank] = notation_letter;
	if (king_is_in_check(vector_of_pieces, 1)) { //finds white king and returns true if in check
												 //if in check return board to previous state
		board[file][rank] = notation_letter;
		board[new_file][new_rank] = ' ';
		return false;
	}
	else {
		//if not in check update pieces location
		file = new_file; rank = new_rank;
		return true;
	}
}
bool Piece::move_if_black_not_in_check(int new_file, int new_rank, vector<Piece*> &vector_of_pieces) {
	//makes move not happen if it puts black king in check
	board[file][rank] = ' ';
	board[new_file][new_rank] = notation_letter;
	if (king_is_in_check(vector_of_pieces, 0)) { //finds black king and returns true if in check
		//if in check return board to previous state
		board[file][rank] = notation_letter;
		board[new_file][new_rank] = ' ';
		return false;
	}
	else {
		//if not in check update pieces location
		file = new_file; rank = new_rank;
		return true;
	}
}
bool Piece::move_if_check_not_caused(int new_file, int new_rank, vector<Piece*> &vector_of_pieces) {
	//if move doesnt cause check, allows it to occur
	if (is_white(notation_letter)) {
		return move_if_white_not_in_check(new_file, new_rank, vector_of_pieces);
	}
	else {
		return move_if_black_not_in_check(new_file, new_rank, vector_of_pieces);
	}
}
void Piece::change_board_position(int new_file, int new_rank) {
	board[file][rank] = ' ';
	file = new_file; rank = new_rank;
	board[file][rank] = notation_letter;
	return;
}
void Piece::set_piece_state_to_not_captured() {
	has_been_captured = false;
	board[file][rank] = notation_letter;
	return;
}
bool Piece::is_of_colour_to_move( int move_number) {
	//if this piece is of the colour to make a move
	if (is_black(notation_letter) && ((move_number % 2) == 0)) {
		return true;
	}
	else if (is_white(notation_letter) && ((move_number % 2) == 1)) {
		return true;
	}
	else { return false; }
}
//castle moves return false for any piece except king
bool Piece::castle_right(int file_no, int rank_no, std::vector<Piece*> &vector_of_pieces) { return false; }
bool Piece::castle_left(int file_no, int rank_no, std::vector<Piece*> &vector_of_pieces) { return false; }
// --------------------------------------------------------------------- PAWN FUNCTIONS --------------------------------------------------
Pawn::Pawn() {
	rank = 1; file = 1; number_of_moves_made = 0;
}

Pawn::~Pawn() {} // pawn destructor

				 //white pawns
White_pawn::White_pawn() { notation_letter = 'P'; }

White_pawn::White_pawn(int fileno, int rankno) { //initalises data for white pawn
	notation_letter = 'P';
	rank = rankno;
	file = fileno;
	board[file][rank] = notation_letter;
}

bool White_pawn::move(int new_file, int new_rank, vector<Piece*> &vector_of_pieces) {
	if (new_file != file) {
		return false;
	} // ensures that regular move doesn't change pawn's file
	else {
		if (number_of_moves_made == 0) { //on first move pawn can move two places
			if (new_rank - rank > 2 || new_rank - rank <= 0) { // checks that pawn is not moved more than 2 places on first move
				return false;
			}
			else { // if move valid, check there are no other pieces in the way
				if (board[new_file][new_rank] == ' ' && is_legal_rook_move(file,rank,new_file,new_rank)) {
					//finally check if this move will put king in check
					if (move_if_white_not_in_check(new_file, new_rank, vector_of_pieces)) {
						number_of_moves_made++;
						return true;
					}
					else return false;
				}
				else {
					return false;
				}
			}
		}
		else {
			if (new_rank - rank > 1 || new_rank - rank < 0) {
				return false;
			}
			else { // move is valid, check for pieces in the way
				if (board[new_file][new_rank] = ' ') {
					//finally check if this move will put king in check
					if (move_if_check_not_caused(new_file, new_rank, vector_of_pieces)) {
						number_of_moves_made++;
						promote_if_necessary(file, rank, vector_of_pieces);
						return true;
					}
					else return false;
				}
				else return false;
			}
		}
	}
}

bool White_pawn::take(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces) {
	if (board[new_file][new_rank] != ' ' && new_rank - rank == 1 && abs(new_file - file) == 1) {
		char old_piece_notation_letter = board[new_file][new_rank];
		board[new_file][new_rank] = notation_letter;
		board[file][rank] = ' ';
		if (king_is_in_check(vector_of_pieces, 1)) {
			board[new_file][new_rank] = old_piece_notation_letter;
			board[file][rank] = notation_letter;
			return false;
		}
		else {
			remove_piece_from_board(new_file, new_rank, vector_of_pieces);
			file = new_file; rank = new_rank;
			number_of_moves_made++;
			promote_if_necessary(file, rank, vector_of_pieces);
			return true;
		}
	}
	else {
		return false;
	}
}

void White_pawn::promote_if_necessary(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces) {
	if (new_rank == 8) {
		remove_piece_from_board(new_file, new_rank, vector_of_pieces);
		cout << "What would you like to promote this piece to? ( 1. Queen 2. Rook 3. Bishop 4. Knight)" << endl;
		switch (get_menu_entry(4)) {
		case 1:
			vector_of_pieces.push_back(new White_queen(new_file, new_rank)); break;
		case 2:
			vector_of_pieces.push_back(new White_rook(new_file, new_rank)); break;
		case 3:
			vector_of_pieces.push_back(new White_bishop(new_file, new_rank)); break;
		case 4:
			vector_of_pieces.push_back(new White_knight(new_file, new_rank)); break;
		}
	}
	else { return; }
}

//black pawns
Black_pawn::Black_pawn() { notation_letter = 'p'; }

Black_pawn::Black_pawn(int fileno, int rankno) {
	notation_letter = 'p';
	rank = rankno;
	file = fileno;
	board[file][rank] = notation_letter;
}

bool Black_pawn::move(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces) {
	if (new_file != file) { return false; } // ensures that regular move doesn't change pawn's file
	else {
		if (number_of_moves_made == 0) { //on first move pawn can move two places
			if (rank - new_rank > 2 || new_rank - rank >= 0) { // checks that pawn is not moved more than 2 places on first move
				return false;
			}
			else { // if move valid, check there are no pieces in the way
				if (board[new_file][new_rank] == ' ' && is_legal_rook_move(file, rank, new_file, new_rank)) { //checks there is nothing on square the pawn is being moved to
					if (move_if_check_not_caused(new_file, new_rank, vector_of_pieces)) {
						number_of_moves_made++;
						return true;
					}
					else return false;
				}
				else {
					return false;
				}
			}
		}
		else {
			if (rank - new_rank > 1 || new_rank - rank > 0) {
				return false;
			}
			else {
				if (move_if_check_not_caused(new_file, new_rank, vector_of_pieces)) {
					number_of_moves_made++;
					promote_if_necessary(new_file, new_rank, vector_of_pieces);
					return true;
				}
				else return false;
			} 
		}
	}
}

bool Black_pawn::take(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces) {
	if (board[new_file][new_rank] != ' ' && new_rank - rank == -1 && abs(new_file - file) == 1) {
		char old_piece_notation_letter = board[new_file][new_rank];
		board[new_file][new_rank] = notation_letter;
		board[file][rank] = ' ';
		if (king_is_in_check(vector_of_pieces, 0)) {
			board[new_file][new_rank] = old_piece_notation_letter;
			board[file][rank] = notation_letter;
			return false;
		}
		else {
			remove_piece_from_board(new_file, new_rank, vector_of_pieces);
			file = new_file; rank = new_rank;
			number_of_moves_made++;
			promote_if_necessary(new_file, new_rank, vector_of_pieces);
			return true;
		}
	}
	else {
		return false;
	}
}

void Black_pawn::promote_if_necessary(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces) {
	if (new_rank == 1) {
		remove_piece_from_board(new_file, new_rank, vector_of_pieces);
		cout << "What would you like to promote this piece to? ( 1. Queen 2. Rook 3. Bishop 4. Knight)" << endl;
		switch (get_menu_entry(4)) {
		case 1:
			vector_of_pieces.push_back(new Black_queen(new_file, new_rank)); break;
		case 2:
			vector_of_pieces.push_back(new Black_rook(new_file, new_rank)); break;
		case 3:
			vector_of_pieces.push_back(new Black_bishop(new_file, new_rank)); break;
		case 4:
			vector_of_pieces.push_back(new Black_knight(new_file, new_rank)); break;
		}
	}
	else { return; }
}

// --------------------------------------------------------------------- ROOK FUNCTIONS --------------------------------------------------

Rook::Rook() {
	rank = 1; file = 1; number_of_moves_made = 0;
}

Rook::~Rook() {}; //rook destructor

bool Rook::move(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces) {
	if (is_legal_rook_move(file, rank, new_file, new_rank) == true && board[new_file][new_rank] == ' ') {
		return move_if_check_not_caused(new_file, new_rank, vector_of_pieces);
	}
	else {
		return false;
	}
}

// white rook functions
White_rook::White_rook(int file_no, int rank_no) {
	file = file_no; rank = rank_no; notation_letter = 'R';
	board[file][rank] = notation_letter;
}

bool White_rook::take(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces) {
	if (is_black(board[new_file][new_rank]) && is_legal_rook_move(file, rank, new_file, new_rank)) {
		char old_piece_notation_letter = board[new_file][new_rank];
		board[new_file][new_rank] = notation_letter;
		board[file][rank] = ' ';
		if (king_is_in_check(vector_of_pieces, 1)) {
			board[new_file][new_rank] = old_piece_notation_letter;
			board[file][rank] = notation_letter;
			return false;
		}
		else {
			remove_piece_from_board(new_file, new_rank, vector_of_pieces);
			file = new_file; rank = new_rank;
			return true;
		}
	}
	else {
		return false;
	}
}

//black rook functions

Black_rook::Black_rook(int file_no, int rank_no) {
	file = file_no; rank = rank_no; notation_letter = 'r';
	board[file][rank] = notation_letter;
}

bool Black_rook::take(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces) {
	if (is_white(board[new_file][new_rank]) && is_legal_rook_move(file, rank, new_file, new_rank)) {
		char old_piece_notation_letter = board[new_file][new_rank];
		board[new_file][new_rank] = notation_letter;
		board[file][rank] = ' ';
		if (king_is_in_check(vector_of_pieces, 0)) {
			board[new_file][new_rank] = old_piece_notation_letter;
			board[file][rank] = notation_letter;
			return false;
		}
		else {
			remove_piece_from_board(new_file, new_rank, vector_of_pieces);
			file = new_file; rank = new_rank;
			return true;
		}
	}
	else {
		return false;
	}
}

//--------------------------------------------------------------------- BISHOP FUNCTIONS -------------------------------------------------

Bishop::Bishop() {} //default constructor for bishop class

Bishop::~Bishop() {} //destructor for bishop class

bool Bishop::move(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces) {
	if (is_legal_bishop_move(file, rank, new_file, new_rank) == true && board[new_file][new_rank] == ' ') {
		return move_if_check_not_caused(new_file, new_rank, vector_of_pieces);
	}
	else {
		return false;
	}
}

//white bishop parameterised constructor
White_bishop::White_bishop(int file_no, int rank_no) {
	file = file_no; rank = rank_no; notation_letter = 'B';
	board[file][rank] = notation_letter;
}

bool White_bishop::take(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces) {
	if (is_black(board[new_file][new_rank]) && is_legal_bishop_move(file, rank, new_file, new_rank)) {
		char old_piece_notation_letter = board[new_file][new_rank];
		board[new_file][new_rank] = notation_letter;
		board[file][rank] = ' ';
		if (king_is_in_check(vector_of_pieces, 1)) {
			board[new_file][new_rank] = old_piece_notation_letter;
			board[file][rank] = notation_letter;
			return false;
		}
		else {
			remove_piece_from_board(new_file, new_rank, vector_of_pieces);
			file = new_file; rank = new_rank;
			return true;
		}
	}
	else {
		return false;
	}
}

Black_bishop::Black_bishop(int file_no, int rank_no) {
	file = file_no; rank = rank_no; notation_letter = 'b';
	board[file][rank] = notation_letter;
}

bool Black_bishop::take(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces) {
	if (is_white(board[new_file][new_rank]) && is_legal_bishop_move(file, rank, new_file, new_rank)) {
		char old_piece_notation_letter = board[new_file][new_rank];
		board[new_file][new_rank] = notation_letter;
		board[file][rank] = ' ';
		if (king_is_in_check(vector_of_pieces, 0)) {
			board[new_file][new_rank] = old_piece_notation_letter;
			board[file][rank] = notation_letter;
			return false;
		}
		else {
			remove_piece_from_board(new_file, new_rank, vector_of_pieces);
			file = new_file; rank = new_rank;
			return true;
		}
	}
	else {
		return false;
	}
}

//--------------------------------------------------------------------- QUEEN FUNCTIONS -------------------------------------------------

Queen::Queen() {}

Queen::~Queen() {}

bool Queen::move(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces) {
	if (board[new_file][new_rank] == ' ' && is_legal_queen_move(file, rank, new_file, new_rank)) {
		return move_if_check_not_caused(new_file, new_rank, vector_of_pieces);
	}
	else {
		return false;
	}
}
//white queen functions
White_queen::White_queen(int file_no, int rank_no) {
	file = file_no; rank = rank_no; notation_letter = 'Q'; //initalise queen variables
	board[file][rank] = notation_letter;
}

bool White_queen::take(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces) {
	if (is_black(board[new_file][new_rank]) && is_legal_queen_move(file, rank, new_file, new_rank)) {
		char old_piece_notation_letter = board[new_file][new_rank];//save type of taken piece in case it needs to be put back
		board[new_file][new_rank] = notation_letter; //put queen on new square, and clear old square
		board[file][rank] = ' ';
		//check for check and if in check put pieces back as they were and return false
		if (king_is_in_check(vector_of_pieces, 1)) {
			board[new_file][new_rank] = old_piece_notation_letter;
			board[file][rank] = notation_letter;
			return false;
		}
		//if legal move comes here
		else {
			remove_piece_from_board(new_file, new_rank, vector_of_pieces);
			file = new_file; rank = new_rank;
			return true;
		}
	}
	//if there is no black piece on new square or illegal queen move
	else {
		return false;
	}
}

//black queen functions
Black_queen::Black_queen(int file_no, int rank_no) {
	file = file_no; rank = rank_no; notation_letter = 'q';
	board[file][rank] = notation_letter;
}

bool Black_queen::take(int new_file, int new_rank, std::vector<Piece*>& vector_of_pieces) {
	if (is_white(board[new_file][new_rank]) && is_legal_queen_move(file, rank, new_file, new_rank)) {
		char old_piece_notation_letter = board[new_file][new_rank];
		board[new_file][new_rank] = notation_letter;
		board[file][rank] = ' ';
		if (king_is_in_check(vector_of_pieces, 0)) {
			//comes here if move is legal but puts king in check
			board[new_file][new_rank] = old_piece_notation_letter;
			board[file][rank] = notation_letter;
			return false;
		}
		//comes here if move is legal and doesnt put black king in check
		else {
			remove_piece_from_board(new_file, new_rank, vector_of_pieces);
			file = new_file; rank = new_rank;
			return true;
		}
	}
	//if there is no white piece on new square or illegal queen move
	else {
		return false;
	}
}

//--------------------------------------------------------------------- KNIGHT FUNCTIONS -------------------------------------------------
//knight default constructor
Knight::Knight() {}
//knight destructor
Knight::~Knight() {}
//checks knight move is legal, and if so makes the move
bool Knight::move(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces) {
	//check if move is legal
	if (is_legal_knight_move(file, rank, new_file, new_rank) && board[new_file][new_rank] == ' ') {
		return move_if_check_not_caused(new_file, new_rank, vector_of_pieces); //returns true if doesnt put own king in check, false if not
	}
	else {
		//comes here if move isnt legal for a knight
		return false;
	}
}

// white knight functions
White_knight::White_knight(int file_no, int rank_no) {
	//initialise data for white knight
	file = file_no; rank = rank_no; notation_letter = 'N';
	board[file][rank] = notation_letter;
}

bool White_knight::take(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces) {
	//check if black piece on new square and if legal knight move
	if (is_black(board[new_file][new_rank]) && is_legal_knight_move(file, rank, new_file, new_rank)) {
		char old_piece_notation_letter = board[new_file][new_rank];
		board[new_file][new_rank] = notation_letter;
		board[file][rank] = ' ';
		if (king_is_in_check(vector_of_pieces, 1)) {
			//comes here if legal knight move that takes a black piece and doesnt put white king in check
			board[new_file][new_rank] = old_piece_notation_letter;
			board[file][rank] = notation_letter;
			return false;
		}
		else {
			//comes here if move puts white king in check. must put pieces back to how they were and return false
			remove_piece_from_board(new_file, new_rank, vector_of_pieces);
			file = new_file; rank = new_rank;
			return true;
		}
	}
	else {
		return false;
	}
}

// black knight functions
Black_knight::Black_knight(int file_no, int rank_no) {
	file = file_no; rank = rank_no; notation_letter = 'n';
	board[file][rank] = notation_letter;
}

bool Black_knight::take(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces) {
	//check if white piece on new square and legal knight move
	if (is_white(board[new_file][new_rank]) && is_legal_knight_move(file, rank, new_file, new_rank)) {
		char old_piece_notation_letter = board[new_file][new_rank];
		board[new_file][new_rank] = notation_letter;
		board[file][rank] = ' ';
		if (king_is_in_check(vector_of_pieces, 0)) {
			//comes here if legal taking move that doesnt put king in check
			board[new_file][new_rank] = old_piece_notation_letter;
			board[file][rank] = notation_letter;
			return false;
		}
		else {
			//comes here if legal knight move that puts king in check
			remove_piece_from_board(new_file, new_rank, vector_of_pieces);
			file = new_file; rank = new_rank;
			return true;
		}
	}
	else {
		return false;
	}
}

//--------------------------------------------------------------------- KING FUNCTIONS -------------------------------------------------
//king default constructor
King::King() { number_of_moves_made = 0; }
//king destructor
King::~King() {}
//checks if king move is legal and if it is, makes move
bool King::move(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces) {
	if (is_legal_king_move(file, rank, new_file, new_rank) && board[new_file][new_rank] == ' ') {
		//store kings initial location in variables so if it is checked it will be able to move back
		int old_file, old_rank;
		old_file = file, old_rank = rank;
		board[file][rank] = ' ';
		file = new_file; rank = new_rank;
		board[new_file][new_rank] = notation_letter;
		if (this->is_in_check()) { //if king in in check, put back to original square and return false
			board[file][rank] = ' ';
			file = old_file; rank = old_rank;
			board[file][rank] = notation_letter;
			return false;
		}
		else { //valid move, increase move counter by one
			number_of_moves_made++;
			return true;
		}
	}
	else {
		//if not legal move or there is a piece on the new square
		return false;
	}
}

//white king functions
White_king::White_king(int file_no, int rank_no) {
	file = file_no; rank = rank_no; notation_letter = 'K';
	board[file][rank] = notation_letter;
}

bool White_king::take(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces) {
	//if piece on new square is white and this is a legal king move, try making the move
	//then check if in check. If so, move king back to its inital square and return false
	//If not make move
	if (is_black(board[new_file][new_rank]) && is_legal_king_move(file, rank, new_file, new_rank)) {
		char old_piece_notation_letter = board[new_file][new_rank];
		int old_file = file; int old_rank = rank; //store initial file and rank as variables
		board[new_file][new_rank] = notation_letter;
		board[file][rank] = ' ';
		file = new_file; rank = new_rank; //move king to new squares to see if in check
		if (king_is_in_check(vector_of_pieces, 1)) {
			rank = old_rank; file = old_file; //if in check on new square put king back in intial position
			board[new_file][new_rank] = old_piece_notation_letter; //change board notaiton to attempted-to-be-taken piece
			board[file][rank] = notation_letter;
			return false;
		}
		else {
			rank = old_rank; file = old_file; //briefly remove king from new square while old piece on that square is taken off board 
			remove_piece_from_board(new_file, new_rank, vector_of_pieces); //remove taken piece from board
			file = new_file; rank = new_rank; //set king back to new position and complete move
			number_of_moves_made++;
			return true;
		}
	}
	else {
		return false;
	}
}

bool White_king::is_in_check() {
	for (int i = 1; i <= 8; i++) { //loops over each square on chess board. 
								   //if square contains piece of opposite colour, check to see if that piece can take the king.
								   //If it can, king is in check so return TRUE. If not for all squares, return false
		for (int j = 1; j <= 8; j++) {
			if (is_black(board[i][j])) {
				// comes here if piece if square contains black piece
				if (board[i][j] == 'n') { //if knight on square
					if (is_legal_knight_move(i, j, file, rank)) {
						return true;
					}
				}
				if (board[i][j] == 'r') { //if rook on square
					if (is_legal_rook_move(i, j, file, rank)) {

						return true;
					}
				}
				if (board[i][j] == 'b') { //if bishop on square
					if (is_legal_bishop_move(i, j, file, rank)) {
						return true;
					}
				}
				if (board[i][j] == 'q') { //if queen on square
					if (is_legal_queen_move(i, j, file, rank)) {
						return true;
					}
				}
				if (board[i][j] == 'p') { //if black pawn is on square
					if (is_legal_black_pawn_take(i, j, file, rank)) {
						return true;
					}
				}
			}
		}
	}
	return false; //comes here if all squares are found not to threaten the king
}

bool White_king::castle_left(int file_no, int rank_no, std::vector<Piece*> &vector_of_pieces) {
	if (rank_no != 1 || file_no != 3) { return false; } //only legal if user inputs to move to c1
	if ( number_of_moves_made == 0 && get_notation_letter(find_piece(1, 1, vector_of_pieces)) == 'R') {
		if (board[2][1] == ' ' && board[3][1] == ' ' && board[4][1] == ' ') {
			for (int i = 5; i >= 3; i--) {
				change_board_position(i, 1);
				if (is_in_check()) { // check each square king moves across whilst castling to ensure he doesn't get checked
					change_board_position(5, 1);
					return false;
				}
			}
			//if each position doesn't yield check, move rook too
			find_piece(1, 1, vector_of_pieces)->change_board_position(4, 1);
			return true;
		}
		else { return false; }
	}
}

bool White_king::castle_right(int file_no, int rank_no, std::vector<Piece*> &vector_of_pieces) {
	if (rank_no != 1 || file_no != 7) { return false; }
	if (number_of_moves_made == 0 && get_notation_letter(find_piece(8, 1 , vector_of_pieces)) == 'R') {
		if (board[6][1] == ' ' && board[7][1] == ' ') {
			for (int i = 5; i <= 7; i++) {
				change_board_position(i, 1);
				if (is_in_check()) { // check each square king moves across whilst castling to ensure he doesn't get checked
					change_board_position(5, 1);
					return false;
				}
			}
			//if each position doesn't yield check, move rook too
			find_piece(8, 1, vector_of_pieces)->change_board_position(6, 1);
			return true;
		}
		else { return false; }
	}
}
//black king functions
Black_king::Black_king(int file_no, int rank_no) {
	file = file_no; rank = rank_no; notation_letter = 'k';
	board[file][rank] = notation_letter;
}

bool Black_king::take(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces) {
	//if piece on new square is white and this is a legal king move, try making the move
	//then check if in check. If so, move king back to its inital square and return false
	//If not make move
	if (is_white(board[new_file][new_rank]) && is_legal_king_move(file, rank, new_file, new_rank)) {
		char old_piece_notation_letter = board[new_file][new_rank]; 
		int old_file = file; int old_rank = rank; //store initial file and rank as variables
		board[new_file][new_rank] = notation_letter; //
		board[file][rank] = ' ';
		file = new_file; rank = new_rank;
		if (king_is_in_check(vector_of_pieces, 0)) {
			rank = old_rank; file = old_file;
			board[new_file][new_rank] = old_piece_notation_letter;
			board[file][rank] = notation_letter;
			return false;
		}
		else {
			rank = old_rank; file = old_file;
			remove_piece_from_board(new_file, new_rank, vector_of_pieces); //remove old piece from the board
			file = new_file; rank = new_rank;
			number_of_moves_made++;
			return true;
		}
	}
	else {
		return false;
	}
}

bool Black_king::is_in_check() {
	for (int i = 1; i <= 8; i++) { //loops over each square on chess board. 
								   //if square contains piece of opposite colour, check to see if that piece can take the king.
								   //If it can, king is in check so return TRUE. If not for all squares, return false
		for (int j = 1; j <= 8; j++) {
			if (is_white(board[i][j])) {
				// comes here if piece if square contains black piece
				if (board[i][j] == 'N') { //if knight on square
					if (is_legal_knight_move(i, j, file, rank)) {
						return true;
					}
				}
				if (board[i][j] == 'R') { //if rook on square
					if (is_legal_rook_move(i, j, file, rank)) {
						return true;
					}
				}
				if (board[i][j] == 'B') { //if bishop on square
					if (is_legal_bishop_move(i, j, file, rank)) {
						return true;
					}
				}
				if (board[i][j] == 'Q') { //if queen on square
					if (is_legal_queen_move(i, j, file, rank)) {
						return true;
					}
				}
				if (board[i][j] == 'P') { //if black pawn is on square
					if (is_legal_white_pawn_take(i, j, file, rank)) {
						return true;
					}
				}
			}
		}
	}
	return false; //comes here if all squares are found not to threaten the king
}

bool Black_king::castle_left(int file_no, int rank_no, std::vector<Piece*> &vector_of_pieces) {
	if (rank_no != 8 || file_no != 3) { return false; }
	if (number_of_moves_made == 0 && get_notation_letter(find_piece(1, 8, vector_of_pieces)) == 'r') {
		if (board[2][8] == ' ' && board[3][8] == ' ' && board[4][8] == ' ') {
			for (int i = 5; i >= 3; i--) {
				change_board_position(i, 8);
				if (is_in_check()) { // check each square king moves across whilst castling to ensure he doesn't get checked
					change_board_position(5, 8);
					return false;
				}
			}
			//if each position doesn't yield check, move rook too
			find_piece(1, 8, vector_of_pieces)->change_board_position(4, 8);
			return true;
		}
		else { return false; }
	}
}

bool Black_king::castle_right(int file_no, int rank_no, std::vector<Piece*> &vector_of_pieces) {
	if (number_of_moves_made == 0 && get_notation_letter(find_piece(8, 8, vector_of_pieces)) == 'r') {
		if (board[6][8] == ' ' && board[7][8] == ' ') {
			for (int i = 5; i <= 7; i++) {
				change_board_position(i, 8);
				if (is_in_check()) { // check each square king moves across whilst castling to ensure he doesn't get checked
					change_board_position(5, 8);
					return false;
				}
			}
			//if each position doesn't yield check, move rook too
			find_piece(8, 8, vector_of_pieces)->change_board_position(6, 8);
			return true;
		}
		else { return false; }
	}
}