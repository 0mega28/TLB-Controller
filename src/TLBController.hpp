#pragma once

#include <string>
#include <fstream>
#include <iostream>

#include "pageTable.hpp"
#include "TLB.hpp"
#include "config.hpp"

class TLBController
{
private:
	TLB *tlb;
	PageTable *pageTable;

	unsigned int pageSize;

	std::ofstream outfile;

	/* Loads value from file into page table */
	void load_page_table(std::string filename);

public:
	TLBController(unsigned int tlb_size, unsigned int num_ways, unsigned int pageSize, std::string pageTableEntryFile, std::string outputFile);
	~TLBController();

	uint64_t get_pa_from_va(uint64_t va);
};

TLBController::TLBController(unsigned int tlb_size, unsigned int num_ways, unsigned int pageSize, std::string pageTableEntryFile, std::string outputFile)
{
	this->pageSize = pageSize;

	this->tlb = new TLB(tlb_size, num_ways);
	this->pageTable = new PageTable();

	this->load_page_table(pageTableEntryFile);

	this->outfile.open(outputFile);
}

TLBController::~TLBController()
{
	delete this->tlb;
	delete this->pageTable;

	this->outfile.close();
}

void TLBController::load_page_table(std::string filename)
{
	// TODO: Implement (Depends on how the file is formatted)
	std::cout << "Not implemented yet" << std::endl;
	exit(-1);
}

uint64_t TLBController::get_pa_from_va(uint64_t va)
{
	uint64_t pn = va / this->pageSize;
	uint64_t fn = this->tlb->get_frame_number(pn);

	std::string output = "";
	output += "VA: " + std::to_string(va) + "\t";

	if (fn == (uint64_t)-1)
	{
		/* TLB miss */
		fn = this->pageTable->get_frame_number(pn);
		this->tlb->set_block(pn, fn);

		output += "TLB miss";
	}
	else
	{
		output += "TLB hit";
	}

	this->outfile << output << std::endl;

	uint64_t pa = fn * this->pageSize + (va % this->pageSize);

	return pa;
}
