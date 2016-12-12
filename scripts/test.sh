#!/bin/bash

cd ..
cmake ./source
make

PIXFORMAT=png
#PIXFORMAT=svg

irName=ir
aName=ad    # alpha name
maxCtx=21   # real: -=1

for dataset in c #b a
do
    for ir in 0 1
    do
        for alphaDen in 1 10 100
        do
        rm -f $irName$ir$aName$alphaDen.dat
        touch $irName$ir$aName$alphaDen.dat
        echo -e "# ir\talpha\tctx\tbpb" >> $irName$ir$aName$alphaDen.dat
            for((ctx=2; ctx<$maxCtx; ++ctx))
            do
            ./phoenix -m t,$ctx,$alphaDen,$ir -t $dataset.fa >> $irName$ir$aName$alphaDen.dat
            done
        done
    done
done

gnuplot <<- EOF
set xlabel "context"
set ylabel "bpb"
set key right bottom
set term $PIXFORMAT
set output "ir0.dat.$PIXFORMAT"
plot    "ir0ad1.dat" using 3:4  with linespoints ls 6 title "ir=0, alpha=1/1    ",\
        "ir0ad10.dat" using 3:4 with linespoints ls 7 title "ir=0, alpha=1/10  ",\
        "ir0ad100.dat" using 3:4 with linespoints ls 8 title "ir=0, alpha=1/100"
set output "ir1.dat.$PIXFORMAT"
plot    "ir1ad1.dat" using 3:4  with linespoints ls 6 title "ir=1, alpha=1/1    ",\
        "ir1ad10.dat" using 3:4 with linespoints ls 7 title "ir=1, alpha=1/10  ",\
        "ir1ad100.dat" using 3:4 with linespoints ls 8 title "ir=1, alpha=1/100"
EOF