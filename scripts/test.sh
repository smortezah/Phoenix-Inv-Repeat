#!/bin/bash

cd ..
cmake ./source
make

PIXFORMAT=png
#PIXFORMAT=svg

irName=ir
aName=ad    # alpha denominator name
maxCtx=21   # real: -=1

for dataset in b #c b a
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

gnuplot <<- EOF
set xlabel "context"
set ylabel "bpb"
set key right bottom
set term $PIXFORMAT
set output "ir$ir$dataset.$PIXFORMAT"
plot "$irName$ir${aName}1${dataset}.dat" using 3:4  with linespoints ls 6 title "ir=$ir, alpha=1/1,     $dataset", \
     "$irName$ir${aName}10${dataset}.dat" using 3:4 with linespoints ls 7 title "ir=$ir, alpha=1/10,   $dataset", \
     "$irName$ir${aName}100${dataset}.dat" using 3:4 with linespoints ls 8 title "ir=$ir, alpha=1/100, $dataset"
EOF

    done
done

rm -r dat
mkdir -p dat
mv $irName*.dat dat