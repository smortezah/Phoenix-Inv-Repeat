#!/usr/bin/env bash


BACTERIA_NAME="bacteria"

### download FASTA
perl $FLD_script/downloadBacteria.pl


### filter results by the word "complete_genome" and split reads
cat $BACTERIA_NAME.$FILE_TYPE | tr ' ' '_' \
    | $FLD_GOOSE/src/goose-extractreadbypattern complete_genome \
    | $FLD_GOOSE/src/goose-splitreads


### FASTA -> SEQ. rename out$i -> B$i. save in dataset/bacteria folder
mkdir $FLD_dataset/$FLD_bacteria

for i in $B_SEQ_RUN; do
    grep -v ">" out$i.$FILE_TYPE > $FLD_dataset/$FLD_bacteria/$BACTERIA_CHR$i;
done
rm -f out*.fa

#mv $BACTERIA_NAME.$FILE_TYPE $FLD_dataset
