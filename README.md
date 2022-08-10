# SAPPHIRE: A 2-Level TLB Controller

A Translation Lookaside Buffer (TLB) stores the recentmost translations/mappings between the virtual and physical memory hence sometimes referred as the 'address-translation cache'. It is a part of the chip's Memory Management Unit (MMU).  
The search key is the Virtual Page Number (VPN) while the search result is the Physical Frame Number (PFN), the latter being determined by the size of the main memory *i.e.*, the RAM.

A 'hit' occurs when the requested mapping is present in the TLB, using which the physical address is calculated. If the entry is not found, it is a 'miss' and the translation then proceeds by looking up in the next level of TLB and eventually the page table (if it's a 'miss' in every TLB level) in a process called a 'page walk'. A more detailed explanation can be found [here](https://en.wikipedia.org/wiki/Translation_lookaside_buffer).*

SAPPHIRE was written as a part of the project in the course *'CSN506: Advanced Computer Architecture'* taught by Professor [Debiprasanna Sahoo](https://cse.iitr.ac.in/~CSE/Debiprasanna_Sahoo) during the Spring Semester 2022.


## TLB Design
![Cache design](resources/cache_design.png)


## Using SAPPHIRE

First, one needs to download the input benchmark files from [here](https://drive.google.com/file/d/1EG_eODumN_AEQecLzZrg_a1IcUr7ZmjT/view?usp=sharing).

Now, compile and run the benchmark as follows:

- Change into the `src` directory:
	```
	$ cd TLB-Controller/src
	```

- Place the `input.tar.gz` in the `src` directory and extract it:
	```
	$ tar -xvf input.tar.gz
	```

- Create the `output` directory:
	```
	$ mkdir output
	```
- Compile the code and run the benchmark
	```
	$ make -j$(nproc)
	```


## Results
![Results](resources/results.png)

---
*We will intentionally refrain from explaining the simulator flow since we are aware that due to the "open-source" nature of the project, it will be blatantly copied by naive students who do not want to put any sort of effort into their coursework. Therefore, from an academic standpoint, it is appreciated if someone could put the least effort of understanding the code before copying it.
