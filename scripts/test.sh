#!/bin/bash

#***********************************************************
#   change directory to home
#***********************************************************
cd ..


#***********************************************************
#   parameters to install and run needed programs
#***********************************************************
### folders to be used
FLD_archive_dat="archive_dat"
FLD_archive_datasets="archive_datasets"
FLD_chromosomes="chromosomes"
FLD_dat="dat"
FLD_datasets="datasets"
FLD_goose="goose"
FLD_scripts="scripts"
FLD_XS="XS"

DL_HUMAN=0              # download Human choromosomes
DL_CHIMP=0              # download Chimpanzee choromosomes
FASTA2SEQ_HUMAN=0       # FASTA to sequence for Human
FASTA2SEQ_CHIMP=0       # FASTA to sequence for Chimpanzee
INSTALL_XS=0            # install "XS" from Github
INSTALL_goose=0         # install "goose" from Github
GEN_DATASETS=0          # generate datasets using "XS"
GEN_MUTATIONS=0         # generate mutations using "goose"
RUN=0                   # run the program
PLOT_RESULTS=0          # plot results using "gnuplot"
BUILD_MATRIX=1          # build matrix from Human Chimpanzee correspondence
ARCHIVE_DATA=0          # archive data

# mutations list:   `seq -s' ' 1 10`
#MUT_LIST="1 2 3 4 5 6 7 8 9 10 12 14 16 18 20 25 30 35 40 45 50"
#MUT_LIST="1"

HUMAN_CHR_PREFIX="hs_ref_GRCh38.p7_"
CHR="chr"
HUMAN_CHR="HS"
CURR_CHR="21"
chromosomes="$HUMAN_CHR_PREFIX$CHR$CURR_CHR"
HUMAN_CHROMOSOME="$HUMAN_CHR_PREFIX$CHR"

CHIMP_CHR="PT"

#chromosomes=""
#for i in {1..24}
#do  chromosomes+=$HUMAN_CHR_PREFIX$CHR${i}" ";   done

datasets="$HUMAN_CHR$CURR_CHR"
#datasets="tmp"

#datasets=""
##for i in {1..22} X Y #alts unlocalized unplaced
#for i in {1..24}
#do  datasets+=$HUMAN_CHR${i}" ";    done

REF_DATASET="";  for i in 21; do REF_DATASET+=$HUMAN_CHR${i}" "; done # reference dataset
#REF_DATASET="";  for i in 1 2A 2B {3..24}; do REF_DATASET+=CHIMP_CHR{i}" "; done # reference dataset
TAR_DATASET="";  for i in 24; do TAR_DATASET+=$CHIMP_CHR${i}" "; done # target dataset
#TAR_DATASET="";  for i in {1..24}; do TAR_DATASET+=HUMAN_CHR{i}" "; done # target dataset

FILE_TYPE="fa"          # file type
COMP_FILE_TYPE="gz"     # compressed file type
INF_FILE_TYPE="dat"     # information (data) file type
#INF_FILE_TYPE="csv"     # information (data) file type

PIX_FORMAT=png          # output format: png, svg, eps, epslatex (set output x.y)
#rm -f *.$PIX_FORMAT    # remove FORMAT pictures, if they exist
IR_LBL=i                # label for inverted repeat
a_LBL=a                 # label for alpha denominator

INV_REPEATS="0"         # list of inverted repeats      "0 1"
ALPHA_DENS="100"        # list of alpha denominators    "1 20 100"
MIN_CTX=2              # min context-order size
MAX_CTX=4              # max context-order size


#***********************************************************
#   download Human choromosomes
#***********************************************************
if [[ $DL_HUMAN == 1 ]]; then

for i in {1..22} X Y; do
 wget ftp://ftp.ncbi.nlm.nih.gov/genomes/H_sapiens/Assembled_chromosomes/seq/$HUMAN_CHROMOSOME$i.$FILE_TYPE.$COMP_FILE_TYPE;
 gunzip < $HUMAN_CHROMOSOME$i.$FILE_TYPE.$COMP_FILE_TYPE > $FLD_chromosomes/$HUMAN_CHR$i.$FILE_TYPE;
 rm $HUMAN_CHROMOSOME$i.$FILE_TYPE.$COMP_FILE_TYPE
