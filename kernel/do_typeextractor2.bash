#!/bin/bash
echo
echo "Running extractor pass 2"
echo

for i in $BASENAMES; do
    ./extract2.bash "$i.tyty" >"$i.ty2"
done
