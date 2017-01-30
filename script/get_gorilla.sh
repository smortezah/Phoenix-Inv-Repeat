#!/usr/bin/env bash

###*** download FASTA
for i in 1 2A 2B {3..22} X MT; do
 wget $GORILLA_URL/$GORILLA_CHROMOSOME$i.$FILE_TYPE.$COMP_FILE_TYPE;
 gunzip < $GORILLA_CHROMOSOME$i.$FILE_TYPE.$COMP_FILE_TYPE > $FLD_chromosomes/$GORILLA_CHR$i.$FILE_TYPE;
 rm $GORILLA_CHROMOSOME$i.$FILE_TYPE.$COMP_FILE_TYPE
done

for i in unlocalized unplaced; do
 wget $GORILLA_URL/$GORILLA_CHR_PREFIX$i.$FILE_TYPE.$COMP_FILE_TYPE;
 gunzip < $GORILLA_CHR_PREFIX$i.$FILE_TYPE.$COMP_FILE_TYPE > $FLD_chromosomes/$GORILLA_CHR$i.$FILE_TYPE;
 rm $GORILLA_CHR_PREFIX$i.$FILE_TYPE.$COMP_FILE_TYPE
done

###*** rename: GGunlocalized -> GGUL, GGunplaced -> GGUP
mv $FLD_chromosomes/$GORILLA_CHR"unlocalized".$FILE_TYPE $FLD_chromosomes/$GORILLA_CHR"UL".$FILE_TYPE
mv $FLD_chromosomes/$GORILLA_CHR"unplaced".$FILE_TYPE $FLD_chromosomes/$GORILLA_CHR"UP".$FILE_TYPE

###*** FASTA -> SEQ
for i in $GG_SEQ_RUN; do
 grep -v ">" $FLD_chromosomes/$GORILLA_CHR$i.$FILE_TYPE > $FLD_datasets/$GORILLA_CHR$i;
done
