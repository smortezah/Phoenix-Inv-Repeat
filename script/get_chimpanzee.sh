#!/usr/bin/env bash


### download FASTA
for i in 1 2A 2B {3..22} X Y MT; do
    wget $CHIMPANZEE_URL/$CHIMPANZEE_CHROMOSOME$i.$FILE_TYPE.$COMP_FILE_TYPE;
    gunzip < $CHIMPANZEE_CHROMOSOME$i.$FILE_TYPE.$COMP_FILE_TYPE \
        > $FLD_chromosomes/$CHIMPANZEE_CHR$i.$FILE_TYPE;
    rm $CHIMPANZEE_CHROMOSOME$i.$FILE_TYPE.$COMP_FILE_TYPE
done

for i in unlocalized unplaced; do
    wget $CHIMPANZEE_URL/$CHIMPANZEE_CHR_PREFIX$i.$FILE_TYPE.$COMP_FILE_TYPE;
    gunzip < $CHIMPANZEE_CHR_PREFIX$i.$FILE_TYPE.$COMP_FILE_TYPE \
        > $FLD_chromosomes/$CHIMPANZEE_CHR$i.$FILE_TYPE;
    rm $CHIMPANZEE_CHR_PREFIX$i.$FILE_TYPE.$COMP_FILE_TYPE
done

### rename: PTunlocalized -> PTUL, PTunplaced -> PTUP
mv $FLD_chromosomes/$CHIMPANZEE_CHR"unlocalized".$FILE_TYPE \
    $FLD_chromosomes/$CHIMPANZEE_CHR"UL".$FILE_TYPE
mv $FLD_chromosomes/$CHIMPANZEE_CHR"unplaced".$FILE_TYPE \
    $FLD_chromosomes/$CHIMPANZEE_CHR"UP".$FILE_TYPE

### FASTA -> SEQ
for i in $PT_SEQ_RUN; do
    grep -v ">" $FLD_chromosomes/$CHIMPANZEE_CHR$i.$FILE_TYPE \
        > $FLD_dataset/$CHIMPANZEE_CHR$i;
done