done
mv ${HUMAN_CHR}X.$FILE_TYPE ${HUMAN_CHR}23.$FILE_TYPE     # rename chrX to chr23
mv ${HUMAN_CHR}Y.$FILE_TYPE ${HUMAN_CHR}24.$FILE_TYPE     # rename chrY to chr24

fi  # end of download Human choromosomes


#***********************************************************
#   download Chimpanzee choromosomes
#***********************************************************
if [[ $DL_CHIMP == 1 ]]; then

#===============================
###     BY: Diogo Pratas     ###
#===============================
INITALS="PT";
MAX=22;
WGETOP=" --trust-server-names -q ";
ONWAY="ftp://ftp.ncbi.nlm.nih.gov/genomes/Pan_troglodytes/Assembled_chromosomes/seq/ptr_ref_Pan_tro_3.0_chr"
#--------------------------------------------------
function downloadEach
  {
  PATTERN="unexpected";
  for((;;));
    do
    sleep 2;
    xtmp="`wget $1 $2 -O $4-X$3`"
    if [[ "$xtmp" == *"$PATTERN"* ]];
      then
      echo "Unexpected EOF found, re-downloading C$3 ...";
      continue;
    else
      echo "wget stderr:$xtmp";
      echo "Downloaded $4 C$3 with success!";
      break;
    fi
    done
  }

echo "Downloading and filtering $INITALS sequences ..."
for((x=1 ; x <= $MAX ; ++x));
  do
  if [ $x -ne "2" ];
    then
    ZPATH="$ONWAY$x.$FILE_TYPE.$COMP_FILE_TYPE";
    downloadEach "$WGETOP" "$ZPATH" "$x" "$INITALS";
    zcat $INITALS-X$x > $INITALS$x.$FILE_TYPE;
    echo "$INITALS C$x filtered!";
  fi
  done

ZPATH=$ONWAY"2A.$FILE_TYPE.$COMP_FILE_TYPE";
downloadEach "$WGETOP" "$ZPATH" "2A" "$INITALS";
zcat $INITALS-X2A > $INITALS"2A".$FILE_TYPE;
echo "$INITALS C2A filtered";

ZPATH=$ONWAY"2B.$FILE_TYPE.$COMP_FILE_TYPE";
downloadEach "$WGETOP" "$ZPATH" "2B" "$INITALS";
zcat $INITALS-X2B > $INITALS"2B".$FILE_TYPE;
echo "$INITALS C2B filtered";

CHR=23;
FIELD="X";
ZPATH="$ONWAY$FIELD.$FILE_TYPE.$COMP_FILE_TYPE";
downloadEach "$WGETOP" "$ZPATH" "$CHR" "$INITALS";
zcat $INITALS-X$CHR > $INITALS$CHR.$FILE_TYPE;
echo "$INITALS CX filtered";

CHR=24;
FIELD="Y";
ZPATH="$ONWAY$FIELD.$FILE_TYPE.$COMP_FILE_TYPE";
downloadEach "$WGETOP" "$ZPATH" "$CHR" "$INITALS";
zcat $INITALS-X$CHR > $INITALS$CHR.$FILE_TYPE;
echo "$INITALS CY filtered";

CHR=25;
downloadEach "$WGETOP" "ftp://ftp.ncbi.nlm.nih.gov/genomes/Pan_troglodytes/Assembled_chromosomes/seq/ptr_ref_Pan_tro_3.0_unlocalized.fa.gz" "$CHR" "$INITALS";
zcat $INITALS-X$CHR > $INITALS$CHR.$FILE_TYPE;
echo "$INITALS UNLOCALIZED filtered";

CHR=26;
downloadEach "$WGETOP" "ftp://ftp.ncbi.nlm.nih.gov/genomes/Pan_troglodytes/Assembled_chromosomes/seq/ptr_ref_Pan_tro_3.0_unplaced.fa.gz" "$CHR" "$INITALS";
zcat $INITALS-X$CHR > $INITALS$CHR.$FILE_TYPE;
echo "$INITALS UNPLACED filtered";

