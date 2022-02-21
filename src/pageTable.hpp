#pragma once

#include <vector>
#include <stdint.h>
#include <stdlib.h>
#include <iostream>

#include "config.hpp"

struct pageTableEntry
{
	uint64_t page_number;
	uint64_t frame_number;
	bool is_present;
	bool is_protected;
	bool is_dirty;
	bool is_referenced;
	bool is_cached;

	pageTableEntry() {}
	pageTableEntry(uint64_t page_number, uint64_t frame_number) : page_number(page_number), frame_number(frame_number), is_present(true), is_protected(false), is_dirty(false), is_referenced(false), is_cached(false) {}
};

class PageTable
{
private:
	/* Vector to store page table entries */
	std::vector<pageTableEntry> page_table;

public:
	PageTable();
	~PageTable();

	void insert(uint64_t page_number , uint64_t frame_number);

	uint64_t get_frame_number(uint64_t page_number);
};

PageTable::PageTable() {}

PageTable::~PageTable() {}

void PageTable::insert(uint64_t page_number, uint64_t frame_number)
{
	pageTableEntry entry(page_number, frame_number);
	this->page_table.push_back(entry);
}

uint64_t PageTable::get_frame_number(uint64_t page_number)
{
	for (unsigned int i = 0; i < this->page_table.size(); i++)
	{
		if (this->page_table[i].page_number == page_number)
		{
			return this->page_table[i].frame_number;
		}
	}

	std::cout << "Page not found in page table" << std::endl;
	std::cout << "Page number: " << page_number << std::endl;
	exit(EXIT_FAILURE);
}
