#!/bin/bash

var=0
while [ $var -lt 10 ]
do
	time ./Sete_Cliques_threads 8
	var=$((var+1))
done
