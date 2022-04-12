## TLB Controller
A TLB stores the recent translations of virtual memory to physical memory and can be called an address-translation cache.  
It is a part of the chip's memory-management unit (MMU).  
The search key is the virtual page number, the search result being the physical frame number.  
If the requested result is present in the TLB, it is a hit and the physical address is calculated.  
If the entry is not found, it is a miss and the translation proceeds by looking up in the next level of TLB and eventually the page table (if it's a miss in every TLB level) in a process called a page walk.  


### Download input benchmark files
[google drive mirror](https://drive.google.com/file/d/1EG_eODumN_AEQecLzZrg_a1IcUr7ZmjT/view?usp=sharing)

---

### Compile and run the benchmark
```bash
# Download the source code
$ git clone https://github.com/0mega28/TLB-Controller

# cd to src direcory
$ cd TLB-Controller/src

# Place the input.tar.gz in the src directory and extract it
$ tar -xvf input.tar.gz

# Create the output directory
$ mkdir output

# Compile the code and run the benchmark
$ make -j$(nproc)
```

---

### Code organization
![Code organization](resources/code_organization.png)

---

### Cache design
![Cache design](resources/cache_design.png)

---

### Results
![Results](resources/results.png)
