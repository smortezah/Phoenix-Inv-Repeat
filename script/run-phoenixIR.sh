#!/bin/bash


for refNo in $REF_DATASET; do
    for a in $ALPHA_DENS; do
        for ir in $INV_REPS; do
#            for tarNo in $TAR_DATASET; do
   ##             rm -f $IR$ir-$AL$a-${dataset}.$INF_FTYPE
   ##             touch $IR$ir-$AL$a-$dataset.$INF_FTYPE
   #             echo -e "mut\tmin_bpb\tmin_ctx" \
   #              >> $IR$ir-$AL$a-$dataset.$INF_FTYPE
   #              for mut in $MUT_LIST; do
   #              rm -f $IR$ir-$AL$a-${dataset}_$mut.$INF_FTYPE
   #              touch $IR$ir-$AL$a-${dataset}_$mut.$INF_FTYPE
   #              echo -e "# ir\talpha\tctx\tbpb\ttime(s)" \
   #                 >> $IR$ir-$AL$a-${dataset}_$mut.$INF_FTYPE
                echo -e "ref($REF)\ttar($TAR)\tNRC\tir\tctx\talpha\ttime(s)" \
                    > $IR$ir-$REF$refNo-$TAR.$INF_FTYPE
                for ctx in $CTX; do
   #            for((ctx=$MIN_CTX; ctx<=$MAX_CTX; ctx+=1)); do
                   ./phoenix -n $N_THRD -m $ir,$ctx,$a \
                             -r $FLD_dataset/$REF/$refNo -t $MULTITAR_DATASET \
                    >> $IR$ir-$REF$refNo-$TAR.$INF_FTYPE
   #                ./phoenix -n 4 -d -m 0,1,100:0,2,10\
#                    ./phoenix -n 4 -d -m 0,1,100 \
#                    -r HS21 \
#                    -t n
   #                ./phoenix -n 4 -d -m 0,1,100\
   #                -r dataset/HS21
   #                -r HSMT
   #                -t dataset/PTY
#                            > $IR$ir-$refNo-$TAR.$INF_FTYPE
   #                tr -d '\n' < PTY > tmp; diff tmp DECOMP.de; rm -f tmp;
                done
#                done

                #%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                #   sort results based on target datasets
                #%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                cat $IR$ir-$REF$refNo-$TAR.$INF_FTYPE \
                    | (read -r; printf "%s\n" "$REPLY"; sort -k 2 -V) > zzz
                mv zzz $IR$ir-$REF$refNo-$TAR.$INF_FTYPE

   ##          # save "min bpb" and "min ctx" for each dataset
   #          minBpbCtx=$(awk 'NR==1 || $4 < minBpb {minBpb=$4; minCtx=$3}; \
   #                      END {print minBpb"\t"minCtx}' \
   #                          $IR$ir-$AL$a-${dataset}_$mut.$INF_FTYPE)
   #          echo -e "$mut\t$minBpbCtx" \
   #                               >> $IR$ir-$AL$a-$dataset.$INF_FTYPE
#            done   # end tarNo
        done    # end refNo
    done    # end a
done    # end ir


####rm -fr $FLD_dat
#mkdir -p $FLD_dat
#mv ${IR}*.$INF_FTYPE $FLD_dat
