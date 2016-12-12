#!/bin/bash

cd ..
cmake ./source
make

PIXFORMAT=png
#PIXFORMAT=svg

irName=ir
aName=ad    # alpha name
maxCtx=21   # real: -=1

for dataset in b #b a
do
    for ir in 0 1
    do
        for alphaDen in 1 10 100
        do
        rm -f $irName$ir$aName$alphaDen$dataset.dat
        touch $irName$ir$aName$alphaDen$dataset.dat
        echo -e "# ir\talpha\tctx\tbpb" >> $irName$ir$aName$alphaDen$dataset.dat
            for((ctx=2; ctx<$maxCtx; ++ctx))
            do
            ./phoenix -m t,$ctx,$alphaDen,$ir -t $dataset.fa >> $irName$ir$aName$alphaDen$dataset.dat
            done
        done
    done
done

for dataset in b
do
gnuplot <<- EOF
set xlabel "context"
set ylabel "bpb"
set key right bottom
set term $PIXFORMAT
set output "ir0$dataset.dat.$PIXFORMAT"
plot    "ir0ad1$dataset.dat" using 3:4  with linespoints ls 6 title     "ir=0, alpha=1/1,     $dataset",\
        "ir0ad10$dataset.dat" using 3:4 with linespoints ls 7 title     "ir=0, alpha=1/10,   $dataset",\
        "ir0ad100$dataset.dat" using 3:4 with linespoints ls 8 title    "ir=0, alpha=1/100, $dataset"
set output "ir1$dataset.dat.$PIXFORMAT"
plot    "ir1ad1$dataset.dat" using 3:4  with linespoints ls 6 title     "ir=1, alpha=1/1,     $dataset",\
        "ir1ad10$dataset.dat" using 3:4 with linespoints ls 7 title     "ir=1, alpha=1/10,   $dataset",\
        "ir1ad100$dataset.dat" using 3:4 with linespoints ls 8 title    "ir=1, alpha=1/100, $dataset"
EOF
done