#!/bin/bash

min=0.0
max=4.0

for i in $(seq $min 1.0 $max); do
    # position in
    G4-Scanning_MCP B=$i z=-10 N=6 events=100000 nucleus=32Ar
    # position out
    G4-Scanning_MCP B=$i z=-90 N=6 events=100000 nucleus=32Ar
done
