#!/usr/bin/env bash

# cd to src directory
cd "$(dirname "$0")/.." || exit

input_dir="input"
output_dir="output"
program="./tlbcontroller"
outputfile="tlbstatistics.txt"

# Create output directory if it does not exist
mkdir -p "${output_dir}"

# Get all input test cases
input_files=$(find "${input_dir}" -type f)

# Compile tlbcontroller
make --always-make

# Clear output file
rm -f "${outputfile}"

sed -n '/#define/s/#define//p' config.hpp >> "${outputfile}"

for files in $input_files; do
	(
		echo "Running test case: ${files}"

		{
			"${program}" "${files}"
		} >> "${outputfile}"

		echo "Completed test case: ${files}"
	) &
done

wait
