#!/usr/bin/env bash

cd ..
cmake .
make

### folders to be used
FLD_chromosomes="chromosomes"
FLD_dat="dat"
FLD_dataset="dataset"
FLD_archaea="archaea"
FLD_fungi="fungi"
FLD_bacteria="bacteria"
FLD_viruses="viruses"
FLD_GOOSE="goose"
FLD_GULL="GULL"
FLD_XS="XS"
FLD_src="src"
FLD_script="script"

### include par.sh, including paramaters
. $FLD_script/par.sh;


GET_HUMAN=0            # download Human choromosomes and make SEQ out of FASTA
GET_CHIMPANZEE=0       # download Chimpanzee chrs and make SEQ out of FASTA
GET_GORILLA=0          # download Gorilla chrs and make SEQ out of FASTA
GET_CHICKEN=0          # download Chicken chrs and make SEQ out of FASTA
GET_TURKEY=0           # download Turkey chrs and make SEQ out of FASTA
GET_ARCHAEA=0          # get Archaea SEQ using "GOOSE" & downloadArchaea.pl
GET_FUNGI=0            # get Fungi SEQ using "GOOSE" & downloadFungi.pl
GET_BACTERIA=1         # get Bacteria SEQ using "GOOSE" & downloadBacteria.pl
GET_VIRUSES=0          # get Viruses SEQ using "GOOSE" & downloadViruses.pl
INSTALL_XS=0           # install "XS" from Github
INSTALL_GOOSE=0        # install "GOOSE" from Github
INSTALL_GULL=0         # install "GULL" from Github
GEN_DATASET=0          # generate datasets using "XS"
GEN_MUTATIONS=0        # generate mutations using "GOOSE"
RUN_PHOENIX=0          # run Phoenix
PLOT_RESULT=0          # plot results using "gnuplot"
BUILD_MATRIX=0         # build matrix from datasets
PLOT_MATRIX=0          # plot matrix from datasets
PLOT_MATRIX_ARCHEA=0   # plot matrix Archaea from datasets

### reference parameters
REF_SPECIES=$HUMAN_CHR; REF_SPECIES_NAME=$HUMAN_LBL;
#REF_SPECIES=$CHIMPANZEE_CHR; REF_SPECIES_NAME=$CHIMPANZEE_LBL;
#REF_SPECIES=$GORILLA_CHR; REF_SPECIES_NAME=$GORILLA_LBL;
#REF_SPECIES=$CHICKEN_CHR; REF_SPECIES_NAME=$CHICKEN_LBL;
#REF_SPECIES=$TURKEY_CHR; REF_SPECIES_NAME=$TURKEY_LBL;
#REF_SPECIES=$ARCHAEA_CHR; REF_SPECIES_NAME=$ARCHAEA_LBL;
#REF_SPECIES=$FUNGI_CHR; REF_SPECIES_NAME=$FUNGI_LBL;
#REF_SPECIES=$BACTERIA_CHR; REF_SPECIES_NAME=$BACTERIA_LBL;
#REF_SPECIES=$VIRUSES_CHR; REF_SPECIES_NAME=$VIRUSES_LBL;
### all chromosomes for that species, e.g. HS_SEQ_RUN
tempRefSeqRun=${REF_SPECIES}_SEQ_RUN;    REF_SEQ_RUN=${!tempRefSeqRun}

#REF_DATASET="";  for i in MT; do REF_DATASET+=$REF_SPECIES${i}" "; done
REF_DATASET=""; for i in $REF_SEQ_RUN; do REF_DATASET+=$REF_SPECIES${i}" ";done

#multiRef=""; for i in 21 MT; do multiRef+=$FLD_dataset/$REF_SPECIES${i}" ";done
#MULTIREF_DATASET="$(echo $multiRef | sed 's/ /,/g')"
MULTIREF_DATASET="$FLD_dataset/HSMT"
#MULTIREF_DATASET="y"

