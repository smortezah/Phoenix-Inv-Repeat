#!/bin/bash


VIRUSES_NAME="viruses"

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#   download FASTA
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
perl $FLD_script/downloadViruses.pl


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#   filter results by the word "complete_genome" and split reads
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
cat $VIRUSES_NAME.$FTYPE | tr ' ' '_' \
    | $FLD_GOOSE/src/goose-extractreadbypattern complete_genome \
    | $FLD_GOOSE/src/goose-splitreads

if [ ! -d $FLD_chromosomes/$FLD_viruses ]; then 
    mkdir -p $FLD_chromosomes/$FLD_viruses;
fi

mv out* $FLD_chromosomes/$FLD_viruses


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#   FASTA -> SEQ. rename out$i -> V$i. save in dataset/viruses folder
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
if [ ! -d $FLD_dataset/$FLD_viruses ]; then mkdir -p $FLD_dataset/$VIRUSES; fi

for i in $V_SEQ_RUN; do
    cat $FLD_chromosomes/$FLD_viruses/out$i.$FTYPE \
        | grep -v -e "ERROR" -e "eFetchResult" -e "DOCTYPE" -e "xml version" \
                  -e "Unable to obtain" | grep -v -x ">" \
     > $FLD_dataset/$VIRUSES/$i
done

#mv $VIRUSES_NAME.$FTYPE $FLD_dataset
