#!/bin/bash
for i in $BASENAMES; do
    ./extract2.bash "$i.tyty" >"$i.ty2"
done
