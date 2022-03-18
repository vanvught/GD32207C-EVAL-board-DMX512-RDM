#!/bin/bash
NPROC=1

if [ "$(uname)" == "Darwin" ]; then
     NPROC=$(sysctl -a | grep machdep.cpu.core_count | cut -d ':' -f 2)     
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
     NPROC=$(nproc)
fi

cd ..

DIR=gd32_*

for f in $DIR
do
	echo -e "\e[32m[$f]\e[0m"
	if [ -d $f ]; then
		cd "$f"
		
		make -f Makefile.GD32 clean
		make -f Makefile.GD32 -j $NPROC
		retVal=$?
		if [ $retVal -ne 0 ]; then
		 	echo "Error : " "$f"
			exit $retVal
		fi

		cp gd32f20x.bin /tmp/$f.bin
		
		cd -
	fi
done

find . -name gd32f20x.bin | sort | xargs ls -al
find . -name gd32f20x.bin | xargs ls -al | wc -l