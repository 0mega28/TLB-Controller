#pragma once

#include <string>
#include <fstream>
#include <iostream>

#include "pageTable.hpp"
#include "TLB.hpp"
#include "config.hpp"
#include "utils.hpp"

// TODO: Multilevel TLB support
class TLBController
{
private:
	TLB **tlb;
	PageTable *pageTable;

	unsigned int pageSize;
	unsigned int no_of_levels = 2;

	std::ofstream outfile;

	/* TLB Statistics */
	uint64_t total_miss_l1 = 0, total_hit_l1 = 0;
	uint64_t total_miss_l2 = 0, total_hit_l2 = 0;
	uint64_t total_address = 0;

	/* Loads value from file into page table */
	void load_page_table(std::vector<std::pair<uint64_t, uint64_t>> &page_table_vector);

	/* Returns page/frame number from address */
	uint64_t get_index(uint64_t address);

	/* Returns physical address from frame number and virtual address  */
	uint64_t get_address(uint64_t fn, uint64_t va);

public:
	TLBController(unsigned int tlb_size_l1, unsigned int tlb_size_l2, unsigned int num_ways_l1,
		      unsigned int num_ways_l2, unsigned int pageSize, std::string outputFile);
	~TLBController();

	/* Pretty print the output */
	void print_statistics();

	uint64_t get_pa_from_va(uint64_t va);
};

TLBController::TLBController(unsigned int tlb_size_l1, unsigned int tlb_size_l2, unsigned int num_ways_l1,
			     unsigned int num_ways_l2, unsigned int pageSize, std::string outputFile)
{
	this->pageSize = pageSize;

	this->tlb = new TLB *[this->no_of_levels];

	this->tlb[0] = new TLB(tlb_size_l1, num_ways_l1);
	this->tlb[1] = new TLB(tlb_size_l2, num_ways_l2);

	this->pageTable = new PageTable();

	this->outfile.open(outputFile);
}

TLBController::~TLBController()
{
	for (unsigned int i = 0; i < no_of_levels; i++)
	{
		delete this->tlb[i];
	}

	delete[] this->tlb;
	delete this->pageTable;

	this->outfile.close();
}

void TLBController::load_page_table(std::vector<std::pair<uint64_t, uint64_t>> &page_table_vector)
{
	for (unsigned int i = 0; i < page_table_vector.size(); i++)
	{
		uint64_t page_number = this->get_index(page_table_vector[i].first);
		uint64_t frame_number = this->get_index(page_table_vector[i].second);

		this->pageTable->insert(page_number, frame_number);
	}
}

uint64_t TLBController::get_index(uint64_t address)
{
	return address / this->pageSize;
}

uint64_t TLBController::get_address(uint64_t fn, uint64_t va)
{
	return (fn * this->pageSize) + (va % this->pageSize);
}

uint64_t TLBController::get_pa_from_va(uint64_t va)
{
	this->total_address++;
	/* gets page number from virtual address */
	uint64_t pn = this->get_index(va);
	/* get frame number from page number from the TLB */
	uint64_t fn = this->tlb[0]->get_frame_number(pn);

	std::string output = "";
	output += "VA: " + to_hex(va) + "\t";

	Block evicted_block;

	/* If it's a miss in L1 TLB */
	if (fn == BLOCK_NOT_FOUND)
	{
		output += "L1 miss\t";
		this->total_miss_l1++;

		/* Search in L2 TLB */
		fn = this->tlb[1]->get_frame_number(pn);

		/* If it's a miss in L2 TLB */
		if (fn == BLOCK_NOT_FOUND)
		{
			output += "L2 miss\t";
			this->total_miss_l2++;
			
			/* Search in page table */
			fn = this->pageTable->get_frame_number_short(pn);

			/* Insert the block found in page table in L1 TLB  */
			evicted_block = this->tlb[0]->insert_block(pn, fn);

			/* If there was a block evicted from L1 TLB insert into L2 TLB */
			if (evicted_block.get_last_access() != BLOCK_NOT_ACCESSED)
				this->tlb[1]->insert_block(evicted_block.get_page_number(), evicted_block.get_frame_number());
		}
		/* if it's a hit in L2 TLB */
		else
		{
			output += "L2 hit\t";
			this->total_hit_l2++;

			/* remove the block from L2 TLB and insert into L1 TLB according to exclusive cache design */
			evicted_block = this->tlb[1]->remove_block(pn);
			evicted_block = this->tlb[0]->insert_block(evicted_block.get_page_number(), evicted_block.get_frame_number());
			
			/* If there was a block evicted from L1 TLB insert into L2 TLB */
			if (evicted_block.get_last_access() != BLOCK_NOT_ACCESSED)
				this->tlb[1]->insert_block(evicted_block.get_page_number(), evicted_block.get_frame_number());
		}
	}
	/* if it's a hit in L1 TLB */
	else
	{
		output += "L1 hit\t";
		this->total_hit_l1++;
	}

	this->outfile << output << std::endl;
	uint64_t pa = get_address(fn, va);

	return pa;
}

void TLBController::print_statistics()
{
	using namespace std;
	auto total_hit = this->total_hit_l1 + this->total_hit_l2;
	auto total_miss = this->total_miss_l2 + this->total_miss_l1;

	cout << "TLB Statistics: ";
	cout << "\n\tTotal Address: " << this->total_address;
	cout << "\n\tTotal Misses: " << total_miss;
	cout << "\n\tTotal Hits: " << total_hit;
	cout << "\n\tHit Ratio: " << ((total_hit * 100) / this->total_address);
	cout << endl;
}
