#!/usr/bin/env bash

#
##for ir in $INV_REPS; do
#    for alphaDen in $ALPHA_DENS; do
#
#gnuplot <<- EOF
#set terminal $PIX_FORMAT enhanced color size 6.25,2.65     # HS-PT, PT-HS, HS-GG
##set terminal $PIX_FORMAT enhanced color size 8.3,3.7      # GGA-MGA, MGA-GGA
#
#set output "$REF-$TAR.$PIX_FORMAT"
#set multiplot layout 1,2 columnsfirst margins 0.0255,0.9148,0.1065,0.992 \
#    spacing 0.03,0
##set multiplot layout 1,2 columnsfirst margins 0.02,0.918,0.08,0.992 \
##   spacing 0.03,0 # GGA-MGA, MGA-GGA
#
##set offset 0,0,graph 0.1, graph 0.1
##xticsOffset=-1.2
#xticsOffset=0.2 # GGA-MGA, MGA-GGA
#yticsOffset=-0.875 #-2.3
##xlabelOffset=0.99 #-1.2
#xlabelOffset=0.9   # GGA-MGA, MGA-GGA
#ylabelOffset=0 #-2.3
##set size ratio .9 #0.85
#set key off
#
###set tmargin 2.1    ### with title
##set tmargin 0.5     ### without title
##set bmargin 2.7 #4
###set bmargin 7
##set lmargin 5
##set rmargin 1.35
##set pm3d map
#
#set macros
#fontLabelSpecies='font "Latin Modern Math-Italic, 13"'
#fontLabel='font "Latin Modern Math, 13"'
#fontTics='font "Latin Modern Sans, 10"'
#
##set nocbtics
#set cblabel "NRC" @fontLabel offset -1.5,0     #-0.25 or -1.5
#set cbtics scale 0.5 @fontTics offset -0.8,0
##set cbtics
##set cbrange [ 0 : 1 ] noreverse nowriteback
#
##set palette defined (0 "red", 1 "yellow", 2 "green", 3 "white")
##set palette defined (0.2 "red", 0.6 "green", 1 "white")   # HS-PT, PT-HS, HS-GG
#set palette defined (0.65 "red", 0.825 "green", 1 "white")  # GGA-MGA, MGA-GGA
#
###set yrange [2:10]
#
#### reference-target, i0
##set output "$REF-$TAR.$PIX_FORMAT"
##set title "Inverted repeats not considered"
##set title "Relative compression: HS-PT\nReference: HS, Target: PT,\
## inverted repeats: not considered"
#unset colorbox      # remove color palette
##set rmargin 47.5
#
#XTICS="`head -1 \
#    "$FLD_dat/tot-${IR}0-$REF-$TAR.$INF_FTYPE" \
#    | awk -v start_ind="$TAR_LEN_IND" \
#    '{for(i=1;i<=NF;i++) printf "%s\t",substr($i,start_ind);}'`"
#set for [i=1:words(XTICS)] xtics ( word(XTICS,i) i-1 ) right @fontTics \
#    rotate by 90 offset 0,xticsOffset
#set xlabel "$TAR_SNAME" offset 0,xlabelOffset @fontLabelSpecies
#set ylabel "$REF_SNAME" offset ylabelOffset,0 @fontLabelSpecies
#unset ytics
#
#plot "<awk 'NR>1' \
#    '$FLD_dat/tot-${IR}0-$REF-$TAR.$INF_FTYPE' \
#    | cut -f2-" matrix with image
#### ! before any cmd inside gnuplot lets bash command work (e.g. the followings)
###!awk 'NR>1' $FLD_dat/tot-${IR}0-$HUMAN-$CHIMPANZEE.$INF_FTYPE\
###  | cut -f2- > temp
###plot "temp" matrix with image
###!rm temp
#
#### reference-target, i1
##set output "${IR}1-$REF-$TAR.$PIX_FORMAT"
##set title "Inverted repeats considered"
##set title "Relative compression: HS-PT\nReference: HS, Target: PT, \
##   inverted repeats: considered"
#set colorbox        # draw color palette
##set lmargin 44.5
#set lmargin 57    # GGA-MGA, MGA-GGA
#
#YTICS="`awk -v start_ind="$REF_LEN_IND" \
#    'BEGIN{getline}{printf "%s ",substr($1,start_ind)}' \
#    "$FLD_dat/tot-${IR}0-$REF-$TAR.$INF_FTYPE"`"
#set for [i=1:words(YTICS)] ytics ( word(YTICS,i) i-1 ) center @fontTics \
#    offset yticsOffset, 0
#
#unset ylabel
#
#plot "<awk 'NR>1'
#    '$FLD_dat/tot-${IR}1-$REF-$TAR.$INF_FTYPE' \
#    | cut -f2-" matrix with image
#
#unset multiplot; set output
#EOF
#
#    done
##done
#

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#   difference
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#for ir in $INV_REPS; do
    for alphaDen in $ALPHA_DENS; do

gnuplot <<- EOF

