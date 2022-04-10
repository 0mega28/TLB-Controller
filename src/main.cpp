#include <vector>
#include <stdint.h>
#include <sstream>
#include <csignal>

#include "TLBController.hpp"
#include "config.hpp"

void readInputTraces(const char *file,
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
		if (type == pt_entry || type == va_entry)
		{
			ss >> va;

			/* Drop 0x from start */
			va = va.substr(2);
			/*
			 * For every request irrespective of a read or
			 * a write, the job of the TLB is to simply
			 * deliver a physical frame number mapped to a
			 * virtual page number. Therefore, for the
			 * sake of the simulation, the PA is assumed
			 * to be equal to the VA.
			 */
			pa = va;

			uint64_t va_num = std::stoul(va, nullptr, 16);
			uint64_t pa_num = std::stoul(pa, nullptr, 16);
			(void)pa_num;

			test_VA_Vector.push_back(va_num);
		}
		else
		{
			std::cerr << "Error: Invalid trace entry" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
}

TLBController *tlbcontroller;
void sigint_handler(int sig_num)
{
	std::cout << "Interrupt signal (" << sig_num << ") received.\n";

	/* Print Statistics */
	tlbcontroller->print_statistics();
	/* Clean up memory */
	delete tlbcontroller;

	std::cout << "Exiting...\n";
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <trace file>" << std::endl;
		exit(1);
	}

	/*
	 * USIMM traces file have two types of lines
	 * 1) 2 R 0x3a617780 0x114c8
	 * 2) 3 W 0x36167780
	 *
	 * For both the instruction types, treat the 3rd column
	 * as the VA and assume PA is equal to the VA
	 */
	const char *inputfile = argv[1];

	std::string outputfile(argv[1]);
	/*
	 * Replace "input" string from input file name with "output"
	 * e.g. input/black -> output/black
	 */
	outputfile.replace(outputfile.find("input"), sizeof("input") - 1, "output");

	/* Our test cases which test TLB performance */
	std::vector<uint64_t> test_VA_Vector;

	readInputTraces(inputfile, test_VA_Vector);
	tlbcontroller =
	    new TLBController(L1_TLB_SIZE, L2_TLB_SIZE, L1_NUM_OF_WAYS, L2_NUM_OF_WAYS, PAGE_SIZE,
			      outputfile);

	/* Install SIGINT handler */
	std::signal(SIGINT, sigint_handler);

	for (auto &va : test_VA_Vector)
	{
		uint64_t pa = tlbcontroller->get_pa_from_va(va);
		(void)pa; /* suppress unused variable warning */
	}

	/* Print Statistics */
	std::cout << "----------------------------------------" << std::endl;
	std::cout << "Input file: " << inputfile << std::endl;
	tlbcontroller->print_statistics();

	delete (tlbcontroller);
	return 0;
}