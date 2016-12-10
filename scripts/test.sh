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

for FILE in ir.dat
do
    gnuplot <<- EOF
        set xlabel "Label"
        set ylabel "Label2"
        set title "Graph title"   
        set term png
        set output "${FILE}.png"
        plot "${FILE}" using 2:4
EOF
done
