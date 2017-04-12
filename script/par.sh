#!/usr/bin/env bash


### folders to be used
FLD_chromosomes="chromosomes"
FLD_GOOSE="goose"
FLD_GULL="GULL"
FLD_XS="XS"
FLD_dat="dat"
FLD_dataset="dataset"
FLD_archaea="archaea"
FLD_fungi="fungi"
FLD_bacteria="bacteria"
FLD_viruses="viruses"
FLD_src="src"

# mutations list:   `seq -s' ' 1 10`
#MUT_LIST="1 2 3 4 5 6 7 8 9 10 12 14 16 18 20 25 30 35 40 45 50"

HUMAN_URL="ftp://ftp.ncbi.nlm.nih.gov/genomes/\
H_sapiens/Assembled_chromosomes/seq"
CHIMPANZEE_URL="ftp://ftp.ncbi.nlm.nih.gov/genomes/\
Pan_troglodytes/Assembled_chromosomes/seq"
GORILLA_URL="ftp://ftp.ncbi.nlm.nih.gov/genomes/\
Gorilla_gorilla/Assembled_chromosomes/seq"
CHICKEN_URL="ftp://ftp.ncbi.nlm.nih.gov/genomes/\
Gallus_gallus/Assembled_chromosomes/seq"
TURKEY_URL="ftp://ftp.ncbi.nlm.nih.gov/genomes/\
Meleagris_gallopavo/Assembled_chromosomes/seq"

HUMAN_CHR_PREFIX="hs_ref_GRCh38.p7_"
CHIMPANZEE_CHR_PREFIX="ptr_ref_Pan_tro_3.0_"
GORILLA_CHR_PREFIX="9595_ref_gorGor4_"
CHICKEN_CHR_PREFIX="gga_ref_Gallus_gallus-5.0_"
TURKEY_CHR_PREFIX="mga_ref_Turkey_5.0_"

CHR="chr"

HUMAN_SNAME="Homo sapiens"
CHIMPANZEE_SNAME="Pan troglodytes"
GORILLA_SNAME="Gorilla gorilla"
CHICKEN_SNAME="Gallus gallus"
TURKEY_SNAME="Meleagris gallopavo"
ARCHAEA_SNAME="Archaea"
FUNGI_SNAME="Fungi"
BACTERIA_SNAME="Bacteria"
VIRUSES_SNAME="Viruses"

HUMAN="HS"
CHIMPANZEE="PT"
GORILLA="GG"
CHICKEN="GGA"
TURKEY="MGA"
ARCHAEA="A"
FUNGI="F"
BACTERIA="B"
VIRUSES="V"

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
GGA_SEQ_RUN=`seq -s' ' 1 28`; GGA_SEQ_RUN+=" ";
    GGA_SEQ_RUN+=`seq -s' ' 30 33`; GGA_SEQ_RUN+=" LG MT W Z UL UP"
MGA_SEQ_RUN=`seq -s' ' 1 30`; MGA_SEQ_RUN+=" MT W Z UL UP"
A_SEQ_RUN=`seq -s' ' 1 206`
F_SEQ_RUN=`seq -s' ' 1 377`
B_SEQ_RUN=`seq -s' ' 1 3219`
V_SEQ_RUN=`seq -s' ' 1 5687`

datasets="$HUMAN$CURR_CHR"
#datasets="";   for i in $HS_SEQ_RUN; do datasets+=$HUMAN${i}" "; done


FTYPE="fa"          # file type
COMP_FTYPE="gz"     # compressed file type
INF_FTYPE="dat"     # information (data) file type
#INF_FTYPE="csv"     # information (data) file type

PIX_FORMAT=pdf    # output format: pdf, png, svg, eps, epslatex (set output x.y)
IR=i          # label for inverted repeat
AL=a           # label for alpha denominator
