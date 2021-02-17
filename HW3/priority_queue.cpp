#include <iostream>
#include <vector>
#include "priority_queue.h"

using namespace std;

PriorityQueue::PriorityQueue() {
	vector <Comparable> a;
	heap = a;

	vector <int> b;
	locations = b;
}


bool PriorityQueue::isEmpty() {
	if(heap.size() == 0)
		return true;
	return false;
}

void PriorityQueue::insert(Comparable value, int l) {
	if(isEmpty()) {
		heap.push_back(value);
		locations.push_back(0);
		return;
	}

	heap.push_back(value);
	locations.push_back(heap.size() - 1);
	Comparable temp;
	int i = (heap.size() - 1) / 2;
	int j = heap.size() - 1;
	
	// Percolate Up //
	while(i > 0 && heap[i].height < value.height) {
		temp = heap[i];
		heap[i] = value;
		heap[j] = temp;
		locations[l] = i;
		locations[temp.label] = j;

		j = i;
		i = (i - 1) / 2;
	}

	if(heap[i].height < value.height) {
		temp = heap[i];
		heap[i] = value;
		heap[j] = temp;
		locations[l] = i;
		locations[temp.label] = j;
	}

}

Comparable PriorityQueue::remove(int l) {
	int index = locations[l];
	if(index == heap.size() - 1) {
		Comparable temp = heap[index];
		heap.pop_back();
		locations[l] = -1;
		return temp;
	}

	Comparable removed = heap[index];
	heap[index] = heap[heap.size() - 1];
	heap.pop_back();
	locations[heap[index].label] = index;
	locations[removed.label] = -1;

	// Percolate Down
	int i = index;
	Comparable temp;
	while(i < heap.size()) {
		if(2*i + 1 == heap.size() - 1)
			bool s = false;
		else if((2*i + 1) >= heap.size() || (heap[index].height >= heap[2*i + 1].height && heap[index].height >= heap[2*i + 2].height))
			break;

		int other_index;
		if(2*i + 2 < heap.size())
			other_index = heap[2*i + 1].height > heap[2*i + 2].height ? 2*i + 1 : 2*i + 2;
		else
			other_index = 2*i + 1;

		temp = heap[index];
		heap[index] = heap[other_index];
		heap[other_index] = temp;

		locations[heap[index].label] = index;
		locations[heap[other_index].label] = other_index;

		i = other_index;
	}

	return removed;
}

Comparable PriorityQueue::getMax() {
	return heap[0];
}

