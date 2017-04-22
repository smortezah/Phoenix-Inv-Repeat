#!/usr/bin/env bash


BACTERIA_NAME="bacteria"

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#   download FASTA
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
perl $FLD_script/downloadBacteria.pl


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#   filter results by the word "complete_genome" and split reads
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
cat $BACTERIA_NAME.$FTYPE | tr ' ' '_' \
    | $FLD_GOOSE/src/goose-extractreadbypattern complete_genome \
    | $FLD_GOOSE/src/goose-splitreads


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#   FASTA -> SEQ. rename out$i -> B$i. save in dataset/bacteria folder
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
if [ ! -d $FLD_dataset/$FLD_bacteria ]; then 
    mkdir -p $FLD_dataset/$FLD_bacteria; 
fi

for i in $B_SEQ_RUN; do
    grep -v ">" out$i.$FTYPE > $FLD_dataset/$FLD_bacteria/$BACTERIA$i;
done
rm -f out*.fa

#mv $BACTERIA_NAME.$FTYPE $FLD_dataset
