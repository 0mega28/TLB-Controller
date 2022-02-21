#pragma once

#include <stdint.h>

#include "set.hpp"

class TLB
{
private:
	Set **sets;
	unsigned int num_sets;

	unsigned int get_index_block(uint64_t page_number);
public:
	TLB(unsigned int tlb_size, unsigned int num_ways);
	~TLB();

	/* Returns frame_number if block found else -1 */
	uint64_t get_frame_number(uint64_t page_number);

	/* Sets a block by following LRU replacement policy */
	void set_block(uint64_t page_number, uint64_t frame_number);
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

unsigned int TLB::get_index_block(uint64_t page_number)
{
	return page_number % this->num_sets;
}

uint64_t TLB::get_frame_number(uint64_t page_number)
{
	unsigned int index = this->get_index_block(page_number);
	return this->sets[index]->get_frame_number(page_number);
}

void TLB::set_block(uint64_t page_number, uint64_t frame_number)
{
	unsigned int index = this->get_index_block(page_number);
	this->sets[index]->set_block(page_number, frame_number);
}
