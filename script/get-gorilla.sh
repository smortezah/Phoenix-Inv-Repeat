#!/bin/bash


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#   download FASTA
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
for i in 1 2A 2B {3..22} X MT; do
    wget $GORILLA_URL/$GORILLA_CHROMOSOME$i.$FTYPE.$COMP_FTYPE;
    gunzip < $GORILLA_CHROMOSOME$i.$FTYPE.$COMP_FTYPE \
     > $FLD_chromosomes/$GORILLA$i.$FTYPE;
    rm $GORILLA_CHROMOSOME$i.$FTYPE.$COMP_FTYPE
done

for i in unlocalized unplaced; do
    wget $GORILLA_URL/$GORILLA_CHR_PREFIX$i.$FTYPE.$COMP_FTYPE;
    gunzip < $GORILLA_CHR_PREFIX$i.$FTYPE.$COMP_FTYPE \
     > $FLD_chromosomes/$GORILLA$i.$FTYPE;
    rm $GORILLA_CHR_PREFIX$i.$FTYPE.$COMP_FTYPE
done


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#   rename: GGunlocalized -> GGUL, GGunplaced -> GGUP
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
mv $FLD_chromosomes/$GORILLA"unlocalized".$FTYPE \
    $FLD_chromosomes/$GORILLA"UL".$FTYPE
mv $FLD_chromosomes/$GORILLA"unplaced".$FTYPE \
    $FLD_chromosomes/$GORILLA"UP".$FTYPE


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#   FASTA -> SEQ
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
for i in $GG_SEQ_RUN; do
    cat $FLD_chromosomes/$GORILLA$i.$FTYPE \
        | grep -v -e "ERROR" -e "eFetchResult" -e "DOCTYPE" -e "xml version" \
                  -e "Unable to obtain" | grep -v -x ">" \
     > $FLD_dataset/$GORILLA$i
done
