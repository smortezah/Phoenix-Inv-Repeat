#!/usr/bin/env bash


ARCHAEA_NAME="archaea"

### download FASTA
perl $FLD_script/downloadArchaea.pl


### filter results by the word "complete_genome" and split reads
cat $ARCHAEA_NAME.$FILE_TYPE | tr ' ' '_' \
    | $FLD_GOOSE/src/goose-extractreadbypattern complete_genome \
    | $FLD_GOOSE/src/goose-splitreads


### FASTA -> SEQ. rename out$i -> A$i. save in dataset/archaea folder
mkdir $FLD_dataset/$FLD_archaea

for i in $A_SEQ_RUN; do
    grep -v ">" out$i.$FILE_TYPE > $FLD_dataset/$FLD_archaea/$ARCHAEA_CHR$i;
done
rm -f out*.fa

#mv $ARCHAEA_NAME.$FILE_TYPE $FLD_dataset
