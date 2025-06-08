#!/bin/bash

min=-2.0
max=2.0

for i in $(seq $min 0.5 $max); do
    for j in $(seq $min 0.5 $max); do
       for t in $(seq -2.0 0.5 2.0); do
        	echo "Position: ($i, $j, $t)"

             # extract fractional parts
            frac_i=${i#*.}
            frac_j=${j#*.}
            frac_t=${t#*.}

            # if *all three* are integers (i.e. .0), skip this iteration
            if [[ $frac_i -eq 0 && $frac_j -eq 0 && $frac_t -eq 0 ]]; then
                continue
            fi  

            # if y = -2.0 and z < -0.5, skip this iteration
            if [[ $j == -2.0 && $(echo "$i < -0.5" | bc) -eq 1 ]]; then
                continue
            fi

        	G4-Scanning_DetPos nucleus=32Ar N=45 events=10000000 CV=1 x=0.0 y=$i z=$j theta=$t
	    done
    done
done
