#!/bin/bash
for i in $BASENAMES; do
    echo "./extract2.bash \"$i.tyty\" >\"$i.ty2\""
    ./extract2.bash "$i.tyty" >"$i.ty2"
done
