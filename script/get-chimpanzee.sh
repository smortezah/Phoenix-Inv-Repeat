#!/bin/bash


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#   download FASTA
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
for i in 1 2A 2B {3..22} X Y MT; do
    wget $CHIMPANZEE_URL/$CHIMPANZEE_CHROMOSOME$i.$FTYPE.$COMP_FTYPE;
    gunzip < $CHIMPANZEE_CHROMOSOME$i.$FTYPE.$COMP_FTYPE \
     > $FLD_chromosomes/$CHIMPANZEE$i.$FTYPE;
    rm $CHIMPANZEE_CHROMOSOME$i.$FTYPE.$COMP_FTYPE
done

for i in unlocalized unplaced; do
    wget $CHIMPANZEE_URL/$CHIMPANZEE_CHR_PREFIX$i.$FTYPE.$COMP_FTYPE;
    gunzip < $CHIMPANZEE_CHR_PREFIX$i.$FTYPE.$COMP_FTYPE \
     > $FLD_chromosomes/$CHIMPANZEE$i.$FTYPE;
    rm $CHIMPANZEE_CHR_PREFIX$i.$FTYPE.$COMP_FTYPE
done


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#   rename: PTunlocalized -> PTUL, PTunplaced -> PTUP
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
mv $FLD_chromosomes/$CHIMPANZEE"unlocalized".$FTYPE \
    $FLD_chromosomes/$CHIMPANZEE"UL".$FTYPE
mv $FLD_chromosomes/$CHIMPANZEE"unplaced".$FTYPE \
    $FLD_chromosomes/$CHIMPANZEE"UP".$FTYPE


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#   FASTA -> SEQ
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
for i in $PT_SEQ_RUN; do
    cat $FLD_chromosomes/$CHIMPANZEE$i.$FTYPE \
        | grep -v -e "ERROR" -e "eFetchResult" -e "DOCTYPE" -e "xml version" \
                  -e "Unable to obtain" | grep -v ">" \
     > $FLD_dataset/$CHIMPANZEE$i
done
