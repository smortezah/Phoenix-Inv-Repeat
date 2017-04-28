#!/bin/bash


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#   download FASTA
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
for i in {1..22} X Y MT; do
    wget $HUMAN_URL/$HUMAN_CHROMOSOME$i.$FTYPE.$COMP_FTYPE;
    gunzip < $HUMAN_CHROMOSOME$i.$FTYPE.$COMP_FTYPE \
     > $FLD_chromosomes/$HUMAN$i.$FTYPE;
    rm $HUMAN_CHROMOSOME$i.$FTYPE.$COMP_FTYPE
done

for i in alts unlocalized unplaced; do
    wget $HUMAN_URL/$HUMAN_CHR_PREFIX$i.$FTYPE.$COMP_FTYPE;
    gunzip < $HUMAN_CHR_PREFIX$i.$FTYPE.$COMP_FTYPE \
        > $FLD_chromosomes/$HUMAN$i.$FTYPE;
    rm $HUMAN_CHR_PREFIX$i.$FTYPE.$COMP_FTYPE
done


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#   rename: HSalts -> HSAL, HSunlocalized -> HSUL, HSunplaced -> HSUP
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
mv $FLD_chromosomes/$HUMAN"alts".$FTYPE \
    $FLD_chromosomes/$HUMAN"AL".$FTYPE
mv $FLD_chromosomes/$HUMAN"unlocalized".$FTYPE \
    $FLD_chromosomes/$HUMAN"UL".$FTYPE
mv $FLD_chromosomes/$HUMAN"unplaced".$FTYPE \
    $FLD_chromosomes/$HUMAN"UP".$FTYPE


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#   FASTA -> SEQ
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
for i in $HS_SEQ_RUN; do
    grep -v ">" $FLD_chromosomes/$HUMAN$i.$FTYPE \
     > $FLD_dataset/$HUMAN$i;
done
