#!/bin/bash

# change directory to "source" and make the project
cd ..
cmake ./source
make

# list of datasets
#datasets="c"
datasets="b"
#datasets="a"
#datasets="c b"
#datasets="c b a"

# list of inverted repeats
invRepeats="0 1"

# list of inverted repeats
alphaDens="1 10 100"

# set output format
PIXFORMAT=png
#PIXFORMAT=svg

# remove FORMAT pictures, if they exist
rm -f $irName*.$PIXFORMAT

# set names for inverted repeat, alpha denominator, max context size
irName=ir
aName=ad
maxCtx=21   # real: -=1

for dataset in $datasets
do
    for ir in $invRepeats
    do
        for alphaDen in $alphaDens
        do
#        rm -f $irName$ir$aName$alphaDen$dataset.dat
        touch $irName$ir-$aName$alphaDen-$dataset.dat
        echo -e "# ir\talpha\tctx\tbpb" >> $irName$ir-$aName$alphaDen-$dataset.dat

            # execute the code with different parameters
            for((ctx=2; ctx<$maxCtx; ++ctx))
            do
            ./phoenix -m t,$ctx,$alphaDen,$ir -t $dataset.fa >> $irName$ir-$aName$alphaDen-$dataset.dat
            done
        done

# show output in a figure, using gnuplot
gnuplot <<- EOF
set xlabel "context"
set ylabel "bpb"
set key right bottom    # legend position
set term $PIXFORMAT     # set terminal for output picture format
set output "$irName$ir$dataset.$PIXFORMAT"       # set output name

# plot 3 figures at once, for constant "ir", but different "alpha"s and "context"s
plot "$irName$ir-${aName}1-$dataset.dat" using 3:4  with linespoints ls 6 title "ir=$ir, alpha=1/1,     $dataset", \
     "$irName$ir-${aName}10-$dataset.dat" using 3:4 with linespoints ls 7 title "ir=$ir, alpha=1/10,   $dataset", \
     "$irName$ir-${aName}100-$dataset.dat" using 3:4 with linespoints ls 8 title "ir=$ir, alpha=1/100, $dataset"

# the following line (EOF) MUST be left as it is; i.e. no space, etc
EOF
    done
done

rm -rf dat              # remove "dat" folder, if it exists
mkdir -p dat            # make "dat" folder
mv $irName*.dat dat     # move all created dat files to the "dat" folder