#pragma once
#ifndef MY_CLASS_H2 // Will only be true the once!
#define MY_CLASS_H2
#include"Piece_functions_header.h"
#include <string>
#include <vector>

//prints out the current state of the board
void print_board();
//sets colour and background of console
void SetColorAndBackground(int ForgC, int BackC = 0);
//prints text centered in the middle of the console
void print_centered_text(std::string String);
//prints out a new line
void new_line();
//prints intro message
void print_intro_message();
//returns true input_character is a,b,c,d,e,f,g or h
bool is_valid_letter(char input_character);
//returns true input_character is a digit from 1 to 8
bool is_valid_digit(char input_character);
//checks user input is valid (must be 2 characters long where first is letter and second is number from 1 - 8)
bool is_valid_input(std::string input_code);
//extracts file number from a valid input code
int get_file_no(std::string input_code);
//extracts rank number from a valid input code
int get_rank_no(std::string input_code);
// used to check if a notation letter corresponds to white
bool is_white(char notation_letter);
// used to check if a notation letter corresponds to black
bool is_black(char notation_letter);
// used to check menu input (only allows 1, 2 or 3 to be entered)
int get_menu_entry(int i);
//used to make a move
bool make_move(std::vector<Piece*> & vector_of_pieces, int number_of_moves_made, std::ofstream &file_to_write_to, bool record_game);
#endif