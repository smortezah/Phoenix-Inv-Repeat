#!/usr/bin/env bash


VIRUSES_NAME="viruses"

### download FASTA
perl $FLD_script/downloadViruses.pl


### filter results by the word "complete_genome" and split reads
cat $VIRUSES_NAME.$FTYPE | tr ' ' '_' \
    | $FLD_GOOSE/src/goose-extractreadbypattern complete_genome \
    | $FLD_GOOSE/src/goose-splitreads


### FASTA -> SEQ. rename out$i -> V$i. save in dataset/viruses folder
mkdir $FLD_dataset/$FLD_viruses

for i in $V_SEQ_RUN; do
    grep -v ">" out$i.$FTYPE > $FLD_dataset/$FLD_viruses/$VIRUSES$i;
done
rm -f out*.fa

#mv $VIRUSES_NAME.$FTYPE $FLD_dataset
