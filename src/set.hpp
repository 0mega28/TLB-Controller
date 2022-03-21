#pragma once

#include <stdint.h>

#include "block.hpp"
#include "timer.hpp"

#define BLOCK_NOT_FOUND __UINT64_MAX__

class Set
{
private:
	Block **blocks;
	unsigned int ways;
	unsigned int used;
	Timer<uint64_t> timer;

	Block *get_LRU_replacement_block();

public:
	Set(unsigned int ways);
	~Set();

	/* Returns frame_number if block found else BLOCK_NOT_FOUND */
	uint64_t get_frame_number(uint64_t page_number);

	/*
	 * Inserts a block by evicting a block using the LRU replacement
	 * policy. Returns the evicted block so that it can be pushed to
	 * next level of cache. If no eviction is needed, it returns a
	 * block with last accessed time of BLOCK_NOT_ACCESSED.
	 */
	Block insert_block(uint64_t page_number, uint64_t frame_number);
};

Set::Set(unsigned int ways)
{
	this->ways = ways;
	this->used = 0;

	this->blocks = new Block *[ways];
	for (unsigned int i = 0; i < ways; i++)
	{
		this->blocks[i] = new Block();
	}
}

Block *Set::get_LRU_replacement_block()
{
	Block *block = nullptr;
	uint64_t min_last_access = UINT64_MAX;

	for (unsigned int i = 0; i < this->ways; i++)
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
	for (unsigned int i = 0; i < this->ways; i++)
	{
		delete this->blocks[i];
	}
	delete[] this->blocks;
}

uint64_t Set::get_frame_number(uint64_t page_number)
{
	Block *block = nullptr;

	for (unsigned int i = 0; i < this->ways; i++)
	{
		if (this->blocks[i]->get_block_validity() && 
		    this->blocks[i]->get_page_number() == page_number) /* tag match */
		{
			block = this->blocks[i];
			break;
		}
	}

	if (block) /* update the last access time of the current block */
	{
		block->set_last_access(this->timer.get_time());
		return block->get_frame_number();
	}

	return BLOCK_NOT_FOUND;
}

Block Set::insert_block(uint64_t page_number, uint64_t frame_number)
{
	/* Return the evicted block so that it can be pushed to next level of cache */
	Block evicted_block;
	evicted_block.set_last_access(BLOCK_NOT_ACCESSED);

	if (this->used < this->ways)
	{
		/* Block is valid */
		this->blocks[this->used]->init(page_number, frame_number,
					       this->timer.get_time());
		this->used++;
	}
	else
	{
		Block *block = this->get_LRU_replacement_block();
		evicted_block = block->get_clone();
		/* Block is valid */
		block->init(page_number, frame_number,
			    this->timer.get_time());
	}

	return evicted_block;
}
