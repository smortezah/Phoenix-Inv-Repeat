#!/usr/bin/env bash


### download FASTA
for i in {1..28} {30..33} LGE64 MT W Z; do
 wget $CHICKEN_URL/$CHICKEN_CHROMOSOME$i.$FILE_TYPE.$COMP_FILE_TYPE;
 gunzip < $CHICKEN_CHROMOSOME$i.$FILE_TYPE.$COMP_FILE_TYPE > $FLD_chromosomes/$CHICKEN_CHR$i.$FILE_TYPE;
 rm $CHICKEN_CHROMOSOME$i.$FILE_TYPE.$COMP_FILE_TYPE
done

for i in unlocalized unplaced; do
 wget $CHICKEN_URL/$CHICKEN_CHR_PREFIX$i.$FILE_TYPE.$COMP_FILE_TYPE;
 gunzip < $CHICKEN_CHR_PREFIX$i.$FILE_TYPE.$COMP_FILE_TYPE > $FLD_chromosomes/$CHICKEN_CHR$i.$FILE_TYPE;
 rm $CHICKEN_CHR_PREFIX$i.$FILE_TYPE.$COMP_FILE_TYPE
done

### rename: GGALGE64 -> GGALG, GGAunlocalized -> GGAUL, GGAunplaced -> GGAUP
mv $FLD_chromosomes/$CHICKEN_CHR"LGE64".$FILE_TYPE $FLD_chromosomes/$CHICKEN_CHR"LG".$FILE_TYPE
mv $FLD_chromosomes/$CHICKEN_CHR"unlocalized".$FILE_TYPE $FLD_chromosomes/$CHICKEN_CHR"UL".$FILE_TYPE
mv $FLD_chromosomes/$CHICKEN_CHR"unplaced".$FILE_TYPE $FLD_chromosomes/$CHICKEN_CHR"UP".$FILE_TYPE

### FASTA -> SEQ
for i in $GGA_SEQ_RUN; do
 grep -v ">" $FLD_chromosomes/$CHICKEN_CHR$i.$FILE_TYPE > $FLD_datasets/$CHICKEN_CHR$i;
done
