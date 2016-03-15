#!/bin/bash

if [ -z $1 ];
then
	echo ./setenv.sh
	source ./setenv.sh	
else 
	echo ./$1-setenv.sh
	source ./$1-setenv.sh
fi

make clean
make spi 