REF_SPECIES_LEN=${#REF_SPECIES};            # length of string REF_SPECIES
((REF_SPECIES_LEN_IND=REF_SPECIES_LEN+1));  # index of len of string REF_SPECIES


### target parameters
TAR_SPECIES=$HUMAN_CHR; TAR_SPECIES_NAME=$HUMAN_LBL;
#TAR_SPECIES=$CHIMPANZEE_CHR; TAR_SPECIES_NAME=$CHIMPANZEE_LBL;
#TAR_SPECIES=$GORILLA_CHR; TAR_SPECIES_NAME=$GORILLA_LBL;
#TAR_SPECIES=$CHICKEN_CHR; TAR_SPECIES_NAME=$CHICKEN_LBL;
#TAR_SPECIES=$TURKEY_CHR; TAR_SPECIES_NAME=$TURKEY_LBL;
#TAR_SPECIES=$ARCHAEA_CHR; TAR_SPECIES_NAME=$ARCHAEA_LBL;
#TAR_SPECIES=$FUNGI_CHR; TAR_SPECIES_NAME=$FUNGI_LBL;
#TAR_SPECIES=$BACTERIA_CHR; TAR_SPECIES_NAME=$BACTERIA_LBL;
#TAR_SPECIES=$VIRUSES_CHR; TAR_SPECIES_NAME=$VIRUSES_LBL;
### all chromosomes for that species, e.g. HS_SEQ_RUN
tempTarSeqRun=${TAR_SPECIES}_SEQ_RUN;    TAR_SEQ_RUN=${!tempTarSeqRun}

#TAR_DATASET="";  for i in MT; do TAR_DATASET+=$TAR_SPECIES${i}" "; done
TAR_DATASET=""; for i in $TAR_SEQ_RUN; do TAR_DATASET+=$TAR_SPECIES${i}" ";done

#multiTar="";
#   for i in MT UP UL; do multiTar+=$FLD_dataset/$TAR_SPECIES${i}" "; done
#MULTITAR_DATASET="$(echo $multiTar | sed 's/ /,/g')"
MULTITAR_DATASET="$FLD_dataset/PTMT"
#MULTITAR_DATASET="y"

TAR_SPECIES_LEN=${#TAR_SPECIES};            # length of string TAR_SPECIES
((TAR_SPECIES_LEN_IND=TAR_SPECIES_LEN+1));  # index of len of string TAR_SPECIES

N_THREADS=4             # number of threads
INV_REPEATS="0"       # list of inverted repeats
ALPHA_DENS="100"        # list of alpha denominators
CTX=2                   # context-order size
MIN_CTX=10              # min context-order size
MAX_CTX=11              # max context-order size


if [[ $GET_HUMAN      -eq 1 ]];    then . $FLD_script/get_human.sh;           fi
if [[ $GET_CHIMPANZEE -eq 1 ]];    then . $FLD_script/get_chimpanzee.sh;      fi
if [[ $GET_GORILLA    -eq 1 ]];    then . $FLD_script/get_gorilla.sh;         fi
if [[ $GET_CHICKEN    -eq 1 ]];    then . $FLD_script/get_chicken.sh;         fi
if [[ $GET_TURKEY     -eq 1 ]];    then . $FLD_script/get_turkey.sh;          fi
if [[ $GET_ARCHAEA    -eq 1 ]];    then . $FLD_script/get_archaea.sh;         fi
if [[ $GET_FUNGI      -eq 1 ]];    then . $FLD_script/get_fungi.sh;           fi
if [[ $GET_BACTERIA   -eq 1 ]];    then . $FLD_script/get_bacteria.sh;        fi
if [[ $GET_VIRUSES    -eq 1 ]];    then . $FLD_script/get_viruses.sh;         fi
if [[ $INSTALL_XS     -eq 1 ]];    then . $FLD_script/install_XS.sh;          fi
if [[ $INSTALL_GOOSE  -eq 1 ]];    then . $FLD_script/install_GOOSE.sh;       fi
if [[ $INSTALL_GULL   -eq 1 ]];    then . $FLD_script/install_GULL.sh;        fi
if [[ $GEN_DATASET    -eq 1 ]];    then . $FLD_script/generate_dataset.sh;    fi
if [[ $GEN_MUTATIONS  -eq 1 ]];    then . $FLD_script/generate_mutation.sh;   fi
if [[ $RUN_PHOENIX    -eq 1 ]];    then . $FLD_script/run_phoenix.sh;         fi
if [[ $PLOT_RESULT    -eq 1 ]];    then . $FLD_script/plot_result.sh;         fi
if [[ $BUILD_MATRIX   -eq 1 ]];    then . $FLD_script/build_matrix.sh;        fi
if [[ $PLOT_MATRIX    -eq 1 ]];    then . $FLD_script/plot_matrix.sh;         fi
if [[ $PLOT_MATRIX_ARCHEA -eq 1 ]];then . $FLD_script/plot_matrix_archaea.sh; fi

########################
cd $FLD_script
