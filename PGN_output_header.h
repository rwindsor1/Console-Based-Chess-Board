#pragma once
#ifndef MY_CLASS_H3 // Will only be true the once!
#define MY_CLASS_H3
#include<iostream>
#include<fstream>
#include<string>

//this header contains functions used in creates the .PGN output file at the end of the game.

//opens file and initialises tags under name output_file.png
namespace filewrite {
	//asks user for file name to save game under and player's names
	std::ofstream make_PGN_file();
	//used to create a PGN file with output_file.PGN as the file name and adds player names to PGN file
	std::ofstream create_PGN_file(std::string output_file, std::string player1, std::string player2);
	//function called to write down a normal move in the .PGN file move list
	void write_normal_move(char piece_being_moved_notation_letter, int number_of_moves_made, int new_file_no, int new_rank_no, std::ofstream &file,bool is_check);
	//function called to write down a take move in the .PGN file move list
	void write_take_move(char piece_being_moved_notation_letter, int number_of_moves_made, int new_file_no, int new_rank_no, std::ofstream &file, bool is_check);
	//writes 0-0-0 to the PGN move list to symbolise left, queenside castle
	void write_castle_left(int number_of_moves_made, std::ofstream &file, bool is_check);
	//writes 0-0 to the PGN move list to symbolise right, kingside castle
	void write_castle_right(int number_of_moves_made, std::ofstream &file, bool is_check);
}
#endif