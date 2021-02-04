#pragma once
#include <mutex>
#include <thread>

typedef std::mutex lock;

/*
** RPI Game Architecture Engine
**
** Portions adapted from:
** Viper Engine - Copyright (C) 2016 Velan Studios - All Rights Reserved
**
** This file is distributed under the MIT License. See LICENSE.txt.
*/

/*
** Thread-safe queue.
** https://www.research.ibm.com/people/m/michael/podc-1996.pdf
*/
class ga_queue
{
public:

	//Structs for a pointer, node in the queue, and the queue itself
	struct queue_node;
	struct queue_q;

	struct queue_node {
		void* value;
		queue_node* next_node;
	};

	struct queue_q  {
		queue_node* head;
		queue_node* tail;
		lock h_lock;
		lock t_lock;
	};

	queue_q* queue;

	ga_queue(int node_count);
	~ga_queue();

	void push(void* data);
	bool pop(void** data);

	int get_count() const;
};
