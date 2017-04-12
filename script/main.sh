#!/usr/bin/env bash

cd ..
cmake .
make

FLD_script="script"

### parmaters edefined in par.sh
. $FLD_script/par.sh;


GET_HUMAN=0            # download Human choromosomes and make SEQ out of FASTA
GET_CHIMPANZEE=0       # download Chimpanzee chrs and make SEQ out of FASTA
GET_GORILLA=0          # download Gorilla chrs and make SEQ out of FASTA
GET_CHICKEN=0          # download Chicken chrs and make SEQ out of FASTA
GET_TURKEY=0           # download Turkey chrs and make SEQ out of FASTA
GET_ARCHAEA=0          # get Archaea SEQ using "GOOSE" & downloadArchaea.pl
GET_FUNGI=0            # get Fungi SEQ using "GOOSE" & downloadFungi.pl
GET_BACTERIA=0         # get Bacteria SEQ using "GOOSE" & downloadBacteria.pl
GET_VIRUSES=0          # get Viruses SEQ using "GOOSE" & downloadViruses.pl
INSTALL_XS=0           # install "XS" from Github
INSTALL_GOOSE=0        # install "GOOSE" from Github
INSTALL_GULL=0         # install "GULL" from Github
GEN_DATASET=0          # generate datasets using "XS"
GEN_MUTATIONS=0        # generate mutations using "GOOSE"
RUN_PHOENIX=1          # run Phoenix
PLOT_RESULT=0          # plot results using "gnuplot"
BUILD_MATRIX=0         # build matrix from datasets
PLOT_MATRIX=0          # plot matrix from datasets
PLOT_MATRIX_ARCHEA=0   # plot matrix Archaea from datasets

### reference parameters
#REF=$HUMAN;      REF_SNAME=$HUMAN_SNAME;
#REF=$CHIMPANZEE; REF_SNAME=$CHIMPANZEE_SNAME;
#REF=$GORILLA;    REF_SNAME=$GORILLA_SNAME;
#REF=$CHICKEN;    REF_SNAME=$CHICKEN_SNAME;
#REF=$TURKEY;     REF_SNAME=$TURKEY_SNAME;
#REF=$ARCHAEA;    REF_SNAME=$ARCHAEA_SNAME;
REF=$FUNGI;       REF_SNAME=$FUNGI_SNAME;
#REF=$BACTERIA;   REF_SNAME=$BACTERIA_SNAME;
#REF=$VIRUSES;    REF_SNAME=$VIRUSES_SNAME;
### all chromosomes for that species, e.g. HS_SEQ_RUN
tempRefSeqRun=${REF}_SEQ_RUN;    REF_RUN=${!tempRefSeqRun}

REF_DATASET="";  for i in 1; do REF_DATASET+=${i}" "; done
#REF_DATASET=""; for i in $REF_RUN; do REF_DATASET+=$REF${i}" ";done

#multiRef=""; for i in 21 MT; do multiRef+=$FLD_dataset/$REF${i}" ";done
#MULTIREF_DATASET="$(echo $multiRef | sed 's/ /,/g')"
#MULTIREF_DATASET="$FLD_dataset/HSMT"
#MULTIREF_DATASET="y"

REF_LEN=${#REF};            # length of string REF
((REF_LEN_IND=REF_LEN+1));  # index of len of string REF


### target parameters
#TAR=$HUMAN;      TAR_SNAME=$HUMAN_SNAME;
#TAR=$CHIMPANZEE; TAR_SNAME=$CHIMPANZEE_SNAME;
#TAR=$GORILLA;    TAR_SNAME=$GORILLA_SNAME;
#TAR=$CHICKEN;    TAR_SNAME=$CHICKEN_SNAME;
#TAR=$TURKEY;     TAR_SNAME=$TURKEY_SNAME;
#TAR=$ARCHAEA;    TAR_SNAME=$ARCHAEA_SNAME;
TAR=$FUNGI;       TAR_SNAME=$FUNGI_SNAME;
#TAR=$BACTERIA;   TAR_SNAME=$BACTERIA_SNAME;
#TAR=$VIRUSES;    TAR_SNAME=$VIRUSES_SNAME;
### all chromosomes for that species, e.g. HS_SEQ_RUN
tempTarSeqRun=${TAR}_SEQ_RUN;    TAR_RUN=${!tempTarSeqRun}

TAR_DATASET="";  for i in 1; do TAR_DATASET+=${i}" "; done
#TAR_DATASET=""; for i in $TAR_RUN; do TAR_DATASET+=$TAR${i}" ";done

multiTar="";
#for i in MT UP UL; do multiTar+=$FLD_dataset/$TAR/${i}" "; done
for i in 1 2 3 4; do multiTar+=$FLD_dataset/$TAR/${i}" "; done
MULTITAR_DATASET="$(echo $multiTar | sed 's/ /,/g')"
#MULTITAR_DATASET="$FLD_dataset/PTMT"
#MULTITAR_DATASET="y"

TAR_LEN=${#TAR};            # length of string TAR
((TAR_LEN_IND=TAR_LEN+1));  # index of len of string TAR

N_THREADS=4             # number of threads
INV_REPS="0"          # list of inverted repeats
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
