#!/bin/bash

# Loop through all .txt files in the current folder
for file in *.txt; do
    echo "Processing $file..."
    root -l -q "SRIM2ROOT.C(\"$file\")"
done
