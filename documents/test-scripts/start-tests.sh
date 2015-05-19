#!/bin/bash

# define file array
files=(../input-files/*.txt)

# find total number of files in an array
# echo "Total files in array : ${#files[*]}"
# total=${#files[*]}

# processArray=(1 2 4 6 8 10 16)
processArray=(1)

for i in "${processArray[@]}"
do
	echo
	echo " ########################### Start for $i Process ########################### "
	echo
	echo
	for f in "${files[@]}"
	do
		echo "mpirun -n $i ../../Debug/pp-ep03-012015 < $f"
		mpirun -n $i ../../Debug/pp-ep03-012015 < $f
		echo
	done
	echo
	echo " ################################## END ######################################## "
	echo
	echo
	echo
done
echo
echo
echo "IT IS END... "
echo

