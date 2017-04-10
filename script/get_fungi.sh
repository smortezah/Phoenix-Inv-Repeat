#!/usr/bin/env bash


FUNGI_NAME="fungi"

### download FASTA
perl $FLD_script/downloadFungi.pl


### filter results by the word "complete_genome" and split reads
cat $FUNGI_NAME.$FILE_TYPE | tr ' ' '_' \
    | $FLD_GOOSE/src/goose-extractreadbypattern complete_genome \
    | $FLD_GOOSE/src/goose-splitreads


### FASTA -> SEQ. rename out$i -> F$i. save in dataset/fungi folder
mkdir $FLD_dataset/$FLD_fungi

for i in $F_SEQ_RUN; do
    grep -v ">" out$i.$FILE_TYPE > $FLD_dataset/$FLD_fungi/$FUNGI_CHR$i;
done
rm -f out*.fa

#mv $FUNGI_NAME.$FILE_TYPE $FLD_dataset
