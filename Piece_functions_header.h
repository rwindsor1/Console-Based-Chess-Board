#pragma once
#ifndef MY_CLASS_H // Will only be true the once!
#define MY_CLASS_H

#include <string>
#include <vector>

class Piece; //forward decleration so some functions can use piece before it is defined
//---------------------------------------------------PIECE-SPECIFIC NON-MEMBER-FUNCTIONS---------------------------------


//used to check that the piece being moved is of the correct colour for that move, i.e. if move number odd, white moves, else black moves
bool check_colour(Piece* piece_being_moved, int number_of_moves_made);
// Returns true if the king of the colour making a move is in check. If not, returns false
bool king_is_in_check(std::vector<Piece*> vector_of_pieces, int number_of_moves_made);


void print_horizontal_line();
//loops through every possible move to see if it is check mate
bool check_if_checkmate(std::vector<Piece*> vector_of_pieces, int number_of_moves_made);
//--------------------------------------------------------PIECE SPECIFIC FUNCTIONS---------------------------------
//abstract piece base class & declaration of associated friend functions
class Piece {
	// sets up board in its starting position
	friend void setup_board(std::vector<Piece*> & piecesvector);
	// returns what character for square in Piece::board array
	friend char square_occupancy(int rankno, int fileno);
	//used to print out current state of the board
	friend void print_board();
	//returns the pointer of the piece at file = file_no and rank = rank_no. If there
	//is no piece there returns a null pointer
	friend Piece* find_piece(int file_no, int rank_no, std::vector<Piece*> vector_of_pieces);
	//finds king of colour making move
	friend Piece* find_king_of_colour_making_move(std::vector<Piece*> vector_of_pieces, int number_of_moves);
	//returns the notation letter of a Piece pointer
	friend char get_notation_letter(Piece* chosen_piece);

	friend void remove_piece_from_board(int file_no, int rank_no, std::vector<Piece*> vector_of_pieces);
	//returns true if move is a legal bishop move i.e. is diagonal and there are no pieces in the way
	friend bool is_legal_bishop_move(int file, int rank, int new_file, int new_rank);
	//returns true if move is a legal rook move i.e. is horizontal or vertical and there are no pieces in the way
	friend bool is_legal_rook_move(int file, int rank, int new_file, int new_rank);
	//returns true if move is a legal knight move i.e. is horizontal or vertical and there are no pieces in the way
	friend bool is_legal_knight_move(int file, int rank, int new_file, int new_rank);
	//returns true if move is a legal queen move, i.e. is a legal bishop or rook move
	friend bool is_legal_queen_move(int file, int rank, int new_file, int new_rank);
	//returns true if move is a legal king move, i.e. is a legal bishop or rook move and moves less than 1 space
	friend bool is_legal_king_move(int file, int rank, int new_file, int new_rank);
	//returns true if move is a legal taking move for a black pawn
	friend bool is_legal_black_pawn_take(int file, int rank, int new_file, int new_rank);
	//returns true if move is a legal taking move for a white pawn
	friend bool is_legal_white_pawn_take(int file, int rank, int new_file, int new_rank);
protected:
	static char board[9][9];
	int rank, file; // integers from 1 - 8 telling the position of the piece on the board
	int number_of_moves_made; //tells us how many moves the piece has made
	char notation_letter; // letter to distinguish piece
	bool has_been_captured; //initialised as FALSE, changed to TRUE when piece is taken, stops further moves
public:
	bool is_taken();
	void capture_this_piece();
	void set_piece_state_to_not_captured();
	virtual bool move(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces) = 0; // used to move piece
	virtual bool take(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces) = 0; //used to take another piece
	Piece();
	virtual ~Piece() = 0;
	virtual void print_position();//prints out location of piece
	virtual int return_number_of_moves(); //tells us how many moves a piece has made
	int get_file_number();
	int get_rank_number();
	virtual bool is_in_check() { return false; };
	bool move_if_white_not_in_check( int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces);
	bool move_if_black_not_in_check( int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces);
	bool move_if_check_not_caused(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces);
	void change_board_position(int new_file, int new_rank); //chnges the piece's board position
	bool is_of_colour_to_move( int move_number);
	virtual bool castle_right(int file_no, int rank_no, std::vector<Piece*> &vector_of_pieces);
	virtual bool castle_left(int file_no, int rank_no, std::vector<Piece*> &vector_of_pieces);
};

//Pawn Classes
class Pawn : public Piece {
public:
	Pawn(); // declare default constructor for pawn
	~Pawn();//destructor for pawn	
};

class White_pawn : public Pawn {
public:
	White_pawn(); //default constructor for white pawns
	White_pawn(int rankno, int fileno);
	bool move(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces); //function to move pawn
	bool take(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces); //function to take other piece
	void promote_if_necessary(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces); //promotes the piece if it is on the last rank
};

class Black_pawn : public Pawn {
public:
	Black_pawn(); //default constructor for white pawns
	Black_pawn(int fileno, int rankno);
	bool move(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces); //function to move pawn
	bool take(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces); //function to take other piece
	void promote_if_necessary(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces); //promotes the piece if it is on the first rank
};

