#!/usr/bin/env bash


ARCHAEA_NAME="archaea"

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#   download FASTA
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
perl $FLD_script/downloadArchaea.pl


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#   filter results by the word "complete_genome" and split reads
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
cat $ARCHAEA_NAME.$FTYPE | tr ' ' '_' \
    | $FLD_GOOSE/src/goose-extractreadbypattern complete_genome \
    | $FLD_GOOSE/src/goose-splitreads

if [ ! -d $FLD_chromosomes/$FLD_archaea ]; then
    mkdir -p $FLD_chromosomes/$FLD_archaea;
fi

mv out* $FLD_chromosomes/$FLD_archaea


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#   FASTA -> SEQ. rename out$i -> A$i. save in dataset/archaea folder
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
if [ ! -d $FLD_dataset/$FLD_archaea ]; then 
    mkdir -p $FLD_dataset/$FLD_archaea; 
fi

for i in $A_SEQ_RUN; do
    grep -v ">" out$i.$FTYPE > $FLD_dataset/$FLD_archaea/$ARCHAEA$i;
done
rm -f out*.fa

#mv $ARCHAEA_NAME.$FTYPE $FLD_dataset
