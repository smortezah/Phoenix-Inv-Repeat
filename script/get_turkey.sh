#!/usr/bin/env bash


### download FASTA
for i in {1..30} MT W Z; do
    wget $TURKEY_URL/$TURKEY_CHROMOSOME$i.$FILE_TYPE.$COMP_FILE_TYPE;
    gunzip < $TURKEY_CHROMOSOME$i.$FILE_TYPE.$COMP_FILE_TYPE \
        > $FLD_chromosomes/$TURKEY_CHR$i.$FILE_TYPE;
    rm $TURKEY_CHROMOSOME$i.$FILE_TYPE.$COMP_FILE_TYPE
done

for i in unlocalized unplaced; do
    wget $TURKEY_URL/$TURKEY_CHR_PREFIX$i.$FILE_TYPE.$COMP_FILE_TYPE;
    gunzip < $TURKEY_CHR_PREFIX$i.$FILE_TYPE.$COMP_FILE_TYPE \
        > $FLD_chromosomes/$TURKEY_CHR$i.$FILE_TYPE;
    rm $TURKEY_CHR_PREFIX$i.$FILE_TYPE.$COMP_FILE_TYPE
done

### rename: MGAunlocalized -> MGAUL, MGAunplaced -> MGAUP
mv $FLD_chromosomes/$TURKEY_CHR"unlocalized".$FILE_TYPE \
   $FLD_chromosomes/$TURKEY_CHR"UL".$FILE_TYPE
mv $FLD_chromosomes/$TURKEY_CHR"unplaced".$FILE_TYPE \
   $FLD_chromosomes/$TURKEY_CHR"UP".$FILE_TYPE

### FASTA -> SEQ
for i in $MGA_SEQ_RUN; do
    grep -v ">" $FLD_chromosomes/$TURKEY_CHR$i.$FILE_TYPE \
       > $FLD_dataset/$TURKEY_CHR$i;
done
