#!/usr/bin/env bash


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#   total ir=0 & ir=1
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
for ir in $INV_REPS; do
    for alphaDen in $ALPHA_DENS; do

gnuplot <<- EOF

#-------------------------------------------------------------------------------
#	terminal, output 
#-------------------------------------------------------------------------------
set terminal $PIX_FORMAT enhanced color size 7.3,6.6

set output "tot-i$ir-$REF-$TAR.$PIX_FORMAT"

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
set cblabel "NRC_{IR=$ir}" @fontLabel offset 0,0     #-0.25 or -1.5
set cbtics scale 0.5 @fontTics offset -0.65,0
#set cbtics
#unset colorbox
#set cbrange [ -0.05 : 0.3 ] noreverse nowriteback  # diff-HS-PT

#-------------------------------------------------------------------------------
#	color palette, title, tics, label  	
#-------------------------------------------------------------------------------
set palette defined (0.0241 "red", 0.3795 "green", 1 "white")     # tot-i0-F-F

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

plot "<awk 'NR>1' '$FLD_dat/tot_filtered-i$ir-$REF-$TAR.$INF_FTYPE' \
    | cut -f2-" matrix with image

unset multiplot; set output

EOF

    done
done



#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#   difference
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#for ir in $INV_REPS; do
    for alphaDen in $ALPHA_DENS; do

gnuplot <<- EOF

#-------------------------------------------------------------------------------
#	terminal, output 
#-------------------------------------------------------------------------------
set terminal $PIX_FORMAT enhanced color size 7.3,6.6

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
