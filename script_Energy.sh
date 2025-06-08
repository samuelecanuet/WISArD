#!/bin/bash

particle=proton

min=0.1
max=7.0

for i in $(seq $min 0.1 $max); do
    G4-Scanning_Energy particle=$particle N=45 events=10000000 energy=$i
done
