#!/usr/bin/env bash


#for ir in $INV_REPS; do
    for alphaDen in $ALPHA_DENS; do

gnuplot <<- EOF
set terminal $PIX_FORMAT enhanced color size 6.25,2.65     # HS-PT, PT-HS, HS-GG
#set terminal $PIX_FORMAT enhanced color size 8.3,3.7      # GGA-MGA, MGA-GGA

set output "$REF-$TAR.$PIX_FORMAT"
set multiplot layout 1,2 columnsfirst margins 0.0255,0.9148,0.1065,0.992 \
    spacing 0.03,0
#set multiplot layout 1,2 columnsfirst margins 0.02,0.918,0.08,0.992 \
#   spacing 0.03,0 # GGA-MGA, MGA-GGA

#set offset 0,0,graph 0.1, graph 0.1
#xticsOffset=-1.2
xticsOffset=0.2 # GGA-MGA, MGA-GGA
yticsOffset=-0.875 #-2.3
#xlabelOffset=0.99 #-1.2
xlabelOffset=0.9   # GGA-MGA, MGA-GGA
ylabelOffset=0 #-2.3
#set size ratio .9 #0.85
set key off

##set tmargin 2.1    ### with title
#set tmargin 0.5     ### without title
#set bmargin 2.7 #4
##set bmargin 7
#set lmargin 5
#set rmargin 1.35
#set pm3d map

set macros
fontLabelSpecies='font "Latin Modern Math-Italic, 13"'
fontLabel='font "Latin Modern Math, 13"'
fontTics='font "Latin Modern Sans, 10"'

#set nocbtics
set cblabel "NRC" @fontLabel offset -1.5,0     #-0.25 or -1.5
set cbtics scale 0.5 @fontTics offset -0.8,0
#set cbtics
#set cbrange [ 0 : 1 ] noreverse nowriteback

#set palette defined (0 "red", 1 "yellow", 2 "green", 3 "white")
#set palette defined (0.2 "red", 0.6 "green", 1 "white")   # HS-PT, PT-HS, HS-GG
set palette defined (0.65 "red", 0.825 "green", 1 "white")  # GGA-MGA, MGA-GGA

##set yrange [2:10]

### reference-target, i0
#set output "$REF-$TAR.$PIX_FORMAT"
#set title "Inverted repeats not considered"
#set title "Relative compression: HS-PT\nReference: HS, Target: PT,\
# inverted repeats: not considered"
unset colorbox      # remove color palette
#set rmargin 47.5

XTICS="`head -1 \
    "$FLD_dat/tot-${IR}0-$REF-$TAR.$INF_FTYPE" \
    | awk -v start_ind="$TAR_LEN_IND" \
    '{for(i=1;i<=NF;i++) printf "%s\t",substr($i,start_ind);}'`"
set for [i=1:words(XTICS)] xtics ( word(XTICS,i) i-1 ) right @fontTics \
    rotate by 90 offset 0,xticsOffset
set xlabel "$TAR_SNAME" offset 0,xlabelOffset @fontLabelSpecies
set ylabel "$REF_SNAME" offset ylabelOffset,0 @fontLabelSpecies
unset ytics

plot "<awk 'NR>1' \
    '$FLD_dat/tot-${IR}0-$REF-$TAR.$INF_FTYPE' \
    | cut -f2-" matrix with image
### ! before any cmd inside gnuplot lets bash command work (e.g. the followings)
##!awk 'NR>1' $FLD_dat/tot-${IR}0-$HUMAN-$CHIMPANZEE.$INF_FTYPE\
##  | cut -f2- > temp
##plot "temp" matrix with image
##!rm temp

### reference-target, i1
#set output "${IR}1-$REF-$TAR.$PIX_FORMAT"
#set title "Inverted repeats considered"
#set title "Relative compression: HS-PT\nReference: HS, Target: PT, \
#   inverted repeats: considered"
set colorbox        # draw color palette
#set lmargin 44.5
set lmargin 57    # GGA-MGA, MGA-GGA

YTICS="`awk -v start_ind="$REF_LEN_IND" \
    'BEGIN{getline}{printf "%s ",substr($1,start_ind)}' \
    "$FLD_dat/tot-${IR}0-$REF-$TAR.$INF_FTYPE"`"
