#!/usr/bin/env bash


for ir in $INV_REPS; do
    for alphaDen in $ALPHA_DENS; do
#        for dataset in $datasets; do
#            for mut in $MUT_LIST; do

gnuplot <<- EOF
set term $PIX_FORMAT        # set terminal for output picture format

##########################    bpb    ##########################
#set xlabel "% mutation"                 # set label of x axis
#set ylabel "bpb"                        # set label of y axis
##set xtics 0,5,100                      # set steps for x axis
#set xtics add ("1" 1)
#set key bottom right                    # legend position
##set output "$IR$ir-$AL$alphaDen-$dataset-bpb.$PIX_FORMAT"  #out name
##plot "$FLD_dat/$IR$ir-$AL$alphaDen-${dataset}.$INF_FTYPE" \
## using 1:2  with linespoints ls 7 \
## title "$IR=$ir, $AL=1/$alphaDen, $CHR$CURR_CHR"
##set output "$AL$alphaDen-$dataset-bpb.$PIX_FORMAT"     # set output name
##plot "$FLD_dat/${IR}0-$AL$alphaDen-${dataset}.$INF_FTYPE" \
##  using 1:2  with linespoints ls 6 \
##  title "$IR=0, $AL=1/$alphaDen, $CHR$CURR_CHR", \
##  "$FLD_dat/${IR}1-$AL$alphaDen-${dataset}.$INF_FTYPE" using 1:2 \
##  with linespoints ls 7 title "$IR=1, $AL=1/$alphaDen, $CHR$CURR_CHR"
#set output "$IR$ir-$AL$alphaDen-bpb.$PIX_FORMAT"      # set output name
#set title "IR=$ir,   Alpha=$alphaDen"
##plot for [i=1:22] \
##  "$FLD_dat/$IR$ir-$AL$alphaDen-HS".i.".$INF_FTYPE"
##      using 1:2  with linespoints ls "".i."" title "${CHR} ".i."", \
##  "$FLD_dat/$IR$ir-$AL$alphaDen-HSX.$INF_FTYPE" using 1:2 \
##      with linespoints ls 23 title "${CHR} X", \
##  "$FLD_dat/$IR$ir-$AL$alphaDen-HSY.$INF_FTYPE" using 1:2 \
##      with linespoints ls 24 title "${CHR} Y", \
#
#plot \
# for [i=1:8]    "$FLD_dat/$IR$ir-$AL$alphaDen-HS".i.".$INF_FTYPE" \
#   using 1:2  with linespoints ls "".i."" title "${CHR} ".i."", \
# for [i=10:12]  "$FLD_dat/$IR$ir-$AL$alphaDen-HS".i.".$INF_FTYPE" \
#   using 1:2  with linespoints ls "".i."" title "${CHR} ".i."", \
# for [i=18:18]  "$FLD_dat/$IR$ir-$AL$alphaDen-HS".i.".$INF_FTYPE" \
#   using 1:2  with linespoints ls "".i."" title "${CHR} ".i."", \
#       "$FLD_dat/$IR$ir-$AL$alphaDen-HSX.$INF_FTYPE" using 1:2 \
#       with linespoints ls 23 title "${CHR} X", \
# for [i=9:9]    "$FLD_dat/$IR$ir-$AL$alphaDen-HS".i.".$INF_FTYPE" \
#   using 1:2  with linespoints ls "".i."" title "${CHR} ".i."", \
# for [i=16:16]  "$FLD_dat/$IR$ir-$AL$alphaDen-HS".i.".$INF_FTYPE" \
#   using 1:2  with linespoints ls "".i."" title "${CHR} ".i."", \
# for [i=19:19]  "$FLD_dat/$IR$ir-$AL$alphaDen-HS".i.".$INF_FTYPE" \
#   using 1:2  with linespoints ls "".i."" title "${CHR} ".i."", \
## for [i=17:17]  "$FLD_dat/$IR$ir-$AL$alphaDen-HS".i.".$INF_FTYPE" \
##  using 1:2  with linespoints ls "".i."" title "${CHR} ".i."", \
## for [i=21:21]  "$FLD_dat/$IR$ir-$AL$alphaDen-HS".i.".$INF_FTYPE" \
##  using 1:2  with linespoints ls "".i."" title "${CHR} ".i."", \
## for [i=22:22]  "$FLD_dat/$IR$ir-$AL$alphaDen-HS".i.".$INF_FTYPE" \
##  using 1:2  with linespoints ls "".i."" title "${CHR} ".i."", \
## "$FLD_dat/$IR$ir-$AL$alphaDen-HSY.$INF_FTYPE" using 1:2 \
##  with linespoints ls 24 title "${CHR} Y", \
## for [i=13:15]  "$FLD_dat/$IR$ir-$AL$alphaDen-HS".i.".$INF_FTYPE" \
##  using 1:2  with linespoints ls "".i."" title "${CHR} ".i."", \


###########################    ctx    ##########################
##set ylabel "context-order size"
##set terminal pngcairo size 600, 850
#set terminal $PIX_FORMAT size 600, 850
#set output "$IR$ir-$AL$alphaDen-ctx.$PIX_FORMAT"
#set multiplot layout 12,2 columnsfirst margins 0.08,0.98,0.06,0.98 \
#   spacing 0.013,0.0
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
#plot "$FLD_dat/$IR$ir-$AL$alphaDen-HS".i.".$INF_FTYPE" using 1:3 \
#     with lines linetype LT linewidth LW title "".i.""
#}
####   chromosome 12   ###
#set xtics add ("1" 1, "5" 5, "10" 10, "15" 15, "20" 20, "25" 25, "30" 30, \
#   "35" 35, "40" 40, "45" 45, "50  " 50) \
#    scale AxisNumScale offset 0.25,0.4 font ",10"
#plot "$FLD_dat/$IR$ir-$AL$alphaDen-HS12.$INF_FTYPE" using 1:3 \
#     with lines linetype LT linewidth LW title "12"
#
######   second column   #####
#do for [i=13:22] {
#set xtics 5,5,50 scale AxisNumScale
#set xtics format ''
#set ytics format ''
#plot "$FLD_dat/$IR$ir-$AL$alphaDen-HS".i.".$INF_FTYPE" using 1:3 \
#     with lines linetype LT linewidth LW title "".i.""
#}
####   chromosome X   ###
#plot "$FLD_dat/$IR$ir-$AL$alphaDen-HS23.$INF_FTYPE" using 1:3 \
#     with lines linetype LT linewidth LW title "X"
####   chromosome Y   ###
#set xtics add ("  1" 1, "5" 5, "10" 10, "15" 15, "20" 20, "25" 25, "30" 30, \
#   "35" 35, "40" 40, "45" 45, "50" 50) \
#    scale AxisNumScale offset 0.25,0.4 font ",10"
#plot "$FLD_dat/$IR$ir-$AL$alphaDen-HS24.$INF_FTYPE" using 1:3 \
#     with lines linetype LT linewidth LW title "Y"
#
#unset multiplot; set output

# the following line (EOF) MUST be left as it is; i.e. no space, etc
EOF

#            done
#        done
    done
done
