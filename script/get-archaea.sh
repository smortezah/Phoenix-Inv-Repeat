#!/bin/bash


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
if [ ! -d $FLD_dataset/$FLD_archaea ]; then mkdir -p $FLD_dataset/$ARCHAEA; fi

for i in $A_SEQ_RUN; do
    cat $FLD_chromosomes/$FLD_archaea/out$i.$FTYPE \
        | grep -v -e "ERROR" -e "eFetchResult" -e "DOCTYPE" -e "xml version" \
                  -e "Unable to obtain" | grep -v -x ">" \
     > $FLD_dataset/$ARCHAEA/$i
done

#mv $ARCHAEA_NAME.$FTYPE $FLD_dataset