CHR=27;
downloadEach "$WGETOP" "ftp://ftp.ncbi.nlm.nih.gov/genomes/Pan_troglodytes/Assembled_chromosomes/seq/ptr_ref_Pan_tro_3.0_chrMT.fa.gz" "$CHR" "$INITALS";
zcat $INITALS-X$CHR > $INITALS$CHR.$FILE_TYPE;
echo "$INITALS MITOCONDRIA filtered";

rm -f *PT-* $INITALS-X2A $INITALS-X2B;
##cat PT* > PT
echo "Done!"
#==================================================

mv ${CHIMP_CHR}* $FLD_chromosomes/

fi  # end of download Chimpanzee choromosomes


#***********************************************************
#   FASTA to SEQ for Human
#***********************************************************
if [[ $FASTA2SEQ_HUMAN == 1 ]]; then

for i in {1..24}; do
 grep -v ">" $FLD_chromosomes/$HUMAN_CHR$i.$FILE_TYPE > $FLD_datasets/$HUMAN_CHR$i;
done

fi  # end of FASTA to SEQ


#***********************************************************
#   FASTA to SEQ for Chimpanzee
#***********************************************************
if [[ $FASTA2SEQ_CHIMP == 1 ]]; then

for i in 1 2A 2B {3..27}; do
 grep -v ">" $FLD_chromosomes/$CHIMP_CHR$i.$FILE_TYPE > $FLD_datasets/$CHIMP_CHR$i;
done

fi  # end of FASTA to SEQ


#***********************************************************
#   install "XS" from Github
#***********************************************************
if [[ $INSTALL_XS == 1 ]]; then

rm -fr XS
git clone https://github.com/pratas/XS.git
cd $FLD_XS
make
cd ..

fi  # end of installing "XS"


#***********************************************************
#   install "goose" from Github
#***********************************************************
if [[ $INSTALL_goose == 1 ]]; then

rm -fr goose
git clone https://github.com/pratas/goose.git
cd ${FLD_goose}/src
make
cd ../../

fi  # end of installing "goose"


#***********************************************************
#   generate datasets using "XS"
#***********************************************************
if [[ $GEN_DATASETS == 1 ]]; then

XS/XS -ls 100 -n 100000 -rn 0 -f 0.20,0.20,0.20,0.20,0.20 -eh -eo -es datasetXS
echo ">X" > HEADER      # add ">X" as the header of the sequence (build "nonRepX")
cat HEADER datasetXS > dataset
rm -f HEADER

fi  # end of generating datasets using "XS"


#***********************************************************
#   generate mutations using "goose"
#***********************************************************
if [[ $GEN_MUTATIONS == 1 ]]; then

for c in $chromosomes; do
 for x in $MUT_LIST; do      #((x=1; x<$NUM_MUTATIONS; x+=1));
 MRATE=`echo "scale=3;$x/100" | bc -l`;      # handle transition 0.09 -> 0.10
 goose/src/goose-mutatefasta -s $x -a5 -mr $MRATE " " < $FLD_chromosomes/$c.$FILE_TYPE > temp;
 cat temp | grep -v ">" > $HUMAN_CHR${CURR_CHR}_$x      # remove the header line
 done
done
rm -f temp*    # remove temporary files

#-----------------------------------
#   move all generated mutations files to "datasets" folder
#-----------------------------------
##rm -fr $FLD_datasets
#mkdir -p $FLD_datasets
mv ${HUMAN_CHR}* $FLD_datasets

fi  # end of generating mutations using "goose"


#***********************************************************
#   running the program
#***********************************************************
cmake source
make

if [[ $RUN == 1 ]]; then

for ir in $INV_REPEATS; do
 for alphaDen in $ALPHA_DENS; do
  for refDataset in $REF_DATASET; do
