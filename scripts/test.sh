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
INSTALL_XS=0    # to install "XS" from Github
INSTALL_goose=0 # to install "goose" from Github
GEN_DATASETS=0  # generate datasets using "XS"
GEN_MUTATIONS=1 # generate mutations using "goose"
RUN=0           # run the program


#***********************************************************
#   install "XS" from Github
#***********************************************************
if [[ $INSTALL_XS == 1 ]]; then
rm -fr XS
git clone https://github.com/pratas/XS.git
cd XS
make
cd ..
fi  # end of installing "XS"


#***********************************************************
#   install "goose" from Github
#***********************************************************
if [[ $INSTALL_goose == 1 ]]; then
rm -fr goose-* goose/ SAMPLE*
git clone https://github.com/pratas/goose.git
cd goose/src
make
cd ../../
fi  # end of installing "goose"


#***********************************************************
#   generate datasets using "XS"
#***********************************************************
if [[ $GEN_DATASETS == 1 ]]; then
NUM_DATASETS=1  # number of datasets to be generated

XS/XS -ls 100 -n 100000 -rn 0 -f 0.20,0.20,0.20,0.20,0.20 -eh -eo -es nonRep0   # non-repetitive
# add ">X" as the header of the sequence (build "nonRepX")
echo ">X" > HEADER;
cat HEADER nonRep0 > nonRepX;
rm -f HEADER
fi  # end of generating datasets using "XS"


#***********************************************************
#   generate mutations using "goose"
#***********************************************************
if [[ $GEN_MUTATIONS == 1 ]]; then
NUM_MUTATIONS=1 # number of mutations to be generated

for x in 1 2 3 12 #((x=1; x<$((NUM_MUTATIONS+1)); x+=1));
do
MRATE=`echo "scale=3;$x/100" | bc -l`;      # handle transition 0.09 -> 0.10
goose/src/goose-mutatefasta -s $x -a5 -mr $MRATE " " < chromosomes/hs_ref_GRCh38.p7_chr21.fa > chr21temp$x;
cat chr21temp$x | grep -v ">" > chr21_$x    # remove the header line
done
rm -f chr21temp*    # remove temporary files

#-----------------------------------
#   move all generated dataset files to "datasets" folder
#-----------------------------------
rm -fr datasets
mkdir -p datasets
mv ./chr21* datasets

fi  # end of generating mutations using "goose"


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