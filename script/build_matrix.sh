#!/usr/bin/env bash


cd $FLD_dat

for i in $TAR_SEQ_RUN; do
    printf "\t%s" "$TAR_SPECIES$i" >> "${TAR_SPECIES}_HORIZ_PAD";
done
echo >> "${TAR_SPECIES}_HORIZ_PAD"

for alphaDen in $ALPHA_DENS; do
    for i in $INV_REPEATS; do
        cat "${TAR_SPECIES}_HORIZ_PAD" \
           >> "tot-$IR_LBL$i-$REF_SPECIES-$TAR_SPECIES.$INF_FILE_TYPE"

           for c in $REF_SEQ_RUN; do
               awk -F "\t" '{print $7}' \
                "$IR_LBL$i-$REF_SPECIES$c-$TAR_SPECIES.$INF_FILE_TYPE" \
                | awk -v ref_ch=$REF_SPECIES$c 'NR == 1 {print ref_ch; next} {print}' \
                | tr '\n' '\t' | tr ',' '.' \
                >> "tot-$IR_LBL$i-$REF_SPECIES-$TAR_SPECIES.$INF_FILE_TYPE"
               echo >> "tot-$IR_LBL$i-$REF_SPECIES-$TAR_SPECIES.$INF_FILE_TYPE"
           done

        ### filter NRC values greater than 1
        awk 'NR>1 ' tot-$IR_LBL$i-$REF_SPECIES-$TAR_SPECIES.dat \
            | awk '{for (i=1;i<=NF;i++) if($i!=$i+0) printf "%s\t", $i; \
            else if($i==$i+0 && $i>1) printf "%.5f\t", 1; \
   	    	else if($i==$i+0 && $i<=1) printf "%.5f\t", $i; print ""}' > temp;
        cat "${TAR_SPECIES}_HORIZ_PAD" > "temp-tot"
        cat "temp" >> "temp-tot"
        rm -f "tot-$IR_LBL$i-$REF_SPECIES-$TAR_SPECIES.dat";
        mv "temp-tot" "tot-$IR_LBL$i-$REF_SPECIES-$TAR_SPECIES.dat";
        rm -f "temp" "temp-tot"
    done

#       | tr ',' '.' | awk 'NR == 1 {print; next} {print}' \
#       | awk '{for (i=1;i<=NF/2;i++) printf "%s\t", \
#       ($i==$i+0)?$i-$(i+NF/2):$i; print ""}' \
    paste "tot-${IR_LBL}0-$REF_SPECIES-$TAR_SPECIES.$INF_FILE_TYPE" \
        "tot-${IR_LBL}1-$REF_SPECIES-$TAR_SPECIES.$INF_FILE_TYPE" \
        | tr ',' '.' | awk '{for (i=1;i<=NF/2;i++) \
        if($i==$i+0) {printf "%0.5f\t", $i-$(i+NF/2);} \
        else if($i!=$i+0) {printf "%s\t", $i;} print ""}' \
        > "diff-$REF_SPECIES-$TAR_SPECIES.$INF_FILE_TYPE"

    echo -e "\t$(cat "diff-$REF_SPECIES-$TAR_SPECIES.$INF_FILE_TYPE")" \
        > "diff-$REF_SPECIES-$TAR_SPECIES.$INF_FILE_TYPE"
done

rm -f *HORIZ_PAD

cd ..
