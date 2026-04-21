#!/bin/sh
# finder script for assignment 1

if [ $# -lt 2 ]; then
    echo "Error: Two arguments required."
    echo "Usage: $0 <filesdir> <searchstr>"
    exit 1
fi
 
filesdir="$1"
searchstr="$2"
 
if [ ! -d "$filesdir" ]; then
	echo "Error: '$filesdir' is not a valid directory on the filesystem."
	exit 1
else
	echo "dir path: $filesdir"
fi
 
# Count number of files containing the string
X=$(grep -rl "$searchstr" "$filesdir" | wc -l)

# Count total number of matching lines
Y=$(grep -r "$searchstr" "$filesdir" | wc -l)

 
echo "The number of files are $X and the number of matching lines are $Y"