#   echo -e "# ref    tar    ir   alpha    ctx   bpb        NRC        time(s)" \
##        >> $IR_LBL$ir-$a_LBL$alphaDen-$refDataset.$INF_FILE_TYPE
   echo -e "ref\ttar\tir\talpha\tctx\tbpb\tNRC\ttime(s)" \
        >> $IR_LBL$ir-$a_LBL$alphaDen-$refDataset.$INF_FILE_TYPE
   for tarDataset in $TAR_DATASET; do
##   rm -f $IR_LBL$ir-$a_LBL$alphaDen-${dataset}.$INF_FILE_TYPE
#   touch $IR_LBL$ir-$a_LBL$alphaDen-$dataset.$INF_FILE_TYPE
#   echo -e "# mut\tmin_bpb\tmin_ctx" >> $IR_LBL$ir-$a_LBL$alphaDen-$dataset.$INF_FILE_TYPE
#    for mut in $MUT_LIST; do
#    rm -f $IR_LBL$ir-$a_LBL$alphaDen-${dataset}_$mut.$INF_FILE_TYPE
#    touch $IR_LBL$ir-$a_LBL$alphaDen-${dataset}_$mut.$INF_FILE_TYPE
#    echo -e "# ir\talpha\tctx\tbpb\ttime(s)" >> $IR_LBL$ir-$a_LBL$alphaDen-${dataset}_$mut.$INF_FILE_TYPE
     for((ctx=$MIN_CTX; ctx<=$MAX_CTX; ctx+=1)); do
#     for ctx in {2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20}; do
     ./phoenix -m r,$ctx,$alphaDen,$ir -t $FLD_datasets/$tarDataset -r $FLD_datasets/$refDataset \
               >> $IR_LBL$ir-$a_LBL$alphaDen-$refDataset.$INF_FILE_TYPE
     done
#    done
   done
##    # save "min bpb" and "min ctx" for each dataset
#    minBpbCtx=$(awk 'NR==1 || $4 < minBpb {minBpb=$4; minCtx=$3}; \
#                END {print minBpb"\t"minCtx}' $IR_LBL$ir-$a_LBL$alphaDen-${dataset}_$mut.$INF_FILE_TYPE)
#    echo -e "  $mut\t$minBpbCtx" >> $IR_LBL$ir-$a_LBL$alphaDen-$dataset.$INF_FILE_TYPE
#   done
  done
 done
done

#-----------------------------------
#   create "dat" folder and save the results
#-----------------------------------
###rm -fr $FLD_dat              # remove "dat" folder, if it already exists
##mkdir -p $FLD_dat             # make "dat" folder
#mv ${IR_LBL}*.$INF_FILE_TYPE $FLD_dat    # move all created dat files to the "dat" folder

fi  # end of running the program


#***********************************************************
#   plot results using "gnuplot"
#***********************************************************
if [[ $PLOT_RESULTS == 1 ]]; then

#for ir in $INV_REPEATS; do
for ir in 0; do
 for alphaDen in $ALPHA_DENS; do
#  for dataset in $datasets; do
#   for mut in $MUT_LIST; do

gnuplot <<- EOF
set term $PIX_FORMAT        # set terminal for output picture format


