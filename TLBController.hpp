#pragma once

#include "pageTable.hpp"
#include "TLB.hpp"
#include "config.hpp"

class TLBController
{
private:
	TLB tlb;
	PageTable pageTable;

public:
	TLBController();
	~TLBController();

	uint64_t get_pa_from_va(uint64_t vpn);
};