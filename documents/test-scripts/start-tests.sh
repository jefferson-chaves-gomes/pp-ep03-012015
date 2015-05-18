#!/bin/bash

NUM_THREADS=1

while [ $NUM_THREADS -le 10 ]; do 
	echo
	echo " ########################### Start process for "$NUM_THREADS "Thread(s) ########################### "
	echo
	echo
	
	echo "exemplo-00.txt - 5 vertices"
	../../Debug/pp-ep02-012015 all $NUM_THREADS < ../input-files/exemplo-00.txt
	echo
	echo
	echo "exemplo-01.txt - 2251 vertices"
	../../Debug/pp-ep02-012015 all $NUM_THREADS < ../input-files/exemplo-01.txt
	echo
	echo
	echo "exemplo-02.txt - 25748 vertices"
	../../Debug/pp-ep02-012015 all $NUM_THREADS < ../input-files/exemplo-02.txt
	echo
	echo
	echo "exemplo-03.txt - 11954 vertices"
	../../Debug/pp-ep02-012015 all $NUM_THREADS < ../input-files/exemplo-03.txt 
	echo
	echo
	echo "exemplo-04.txt - 310398 vertices"
	../../Debug/pp-ep02-012015 all $NUM_THREADS < ../input-files/exemplo-04.txt
	echo
	echo
	echo "in-01.txt 	 - 10000003 vertices"
	../../Debug/pp-ep02-012015 all $NUM_THREADS < ../input-files/in-01.txt
	echo
	echo
	echo "in-02.txt 	 - 30000003 vertices"
	../../Debug/pp-ep02-012015 all $NUM_THREADS < ../input-files/in-02.txt
	echo
	echo
	echo "in-03.txt 	- 50000003 vertices"
	../../Debug/pp-ep02-012015 all $NUM_THREADS < ../input-files/in-03.txt 
	echo
	echo
	echo "in-04.txt 	- 70000003 vertices"
	../../Debug/pp-ep02-012015 all $NUM_THREADS < ../input-files/in-04.txt 

	if [ "$NUM_THREADS" -eq "1" ]; then 
		((NUM_THREADS = $NUM_THREADS + 3)) 
	else 
		((NUM_THREADS = $NUM_THREADS + 2)) 
	fi
done
echo
echo
echo "IT IS END... "
echo

