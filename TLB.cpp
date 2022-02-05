#include <TLB.hpp>
#include <cassert>
#include <algorithm>

TLB::TLB(std::size_t tlb_size)
{
	this->tlb_size = tlb_size;
}

void TLB::remove_lru_page()
{
	assert(vpn_to_pfn.size() != 0 && "TLB: remove_lru_page: vpn_to_pfn is empty");
	assert(lru_queue.size() != 0 && "TLB: remove_lru_page: lru_queue is empty");

	uint64_t lru_page = lru_queue.front();
	lru_queue.pop_front();
	vpn_to_pfn.erase(lru_page);
}

void TLB::insert(uint64_t vpn, uint64_t pfn)
{
	/*
	 * Since page was already present in TLB map controller should have
	 * checked is_page_present() before calling this function.
	 */
	assert(is_page_present(vpn) && "TLB: insert: Page already present in TLB");

	if (lru_queue.size() == tlb_size)
		remove_lru_page();

	lru_queue.push_back(vpn);
	vpn_to_pfn[vpn] = pfn;
}

bool TLB::is_page_present(uint64_t vpn)
{
	return vpn_to_pfn.find(vpn) != vpn_to_pfn.end();
}

uint64_t TLB::get_pfn(uint64_t vpn)
{
	/*
	 * TLB controller should have checked is_page_present before calling
	 * get_pfn
	 */
	assert(is_page_present(vpn) && "TLB: get_pfn: Page not present in TLB");

	/* Update the access time of vpn */
	lru_queue.erase(std::find(lru_queue.begin(), lru_queue.end(), vpn));
	lru_queue.push_back(vpn);

	return vpn_to_pfn[vpn];
}
