#include <iostream>
#include <iomanip>
#include "my_heap.h"

using namespace std;

My_heap::My_heap() {
	heap_begin = NULL;
	blk = NULL;
	used_bytes = 0;
}

memory_block * My_heap::bump_allocate(int num_bytes) {
	memory_block * temp;

	if(used_bytes + num_bytes > MAX_CAPACITY)
		return NULL;
	else if(heap_begin == NULL) {
		temp = new memory_block;
		temp->left = NULL;
		temp->right = NULL;
		temp->size = num_bytes;
		temp->used = true;
		temp->starting_address = 0;
		
		heap_begin = temp;
		blk = temp;
		used_bytes += num_bytes;
	} else {
		temp = blk;
		
		temp->right = new memory_block;
		temp = temp->right;
		temp->left = blk;
		temp->right = NULL;
		temp->size = num_bytes;
		temp->used = true;
		temp->starting_address = used_bytes;

		blk = temp;
		used_bytes += num_bytes;
	}
	return temp;
}

void My_heap::deallocate(memory_block * to_delete) {
	if(to_delete == NULL)
		return;
	
	memory_block * temp = heap_begin;
	
	if(to_delete == temp) {
		used_bytes -= temp->size;
		if(temp->used == false)
			return;

		temp->used = false;
		if(temp->right->used == false) {
			temp = temp->right;
			heap_begin->size += temp->size;
			heap_begin->right = temp->right;
			delete temp;
		}
	} else if(to_delete == blk) {
		if(blk->used == false)
			return;

		used_bytes -= blk->size;
		blk->used = false;
		if(blk->left->used == false) {
			temp = blk;
			blk = blk->left;
			blk->size += temp->size;
			blk->right = NULL;
			delete temp;
		}
	} else {
		while(temp != to_delete) {
			temp = temp->right;
		}
		if(temp->used == false)
			return;

		temp->used = false;
		used_bytes -= temp->size;
		if(temp->right->used == false) {
			memory_block * next_temp = temp->right;
			temp->size += next_temp->size;
			temp->right = next_temp->right;
			if(next_temp->right != NULL)
				next_temp->left = temp;
			else
				blk = temp;
			delete next_temp;
		}

		if(temp->left->used == false) {
			memory_block * before_temp = temp->left;
			before_temp->size += temp->size;
			before_temp->right = temp->right;
			if(temp->right != NULL)
				temp->right->left = before_temp;
			else
				blk = before_temp;
			delete temp;
		}
	}
}

memory_block * My_heap::first_fit_allocate(int num_bytes) {
	memory_block * temp = heap_begin;
	while(temp != NULL && !(temp->used == false && temp->size >= num_bytes))  {
		temp = temp->right;
	}

	if(temp == NULL) {
		return bump_allocate(num_bytes);
	} else {
		temp->used = true;
		used_bytes += temp->size;
	}
	return temp;
}

memory_block * My_heap::best_fit_allocate(int num_bytes) {
	memory_block * temp = heap_begin;
	int min = INT_MAX;
	memory_block * min_memory = NULL;
	while(temp != NULL) {
		if(temp->used == false && temp->size >= num_bytes && temp->size <= min) {
			min = temp->size;
			min_memory = temp;
		}
		temp = temp->right;
	}

	if(min_memory == NULL) {
		return bump_allocate(num_bytes);
	} else {
		min_memory->used = true;
		used_bytes += min_memory->size;
	}
	return min_memory;
}

memory_block * My_heap::first_fit_split_allocate(int num_bytes) {
	memory_block * temp = first_fit_allocate(num_bytes);

	if(temp == NULL)
		return NULL;
	else if(num_bytes == temp->size) {
		return temp;
	} else {
		int split_byte = temp->size - num_bytes;
		memory_block * split_temp = new memory_block;
		split_temp->left = temp;
		split_temp->right = temp->right; 
		if(temp->right != NULL)
			temp->right->left = split_temp;
		split_temp->size = split_byte; temp->size = num_bytes;
		split_temp->starting_address = temp->starting_address + temp->size;
		split_temp->used = false; temp->used = true;

		temp->right = split_temp;
		used_bytes -= split_temp->size;
	}
	return temp;
}

float My_heap::get_fragmantation() {
	int free_memory = MAX_CAPACITY - used_bytes;
	int biggest_free_block = 0;
	
	if(heap_begin == NULL) {
		biggest_free_block = free_memory;
	} else {
		memory_block * temp = heap_begin;
		int number = 0;
		while(temp->right != NULL) {
			if(temp->used == false) {
				number += temp->size;
			} else {
				if(biggest_free_block < number) {
					biggest_free_block = number;
				}
				number = 0;
			}
			temp = temp->right;
		}
		if(MAX_CAPACITY - (blk->starting_address + blk->size) > biggest_free_block) {
			biggest_free_block = MAX_CAPACITY - (blk->starting_address + blk->size);
		}
	}
	float fragmantation = ((free_memory - biggest_free_block) * 1.0) / (free_memory * 1.0) * 100.0;

	return fragmantation;
}

void My_heap::print_heap() {
	cout << "Maximum capacity of heap: " << MAX_CAPACITY << "B" << endl;
	cout << "Currently used memory (B): " << used_bytes << endl;

	int count = 0, used_count = 0;
	memory_block * temp = heap_begin;
	while(temp != NULL) {
		if(temp->used == true)
			used_count++;

		temp = temp->right;
		count++;
	}

	cout << "Total memory blocks: " << count << endl;
	cout << "Total used memory blocks: " << used_count << endl;
	cout << "Total free memory blocks: " << count - used_count << endl;

	cout << "Fragmentation: " << get_fragmantation() << "%" << endl;
	cout << "------------------------------" << endl;
	
	count = 0;
	temp = heap_begin;
	while(temp != NULL) {
		cout << "Block " << count << "\t\t Used: ";
		if(temp->used)
			cout << "True\t";
		else
			cout << "False\t";

		cout << "Size (B): " << temp->size << "\t";
		cout << "Starting Address: 0x";

		cout << hex << temp->starting_address << dec << endl;
		
		temp = temp->right;
	}
	cout << "------------------------------" << endl;
	cout << "------------------------------" << endl;
}

My_heap::~My_heap() {
	if(heap_begin == NULL)
		return;
	
	memory_block * temp = heap_begin;
	int count = 0;
	while(temp != NULL) {
		if(temp->used == true) {
			count += temp->size;
		}
		memory_block * to_be_deleted = temp;
		temp = temp->right;
		delete to_be_deleted;
	}
	cout << "At destruction, the heap had a memory leak of " << count << " bytes" << endl;
}