#-------------------------------------------------------------------------------
#	terminal, output 
#-------------------------------------------------------------------------------
set terminal $PIX_FORMAT enhanced color size 7.3,6.5

set output "diff-$REF-$TAR.$PIX_FORMAT"

#-------------------------------------------------------------------------------
#	multiplot layout, offset, size ratio, key
#-------------------------------------------------------------------------------
#set offset 0,0,graph 0.1, graph 0.1

xticsOffset=0.2
yticsOffset=0
xlabelOffset=18.5
ylabelOffset=31

#set size ratio .9  #0.85

set key off

#-------------------------------------------------------------------------------
#	margin	
#-------------------------------------------------------------------------------
##set tmargin 2.1    ### with title
set tmargin 0.5    ### without title
set bmargin 23.25 
set lmargin 42 
set rmargin 2 

#-------------------------------------------------------------------------------
#	font, colorbox  	
#-------------------------------------------------------------------------------
#set pm3d map
set macros
fontLabelSpecies='font "Latin Modern Math Italic, 14"'
fontLabel='font "Latin Modern Math, 14"'
fontTics='font "Latin Modern Sans, 10"'

#set nocbtics
set cblabel "NRC_{IR=0} - NRC_{IR=1}" @fontLabel offset 0,0     #-0.25 or -1.5
set cbtics scale 0.5 @fontTics offset -0.65,0
#set cbtics
#unset colorbox
#set cbrange [ -0.05 : 0.3 ] noreverse nowriteback  # diff-HS-PT

#-------------------------------------------------------------------------------
#	color palette, title, tics, label  	
#-------------------------------------------------------------------------------
set palette defined (-0.1 "white", 0.45 "green", 1 "red")     # diff-F-F

#set title "The difference"
#set title "Relative compression: HS-PT\nDifference between considering and\
#   not considering inverted repeats\nReference: HS, Target: PT"

set xtics( \
"Gigaspora margarita mitochondrion"					   0 , \
"Colletotrichum fioriniae mitochondrion"				   1,  \
"Colletotrichum acutatum strain KC05 mitochondrion" 			   2 , \
"Colletotrichum tamarilloi mitochondrion" 				   3,  \
"Verticillium nonalfalfae mitochondrion" 				   4,  \
"Colletotrichum lupini strain CBS 119142 culture-collection mitochondrion" 5,  \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c49"	 	   6,  \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c48"	 	   7,  \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c47"		   8,  \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c46" 		   9,  \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c44"		   10, \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c42"		   11, \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c39"	 	   12, \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c36"	 	   13, \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c35"		   14, \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c34" 		   15, \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c32"		   16, \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c31" 		   17, \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c30"	 	   18, \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c28"	 	   19, \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c27"		   20, \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c25" 		   21, \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c11"		   22, \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c10" 		   23, \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c01" 		   24, \
"Aspergillus oryzae 3.042 mitochondrion"				   25, \
"Gigaspora rosea mitochondrion" 					   26, \
"Aspergillus oryzae mitochondrion" 					   27, \
"Verticillium dahliae mitochondrion" 					   28, \
"Madurella mycetomatis mitochondrion"					   29, \
         ) right @fontTics rotate by 90 offset 0,xticsOffset


set ytics( \
"Pseudogymnoascus destructans st. 20631-21 mitochondrion"		   0,  \
"Candida phangngaensis voucher CBS:10407 mitochondrion" 		   1,  \
"Lecanora strobilina mitochondrion" 					   2,  \
"Colletotrichum lindemuthianum mitochondrion" 				   3,  \
"Pyronema omphalodes st. CBS 100304 culture-collection mitochondrion" 	   4,  \
"Colletotrichum lupini st. CBS 119142 culture-collection mitochondrion"	   5,  \
"Rhizophagus fasciculatus isolate DAOM240159 mitochondrion" 		   6,  \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c48"	 	   7,  \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c47" 		   8,  \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c46"	 	   9,  \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c45" 		   10, \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c43" 		   11, \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c41" 		   12, \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c38" 		   13, \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c35" 		   14, \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c34" 		   15, \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c33" 		   16, \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c31" 		   17, \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c30" 		   18, \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c29" 		   19, \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c27" 		   20, \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c26" 		   21, \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c24" 		   22, \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c10" 		   23, \
"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c09" 		   24, \
"Talaromyces marneffei mitochondrion" 					   25, \
"Gigaspora rosea mitochondrion" 					   26, \
"Marssonina brunnea f. sp. 'multigermtubi' mitochondrion" 		   27, \
"Verticillium dahliae mitochondrion" 					   28, \
"Metarhizium anisopliae mitochondrion"					   29, \
	) right @fontTics rotate by 0 offset yticsOffset,0

set xlabel "$TAR_SNAME" offset 0,xlabelOffset @fontLabelSpecies
set ylabel "$REF_SNAME" offset ylabelOffset,0 @fontLabelSpecies

plot "<awk 'NR>1' '$FLD_dat/diff_filtered-$REF-$TAR.$INF_FTYPE' \
    | cut -f2-" matrix with image

unset multiplot; set output

EOF

    done
#done
