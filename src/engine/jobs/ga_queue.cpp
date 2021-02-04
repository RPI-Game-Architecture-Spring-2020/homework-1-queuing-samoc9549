/*
** RPI Game Architecture Engine
**
** Portions adapted from:
** Viper Engine - Copyright (C) 2016 Velan Studios - All Rights Reserved
**
** This file is distributed under the MIT License. See LICENSE.txt.
*/

#include "ga_queue.h"

ga_queue::ga_queue(int node_count)
{
	// TODO:
	// Initialize the queue.
	// For extra credit, preallocate 'node_count' elements (instead of
	// allocating on push).
	// See https://www.research.ibm.com/people/m/michael/podc-1996.pdf

	//Initialize the first node of th queue
	queue_node* node = new queue_node();
	node -> value = nullptr;

	//Initialize the queue, set its head and tail equal to the above node
	queue = new queue_q();
	queue -> head = node;
	queue -> tail = node;

	//Preallocate the queue size (node_count) and set each node's next node
	for(int i = 0; i < node_count; i++)  {
		queue_node* n = new queue_node();
		n -> value = nullptr;
		node -> next_node = n;
		node = n;
	}
}

ga_queue::~ga_queue()
{
	// TODO:
	// Free any resources held by the queue.
	// See https://www.research.ibm.com/people/m/michael/podc-1996.pdf

	//Lock both head and tail locks
	queue -> h_lock.lock();
	queue -> t_lock.lock();

	//Start at the head of the queue
	queue_node* current_node = queue -> head;

	//Iterate through the queue and delete each node
	while(current_node != nullptr)  {
		queue_node* node = current_node -> next_node;
		delete current_node;
		current_node = node;
	}

	//Unlock both locks
	queue -> t_lock.unlock();
	queue -> h_lock.unlock();
}

void ga_queue::push(void* data)
{
	// TODO:
	// Push 'data' onto the queue in a thread-safe manner.
	// If you preallocated 'node_count' elements, and if the queue is full when
	// this function is called, you must block until another thread pops an
	// element off the queue.
	// See https://www.research.ibm.com/people/m/michael/podc-1996.pdf

	//Push adds to the end; lock the tail lock
	queue -> t_lock.lock();

	//Make a new node, give it data, and make it the tail
	queue_node* new_node = new queue_node();
	new_node -> value = data;
	new_node -> next_node = nullptr;

	queue -> tail -> next_node = new_node;
	queue -> tail = queue -> tail -> next_node;

	//Unlock tail lock
	queue -> t_lock.unlock();
}

bool ga_queue::pop(void** data)
{
	// TODO:
	// Pop one element off the queue in a thread-safe manner and place it in
	// the memory pointed to by 'data'.
	// If the queue is empty when this function is called, return false.
	// Otherwise return true.
	// See https://www.research.ibm.com/people/m/michael/podc-1996.pdf

	//Pop adds to the front; lock the head lock
	queue -> h_lock.lock();

	//Head of queue stored in h_node, next queue_node after head stored in next_node
	queue_node* h_node = queue -> head;
	queue_node* next_node = h_node -> next_node;

	//If the head and tail are the same/next_node is null/*QUEUE IS EMPTY*, unlock and return false
	if(h_node == queue -> tail || next_node == nullptr)  {
		queue -> h_lock.unlock();
		return false;
	}

	//Put the popped head's value into data and set the head to be the next_node
	*data = next_node -> value;
	queue -> head = next_node;

	//Unlock the head lock
	queue -> h_lock.unlock();

	//Free the head that has been popped
	delete(h_node);
	return true;
}

int ga_queue::get_count() const
{
	// TODO:
	// Get the number of elements currently in the queue.

	//Stores how many nodes there are in the queue
	int node_count = 0;

	//Start at the head of the queue
	queue_node* current_node = queue -> head;

	//Iterate through the queue, adding one for each node
	while(current_node != queue -> tail)  {
		current_node = current_node -> next_node;
		node_count++;
	}
	
	return node_count;
}
