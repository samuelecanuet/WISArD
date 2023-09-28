#!/bin/bash

# Liste des noms de noyaux
nuclei=("H" "He" "Li" "Be" "B" "C" "N" "O" "F" "Ne" "Na" "Mg" "Al" "Si" "P" "S" "Cl" "Ar" "K" "Ca" "Sc" "Ti" "V" "Cr" "Mn" "Fe" "Co" "Ni" "Cu" "Zn" "Ga" "Ge" "As" "Se" "Br" "Kr" "Rb" "Sr" "Y" "Zr" "Nb" "Mo" "Tc" "Ru" "Rh" "Pd" "Ag" "Cd" "In" "Sn" "Sb" "Te" "I" "Xe" "Cs" "Ba" "La" "Ce" "Pr" "Nd" "Pm" "Sm" "Eu" "Gd" "Tb" "Dy" "Ho" "Er" "Tm" "Yb" "Lu" "Hf" "Ta" "W" "Re" "Os" "Ir" "Pt" "Au" "Hg" "Tl" "Pb" "Bi" "Po" "At" "Rn" "Fr" "Ra" "Ac" "Th" "Pa" "U" "Np" "Pu" "Am" "Cm" "Bk" "Cf" "Es" "Fm" "Md" "No" "Lr" "Rf" "Db" "Sg" "Bh" "Hs" "Mt")

# Créez un tableau associatif pour associer chaque nom de noyau à son numéro atomique (Z)
declare -A nucleus_Z

# Ajoutez des entrées pour chaque noyau en utilisant l'index de la liste comme Z
for ((i=1; i<${#nuclei[@]}+1; i++)); do
    nucleus_Z[${nuclei[$i-1]}]=$i
done

Nucleus=$(echo "$Nuclei" | sed -n 's/.*\([A-Z][a-z]*\)$/\1/p')
if [[ -n ${nucleus_Z[$Nucleus]} ]]; then
    Z=${nucleus_Z[$Nucleus]}






