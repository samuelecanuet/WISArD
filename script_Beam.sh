#!/bin/bash

sx=0.5
sy=0.5

min=0.0
max=5.0

for i in $(seq $min 1.0 $max); do
    G4-Scanning_Beam sx=$sx sy=$sy y=$i N=45 events=100000000 CV=1 nucleus=32Ar
done
