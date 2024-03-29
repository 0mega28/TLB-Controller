#pragma once

#include <stdint.h>

#include "set.hpp"

class TLB
{
private:
	Set **sets;
	unsigned int num_sets;

	unsigned int get_index_of_set(uint64_t page_number);

public:
	/*
	 * @param tlb_size Number of blocks in the TLB
	 * @param num_ways Number of blocks in a set
	 */
	TLB(unsigned int tlb_size, unsigned int num_ways);
	~TLB();

	/* Returns frame_number if block found else BLOCK_NOT_FOUND */
	uint64_t get_frame_number(uint64_t page_number);

	/*
	 * Inserts a block by evicting a block using the LRU replacement
	 * policy. Returns the evicted block so that it can be pushed to
	 * next level of cache. If no eviction is needed, it returns a
	 * block with last accessed time of BLOCK_NOT_ACCESSED.
	 */
	Block insert_block(uint64_t page_number, uint64_t frame_number);
	/*
	 * Removes the block of given page number from the TLB
	 * i.e. set validity of that block to false
	 */
	Block remove_block(uint64_t page_number);
};

TLB::TLB(unsigned int tlb_size, unsigned int num_ways)
{
	this->num_sets = tlb_size / num_ways;
	this->sets = new Set *[this->num_sets];
	for (unsigned int i = 0; i < this->num_sets; i++)
	{
		this->sets[i] = new Set(num_ways);
	}
}

TLB::~TLB()
{
	for (unsigned int i = 0; i < this->num_sets; i++)
	{
		delete this->sets[i];
	}
	delete[] this->sets;
}

unsigned int TLB::get_index_of_set(uint64_t page_number)
{
	return page_number % this->num_sets;
}

uint64_t TLB::get_frame_number(uint64_t page_number)
{
	unsigned int index = this->get_index_of_set(page_number);
	return this->sets[index]->get_frame_number(page_number);
}

Block TLB::remove_block(uint64_t page_number)
{
	unsigned int index = this->get_index_of_set(page_number);
	return this->sets[index]->remove_block(page_number);
}

Block TLB::insert_block(uint64_t page_number, uint64_t frame_number)
{
	unsigned int index = this->get_index_of_set(page_number);
	return this->sets[index]->insert_block(page_number, frame_number);
}
