#!/bin/sh
# Writer script for assignment 1

if [ $# -lt 2 ]; then
	echo "Error: two argument required"
	echo "Usage: $0 <writefile> <writestr> expected."
	exit 1
fi

writefile="$1"
writestr="$2"

# create directory path if it does not exist
dirpath=$(dirname "$writefile")
mkdir -p "$dirpath"

#Create/overwrite file with content
if echo "$writestr" > "$writefile"; then
	echo "File created successfully"
else
	echo "Error: couold not create file"
	exit 1
fi



