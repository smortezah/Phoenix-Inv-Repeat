#!/bin/bash

cd ..
cmake ./source
make

touch ir.dat
echo -e "# ctx\talpha\tir\tbpb" >> ir.dat

for ir in 0 1
do
	for alphaDen in 1
	do
		for ctx in 2 3 4
		do
		./phoenix -m t,$ctx,$alphaDen,$ir -t b.fa >> ir.dat
		done
	done
done
