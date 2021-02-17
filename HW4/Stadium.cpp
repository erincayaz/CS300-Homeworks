#include <iostream>
#include "Stadium.h"

using namespace std;

Stadium::Stadium(vector <string> blocks, vector <string> rows, int column) {
	for(int i = 0; i < blocks.size(); i++)
		for(int j = 0; j < rows.size(); j++)
			for(int k = 0; k < column; k++)
				seats[blocks[i]][rows[j]][k] = "---";
	
	for(int i = 0; i < blocks.size(); i++)
		orders[blocks[i]] = i;

	for(int i = 0; i < rows.size(); i++) {
		vector <pair <string, int>> temp;
		for(int j = 0; j < blocks.size(); j++) {
			temp.push_back(make_pair(blocks[j], column));
			orders[blocks[j]] = j;
		}
		priority_queue[rows[i]] = temp;
	}
}

void Stadium::print(ofstream & ofs) {
	ofs << endl;
	string temp = "";
	for(auto i : seats) {
		ofs << i.first << endl;
		ofs << "~~~~~~~" << endl;
		for(auto j : i.second) {
			ofs << j.first << " : ";
			for(auto k : j.second) {
				ofs << k.second << " ";
			}
			ofs << endl;
		}
		ofs << "=======" << endl << endl;
	}
}

void Stadium::get_seat(string name, ofstream & ofs) {
	if(reservations.find(name) == reservations.end()) {
		ofs << "There is no reservation made for " << name << endl;
		return;
	}

	tuple <string, string, int> temp = reservations[name];
	ofs << "Found that " << name << " has a reservation in " << get<0>(temp) << " " << get<1>(temp) << "-" << get<2>(temp) << endl;
}

void Stadium::reserve_seat(string name, string block, string row, int column, ofstream & ofs) {
	int idx = index[row][block];
	if(priority_queue[row][idx].second == 0 || reservations.find(name) != reservations.end() || seats[block][row][column] != "---") {
		ofs << name << " could not reserve a seat!" << endl;
		return;
	}

	tuple <string, string, int> temp;
	get<0>(temp) = block;
	get<1>(temp) = row;
	get<2>(temp) = column;
	reservations[name] = temp;
	seats[block][row][column] = name.substr(0, 3);

	priority_queue[row][idx] = make_pair(block, priority_queue[row][idx].second - 1);
	int i = idx;
	vector< pair <string, int>> arr = priority_queue[row];
	
	// Percolate Down
	// There is 2 while loop but it is still logB, because second while loop does not start over, it continue from the index where first while left
	// The reason I use 2 while loop is because otherwise it would be really difficult to manage all these conditions. So I created first loop to
	// percolate down by looking empty seats, the other while loop percolate down by looking orders (which is explained in the header).

	while(i < arr.size()) {
		if(2*i + 1 == arr.size() - 1)
			bool s = false;
		else if((2*i + 1) >= arr.size() || (arr[i].second >= arr[2*i + 1].second && arr[i].second >= arr[2*i + 2].second))
			break;

		int other_index;
		if(2*i + 2 < arr.size())
			if(arr[2*i + 1].second == arr[2*i + 2].second)
				other_index = orders[arr[2*i + 1].first] < orders[arr[2*i + 2].first] ? 2*i + 1 : 2*i + 2;
			else
				other_index = arr[2*i + 2].second > arr[2*i + 1].second ? 2*i + 2 : 2*i + 1;
		else
			other_index = 2*i + 1;

		index[row][arr[i].first] = other_index;
		index[row][arr[other_index].first] = i;
		
		pair <string, int> tmp = arr[i];
		arr[i] = arr[other_index];
		arr[other_index] = tmp;

		i = other_index;
	}

	while(i < arr.size()) {
		if(2*i + 1 == arr.size() - 1)
			bool s = false;
		else if((2*i + 1) >= arr.size() || (arr[i].second > arr[2*i + 1].second && arr[i].second > arr[2*i + 2].second) || (orders[arr[i].first] < orders[arr[2*i + 1].first] && orders[arr[i].first] < orders[arr[2*i + 2].first]))
			break;

		int other_index;
		if(2*i + 2 < arr.size()) {
			if(arr[2*i + 1].second == arr[2*i + 2].second)
				other_index = orders[arr[2*i + 1].first] < orders[arr[2*i + 2].first] ? 2*i + 1 : 2*i + 2;
			else
				other_index = arr[2*i + 1] == arr[i] ? 2*i + 1 : 2*i + 2;
		} else
			other_index = 2*i + 1;

		index[row][arr[i].first] = other_index;
		index[row][arr[other_index].first] = i;

		pair <string, int> tmp = arr[i];
		arr[i] = arr[other_index];
		arr[other_index] = tmp;

		i = other_index;
	}

	priority_queue[row] = arr;
	ofs << name << " has reserved " << block << " " << row << "-" << column << endl;
}

