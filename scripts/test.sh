#!/bin/bash

## change directory to "source" and make the project
#cd ..
#cmake ./source
#make
#
#GET_XS=0        # to get XS code from Github
#INSTALL_XS=0    # to install XS code from Github
#GEN_DATASET=1   # generate dataset
#
## get XS code from Github
#if [[ $GET_XS == 1 ]]; then
#git clone https://github.com/pratas/XS.git
#fi
#
## install XS code from Github
#if [[ $INSTALL_XS == 1 ]]; then
#cd ./XS
#make
#cd ..
#fi
#
## generate dataset using XS and save in "dataset" directory
#if [[ $GEN_DATASET == 1 ]]; then
#cd ./XS
#
##./XS -v -t 1 -i n=MySeq -ls 70 -n 10000 -rn 1000 -rr -eh -eo -es a.fa       # the most repetitive
##./XS -v -t 1 -i n=MySeq -ls 70 -n 10000 -rn 50 -rr -eh -eo -es b.fa         # in the middle
##./XS -v -t 1 -i n=MySeq -ls 70 -n 10000 -rn 0 -eh -eo -es c.fa              # the least repetitive
##mv a.fa ..
##mv b.fa ..
##mv c.fa ..
##cd ..
#
## dataset names: tooRep=too repetitve,  midRep=mid repetitve,   nonRep=non repetitve
#for((i=0; i!=2; ++i))
#do
#./XS -t 1 -i n=MySeq -ls 100 -n 100 -rn 50 -rr -rm 0.0$i -eh -eo -es tooRep$i.fa       # the most repetitive
#done
#cd ..
#rm -rf datasets
#mkdir -p datasets
#mv ./XS/tooRep*.fa datasets
#fi
#
## list of datasets
##datasets="a"
##datasets="tooRep0 tooRep1"
datasets="tooRep"
echo $datasets

##invRepeats="0 1"            # list of inverted repeats
##alphaDens="1 10 100"        # list of alpha denominators
#invRepeats="0"
#alphaDens="20"
#
## set output format
#PIXFORMAT=png
##PIXFORMAT=svg
#
#rm -f $irName*.$PIXFORMAT   # remove FORMAT pictures, if they exist
#
## set names for inverted repeat, alpha denominator, max context size
#irName=ir
#aName=ad
#maxCtx=21   # real: -=1
#
##for
##do
#for dataset in $datasets
#do
#    for ir in $invRepeats
#    do
#        for alphaDen in $alphaDens
#        do
##        rm -f $irName$ir$aName$alphaDen$dataset.dat
#        touch $irName$ir-$aName$alphaDen-$dataset.dat
#        echo -e "# mut\tir\talpha\tctx\tbpb" >> $irName$ir-$aName$alphaDen-$dataset.dat
#
#            # execute the code with different parameters
#            for((ctx=2; ctx<$maxCtx; ++ctx))
#            do
#            ./phoenix -m t,$ctx,$alphaDen,$ir -t $dataset.fa >> $irName$ir-$aName$alphaDen-$dataset.dat
#            done
#        done
#
## show output in a figure, using gnuplot
#gnuplot <<- EOF
#set xlabel "context"
#set ylabel "bpb"
#set key right top       # legend position
#set term $PIXFORMAT     # set terminal for output picture format
#set output "$irName$ir$dataset.$PIXFORMAT"       # set output name
#
## plot 3 figures at once, for constant "ir", but different "alpha"s and "context"s
#plot "$irName$ir-${aName}1-$dataset.dat" using 3:4  with linespoints ls 6 title "ir=$ir, alpha=1/1,     $dataset", \
#     "$irName$ir-${aName}10-$dataset.dat" using 3:4 with linespoints ls 7 title "ir=$ir, alpha=1/10,   $dataset", \
#     "$irName$ir-${aName}100-$dataset.dat" using 3:4 with linespoints ls 8 title "ir=$ir, alpha=1/100, $dataset"
#
## the following line (EOF) MUST be left as it is; i.e. no space, etc
#EOF
#    done
#done
##done
#
#rm -rf dat              # remove "dat" folder, if it exists
#mkdir -p dat            # make "dat" folder
#mv $irName*.dat dat     # move all created dat files to the "dat" folder