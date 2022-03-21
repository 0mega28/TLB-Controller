#!/bin/bash

if [ "$#" != 1 ]; then
	echo "$0" '<inputfile>'
	exit 1
fi

file="$1"
hits=$(rg hit "${file}" | wc -l)
miss=$(rg miss "${file}" | wc -l)
total=$((("${hits}" + "${miss}")))
echo hitrate: $(((("${hits}" * 100) / "${total}")))