void Stadium::reserve_seat_by_row(string name, string row, ofstream & ofs) {
	if(priority_queue[row][0].second == 0 || reservations.find(name) != reservations.end()) {
		ofs << name << " could not reserve a seat!" << endl;
		return;
	}

	string block = priority_queue[row][0].first;

	for(auto i : seats[block][row]) {
		if(i.second == "---") {
			seats[block][row][i.first] = name.substr(0, 3);
			ofs << name << " has reserved " << block << " " << row << "-" << i.first << " by emptiest block" << endl;
			tuple <string, string, int> temp;
			get<0>(temp) = block;
			get<1>(temp) = row;
			get<2>(temp) = i.first;
			reservations[name] = temp;
			priority_queue[row][0] = make_pair(block, priority_queue[row][0].second - 1);

			break;
		}
	}

	int i = 0;
	vector< pair <string, int>> arr = priority_queue[row];

	// Percolate Down
	while(i < arr.size()) {
		if(2*i + 1 == arr.size() - 1)
			bool s = false;
		else if((2*i + 1) >= arr.size() || (arr[i].second >= arr[2*i + 1].second && arr[i].second >= arr[2*i + 2].second))
			break;

		int other_index;
		if(2*i + 2 < arr.size()) {
			if(arr[2*i + 1].second == arr[2*i + 2].second)
				other_index = orders[arr[2*i + 1].first] < orders[arr[2*i + 2].first] ? 2*i + 1 : 2*i + 2;
			else
				other_index = arr[2*i + 2].second > arr[2*i + 1].second ? 2*i + 2 : 2*i + 1;
		}
		else
			other_index = 2*i + 1;

		index[row][arr[i].first] = other_index;
		index[row][arr[other_index].first] = i;
		
		pair <string, int> tmp = arr[i];
		arr[i] = arr[other_index];
		arr[other_index] = tmp;

		i = other_index;
	}

	while(i < arr.size()) {
		if(2*i + 1 == arr.size() - 1)
			bool s = false;
		else if((2*i + 1) >= arr.size() || (arr[i].second > arr[2*i + 1].second && arr[i].second > arr[2*i + 2].second) || (orders[arr[i].first] < orders[arr[2*i + 1].first] && orders[arr[i].first] < orders[arr[2*i + 2].first]))
			break;

		int other_index;
		if(2*i + 2 < arr.size()) {
			if(arr[2*i + 1].second == arr[2*i + 2].second)
				other_index = orders[arr[2*i + 1].first] < orders[arr[2*i + 2].first] ? 2*i + 1 : 2*i + 2;
			else
				other_index = arr[2*i + 1] == arr[i] ? 2*i + 1 : 2*i + 2;
		} else
			other_index = 2*i + 1;

		index[row][arr[i].first] = other_index;
		index[row][arr[other_index].first] = i;

		pair <string, int> tmp = arr[i];
		arr[i] = arr[other_index];
		arr[other_index] = tmp;

		i = other_index;
	}

	priority_queue[row] = arr;
}

void Stadium::cancel_reservation(string name, ofstream & ofs) {
	if(reservations.find(name) == reservations.end()) {
		ofs << "Could not cancel the reservation for " << name << "; no reservation found!" << endl;
		return;
	}

	tuple <string, string, int> temp = reservations[name];
	string block = get<0>(temp), row = get<1>(temp);
	int column = get<2>(temp);

	reservations.erase(name);
	seats[block][row][column] = "---";
	ofs << "Cancelled the reservation of " << name << endl;

	int idx = index[row][block];
	priority_queue[row][idx] = make_pair(block, priority_queue[row][idx].second + 1);

	vector< pair <string, int>> arr = priority_queue[row];
	int i = idx, j = (i - 1)/ 2;

	while(j > 0 && arr[j].second < arr[i].second) {
		pair <string, int> tmp = arr[j];

		index[row][arr[i].first] = j;
		index[row][arr[j].first] = i;

		arr[j] = arr[i];
		arr[i] = tmp;
		

		i = j;
		j = (j - 1) / 2;
	}

	if(arr[j].second < arr[i].second) {
		pair <string, int> tmp = arr[j];

		index[row][arr[i].first] = j;
		index[row][arr[j].first] = i;

		arr[j] = arr[i];
		arr[i] = tmp;

		i = j;
		j = (j - 1) / 2;
	}

	while(j > 0) {
		if(arr[j].second < arr[i].second)
			break;
		else if(arr[i].second == arr[j].second && orders[arr[i].first] > orders[arr[j].first])
			break;

		pair <string, int> tmp = arr[j];
		index[row][arr[i].first] = j;
		index[row][arr[j].first] = i;

		arr[j] = arr[i];
		arr[i] = tmp;

		i = j;
		j = (j - 1) / 2;
	}

	if(arr[j].second == arr[i].second && orders[arr[i].first] < orders[arr[j].first]) {
		pair <string, int> tmp = arr[j];
		index[row][arr[i].first] = j;
		index[row][arr[j].first] = i;

		arr[j] = arr[i];
		arr[i] = tmp;

		i = j;
		j = (j - 1) / 2;
	}

	priority_queue[row] = arr;
}