set for [i=1:words(YTICS)] ytics ( word(YTICS,i) i-1 ) center @fontTics \
    offset yticsOffset, 0

unset ylabel

plot "<awk 'NR>1'
    '$FLD_dat/tot-${IR}1-$REF-$TAR.$INF_FTYPE' \
    | cut -f2-" matrix with image

unset multiplot; set output
EOF

    done
#done


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#   difference
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#for ir in $INV_REPS; do
    for alphaDen in $ALPHA_DENS; do

gnuplot <<- EOF
set terminal $PIX_FORMAT enhanced color size 3.05,2.8   # diff-PT-HS, diff-HS-GG
#set terminal $PIX_FORMAT enhanced color size 3.0,2.8   # diff-HS-PT
#set terminal $PIX_FORMAT enhanced color size 3.75,3.65 # diff-GGA-MGA
#set terminal $PIX_FORMAT enhanced color size 4.3,3.6   # diff-MGA-GGA
set output "diff-$REF-$TAR.$PIX_FORMAT"
#set multiplot layout 1,1 columnsfirst #margins 0.0255,0.9147,0.105,0.992 \
#   spacing 0.03,0
#set offset 0,0,graph 0.1, graph 0.1

xticsOffset=0.2     #-1.2
yticsOffset=0       #-2.3
xlabelOffset=0.8    #-1.2
ylabelOffset=0.81   #-2.3
#set size ratio .9  #0.85
set key off

##set tmargin 2.1    ### with title
set tmargin 0.18     ### without title
set bmargin 2.6 #4 7
#set bmargin 2.42 # diff-GGA-MGA
set lmargin 4.55
#set rmargin 1.37
set rmargin 0.2    # diff-HS-PT, diff-GGA-MGA
#set rmargin 1.6    # diff-MGA-GGA
#set pm3d map
set macros
fontLabelSpecies='font "Latin Modern Math-Italic, 13"'
fontLabel='font "Latin Modern Math, 13"'
fontTics='font "Latin Modern Sans, 10"'

#set nocbtics
set cblabel "NRC_{IR=0} - NRC_{IR=1}" @fontLabel offset -1,0     #-0.25 or -1.5
set cbtics scale 0.5 @fontTics offset -0.65,0
#set cbtics
#unset colorbox
#set cbrange [ -0.05 : 0.3 ] noreverse nowriteback  # diff-HS-PT
#set cbrange [ -0.05 : 0.5 ] noreverse nowriteback  # diff-HS-GG
#set cbrange [ -0.01 : 0.09 ] noreverse nowriteback # diff-GGA-MGA, diff-MGA-GGA

### diff-HS-PT, diff-HS-PT
set palette defined (-0.05 "white", 0.125 "green", 0.3 "red")
#set palette defined (-0.05 "white", 0.225 "green", 0.5 "red")     # diff-HS-GG
### diff-GGA-MGA, diff-MGA-GGA
#set palette defined (-0.01 "white", 0.04 "green", 0.09 "red")

#set title "The difference"
#set title "Relative compression: HS-PT\nDifference between considering and\
#   not considering inverted repeats\nReference: HS, Target: PT"

YTICS="`awk -v start_ind="$REF_LEN_IND" \
    'BEGIN{getline}{printf "%s ",substr($1,start_ind)}' \
    "$FLD_dat/diff-$REF-$TAR.$INF_FTYPE"`"
XTICS="`head -1 "$FLD_dat/diff-$REF-$TAR.$INF_FTYPE" \
    | awk -v start_ind="$TAR_LEN_IND" \
    '{for(i=1;i<=NF;i++) printf "%s\t",substr($i,start_ind);}'`"
set for [i=1:words(XTICS)] xtics ( word(XTICS,i) i-1 ) right @fontTics \
    rotate by 90 offset 0,xticsOffset
set for [i=1:words(YTICS)] ytics ( word(YTICS,i) i-1 ) @fontTics \
    offset yticsOffset, 0
set xlabel "$TAR_SNAME" offset 0,xlabelOffset @fontLabelSpecies
set ylabel "$REF_SNAME" offset ylabelOffset,0 @fontLabelSpecies

plot "<awk 'NR>1' '$FLD_dat/diff-$REF-$TAR.$INF_FTYPE' \
    | cut -f2-" matrix with image

unset multiplot; set output

EOF

    done
#done
