#!/usr/bin/env bash


#for ir in $INV_REPEATS; do
 for alphaDen in $ALPHA_DENS; do

gnuplot <<- EOF
set terminal $PIX_FORMAT enhanced color #size 4,3
set output "$REF_SPECIES-$TAR_SPECIES.$PIX_FORMAT"
set multiplot layout 1,2 #columnsfirst margins 0.08,0.98,0.06,0.98 spacing 0.013,0.0
xticsOffset=0.2 #-1.2
yticsOffset=0.2 #-2.3
#set size ratio .9 #0.85
set key off
#set tmargin 2.1    ### with title
set tmargin 0.5     ### without title
set bmargin 2.7 #4
set lmargin 5
set rmargin 1.35
set pm3d map

#set nocbtics
set cblabel "NRC" font ",11" offset -0.25,0     # works for some files
#set cblabel "NRC" font ",11" offset -1.1,0     # works for some other files
set cbtics scale 1 font ",9.5"
#set cbtics
#set cbrange [ 0.2 : 1 ] noreverse nowriteback

set palette defined (0 "red", 1 "green", 2 "white")







#set multiplot layout 2,1 columnsfirst margins 0.08,0.98,0.06,0.98 spacing 0.013,0.0
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
##set xtics 5,5,50 scale 0.35                             # set steps for x axis
##set ytics 2,2,10 scale 0.5 offset 0.4,0 font ",10"      # set steps for y axis
##set yrange [2:10]
##
#######   first column   #####
##do for [i=1:11] {
##set xtics format ''
##plot "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HS".i.".$INF_FILE_TYPE" using 1:3 \
##     with lines linetype LT linewidth LW title "".i.""
##}
##
#######   second column   #####
##do for [i=13:22] {
##set xtics 5,5,50 scale AxisNumScale
##set xtics format ''
##set ytics format ''
##plot "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HS".i.".$INF_FILE_TYPE" using 1:3 \
##     with lines linetype LT linewidth LW title "".i.""
##}



### reference-target, i0
#set output "$REF_SPECIES-$TAR_SPECIES.$PIX_FORMAT"
#set title "Inverted repeats considered"
#set title "Relative compression: HS-PT\nReference: HS, Target: PT, inverted repeats: not considered"

YTICS="`awk 'BEGIN{getline}{printf "%s ",$1}' "$FLD_dat/tot-${IR_LBL}0-$REF_SPECIES-$TAR_SPECIES.$INF_FILE_TYPE"`"
XTICS="`head -1 "$FLD_dat/tot-${IR_LBL}0-$REF_SPECIES-$TAR_SPECIES.$INF_FILE_TYPE"`"
set for [i=1:words(XTICS)] xtics ( word(XTICS,i) i-1 ) right font ",9" rotate by 90 offset 0,xticsOffset
set for [i=1:words(YTICS)] ytics ( word(YTICS,i) i-1 ) right font ",9" offset yticsOffset,0

plot "<awk 'NR>1' '$FLD_dat/tot-${IR_LBL}0-$REF_SPECIES-$TAR_SPECIES.$INF_FILE_TYPE' | cut -f2-" matrix with image
### ! before any command inside gnuplot lets bash command work (e.g. the followings)
##!awk 'NR>1' $FLD_dat/tot-${IR_LBL}0-$HUMAN_CHR-$CHIMPANZEE_CHR.$INF_FILE_TYPE | cut -f2- > temp
##plot "temp" matrix with image
##!rm temp

### reference-target, i1
#set output "${IR_LBL}1-$REF_SPECIES-$TAR_SPECIES.$PIX_FORMAT"
#set title "Inverted repeats not considered"
#set title "Relative compression: HS-PT\nReference: HS, Target: PT, inverted repeats: considered"

YTICS="`awk 'BEGIN{getline}{printf "%s ",$1}' "$FLD_dat/tot-${IR_LBL}1-$REF_SPECIES-$TAR_SPECIES.$INF_FILE_TYPE"`"
XTICS="`head -1 "$FLD_dat/tot-${IR_LBL}1-$REF_SPECIES-$TAR_SPECIES.$INF_FILE_TYPE"`"
set for [i=1:words(XTICS)] xtics ( word(XTICS,i) i-1 ) font ",9" rotate by 90 offset 0,xticsOffset
set for [i=1:words(YTICS)] ytics ( word(YTICS,i) i-1 ) font ",9" offset yticsOffset,0

plot "<awk 'NR>1' '$FLD_dat/tot-${IR_LBL}1-$REF_SPECIES-$TAR_SPECIES.$INF_FILE_TYPE' | cut -f2-" matrix with image


#set palette defined (0 "white", 1 "green", 2 "red")
##set tmargin 4.5
#set cblabel "The difference between NRCs (NRC_{IR=0} - NRC_{IR=1})" font ",11" offset -1.5,0
#
#### reference-target, difference between i0 and i1
#set output "diff-$REF_SPECIES-$TAR_SPECIES.$PIX_FORMAT"
##set title "The difference"
##set title "Relative compression: HS-PT\nDifference between considering and not considering inverted repeats\nReference: HS, Target: PT"
#
#YTICS="`awk 'BEGIN{getline}{printf "%s ",$1}' "$FLD_dat/diff-$REF_SPECIES-$TAR_SPECIES.$INF_FILE_TYPE"`"
#XTICS="`head -1 "$FLD_dat/diff-$REF_SPECIES-$TAR_SPECIES.$INF_FILE_TYPE"`"
#set for [i=1:words(XTICS)] xtics ( word(XTICS,i) i-1 ) font ",9" rotate by 90 offset 0,xticsOffset
#set for [i=1:words(YTICS)] ytics ( word(YTICS,i) i-1 ) font ",9" offset yticsOffset,0
#
#plot "<awk 'NR>1' '$FLD_dat/diff-$REF_SPECIES-$TAR_SPECIES.$INF_FILE_TYPE' | cut -f2-" matrix with image

unset multiplot; set output

EOF

 done
#done
