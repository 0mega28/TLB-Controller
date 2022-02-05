#include <vector>
#include <stdint.h>
#include "config.hpp"

struct pageTableEntry
{
	uint64_t vpn;
	uint64_t pfn;
	bool is_present;
	bool is_protected;
	bool is_dirty;
	bool is_referenced;
	bool is_cached;

	pageTableEntry() {}
	pageTableEntry(uint64_t vpn, uint64_t pfn) : vpn(vpn), pfn(pfn), is_present(true), is_protected(false), is_dirty(false), is_referenced(false), is_cached(false) {}
};

class page_fault_exception
{
};

class PageTable
{
private:
	/* Vector to store page table entries */
	std::vector<pageTableEntry> page_table;

	/* Number of entries in page table */
	uint64_t pt_size;

	/* Number of bits in addressing */
	unsigned int bits_in_addressing;

	/* Page/Frame size size */
	unsigned int page_size;

	/* Return vpn from VA and pfn from PA */
	uint64_t get_num_from_addr(uint64_t addr);

public:
	PageTable();
	~PageTable();

	/* Insert a new mapping from Virtual address to Physical address */
	void insert(uint64_t va, uint64_t pa);

	/*
	 * If found returns the page table entry of the Virtual Page Number
	 * else throw an exception "page_fault_exception"
	 */
	pageTableEntry get_page_entry(uint64_t va);
};
