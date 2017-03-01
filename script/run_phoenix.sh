#!/usr/bin/env bash


for ir in $INV_REPEATS; do
 for alphaDen in $ALPHA_DENS; do
  for refDataset in $REF_DATASET; do
   for tarDataset in $TAR_DATASET; do
##   rm -f $IR_LBL$ir-$a_LBL$alphaDen-${dataset}.$INF_FILE_TYPE
##   touch $IR_LBL$ir-$a_LBL$alphaDen-$dataset.$INF_FILE_TYPE
#   echo -e "mut\tmin_bpb\tmin_ctx" >> $IR_LBL$ir-$a_LBL$alphaDen-$dataset.$INF_FILE_TYPE
#    for mut in $MUT_LIST; do
#    rm -f $IR_LBL$ir-$a_LBL$alphaDen-${dataset}_$mut.$INF_FILE_TYPE
#    touch $IR_LBL$ir-$a_LBL$alphaDen-${dataset}_$mut.$INF_FILE_TYPE
#    echo -e "# ir\talpha\tctx\tbpb\ttime(s)" >> $IR_LBL$ir-$a_LBL$alphaDen-${dataset}_$mut.$INF_FILE_TYPE
     for ctx in $CTX; do
#     for((ctx=$MIN_CTX; ctx<=$MAX_CTX; ctx+=1)); do
#     ./phoenix -m $ir,$ctx,$alphaDen -r $FLD_dataset/$refDataset -t $FLD_dataset/$tarDataset \
#      ./phoenix -n 4 -m 0,13,10\
      ./phoenix -n 4 -m 0,1,1:0,2,1\
      -r x -t y \
#       -r dataset/HS21 -t dataset/PT21 \
#               > $IR_LBL$ir-$refDataset-$TAR_SPECIES.$INF_FILE_TYPE
     done
#    done
     ###== sort results based on target datasets, and add header line
#     cat $IR_LBL$ir-$refDataset-$TAR_SPECIES.$INF_FILE_TYPE  | sort -k 2 -V \
#      >> zzz
#     sed -i '1 i\ref\ttar\tir\talpha\tctx\tbpb\tNRC' zzz
#     mv zzz $IR_LBL$ir-$refDataset-$TAR_SPECIES.$INF_FILE_TYPE
   done

##    # save "min bpb" and "min ctx" for each dataset
#    minBpbCtx=$(awk 'NR==1 || $4 < minBpb {minBpb=$4; minCtx=$3}; \
#                END {print minBpb"\t"minCtx}' $IR_LBL$ir-$a_LBL$alphaDen-${dataset}_$mut.$INF_FILE_TYPE)
#    echo -e "$mut\t$minBpbCtx" >> $IR_LBL$ir-$a_LBL$alphaDen-$dataset.$INF_FILE_TYPE
#   done
  done
 done
done


####rm -fr $FLD_dat
#mkdir -p $FLD_dat
#mv ${IR_LBL}*.$INF_FILE_TYPE $FLD_dat
