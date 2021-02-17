#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "quad_tree.h"
#include <vector>

using namespace std;

ifstream txtOpener() {
	ifstream a;
	string fileName;
	cin >> fileName;
	do {
		a.open(fileName.c_str());
		if(a.fail()) {
			cout << "Cannot find a file named " << fileName << endl;
			cout << "Please enter file name: ";
			cin >> fileName;
		} else
			break;
	} while(!a.fail());

	return a;
}

int main() {
	ifstream city_input = txtOpener();
	QuadTree tree;

	string line, city;
	int maxX, maxY;
	getline(city_input, line);
	istringstream ssLine(line);
	ssLine >> maxX >> maxY;

	int x, y;
	while(getline(city_input, line)) {
		istringstream sLine(line);
		sLine >> city >> x >> y;
		if(!(x < 0 || y < 0 || x > maxX || y > maxY)) {
			node * root = tree.returnRoot();
			root = tree.insert(root, city, x, y);
		}
	}

	tree.pretty_print(tree.returnRoot());

	ifstream queries_input = txtOpener();
	int xi, yi, ri;
	string xs, ys, rs;
	while(getline(queries_input, line)) {
		istringstream sssLine(line);
		sssLine >> xs >> ys >> rs;
		xs = xs.substr(0, xs.length() - 1); ys = ys.substr(0, ys.length() - 1);
		xi = stoi(xs); yi = stoi(ys); ri = stoi(rs);

		tree.search(tree.returnRoot(), xi, yi, ri);
		tree.print();
		cout << endl;
	}

	return 0;
}