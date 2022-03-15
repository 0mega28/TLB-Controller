#include <vector>
#include <stdint.h>
#include <sstream>

#include "TLBController.hpp"
#include "config.hpp"

void readInputTraces(const char *file,
		     std::vector<std::pair<uint64_t, uint64_t>> &page_table_vector,
		     std::vector<uint64_t> &test_VA_Vector)
{
	char pt_entry = 'R';
	char va_entry = 'W';

	std::ifstream infile(file);

	if (!infile.is_open())
	{
		std::cerr << "Error opening file" << std::endl;
		exit(1);
	}

	std::string line;
	while (std::getline(infile, line))
	{
		std::stringstream ss(line);

		int drop;
		ss >> drop;

		char type;
		std::string va;
		std::string pa;

		ss >> type;
		if (type == pt_entry)
		{
			// 2246 R 0x3ad97780 0x114ac
			ss >> va;
			ss >> pa;

			/* Drop 0x from start */
			va = va.substr(2);
			pa = pa.substr(2);

			uint64_t va_num = std::stoul(va, nullptr, 16);
			uint64_t pa_num = std::stoul(pa, nullptr, 16);

			page_table_vector.push_back(std::make_pair(va_num, pa_num));
		}
		else if (type == va_entry)
		{
			// 3 W 0x36167780
			ss >> va;

			/* Drop 0x from start */
			va = va.substr(2);

			uint64_t va_num = std::stoul(va, nullptr, 16);
			test_VA_Vector.push_back(va_num);
		}
		else
		{
			std::cerr << "Error: Invalid trace entry" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <trace file>"  << std::endl;
		exit(1);
	}
	/*
	 * USIMM traces file have two types of lines
	 * 1) 2 R 0x3a617780 0x114c8
	 * 2) 3 W 0x36167780
	 *
	 * For R instruction store treat 3rd and 4th column
	 * as VA and PA respectively and store them in page_table_vector
	 *
	 * For W instruction store 3rd column as VA
	 * and use the address to find the corresponding PA using TLB
	 */

	const char *inputfile = argv[1];
	// TODO: unhardcode this and make output file name dynamic from input file
	const char *outputfile = "output.txt";

	/* Stores VA to PA mapping to be finally loaded in page table */
	std::vector<std::pair<uint64_t, uint64_t>> page_table_vector;

	/* Our test cases which test TLB performance */
	std::vector<uint64_t> test_VA_Vector;

	readInputTraces(inputfile, page_table_vector, test_VA_Vector);

	TLBController *tlbcontroller =
	    new TLBController(TLB_SIZE, NUM_OF_WAYS, PAGE_SIZE,
			      page_table_vector, outputfile);

	for (auto &va : test_VA_Vector)
	{
		uint64_t pa = tlbcontroller->get_pa_from_va(va);
		(void)pa; /* suppress unused variable warning */
	}

	delete (tlbcontroller);
	return 0;
}