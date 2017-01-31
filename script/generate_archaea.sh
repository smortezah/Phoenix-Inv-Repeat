#!/usr/bin/env bash


$FLD_goose/src/goose-splitreads < "$FLD_dataset/DB.mfa"

for i in {1..206}; do
 grep -v ">" out$i.$FILE_TYPE > $FLD_dataset/A$i;
done

rm -f out*.fa
