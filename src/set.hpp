#pragma once

#include <stdint.h>

#include "block.hpp"
#include "timer.hpp"

#define BLOCK_NOT_FOUND __UINT64_MAX__

class Set
{
private:
	Block **blocks;
	unsigned int size;
	unsigned int used;
	Timer<uint64_t> timer;

	Block *get_LRU_replacement_block();

public:
	Set(unsigned int size);
	~Set();

	/* Returns frame_number if block found else BLOCK_NOT_FOUND */
	uint64_t get_frame_number(uint64_t page_number);

	/* Sets a block by following LRU replacement policy */
	void set_block(uint64_t page_number, uint64_t frame_number);
};

Set::Set(unsigned int size)
{
	this->size = size;
	this->used = 0;

	this->blocks = new Block *[size];
	for (unsigned int i = 0; i < size; i++)
	{
		this->blocks[i] = new Block();
	}
}

Block *Set::get_LRU_replacement_block()
{
	Block *block = nullptr;
	uint64_t min_last_access = UINT64_MAX;

	for (unsigned int i = 0; i < this->size; i++)
	{
		if (this->blocks[i]->get_last_access() < min_last_access)
		{
			min_last_access = this->blocks[i]->get_last_access();
			block = this->blocks[i];
		}
	}

	return block;
}

Set::~Set()
{
	for (unsigned int i = 0; i < this->size; i++)
	{
		delete this->blocks[i];
	}
	delete[] this->blocks;
}

uint64_t Set::get_frame_number(uint64_t page_number)
{
	Block *block = nullptr;

	for (unsigned int i = 0; i < this->size; i++)
	{
		if (this->blocks[i]->get_page_number() == page_number)
		{
			block = this->blocks[i];
			break;
		}
	}

	if (block)
	{
		block->set_last_access(this->timer.get_time());
		return block->get_frame_number();
	}

	return BLOCK_NOT_FOUND;
}

void Set::set_block(uint64_t page_number, uint64_t frame_number)
{
	if (this->used < this->size)
	{
		this->blocks[this->used]->set(page_number, frame_number, this->timer.get_time());
		this->used++;
	}
	else
	{
		Block *block = this->get_LRU_replacement_block();
		block->set(page_number, frame_number, this->timer.get_time());
	}
}
