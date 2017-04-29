#!/bin/bash


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#   download FASTA
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
for i in {1..30} MT W Z; do
    wget $TURKEY_URL/$TURKEY_CHROMOSOME$i.$FTYPE.$COMP_FTYPE;
    gunzip < $TURKEY_CHROMOSOME$i.$FTYPE.$COMP_FTYPE \
     > $FLD_chromosomes/$TURKEY$i.$FTYPE;
    rm $TURKEY_CHROMOSOME$i.$FTYPE.$COMP_FTYPE
done

for i in unlocalized unplaced; do
    wget $TURKEY_URL/$TURKEY_CHR_PREFIX$i.$FTYPE.$COMP_FTYPE;
    gunzip < $TURKEY_CHR_PREFIX$i.$FTYPE.$COMP_FTYPE \
     > $FLD_chromosomes/$TURKEY$i.$FTYPE;
    rm $TURKEY_CHR_PREFIX$i.$FTYPE.$COMP_FTYPE
done


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#   rename: MGAunlocalized -> MGAUL, MGAunplaced -> MGAUP
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
mv $FLD_chromosomes/$TURKEY"unlocalized".$FTYPE \
    $FLD_chromosomes/$TURKEY"UL".$FTYPE
mv $FLD_chromosomes/$TURKEY"unplaced".$FTYPE \
    $FLD_chromosomes/$TURKEY"UP".$FTYPE


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#   FASTA -> SEQ
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
for i in $MGA_SEQ_RUN; do
    cat $FLD_chromosomes/$TURKEY$i.$FTYPE \
        | grep -v -e "ERROR" -e "eFetchResult" -e "DOCTYPE" -e "xml version" \
                  -e "Unable to obtain" | grep -v -x ">" \
     > $FLD_dataset/$TURKEY$i
done
