#include <assert.h>
#include <math.h>
#include "pageTable.hpp"

uint64_t PageTable::get_num_from_addr(uint64_t address)
{
	return (address / page_size);
}

PageTable::PageTable()
{
	this->pt_size = pow(2, bits_in_addressing - log2(page_size));
}

void PageTable::insert(uint64_t va, uint64_t pa)
{
	assert(page_table.size() == pt_size && "Page table is full");

	uint64_t vpn = get_num_from_addr(va);
	uint64_t pfn = get_num_from_addr(pa);

	pageTableEntry newEntry(vpn, pfn);

	page_table.push_back(newEntry);
}

pageTableEntry PageTable::get_page_entry(uint64_t va)
{
	uint64_t vpn = get_num_from_addr(va);

	for (size_t i = 0; i < page_table.size(); i++)
		if (page_table[i].vpn == vpn)
			return page_table[i];

	throw page_fault_exception();
}