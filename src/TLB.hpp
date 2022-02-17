#pragma once

#include <unordered_map>
#include <queue>
#include <list>
#include <utility>
#include <stdint.h>

class TLB
{
private:
	/* Max TLB size */
	std::size_t tlb_size;

	/* UMap cache to store mapping from Virtual Page Number to Physical Frame Number */
	std::unordered_map<uint64_t, uint64_t> vpn_to_pfn;

	/* Queue to store the LRU order of the pages <vpn> */
	std::list<uint64_t> lru_queue;

	/* Remove least recently used page from lru_queue and vpn_to_pfn */
	void remove_lru_page();

public:
	TLB(std::size_t tlb_size);
	~TLB();

	/* Insert a new mapping from Virtual Page Number to Physical Frame Number */
	void insert(uint64_t vpn, uint64_t pfn);

	/* Checks if Virtual Page Number is present */
	bool is_page_present(uint64_t vpn);

	/* Returns the Physical Frame Number of the Virtual Page Number */
	uint64_t get_pfn(uint64_t vpn);
};