#!/bin/bash

# change directory to "source" and make the project
cd ..
cmake source
make

GET_XS=0        # to get XS code from Github
INSTALL_XS=0    # to install XS from Github
GEN_DATASET=0   # generate dataset using XS
INSTALL_goose=0 # to install goose from Github

numDataset=1    # number of generated datasets

# get XS code from Github
if [[ $GET_XS == 1 ]]; then
git clone https://github.com/pratas/XS.git
fi

# install XS code from Github
if [[ $INSTALL_XS == 1 ]]; then
cd XS
make
cd ..
fi

# generate dataset using XS and save in "dataset" directory
#if [[ $GEN_DATASET == 1 ]]; then

#./XS -v -t 1 -i n=MySeq -ls 70 -n 10000 -rn 1000 -rr -eh -eo -es a.fa       # the most repetitive
#./XS -v -t 1 -i n=MySeq -ls 70 -n 10000 -rn 50 -rr -eh -eo -es b.fa         # in the middle
#./XS -v -t 1 -i n=MySeq -ls 70 -n 10000 -rn 0 -eh -eo -es c.fa              # the least repetitive
#mv a.fa ..
#mv b.fa ..
#mv c.fa ..
#cd ..

    if [[ $INSTALL_XS == 1 ]]; then
    rm -fr goose-* goose/ SAMPLE* DB-mfa;
    # GET GOOSE FRAMEWORK =========================================================
    git clone https://github.com/pratas/goose.git
    cd goose/src
    make
    cd ../../
    fi

#XS/XS -v -ls 100 -n 100 -s 0 SAMPLE.fq
XS/XS -ls 100 -n 100 -rn 1000 -rr -eh -eo -es SAMPLE       # the most repetitive

# MUTATE ======================================================================
#goose/src/goose-fastq2fasta < SAMPLE.fq > SAMPLE.fa
#goose/src/goose-fasta2seq   < SAMPLE.fa > SAMPLE
#goose/src/goose-seq2fasta -n "Substitution0" < SAMPLE > SAMPLE0.fa
#cat SAMPLE0.fa > DB.mfa;
for((x=1 ; x<2 ; ++x));
  do
  MRATE=`echo "scale=3;$x/100" | bc -l`;
  echo "Substitutions rate: $MRATE";
  goose/src/goose-mutatedna -s $x -mr $MRATE " " < SAMPLE > SAMPLE$x;
#  goose/src/goose-seq2fasta -n "Substitution$x" < SAMPLE$x > SAMPLE$x.fa
#  cat SAMPLE$x.fa >> DB.mfa;
  done


#
## dataset names: tooRep=too repetitve,  midRep=mid repetitve,   nonRep=non repetitve
#for((i=0; i!=$numDataset; ++i))
#do
#./XS -t 1 -i n=MySeq -ls 100 -n 100000 -rn 50000 -rr -rm 0.0$i -eh -eo -es tooRep$i.fa       # the most repetitive
#done
#cd ..
#rm -rf datasets
#mkdir -p datasets
#mv ./XS/tooRep*.fa datasets
#fi


## list of datasets
#tooRep="tooRep"
#datasets="tooRep midRep nonRep"
#
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
## create new files to save per mutation results
#for ir in $invRepeats
#do
#    for alphaDen in $alphaDens
#    do
##    rm -f $irName$ir-$aName$alphaDen-$tooRep.dat
#    touch $irName$ir-$aName$alphaDen-$tooRep.dat
#    echo -e "# mut\tmin_bpb" >> $irName$ir-$aName$alphaDen-$tooRep.dat
#    done
#done
#
#for mut in 0 #{0..49}
#do
#    for dataset in "$tooRep$mut"
#    do
#        for ir in $invRepeats
#        do
#            for alphaDen in $alphaDens
#            do
##            rm -f $irName$ir-$aName$alphaDen-$dataset.dat
#            touch $irName$ir-$aName$alphaDen-$dataset.dat
#            echo -e "# ir\talpha\tctx\tbpb" >> $irName$ir-$aName$alphaDen-$dataset.dat
#
#                # execute the code with different parameters
#                for((ctx=2; ctx<$maxCtx; ++ctx))
#                do
#                ./phoenix -m t,$ctx,$alphaDen,$ir -t ./datasets/$dataset.fa >> $irName$ir-$aName$alphaDen-$dataset.dat
#                done
#                # save min bpb for each dataset
#                minBpb=$(awk 'NR==1 || $4 < min {min = $4}; END {print min}' $irName$ir-$aName$alphaDen-$dataset.dat)
#                echo -e "  $mut\t$minBpb" >> $irName$ir-$aName$alphaDen-$tooRep.dat
#            done
#
### show output in a figure, using gnuplot
##gnuplot <<- EOF
###set xlabel "context"
##set xlabel "% mutation"
##set ylabel "bpb"
##set key right top                   # legend position
##set term $PIXFORMAT                 # set terminal for output picture format
##set output "$irName$ir.$PIXFORMAT"  # set output name
##
##plot for [i=5:13] 'immigration.dat' using 1:(column(i)/Sum[i]) title columnhea
##plot for [i=0:1]
#### find min bpb for each dataset
###stats "$irName$ir-$aName$alphaDens-$dataset.dat" using 4 name "bpb" nooutput
###plot $mut $bpb_min
##
#### plot 3 figures at once, for constant "ir", but different "alpha"s and "context"s
###plot "$irName$ir-${aName}1-$dataset.dat" using 3:4  with linespoints ls 6 title "ir=$ir, alpha=1/1,     $dataset", \
###     "$irName$ir-${aName}10-$dataset.dat" using 3:4 with linespoints ls 7 title "ir=$ir, alpha=1/10,   $dataset", \
###     "$irName$ir-${aName}100-$dataset.dat" using 3:4 with linespoints ls 8 title "ir=$ir, alpha=1/100, $dataset"
##
### the following line (EOF) MUST be left as it is; i.e. no space, etc
##EOF
#        done
#    done
#done
#
## show output in a figure, using gnuplot
#gnuplot <<- EOF
##set xlabel "context"
#set xlabel "% mutation"
#set ylabel "bpb"
#set key right top                   # legend position
#set term $PIXFORMAT                 # set terminal for output picture format
#set output "$irName$ir.$PIXFORMAT"  # set output name
#
#plot "$irName$ir-${aName}20-$tooRep.dat" using 1:2  with linespoints ls 7 title "ir=$ir, alpha=1/20, $tooRep"
#
## the following line (EOF) MUST be left as it is; i.e. no space, etc
#EOF
#
#rm -rf dat              # remove "dat" folder, if it exists
#mkdir -p dat            # make "dat" folder
#mv $irName*.dat dat     # move all created dat files to the "dat" folder
