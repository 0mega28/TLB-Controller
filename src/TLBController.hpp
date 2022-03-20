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
	TLB *tlb;
	PageTable *pageTable;

	unsigned int pageSize;

	std::ofstream outfile;

	/* Loads value from file into page table */
	void load_page_table(std::vector<std::pair<uint64_t, uint64_t>> &page_table_vector);

	/* Returns page/frame number from address */
	uint64_t get_index(uint64_t address);

	/* Returns address from page number and virtual address  */
	uint64_t get_address(uint64_t pn, uint64_t va);

public:
	TLBController(unsigned int tlb_size, unsigned int num_ways, unsigned int pageSize,
		      std::vector<std::pair<uint64_t, uint64_t>> &page_table_vector,
		      std::string outputFile);
	~TLBController();

	uint64_t get_pa_from_va(uint64_t va);
};

TLBController::TLBController(unsigned int tlb_size, unsigned int num_ways, unsigned int pageSize,
			     std::vector<std::pair<uint64_t, uint64_t>> &page_table_vector,
			     std::string outputFile)
{
	this->pageSize = pageSize;

	this->tlb = new TLB(tlb_size, num_ways);
	this->pageTable = new PageTable();

	this->load_page_table(page_table_vector);

	this->outfile.open(outputFile);
}

TLBController::~TLBController()
{
	delete this->tlb;
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

uint64_t TLBController::get_address(uint64_t pn, uint64_t va)
{
	return (pn * this->pageSize) + (va % this->pageSize);
}

uint64_t TLBController::get_pa_from_va(uint64_t va)
{
	uint64_t pn = this->get_index(va);
	uint64_t fn = this->tlb->get_frame_number(pn);

	std::string output = "";
	output += "VA: " + to_hex(va) + "\t";

	if (fn == BLOCK_NOT_FOUND)
	{
		/* TLB miss */
		fn = this->pageTable->get_frame_number(pn);

		if (fn == PAGE_FAULT)
		{
			output = "";
			output = "Page Fault for VA: " + to_hex(va);
			std::cerr << output << std::endl;
			this->outfile << output << std::endl;
			/* PAGE_FAULT */
			return fn;
		}

		/*
		 * If we found the block in the page table, it needs
		 * to be inserted into the TLB. If there exists a block
		 * which needs to be evicted from the TLB due to capacity
		 * constraints, store it and push into the lower level
		 * of the TLB.
		 */
		Block evicted_block = this->tlb->insert_block(pn, fn);

		if (evicted_block.get_last_access() != BLOCK_NOT_ACCESSED)
		{
			// TODO: handle block eviction
			// The block can be pushed to next level of cache
		}

		output += "TLB miss";
	}
	else
	{
		/* TLB hit */
		output += "TLB hit";
	}

	this->outfile << output << std::endl;

	uint64_t pa = get_address(fn, va);

	return pa;
}
