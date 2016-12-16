#!/bin/bash

#***********************************************************
#   change directory to "source" and make the project
#***********************************************************
cd ..
cmake source
make


#***********************************************************
#   parameters to install and run needed programs
#***********************************************************
GEN_DATASET=0   # generate dataset using "XS"
INSTALL_XS=0    # to install "XS" from Github
INSTALL_goose=0 # to install "goose" from Github
RUN=1           # run the program

NUM_DATASETS=1   # number of datasets to be generated

#***********************************************************
#   generate dataset using "XS" and "goose"
#***********************************************************
if [[ $GEN_DATASET == 1 ]]; then

# dataset names: nonRep=non repetitve,  midRep=mid repetitve,   tooRep=too repetitve

#-----------------------------------
#   install "XS" from Github
#-----------------------------------
    if [[ $INSTALL_XS == 1 ]]; then
    rm -fr XS
    git clone https://github.com/pratas/XS.git
    cd XS
    make
    cd ..
    fi  # end of installing "XS"


#./XS -v -t 1 -i n=MySeq -ls 70 -n 10000 -rn 1000 -rr -eh -eo -es a     # the most repetitive
#./XS -v -t 1 -i n=MySeq -ls 70 -n 10000 -rn 50 -rr -eh -eo -es b       # in the middle
#./XS -v -t 1 -i n=MySeq -ls 70 -n 10000 -rn 0 -eh -eo -es c            # the least repetitive
#mv a ..
#mv b ..
#mv c ..
#cd ..

#-----------------------------------
#   install "goose" from Github
#-----------------------------------
    if [[ $INSTALL_goose == 1 ]]; then
    rm -fr goose-* goose/ SAMPLE*
    git clone https://github.com/pratas/goose.git
    cd goose/src
    make
    cd ../../
    fi  # end of installing "goose"

#-----------------------------------
#   generate the original sequence ("nonRep0")
#-----------------------------------
XS/XS -ls 100 -n 100000 -rn 0 -f 0.20,0.20,0.20,0.20,0.20 -eh -eo -es nonRep0   # non-repetitive

# add ">X" as the header of the sequence (build "nonRepX")
echo ">X" > HEADER;
cat HEADER nonRep0 > nonRepX;
rm -f HEADER

#-----------------------------------
#   generate the mutated sequences
#-----------------------------------
for((x=1; x<$((NUM_DATASETS+1)); x+=80));
do
MRATE=`echo "scale=3;$x/100" | bc -l`;      # handle transition 0.09 -> 0.10
#echo "Substitutions rate: $MRATE";
goose/src/goose-mutatefasta -s $x -a5 -mr $MRATE " " < nonRepX > nonRepTemp$x;
cat nonRepTemp$x | grep -v ">" > nonRep$x   # remove the header line
done
rm -f nonRepX nonRepTemp*                   # remove temporary files

##-----------------------------------
##   move all generated dataset files to "datasets" folder
##-----------------------------------
#rm -fr datasets
#mkdir -p datasets
#mv ./nonRep* datasets

fi  # end of generating dataset


#***********************************************************
#   running the program
#***********************************************************
if [[ $RUN == 1 ]]; then

#-----------------------------------
#   set output format
#-----------------------------------
PIX_FORMAT=png
#PIX_FORMAT=svg

#rm -f $IR_NAME*.$PIX_FORMAT   # remove FORMAT pictures, if they exist

#-----------------------------------
#   list of datasets
#-----------------------------------
nonRep="nonRep"
midRep="midRep"
tooRep="tooRep"
datasets="nonRep midRep tooRep"

#-----------------------------------
#   names for inverted repeat and alpha denominator
#-----------------------------------
IR_NAME=ir
a_NAME=ad

#-----------------------------------
#   list of inverted repeats and alpha denominators
#-----------------------------------
INV_REPEATS="0"       # "0 1"        list of inverted repeats
ALPHA_DENS="20"        # "1 20 100"   list of alpha denominators

#-----------------------------------
#   max context size
#-----------------------------------
MAX_CTX=21   # 21    real: -=1


#-----------------------------------
#   create a couple of files to save per mutation results
#-----------------------------------
for ir in $INV_REPEATS
do
    for alphaDen in $ALPHA_DENS
    do
    rm -f $IR_NAME$ir-$a_NAME$alphaDen-$nonRep*.dat
    touch $IR_NAME$ir-$a_NAME$alphaDen-$nonRep.dat
    echo -e "# mut\tmin_bpb\tmin_ctx" >> $IR_NAME$ir-$a_NAME$alphaDen-$nonRep.dat
    done
