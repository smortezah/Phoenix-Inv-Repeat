#!/usr/bin/env bash

###*** download FASTA
for i in {1..22} X Y MT; do
 wget $HUMAN_URL/$HUMAN_CHROMOSOME$i.$FILE_TYPE.$COMP_FILE_TYPE;
 gunzip < $HUMAN_CHROMOSOME$i.$FILE_TYPE.$COMP_FILE_TYPE > $FLD_chromosomes/$HUMAN_CHR$i.$FILE_TYPE;
 rm $HUMAN_CHROMOSOME$i.$FILE_TYPE.$COMP_FILE_TYPE
done

for i in alts unlocalized unplaced; do
 wget $HUMAN_URL/$HUMAN_CHR_PREFIX$i.$FILE_TYPE.$COMP_FILE_TYPE;
 gunzip < $HUMAN_CHR_PREFIX$i.$FILE_TYPE.$COMP_FILE_TYPE > $FLD_chromosomes/$HUMAN_CHR$i.$FILE_TYPE;
 rm $HUMAN_CHR_PREFIX$i.$FILE_TYPE.$COMP_FILE_TYPE
done

###*** rename: HSalts -> HSAL, HSunlocalized -> HSUL, HSunplaced -> HSUP
mv $FLD_chromosomes/$HUMAN_CHR"alts".$FILE_TYPE $FLD_chromosomes/$HUMAN_CHR"AL".$FILE_TYPE
mv $FLD_chromosomes/$HUMAN_CHR"unlocalized".$FILE_TYPE $FLD_chromosomes/$HUMAN_CHR"UL".$FILE_TYPE
mv $FLD_chromosomes/$HUMAN_CHR"unplaced".$FILE_TYPE $FLD_chromosomes/$HUMAN_CHR"UP".$FILE_TYPE

###*** FASTA -> SEQ
for i in $HS_SEQ_RUN; do
 grep -v ">" $FLD_chromosomes/$HUMAN_CHR$i.$FILE_TYPE > $FLD_datasets/$HUMAN_CHR$i;
done
