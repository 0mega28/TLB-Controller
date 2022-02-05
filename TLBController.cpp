#include "TLBController.hpp"
#include "pageTable.hpp"
#include "iostream"
#include "TLB.hpp"
#include "pageTable.hpp"
#include "config.hpp"

TLBController::TLBController()
{
	tlb = TLB(tlb_size);
	pageTable = PageTable();
}

uint64_t TLBController::get_pa_from_va(uint64_t va)
{
	uint64_t vpn = va / page_size;

	if (tlb.is_page_present(vpn))
		return tlb.get_pfn(vpn);
	else
	{
		try
		{
			pageTableEntry pte = pageTable.get_page_entry(va);
			tlb.insert(vpn, pte.pfn);
			return pte.pfn * page_size + va % page_size;
		}
		/*
		 * Is this a real page fault?? 
		 * check is_present entry for page fault
		 */
		catch (page_fault_exception p)
		{
			/* Page not found in page table */
			std::cerr << "Page fault exception" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
}