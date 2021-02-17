#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "priority_queue.h"

using namespace std;

void swapp(vector <vector<int>> & a, int i, int j) {
	vector <int> temp = a[i];
	a[i] = a[j];
	a[j] = temp;
}

inline int leftChild(int i) {
	return 2*i + 1;
}

void percDown(vector <vector <int>> & vec, int i, int n) {
	int child;
	vector <int> temp;

	for(temp = vec[i]; leftChild(i) < n; i = child) {
		child = leftChild(i);
		if(child != n-1 && vec[child][0] < vec[child + 1][0])
			child++;
		if(vec[child][0] > temp[0])
			vec[i] = vec[child];
		else
			break;
	}
	vec[i] = temp;
}

void heapSort(vector <vector <int>> & a) {  // Heap sort is taken from slides.
	for(int i = a.size()/2; i >= 0; i--)
		percDown(a, i, a.size());

	for(int j = a.size() - 1; j > 0; j--) {
		swap(a[0], a[j]);
		percDown(a, 0, j);
	}
}


int main() {
	ifstream file;
	file.open("input.txt"); // File's name is "input.txt"

	if(!file.is_open()) {
		cout << "Couldn't open the file!" << endl;
		file.close();
		return 0;
	}

	int number;
	string line, word;
	getline(file, line);
	istringstream sLine(line);
	sLine >> number;

	int xLeft, y, xRight;
	vector <vector <int>> coordinates(number, vector <int>(3));
	int i = 0;
	while(getline(file, line)) {
		istringstream ssLine(line);
		ssLine >> xLeft >> y >> xRight;
		coordinates[i][0] = xLeft; coordinates[i][1] = y; coordinates[i][2] = xRight;
		i++;
	}

	heapSort(coordinates);

	// I didn't follow the logic that told in the assignment paper, I don't know why but I couldn't understand it when I wrote this code. Instead I wrote a more complicated one
	// To sum up this code not look if the taken coordinate is begging or not to insert them or remove them but instead only look the beggining of the upcoming and ending of the heap max's coordinates to compare them.
	// For example if in the heap we have a coordinate which's ending coordinate is 10, and a coordinate which start at 15. This code compare Heap's max's ending coordinate with the upcoming
	// coordinate's begginig and if begging is bigger than the max's ending coordinate then it will remove it and continue this process until heap is empty or heap's max's ending is bigger or equal to
	// upcoming's begginig. And if upcoming coordinate's beggining is not bigger than the max's ending then it will insert this ending and height of the upcoming one and continue this process until inserted coordinate
	// is not equal to cur_x. And then it will push the coordinate and height to res vector. Again I don't know why I didn't print them instead but right now I am too scared to change it. 
	vector<pair<int, int>> res;
	int cur = 0, cur_X, cur_H = -1;
	PriorityQueue heap;
	while(cur < number || !heap.isEmpty()) {
		cur_X = heap.isEmpty() ? coordinates[cur][0] : heap.getMax().end;

		if(cur >= number || coordinates[cur][0] > cur_X) {
			while(!heap.isEmpty() && (heap.getMax().end <= cur_X)) {
				int l = heap.getMax().label;
				heap.remove(l);
			}
		}

		else {
			cur_X = coordinates[cur][0];
			while(cur < number && coordinates[cur][0] == cur_X) {
				Comparable temp(cur, coordinates[cur][1], coordinates[cur][2]);
				heap.insert(temp, cur);
				cur++;
			}
		}
		cur_H = heap.isEmpty() ? 0 : heap.getMax().height;
		if(res.empty() || (res.back().second != cur_H) ) res.push_back(make_pair(cur_X, cur_H));
	}

	if(coordinates[0][0] != 0)
		cout << 0 << " " << 0 << endl;
	for(int i = 0; i < res.size(); i++) {
		cout << res[i].first << " " << res[i].second << endl;
	}
	
	file.close();
	return 0;
}