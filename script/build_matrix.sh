#!/usr/bin/env bash


#cd $FLD_dat

##//todo. {1..3} TAR ha hastand
for i in {1..3}; do printf "\t%s" "$i" >> ${TAR}_HORIZ_PAD; done
echo >> ${TAR}_HORIZ_PAD

for alphaDen in $ALPHA_DENS; do

    #%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    # total ir=0 & total ir=1
    #%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    for i in $INV_REPS; do
        cat ${TAR}_HORIZ_PAD > tot-$IR$i-$REF-$TAR.$INF_FTYPE

        for c in $REF_DATASET; do
            awk -F "\t" '{print $3}' $IR$i-$REF$c-$TAR.$INF_FTYPE \
             | awk -v ref_ch=$c 'NR == 1 {print ref_ch; next} {print}' \
             | tr '\n' '\t' | tr ',' '.' >> tot-$IR$i-$REF-$TAR.$INF_FTYPE
            echo >> tot-$IR$i-$REF-$TAR.$INF_FTYPE
        done
    done

    #%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    # difference ir=0 & ir=1
    #%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    cat ${TAR}_HORIZ_PAD > diff-$REF-$TAR.$INF_FTYPE
    paste tot-${IR}0-$REF-$TAR.$INF_FTYPE tot-${IR}1-$REF-$TAR.$INF_FTYPE \
     | awk 'NR>1' \
     | awk '{printf "%d", $1; \
        for (i=2;i<=NF/2;i++) printf "\t%.4f", $i-$(i+NF/2); print ""}' \
     >> diff-$REF-$TAR.$INF_FTYPE

    #%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    # filter NRC > 1
    #%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    for i in $INV_REPS; do
        cat "${TAR}_HORIZ_PAD" > "temp-tot"
        awk 'NR>1' tot-$IR$i-$REF-$TAR.$INF_FTYPE \
         | awk '{printf "%d", $1; \
                 for (i=2;i<=NF;i++)
                     if($i>1) printf "\t%.4f", 1; \
                     else     printf "\t%.4f", $i; print ""}' >> "temp-tot";
        rm -f "tot-$IR$i-$REF-$TAR.$INF_FTYPE";
        mv "temp-tot" "tot-$IR$i-$REF-$TAR.$INF_FTYPE";
        rm -f "temp-tot"
    done
done

rm -f *HORIZ_PAD









################################################################################
###  old original code  ########################################################
################################################################################

#cd $FLD_dat
#
#for i in $TAR_RUN; do
#    printf "\t%s" "$TAR$i" >> "${TAR}_HORIZ_PAD";
#done
#echo >> "${TAR}_HORIZ_PAD"
#
#for alphaDen in $ALPHA_DENS; do
#    for i in $INV_REPS; do
#        cat "${TAR}_HORIZ_PAD" \
#           >> "tot-$IR$i-$REF-$TAR.$INF_FTYPE"
#
#           for c in $REF_RUN; do
#               awk -F "\t" '{print $7}' \
#                "$IR$i-$REF$c-$TAR.$INF_FTYPE" \
#                | awk -v ref_ch=$REF$c 'NR == 1 {print ref_ch; next} {print}' \
#                | tr '\n' '\t' | tr ',' '.' \
#                >> "tot-$IR$i-$REF-$TAR.$INF_FTYPE"
#               echo >> "tot-$IR$i-$REF-$TAR.$INF_FTYPE"
#           done
#
#        ### filter NRC values greater than 1
#        awk 'NR>1 ' tot-$IR$i-$REF-$TAR.$INF_FTYPE \
#            | awk '{for (i=1;i<=NF;i++) if($i!=$i+0) printf "%s\t", $i; \
#            else if($i==$i+0 && $i>1) printf "%.5f\t", 1; \
#   	     else if($i==$i+0 && $i<=1) printf "%.5f\t", $i; print ""}' > temp;
#        cat "${TAR}_HORIZ_PAD" > "temp-tot"
#        cat "temp" >> "temp-tot"
#        rm -f "tot-$IR$i-$REF-$TAR.$INF_FTYPE";
#        mv "temp-tot" "tot-$IR$i-$REF-$TAR.$INF_FTYPE";
#        rm -f "temp" "temp-tot"
#    done
#
##       | tr ',' '.' | awk 'NR == 1 {print; next} {print}' \
##       | awk '{for (i=1;i<=NF/2;i++) printf "%s\t", \
##       ($i==$i+0)?$i-$(i+NF/2):$i; print ""}' \
#    paste "tot-${IR}0-$REF-$TAR.$INF_FTYPE" \
#        "tot-${IR}1-$REF-$TAR.$INF_FTYPE" \
#        | tr ',' '.' | awk '{for (i=1;i<=NF/2;i++) \
#        if($i==$i+0) {printf "%0.5f\t", $i-$(i+NF/2);} \
#        else if($i!=$i+0) {printf "%s\t", $i;} print ""}' \
#        > "diff-$REF-$TAR.$INF_FTYPE"
#
#    echo -e "\t$(cat "diff-$REF-$TAR.$INF_FTYPE")" \
#        > "diff-$REF-$TAR.$INF_FTYPE"
#done
#
#rm -f *HORIZ_PAD
#
#cd ..
