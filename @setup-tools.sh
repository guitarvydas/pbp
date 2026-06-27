#!/bin/bash
# git clone https://github.com/guitarvydas/pbp
./pbp/@setup-init.sh
cp ./pbp/kernel/kernel0d.py ./pbp
cp ./pbp/kernel/repl.py ./pbp
cp ./pbp/kernel/splitoutput.js ./pbp
rm -rf ./pbp/kernel
mkdir ./pbp/kernel
mv ./pbp/kernel0d.py ./pbp/kernel
mv ./pbp/repl.py ./pbp/kernel
mv ./pbp/splitoutput.js ./pbp/kernel

cp ./pbp/@make-proto/@make .
cp ./pbp/@make-proto/@defc .
cp ./pbp/@make-proto/@makec .
cp ./pbp/main.py .

# keep ./pbp/das
# keep ./pbp/t2td
# keep all of the scripts in ./pbp
rm -rf ./pbp/@make-proto
rm -rf ./pbp/tas*
rm -rf *.png
rm -rf ./pbp/.git
rm -rf ./pbp/attic
rm -rf ./pbp/doc
rm -rf ./pbp/kernel-self
rm ./pbp/api.md
rm ./pbp/README.md
rm pbp-lifecycle.*
