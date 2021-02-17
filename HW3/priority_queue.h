#ifndef _PRIORTY_QUEUE_H
#define _PRIORTY_QUEUE_H

#include <vector>

using namespace std;

struct Comparable {
	int label;
	int height;
	int end;

	Comparable() {
		label = -1;
		height = -1;
		end = -1;
	}
		
	Comparable(int l, int h, int e) {
		label = l;
		height = h;
		end = e;
	}
};

class PriorityQueue {
public:
	PriorityQueue();
	PriorityQueue(int n);
	void insert(Comparable value, int label);
	Comparable remove(int label);
	Comparable getMax();
	bool isEmpty();

private:
	vector <Comparable> heap;
	vector <int> locations;
};


#endif