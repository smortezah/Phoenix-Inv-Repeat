#!/bin/bash

cd ..
cmake ./source
make

PIXFORMAT=png
#PIXFORMAT=svg

irName=ir
aName=ad    # alpha name
maxCtx=21   # real: -=1

for ir in 0 #1
do
    for alphaDen in 1 10 100
    do
    rm -f $irName$ir$aName$alphaDen.dat
    touch $irName$ir$aName$alphaDen.dat
    echo -e "# ir\talpha\tctx\tbpb" >> $irName$ir$aName$alphaDen.dat
        for((ctx=2; ctx<$maxCtx; ++ctx))
        do
        ./phoenix -m t,$ctx,$alphaDen,$ir -t b.fa >> $irName$ir$aName$alphaDen.dat
        done
    done
done

for FILE in ir0ad1.dat ir0ad10.dat ir0ad100.dat
do
#filenames="ir0ad1.dat ir0ad10.dat ir0ad100.dat"
gnuplot <<- EOF
set xlabel "context"
set ylabel "bpb"
set key right bottom
set term $PIXFORMAT
set output "${FILE}.$PIXFORMAT"
#plot for [file in filenames] file."dat" using 3:4 with lines
plot "${FILE}" using 3:4 with linespoints ls 7 title "${FILE}"
EOF
done