##########################    bpb    ##########################
#set xlabel "% mutation"                 # set label of x axis
#set ylabel "bpb"                        # set label of y axis
##set xtics 0,5,100                      # set steps for x axis
#set xtics add ("1" 1)
#set key bottom right                    # legend position
##set output "$IR_LBL$ir-$a_LBL$alphaDen-$dataset-bpb.$PIX_FORMAT"       # set output name
##plot "$FLD_dat/$IR_LBL$ir-$a_LBL$alphaDen-${dataset}.$INF_FILE_TYPE" using 1:2  with linespoints ls 7 title "$IR_LBL=$ir, $a_LBL=1/$alphaDen, $CHR$CURR_CHR"
##set output "$a_LBL$alphaDen-$dataset-bpb.$PIX_FORMAT"       # set output name
##plot "$FLD_dat/${IR_LBL}0-$a_LBL$alphaDen-${dataset}.$INF_FILE_TYPE" using 1:2  with linespoints ls 6 title "$IR_LBL=0, $a_LBL=1/$alphaDen, $CHR$CURR_CHR", \
##     "$FLD_dat/${IR_LBL}1-$a_LBL$alphaDen-${dataset}.$INF_FILE_TYPE" using 1:2  with linespoints ls 7 title "$IR_LBL=1, $a_LBL=1/$alphaDen, $CHR$CURR_CHR"
#set output "$IR_LBL$ir-$a_LBL$alphaDen-bpb.$PIX_FORMAT"       # set output name
#set title "IR=$ir,   Alpha=$alphaDen"
##plot for [i=1:22] "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HS".i.".$INF_FILE_TYPE" using 1:2  with linespoints ls "".i."" title "${CHR} ".i."", \
##     "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HSX.$INF_FILE_TYPE" using 1:2  with linespoints ls 23 title "${CHR} X", \
##     "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HSY.$INF_FILE_TYPE" using 1:2  with linespoints ls 24 title "${CHR} Y", \
#
#plot \
# for [i=1:8]    "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HS".i.".$INF_FILE_TYPE" using 1:2  with linespoints ls "".i."" title "${CHR} ".i."", \
# for [i=10:12]  "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HS".i.".$INF_FILE_TYPE" using 1:2  with linespoints ls "".i."" title "${CHR} ".i."", \
# for [i=18:18]  "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HS".i.".$INF_FILE_TYPE" using 1:2  with linespoints ls "".i."" title "${CHR} ".i."", \
#                "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HSX.$INF_FILE_TYPE" using 1:2  with linespoints ls 23 title "${CHR} X", \
# for [i=9:9]    "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HS".i.".$INF_FILE_TYPE" using 1:2  with linespoints ls "".i."" title "${CHR} ".i."", \
# for [i=16:16]  "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HS".i.".$INF_FILE_TYPE" using 1:2  with linespoints ls "".i."" title "${CHR} ".i."", \
# for [i=19:19]  "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HS".i.".$INF_FILE_TYPE" using 1:2  with linespoints ls "".i."" title "${CHR} ".i."", \
## for [i=17:17]  "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HS".i.".$INF_FILE_TYPE" using 1:2  with linespoints ls "".i."" title "${CHR} ".i."", \
## for [i=21:21]  "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HS".i.".$INF_FILE_TYPE" using 1:2  with linespoints ls "".i."" title "${CHR} ".i."", \
## for [i=22:22]  "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HS".i.".$INF_FILE_TYPE" using 1:2  with linespoints ls "".i."" title "${CHR} ".i."", \
## "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HSY.$INF_FILE_TYPE" using 1:2  with linespoints ls 24 title "${CHR} Y", \
## for [i=13:15]  "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HS".i.".$INF_FILE_TYPE" using 1:2  with linespoints ls "".i."" title "${CHR} ".i."", \


###########################    ctx    ##########################
##set ylabel "context-order size"
##set terminal pngcairo size 600, 850
#set terminal $PIX_FORMAT size 600, 850
#set output "$IR_LBL$ir-$a_LBL$alphaDen-ctx.$PIX_FORMAT"
#set multiplot layout 12,2 columnsfirst margins 0.08,0.98,0.06,0.98 spacing 0.013,0.0
#set offset 0,0,graph 0.1, graph 0.1
#set key top right samplen 2 spacing 1.5 font ",11"
#
#LT=7                # linetype
#LW=2.0              # linewidth
#AxisNumScale=0.35   # axis numbers scale
#
#set grid
#set label 1 '%mutation' at screen 0.47,0.015
#set label 2 'context-order size' at screen 0.02,0.47 rotate by 90
#set xtics 5,5,50 scale 0.35                             # set steps for x axis
#set ytics 2,2,10 scale 0.5 offset 0.4,0 font ",10"      # set steps for y axis
#set yrange [2:10]
#
######   first column   #####
#do for [i=1:11] {
#set xtics format ''
#plot "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HS".i.".$INF_FILE_TYPE" using 1:3 \
#     with lines linetype LT linewidth LW title "".i.""
#}
####   chromosome 12   ###
#set xtics add ("1" 1, "5" 5, "10" 10, "15" 15, "20" 20, "25" 25, "30" 30, "35" 35, "40" 40, "45" 45, "50  " 50) \
#    scale AxisNumScale offset 0.25,0.4 font ",10"
#plot "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HS12.$INF_FILE_TYPE" using 1:3 \
#     with lines linetype LT linewidth LW title "12"
#
######   second column   #####
#do for [i=13:22] {
#set xtics 5,5,50 scale AxisNumScale
#set xtics format ''
#set ytics format ''
#plot "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HS".i.".$INF_FILE_TYPE" using 1:3 \
#     with lines linetype LT linewidth LW title "".i.""
#}
####   chromosome X   ###
#plot "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HS23.$INF_FILE_TYPE" using 1:3 \
#     with lines linetype LT linewidth LW title "X"
####   chromosome Y   ###
#set xtics add ("  1" 1, "5" 5, "10" 10, "15" 15, "20" 20, "25" 25, "30" 30, "35" 35, "40" 40, "45" 45, "50" 50) \
#    scale AxisNumScale offset 0.25,0.4 font ",10"
#plot "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HS24.$INF_FILE_TYPE" using 1:3 \
#     with lines linetype LT linewidth LW title "Y"
#
#unset multiplot; set output

