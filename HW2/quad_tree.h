#ifndef CITY_QT
#define CITY_QT

#include <string>
#include <vector>

using namespace std;

struct node {
	string name;
	int x_cord, y_cord;
	node * NW;
	node * NE;
	node * SW;
	node * SE;

	node() {NW = NULL; NE = NULL; SW = NULL; SE = NULL;}
	node(int x, int y, string n) {x_cord = x; y_cord = y; name = n; NW = NULL; NE = NULL; SW = NULL; SE = NULL;}
};

class QuadTree {
private:
	node * root;
	vector <string> visited;
	vector <string> inRad;

public:
	node * insert(node * current, string name, int x, int y);
	void search(node * current, int x, int y, int r);
	void pretty_print(node * current);
	void print();
	void del(node * cur);
	bool in_radius(int x, int y, int r, node * c);
	node * returnRoot();
	
	QuadTree();
	~QuadTree();
};


#endif