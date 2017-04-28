#!/bin/bash


for c in $chromosomes; do
    for x in $MUT_LIST; do      #((x=1; x<$NUM_MUTATIONS; x+=1));
        MRATE=`echo "scale=3;$x/100" | bc -l`;  # handle transition 0.09 -> 0.10
#        goose/src/goose-mutatefasta -s $x -a5 -mr $MRATE " " \
#            < $FLD_chromosomes/$c.$FTYPE > temp;
#        cat temp | grep -v ">" > $HUMAN${CURR_CHR}_$x   # remove header line
        goose/src/goose-mutatefasta -s $x -a5 -mr $MRATE " " \
         < $FLD_chromosomes/$c.$FTYPE \
         | grep -v ">" > $HUMAN${CURR_CHR}_$x    # remove header line
    done
done
#rm -f temp*

mv ${HUMAN}* $FLD_dataset
