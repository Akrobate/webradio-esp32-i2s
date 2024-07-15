#!/bin/bash

# Calculate the sum of file sizes
sum=0

# Loop through the files and add their sizes to the sum
for file in index.html index.js style.css; do
    if [ -f "$file" ]; then
        size=$(stat -c "%s" "$file")

        echo "- $file \t $size bytes"
        sum=$((sum + size))
    fi
done

# Print the sum of file sizes
echo "total $sum bytes"