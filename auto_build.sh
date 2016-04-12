#!/bin/bash

if [ -z $1 ];
then
	echo ./setenv.sh 
	source ./setenv.sh
else 
	echo ./$1-setenv.sh
	source ./$1-setenv.sh
fi

if [ -z $2 ] || [ "$2" != "PA" ] && [ "$2" != "PB" ];then
	p_switch="PA"
else
	p_switch=$2
fi

export p_switch

make clean
make spi 
