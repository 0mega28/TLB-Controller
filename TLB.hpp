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
	std::unordered_map<unsigned int, unsigned int> vpn_to_pfn;

	/* Queue to store the LRU order of the pages <vpn> */
	std::list<unsigned int> lru_queue;

	/* Remove least recently used page from lru_queue and vpn_to_pfn */
	void remove_lru_page();

public:
	TLB(std::size_t tlb_size);
	~TLB();

	/* Insert a new mapping from Virtual Page Number to Physical Frame Number */
	void insert(unsigned int vpn, unsigned int pfn);

	/* Checks if Virtual Page Number is present */
	bool is_page_present(unsigned int vpn);

	/* Returns the Physical Frame Number of the Virtual Page Number */
	unsigned int get_pfn(unsigned int vpn);
};
