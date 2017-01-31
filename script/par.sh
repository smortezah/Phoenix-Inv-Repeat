#!/usr/bin/env bash

### folders to be used
FLD_archive_dat="archive_dat"
FLD_chromosomes="chromosomes"
FLD_dat="dat"
FLD_dataset="dataset"
FLD_GOOSE="goose"
FLD_GULL="GULL"
FLD_XS="XS"
FLD_src="src"
FLD_script="script"

# mutations list:   `seq -s' ' 1 10`
#MUT_LIST="1 2 3 4 5 6 7 8 9 10 12 14 16 18 20 25 30 35 40 45 50"

HUMAN_URL="ftp://ftp.ncbi.nlm.nih.gov/genomes/H_sapiens/Assembled_chromosomes/seq"
CHIMPANZEE_URL="ftp://ftp.ncbi.nlm.nih.gov/genomes/Pan_troglodytes/Assembled_chromosomes/seq"
GORILLA_URL="ftp://ftp.ncbi.nlm.nih.gov/genomes/Gorilla_gorilla/Assembled_chromosomes/seq"
CHICKEN_URL="ftp://ftp.ncbi.nlm.nih.gov/genomes/Gallus_gallus/Assembled_chromosomes/seq"
TURKEY_URL="ftp://ftp.ncbi.nlm.nih.gov/genomes/Meleagris_gallopavo/Assembled_chromosomes/seq"

HUMAN_CHR_PREFIX="hs_ref_GRCh38.p7_"
CHIMPANZEE_CHR_PREFIX="ptr_ref_Pan_tro_3.0_"
GORILLA_CHR_PREFIX="9595_ref_gorGor4_"
CHICKEN_CHR_PREFIX="gga_ref_Gallus_gallus-5.0_"
TURKEY_CHR_PREFIX="mga_ref_Turkey_5.0_"

CHR="chr"

HUMAN_CHR="HS"
CHIMPANZEE_CHR="PT"
GORILLA_CHR="GG"
ARCHAEA_CHR="A"
CHICKEN_CHR="GGA"
TURKEY_CHR="MGA"

CURR_CHR="21"
chromosomes="$HUMAN_CHR_PREFIX$CHR$CURR_CHR"

HUMAN_CHROMOSOME="$HUMAN_CHR_PREFIX$CHR"
CHIMPANZEE_CHROMOSOME="$CHIMPANZEE_CHR_PREFIX$CHR"
GORILLA_CHROMOSOME="$GORILLA_CHR_PREFIX$CHR"
CHICKEN_CHROMOSOME="$CHICKEN_CHR_PREFIX$CHR"
TURKEY_CHROMOSOME="$TURKEY_CHR_PREFIX$CHR"

HS_SEQ_RUN=`seq -s' ' 1 22`; HS_SEQ_RUN+=" X Y MT AL UL UP"
PT_SEQ_RUN="1 2A 2B "; PT_SEQ_RUN+=`seq -s' ' 3 22`; PT_SEQ_RUN+=" X Y MT UL UP"
GG_SEQ_RUN="1 2A 2B "; GG_SEQ_RUN+=`seq -s' ' 3 22`; GG_SEQ_RUN+=" X MT UL UP"
A_SEQ_RUN=`seq -s' ' 1 206`
GGA_SEQ_RUN=`seq -s' ' 1 28`; GGA_SEQ_RUN+=" "; GGA_SEQ_RUN+=`seq -s' ' 30 33`; GGA_SEQ_RUN+=" LG MT W Z UL UP"
MGA_SEQ_RUN=`seq -s' ' 1 30`; MGA_SEQ_RUN+=" MT W Z UL UP"

datasets="$HUMAN_CHR$CURR_CHR"
#datasets="";   for i in $HS_SEQ_RUN; do datasets+=$HUMAN_CHR${i}" "; done


FILE_TYPE="fa"          # file type
COMP_FILE_TYPE="gz"     # compressed file type
INF_FILE_TYPE="dat"     # information (data) file type
#INF_FILE_TYPE="csv"     # information (data) file type

PIX_FORMAT=pdf          # output format: pdf, png, svg, eps, epslatex (set output x.y)
IR_LBL=i                # label for inverted repeat
a_LBL=a                 # label for alpha denominator