//Rook Classes
class Rook : public Piece {
public:
	Rook(); //declares default constructor for rook
	~Rook();
	bool move(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces); //function to move rook

};

class White_rook : public Rook {
public:
	White_rook(int file_no, int rank_no);
	bool take(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces); //function to take other piece
	~White_rook() {}
};

class Black_rook : public Rook {
public:
	Black_rook(int file_no, int rank_no);
	bool take(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces);
	~Black_rook() {}
};

//Bishop Classes
class Bishop : public Piece {
public:
	//default constructor for a bishop. Called everytime White or black bishop is instanced
	Bishop();
	//destructor for bishop
	~Bishop();
	//moves bishop from board[file][rank] to
	//board[new_file][new_rank]. Checks move is legal and nothing is in the way first
	bool move(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces);
};

class White_bishop : public Bishop {
public:
	//parameterised constructor for a white bishop. places it on board[file_no][rank_no]
	White_bishop(int file_no, int rank_no);
	//checks there is a black piece on board[file_no][rank_no]. 
	//if there is, moves bishop to this square and deletes old piece
	//otherwise prints error message
	bool take(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces);

	~White_bishop() {}
};

class Black_bishop : public Bishop {
public:
	//parameterised constructor for a black bishop. places it on board[file_no][rank_no]
	Black_bishop(int file_no, int rank_no);
	//checks there is a white piece on board[file_no][rank_no]. 
	//if there is, moves bishop to this square and deletes old piece
	//otherwise prints error message
	bool take(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces);
	//destructor for white bishop
	~Black_bishop() {}
};

//Queen Classes
class Queen : public Piece {
public:
	Queen(); // default constructor for a queen. Called everytime queen is instanced
	~Queen(); //destructor for queen
			  //moves queen to board[new_file][new_rank] if the move is legal
	bool move(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces);
};

class White_queen : public Queen {
public:
	//parameterised constructor for a white queen. places it on board[file_no][rank_no]
	White_queen(int file_no, int rank_no);
	//checks there is a black piece on board[file_no][rank_no]. 
	//if there is, moves queen to this square and deletes old piece
	//otherwise prints error message
	bool take(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces);
	~White_queen() {}
};

class Black_queen : public Queen {
public:
	//parameterised constructor for a white queen. places it on board[file_no][rank_no]
	Black_queen(int file_no, int rank_no);
	//checks there is a black piece on board[file_no][rank_no]. 
	//if there is, moves queen to this square and deletes old piece
	//otherwise prints error message
	bool take(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces);
	~Black_queen() {}
};

//Knight Classes
class Knight : public Piece {
public:
	Knight();
	~Knight();
	bool move(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces);
};

class White_knight : public Knight {
public:
	//parameterised constructor for a white knight. places it on board[file_no][rank_no]
	White_knight(int file_no, int rank_no);
	//checks there is a black piece on board[file_no][rank_no]. 
	//if there is, moves knight to this square and deletes old piece
	//otherwise prints error message
	bool take(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces);
	//white knight destructor
	~White_knight() {}
};

class Black_knight : public Knight {
public:
	//parameterised constructor for a black knight. places it on board[file_no][rank_no]
	Black_knight(int file_no, int rank_no);
	//checks there is a white piece on board[file_no][rank_no]. 
	//if there is, moves knight to this square and deletes old piece
	//otherwise prints error message
	bool take(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces);
	//white knight destructor
	~Black_knight() {}
};

//King Classes
class King : public Piece {
public:
	King(); //default constructor for king, empty function
	~King(); //destructor for king
			 //moves king to board[new_file][new_rank] if the move is legal. returns true if move is carried out
	bool move(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces);
};

class White_king : public King {
public:
	//paramaterised constructor for a white king. places king on board[file_no][rank_no]
	White_king(int file_no, int rank_no);
	//checks there is a black piece on board[file_no][rank_no]. 
	//if there is, moves king to this square and deletes old piece
	//otherwise prints error message
	bool take(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces);
	//white king destructor
	~White_king() {}
	bool is_in_check(); //returns true if king is in check, false otherwise
	bool castle_left(int file_no, int rank_no, std::vector<Piece*> &vector_of_pieces);
	bool castle_right(int file_no, int rank_no, std::vector<Piece*> &vector_of_pieces);
};

class Black_king : public King {
public:
	//paramaterised constructor for a black king. places king on board[file_no][rank_no]
	Black_king(int file_no, int rank_no);
	//checks there is a black piece on board[file_no][rank_no]. 
	//if there is, moves king to this square and deletes old piece
	//otherwise prints error message
	bool take(int new_file, int new_rank, std::vector<Piece*> &vector_of_pieces);
	//white king destructor
	~Black_king() {}
	bool is_in_check(); //returns true if king is in check, false otherwise
	bool castle_right(int file_no, int rank_no, std::vector<Piece*> &vector_of_pieces);
	bool castle_left(int file_no, int rank_no, std::vector<Piece*> &vector_of_pieces);
};
#endif
