#include <fstream>
#include <sstream>
#include <iostream>
#include <ostream>
#include <vector>
#include "Stadium.h"

using namespace std;

int main() {
	ifstream file;
	file.open("inputs.txt"); // file's name is inputs.txt
	
	ofstream ofs("output.txt", ofstream::out); // out file should be output.txt

	if(file.is_open() == false) {
		cout << "Couldn't open the file!" << endl;
		file.close();
		ofs.close();
		return 0;
	}

	vector <string> blocks, rows;
	string line, word;
	getline(file, line);
	
	istringstream blockLine(line);
	while(blockLine >> word)
		blocks.push_back(word);

	getline(file, line);
	istringstream rowLine(line);
	while(rowLine >> word)
		rows.push_back(word);

	int number;
	getline(file, line);
	istringstream columnLine(line);
	columnLine >> number;

	Stadium stad(blocks, rows, number);
	
	while(getline(file, line)) {
		istringstream ssLine(line);
		ssLine >> word;
		if(word == "print") {
			stad.print(ofs);
		} else if(word == "reserve_seat") {
			string name, block, row;
			int column;

			ssLine >> name >> block >> row >> column;
			stad.reserve_seat(name, block, row, column, ofs);
		} else if(word == "cancel_reservation") {
			string name;
			ssLine >> name;

			stad.cancel_reservation(name, ofs);
		} else if(word == "get_seat") {
			string name;
			ssLine >> name;

			stad.get_seat(name, ofs);
		} else if(word == "reserve_seat_by_row") {
			string name, row;
			ssLine >> name >> row;

			stad.reserve_seat_by_row(name, row, ofs);
		}
		word = "";
	}

	file.close();
	ofs.close();


	return 0;
}