# the following line (EOF) MUST be left as it is; i.e. no space, etc
EOF

#   done
#  done
 done
done

fi  #end of plot output using "gnuplot"


#***********************************************************
#   build matrix from Human Chimpanzee correspondence
#***********************************************************
if [[ $BUILD_MATRIX == 1 ]]; then

cd $FLD_dat

#echo "" >> HUMAN_PAD.dat;   for ch_HS in {1..24}; do echo "$HUMAN_CHR$ch_HS" >> HUMAN_PAD.dat; done
#echo "" >> CHIMP_PAD.dat;   for ch_PT in 1 2A 2B {3..24}; do echo "$CHIMP_CHR$ch_PT" >> CHIMP_PAD.dat; done

for ch_PT in 1 2A 2B {3..24}; do printf "\t%s" "$CHIMP_CHR$ch_PT" >> "${CHIMP_CHR}_HORIZ_PAD"; done;  echo >> "${CHIMP_CHR}_HORIZ_PAD"
for ch_HS in {1..24}; do printf "\t%s" "$HUMAN_CHR$ch_HS" >> "${HUMAN_CHR}_HORIZ_PAD"; done;  echo >> "${HUMAN_CHR}_HORIZ_PAD"

for alphaDen in $ALPHA_DENS; do
 for i in 0 1; do

  cat "${CHIMP_CHR}_HORIZ_PAD" >> "mat_$IR_LBL$i-$a_LBL$alphaDen-$HUMAN_CHR.$INF_FILE_TYPE"
  cat "${HUMAN_CHR}_HORIZ_PAD" >> "mat_$IR_LBL$i-$a_LBL$alphaDen-$CHIMP_CHR.$INF_FILE_TYPE"

  ### reference = Human
  for ch_HS in {1..24}; do
#   awk -F "\t" '{print $7}' "$IR_LBL$i-$a_LBL$alphaDen-$HUMAN_CHR$ch_HS.$INF_FILE_TYPE" | awk 'NR == 1 {next} {print}' \
#       | tr '\n' '\t' >> "mat_$IR_LBL$i-$a_LBL$alphaDen-$HUMAN_CHR.$INF_FILE_TYPE"
   awk -F "\t" '{print $7}' "$IR_LBL$i-$a_LBL$alphaDen-$HUMAN_CHR$ch_HS.$INF_FILE_TYPE" \
        | awk -v HS_ch=$HUMAN_CHR$ch_HS 'NR == 1 {print HS_ch; next} {print}' | tr '\n' '\t' \
        >> "mat_$IR_LBL$i-$a_LBL$alphaDen-$HUMAN_CHR.$INF_FILE_TYPE"
   echo >> "mat_$IR_LBL$i-$a_LBL$alphaDen-$HUMAN_CHR.$INF_FILE_TYPE"
  done

  ### reference = Chimpanzee
  for ch_PT in 1 2A 2B {3..24}; do
