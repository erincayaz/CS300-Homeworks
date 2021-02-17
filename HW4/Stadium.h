#ifndef _STADIUM_H
#define _STADIUM_H

#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>

using namespace std;

class Stadium {
public:
	Stadium(vector <string>, vector <string>, int);
	void print(ofstream &);
	void reserve_seat_by_row(string name, string row, ofstream &);
	void reserve_seat(string name, string block, string row, int column, ofstream &);
	void get_seat(string name, ofstream &);
	void cancel_reservation(string name, ofstream &);

private:
	unordered_map <string, unordered_map<string, unordered_map<int, string>>> seats; // all of the seats
	unordered_map <string, vector< pair <string, int> >> priority_queue; // for min row management
	unordered_map <string, tuple <string, string, int>> reservations; // reservations made by people
	unordered_map <string, unordered_map<string, int>> index; // to access indexes of row's blocks
	unordered_map <string, int> orders; // orders of blocks (if "A" is the first block its order will be 0)
};



#endif