#!/bin/bash

if [ $# -lt 2 ] || [ "$2" != "PA" ] && [ "$2" != "PB" ];then
	echo ./setenv.sh "PA"
	source ./setenv.sh "PA"	
else
	echo ./$1-setenv.sh $2
	source ./$1-setenv.sh $2	
fi 

make clean
make spi 
