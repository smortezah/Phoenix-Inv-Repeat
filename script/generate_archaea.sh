#!/usr/bin/env bash


$FLD_goose/src/goose-splitreads < "$FLD_archive_datasets/DB.mfa"

for i in {1..206}; do
 grep -v ">" out$i.$FILE_TYPE > $FLD_datasets/A$i;
done

rm -f out*.fa