#   awk -F "\t" '{print $7}' "$IR_LBL$i-$a_LBL$alphaDen-$CHIMP_CHR$ch_PT.$INF_FILE_TYPE" | awk 'NR == 1 {next} {print}' \
#       | tr '\n' '\t' >> "mat_$IR_LBL$i-$a_LBL$alphaDen-$CHIMP_CHR.$INF_FILE_TYPE"
   awk -F "\t" '{print $7}' "$IR_LBL$i-$a_LBL$alphaDen-$CHIMP_CHR$ch_PT.$INF_FILE_TYPE" \
        | awk -v PT_ch=$CHIMP_CHR$ch_PT 'NR == 1 {print PT_ch; next} {print}' | tr '\n' '\t' \
        >> "mat_$IR_LBL$i-$a_LBL$alphaDen-$CHIMP_CHR.$INF_FILE_TYPE"
   echo >> "mat_$IR_LBL$i-$a_LBL$alphaDen-$CHIMP_CHR.$INF_FILE_TYPE"
  done
 done
#
# ### making diff matrix (Human)
# for ch_PT in 1 2A 2B {3..24}; do printf "\t%s" "$CHIMP_CHR$ch_PT" >> "mat_diff_$a_LBL$alphaDen-$HUMAN_CHR.$INF_FILE_TYPE"; done \
#     >> "mat_diff_$a_LBL$alphaDen-$HUMAN_CHR.$INF_FILE_TYPE"
# ### making diff matrix (Chimpanzee)
# for ch_HS in {1..24}; do printf "\t%s" "$HUMAN_CHR$ch_HS" >> "mat_diff_$a_LBL$alphaDen-$CHIMP_CHR.$INF_FILE_TYPE"; done \
#     >> "mat_diff_$a_LBL$alphaDen-$CHIMP_CHR.$INF_FILE_TYPE"
# ### ($i==$i+0) in awk checks if the column is not numeric
# ### paste "z1" "z2" | tr ',' '.' | awk '{for (i=1;i<=NF/2;i++) printf "%s\t", ($i==$i+0)?$i-$(i+NF/2):$i; print ""}' > zz

 for chr in $HUMAN_CHR $CHIMP_CHR; do
  paste "mat_${IR_LBL}0-$a_LBL$alphaDen-$chr.$INF_FILE_TYPE" "mat_${IR_LBL}1-$a_LBL$alphaDen-$chr.$INF_FILE_TYPE" | tr ',' '.' \
        | awk 'NR == 1 {print; next} {print}' \
        | awk '{for (i=1;i<=NF/2;i++) printf "%s\t", ($i==$i+0)?$i-$(i+NF/2):$i; print ""}' \
        >> "mat_diff_$a_LBL$alphaDen-$chr.$INF_FILE_TYPE"

  echo -e "\t$(cat "mat_diff_$a_LBL$alphaDen-$chr.$INF_FILE_TYPE")" > "mat_diff_$a_LBL$alphaDen-$chr.$INF_FILE_TYPE"
 done

#paste "HUMAN_PAD.dat" "TEMP_mat_diff_$a_LBL$alphaDen-$HUMAN_CHR.$INF_FILE_TYPE" >> "mat_diff_$a_LBL$alphaDen-$HUMAN_CHR.$INF_FILE_TYPE"
#paste "CHIMP_PAD.dat" "TEMP_mat_diff_$a_LBL$alphaDen-$CHIMP_CHR.$INF_FILE_TYPE" >> "mat_diff_$a_LBL$alphaDen-$CHIMP_CHR.$INF_FILE_TYPE"

rm -f "HUMAN_PAD.dat" "CHIMP_PAD.dat" "${HUMAN_CHR}_HORIZ_PAD" "${CHIMP_CHR}_HORIZ_PAD"
done

cd ..

fi


#***********************************************************
#   archive data
#***********************************************************
if [[ $ARCHIVE_DATA == 1 ]]; then

#mkdir -p archive
mv $FLD_dat/* $FLD_archive_dat/

fi


########################
cd $FLD_scripts
