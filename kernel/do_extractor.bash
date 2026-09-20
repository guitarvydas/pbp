#!/bin/bash
echo
echo "Running extractor"
echo
node das2json.mjs ${EXTRACTOR_TARGET}.drawio

for i in $BASENAMES; do
    ./extract.bash "$i"
done