done

#-----------------------------------
#   run the program for different datasets, ir, alphaDen and context sizes
#-----------------------------------
for mut in 1 #`seq -s' ' 1 $NUM_DATASETS`
do
    for dataset in "hs_ref_GRCh38.p7_chr21.fa" #"$nonRep$mut"
    do
        for ir in $INV_REPEATS
        do
            for alphaDen in $ALPHA_DENS
            do
#            rm -f $IR_NAME$ir-$a_NAME$alphaDen-$dataset.dat
            touch $IR_NAME$ir-$a_NAME$alphaDen-$dataset.dat
            echo -e "# ir\talpha\tctx\tbpb\ttime(s)" >> $IR_NAME$ir-$a_NAME$alphaDen-$dataset.dat

                for((ctx=2; ctx<$MAX_CTX; ++ctx))
                do
                ./phoenix -m t,$ctx,$alphaDen,$ir -t ./datasets/$dataset >> $IR_NAME$ir-$a_NAME$alphaDen-$dataset.dat
                done
                # save "min bpb" and "min ctx" for each dataset
                minBpbCtx=$(awk 'NR==1 || $4 < minBpb {minBpb=$4; minCtx=$3}; \
                            END {print minBpb"\t"minCtx}' $IR_NAME$ir-$a_NAME$alphaDen-$dataset.dat)
                echo -e "  $mut\t$minBpbCtx" >> $IR_NAME$ir-$a_NAME$alphaDen-$nonRep.dat
            done

### show output in a figure, using gnuplot
##gnuplot <<- EOF
###set xlabel "context"
##set xlabel "% mutation"
##set ylabel "bpb"
##set key right top                   # legend position
##set term $PIX_FORMAT                 # set terminal for output picture format
##set output "$IR_NAME$ir.$PIX_FORMAT"  # set output name
##
##plot for [i=5:13] 'immigration.dat' using 1:(column(i)/Sum[i]) title columnhea
##plot for [i=0:1]
#### find min bpb for each dataset
###stats "$IR_NAME$ir-$a_NAME$ALPHA_DENS-$dataset.dat" using 4 name "bpb" nooutput
###plot $mut $bpb_min
##
#### plot 3 figures at once, for constant "ir", but different "alpha"s and "context"s
###plot "$IR_NAME$ir-${a_NAME}1-$dataset.dat" using 3:4  with linespoints ls 6 title "ir=$ir, alpha=1/1,     $dataset", \
###     "$IR_NAME$ir-${a_NAME}10-$dataset.dat" using 3:4 with linespoints ls 7 title "ir=$ir, alpha=1/10,   $dataset", \
###     "$IR_NAME$ir-${a_NAME}100-$dataset.dat" using 3:4 with linespoints ls 8 title "ir=$ir, alpha=1/100, $dataset"
##
### the following line (EOF) MUST be left as it is; i.e. no space, etc
##EOF
        done
    done
done

#-----------------------------------
#   plot output using "gnuplot"
#-----------------------------------
for ir in $INV_REPEATS
do
gnuplot <<- EOF
set xlabel "% mutation"                 # set label of x axis
set ylabel "bpb"                        # set label of y axis
set xtics 0,5,100                       # set steps for x axis
set xtics add ("1" 1)
set key right                           # legend position
set term $PIX_FORMAT                    # set terminal for output picture format
set output "$IR_NAME$ir.$PIX_FORMAT"    # set output name
plot "$IR_NAME$ir-${a_NAME}20-$nonRep.dat" using 1:2  with linespoints ls 7 title "ir=$ir, alpha=1/20, $nonRep"

set ylabel "context size"               # set label of y axis
set ytics 2,1,20                        # set steps for y axis
set output "$IR_NAME$ir-ctx.$PIX_FORMAT"    # set output name
plot "$IR_NAME$ir-${a_NAME}20-$nonRep.dat" using 1:3  with linespoints ls 7 title "ir=$ir, alpha=1/20, $nonRep"

# the following line (EOF) MUST be left as it is; i.e. no space, etc
EOF
done

fi  # end of running the program


##-----------------------------------
##   create "dat" folder to save the results of running
##-----------------------------------
#rm -fr dat              # remove "dat" folder, if it already exists
#mkdir -p dat            # make "dat" folder
#mv $IR_NAME*.dat dat     # move all created dat files to the "dat" folder