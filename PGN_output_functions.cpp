#include "PGN_output_header.h"
#include<iostream>
#include<fstream>
#include<string>
#include <time.h>  
#include<array>

//this file defines functions used in creates the .PGN output file at the end of the game.

std::array<char, 8> file_converter = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };

namespace filewrite {
	std::ofstream create_PGN_file(std::string output_file, std::string player1, std::string player2) {
		time_t rawtime; //get raw time from time.h
		struct tm timeinfo;
		//put time informatino into struct tm timeinfo
		time(&rawtime); 
		localtime_s(&timeinfo, &rawtime);
		std::ofstream file;
		file.open(output_file + ".PGN");
		//print out basic tag pairs for a PGN file
		file << "[Event \"Rhydian's Chess Game Match\"] \n"; 
		file << "[Site \"Manchester, LANCS GBR\"] \n";
		file << "[Date \""<<(timeinfo.tm_year + 1900)<<"."<< (timeinfo.tm_mon + 1)<<"."<< timeinfo.tm_mday<<"\"] \n";
		file << "[White \""<<player1<<"\"] \n";
		file << "[Black \""<<player2<<"\"] \n";
		file << " \n \n";
		return file;
	}

	std::ofstream make_PGN_file() {
		//asks user for information about the game and then ereate PGN fild
		std::string file_name, player_1, player_2;
		std::cout << "What would you like to save the file under?" << std::endl;
		getline(std::cin, file_name);
		std::cout << "Please enter the first name of player 1 (white):" << std::endl;
		getline(std::cin, player_1);
		std::cout << "Please enter the first name of player 2 (black):" << std::endl;
		getline(std::cin, player_2);
		return filewrite::create_PGN_file(file_name, player_1, player_2); //creates .PGN file using parameters obtained from user input
		
	}

	//writes move number before all white moves according to standard chess notation
	void write_move_number_if_necessary(std::ofstream &file, int number_of_moves_made) {
		if (number_of_moves_made % 2) { //if it's whites turn writes out move number
			int move_number = (number_of_moves_made - 1) / 2 + 1;
			if ((move_number % 5) == false) { file << "\n"; } //starts a new line every fifth move
			file << move_number << ". ";
		}
	}

	void write_normal_move(char piece_being_moved_notation_letter, int number_of_moves_made, int new_file_no, int new_rank_no, std::ofstream &file, bool is_check) {
		write_move_number_if_necessary(file, number_of_moves_made);
		if (piece_being_moved_notation_letter != 'p' && piece_being_moved_notation_letter != 'P') { 
			//if piece being moved isnt a pawn, print notation letter as upper case to move list
			file << (char)toupper(piece_being_moved_notation_letter);
		}
		file << file_converter[new_file_no - 1] << new_rank_no;
		if (is_check) { file << "+"; }
		file << " ";
	}
	
	void write_take_move(char piece_being_moved_notation_letter, int number_of_moves_made, int new_file_no, int new_rank_no, std::ofstream &file, bool is_check) {
		write_move_number_if_necessary(file, number_of_moves_made);
		file << (char)toupper(piece_being_moved_notation_letter) << "x" << file_converter[new_file_no - 1] << new_rank_no << " ";
		if (is_check) { file << "+"; }
		file << " ";
	
	}
	
	void write_castle_left(int number_of_moves_made, std::ofstream &file, bool is_check) {
		write_move_number_if_necessary(file, number_of_moves_made);
		file << "0-0-0 ";
		if (is_check) { file << "+"; }
		file << " ";
	}
	
	void write_castle_right(int number_of_moves_made, std::ofstream &file, bool is_check) {
		write_move_number_if_necessary(file, number_of_moves_made);
		file << "0-0 ";
		if (is_check) { file << "+"; }
		file << " ";
	}
}