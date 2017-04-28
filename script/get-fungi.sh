#!/bin/bash


FUNGI_NAME="fungi"

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#   download FASTA
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
perl $FLD_script/downloadFungi.pl


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#   filter results by the word "complete_genome" and split reads
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
cat $FUNGI_NAME.$FTYPE | tr ' ' '_' \
    | $FLD_GOOSE/src/goose-extractreadbypattern complete_genome \
    | $FLD_GOOSE/src/goose-splitreads

if [ ! -d $FLD_chromosomes/$FLD_fungi ]; then 
    mkdir -p $FLD_chromosomes/$FLD_fungi;
fi

mv out* $FLD_chromosomes/$FLD_fungi


##%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
##   FASTA -> SEQ. rename out$i -> F$i. save in dataset/fungi folder
##%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#if [ ! -d $FLD_dataset/$FLD_fungi ]; then mkdir -p $FLD_dataset/$FLD_fungi; fi 
#
#for i in $F_SEQ_RUN; do
#    grep -v ">" out$i.$FTYPE > $FLD_dataset/$FLD_fungi/$FUNGI$i;
#done
#rm -f out*.fa
#
##mv $FUNGI_NAME.$FTYPE $FLD_dataset
