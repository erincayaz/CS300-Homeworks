#include "quad_tree.h"
#include <iostream>
#include <string>

using namespace std;

QuadTree::QuadTree() {
	root = NULL;
}

node * QuadTree::insert(node * c, string n, int x, int y) {
	if(root == NULL) {
		root = new node(x, y, n);
		return root;
	}
	
	if(c == NULL) {
		c = new node(x, y, n);	
	} else if(x < c->x_cord) {
		if(y < c->y_cord)
			c->SW = insert(c->SW, n, x, y);
		else
			c->NW = insert(c->NW, n, x, y);

	} else {
		if(y < c->y_cord)
			c->SE = insert(c->SE, n, x, y);
		else
			c->NE = insert(c->NE, n, x, y);
	}
	return c;
}

bool QuadTree::in_radius(int x, int y, int r, node * c) {
	int side1 = c->x_cord - x, side2 = c->y_cord - y;
	side1 *= side1; side2 *= side2;
	return (side1 + side2 <= r*r) ? true : false;
}

void QuadTree::search(node * c, int x, int y, int r) {
	if(c != NULL) {
		visited.push_back(c->name);
		if(in_radius(x, y, r, c)) { // 13
			inRad.push_back(c->name);
			search(c->SE, x, y, r);
			search(c->SW, x, y, r);
			search(c->NE, x, y, r);
			search(c->NW, x, y, r);
		} else {
			if((x + r) > c->x_cord && c->x_cord > x) { // 10 - 12
				if((y + r) > c->y_cord && c->y_cord > y) { // 10
					search(c->SE, x, y, r);
					search(c->SW, x, y, r);
					search(c->NW, x, y, r);
				} else if((y - r) < c->y_cord && y > c->y_cord){ // 12
					search(c->SW, x, y, r);
					search(c->NE, x, y, r);
					search(c->NW, x, y, r);
				}
			} else if((x - r) < c->x_cord && x > c->x_cord) { // 9 - 11
				if((y + r) > c->y_cord && c->y_cord > y) { // 9
					search(c->SE, x, y, r);
					search(c->SW, x, y, r);
					search(c->NE, x, y, r);
				} else if((y - r) < c->y_cord && y > c->y_cord) { // 11
					search(c->SE, x, y, r);
					search(c->NE, x, y, r);
					search(c->NW, x, y, r);
				}
			}

			if((y + r) < c->y_cord && c->x_cord < (x + r) && c->x_cord > (x - r)) { // 2
				search(c->SE, x, y, r);
				search(c->SW, x, y, r);
			} else if((y - r) > c->y_cord && c->x_cord < (x + r) && c->x_cord > (x - r)) { // 7
				search(c->NE, x, y, r);
				search(c->NW, x, y, r);
			}
			else if((x + r) < c->x_cord && c->y_cord < (y + r) && c->y_cord > (y - r)) { // 5
				search(c->SW, x, y, r); 
				search(c->NW, x, y, r);
			} else if((x - r) > c->x_cord && c->y_cord < (y + r) && c->y_cord > (y - r)) { // 4
				search(c->SE, x, y, r);
				search(c->NE, x, y, r);
			}

			if((x - r) > c->x_cord) { // 1 - 6
				if((y + r) < c->y_cord) // 1
					search(c->SE, x, y, r);
				else if((y - r) > c->y_cord) // 6
					search(c->NE, x, y, r);
			} else if((x + r) < c->x_cord) { // 3 - 8
				if((y + r) < c->y_cord) // 3
					search(c->SW, x, y, r);
				else if((y - r) > c->y_cord) // 8
					search(c->NW, x, y, r);
			}
		}
	}
}

void QuadTree::pretty_print(node * cur) {
	if(cur != NULL) {
		cout << cur->name << endl;
		pretty_print(cur->SE);
		pretty_print(cur->SW);
		pretty_print(cur->NE);
		pretty_print(cur->NW);
	}
}

void QuadTree::print() {
	if(inRad.empty())
		cout << "<None>" << endl;
	else {
		for(int i = 0; i < inRad.size(); i++) {			
			if(i == inRad.size() - 1)
				cout << inRad[i];
			else
				cout << inRad[i] << ", ";
		}
		cout << endl;
	}
		

	for(int i = 0; i < visited.size(); i++) {			
		if(i == visited.size() - 1)
			cout << visited[i];
		else
			cout << visited[i] << ", ";
	}
	cout << endl;

	inRad.clear();
	visited.clear();
}

node * QuadTree::returnRoot() {
	return root;
}

void QuadTree::del(node * cur) {
	if(cur != NULL) {
		del(cur->SE);
		del(cur->SW);
		del(cur->NE);
		del(cur->NW);

		delete cur;
	}
}

QuadTree::~QuadTree() {
	del(root);
}