#!/bin/bash

declare -g PIECES=(
facade-corner-piece
)

# Model generation
for piece in "${PIECES[@]}"; do
    echo "Pièce : $piece"
    openscad -o stl_files/$piece.stl pieces/$piece.scad
done
