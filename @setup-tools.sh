#!/bin/bash
# git clone https://github.com/guitarvydas/pbp
./pbp/@setup-init.bash
cp ./pbp/kernel0d.py ./pbp
rm -rf ./pbp/kernel
mkdir ./pbp/kernel
mv ./pbp/kernel0d.py ./pbp/kernel

cp ./pbp/@make-proto/@make .
cp ./pbp/@make-proto/@defc .
cp ./pbp/@make-proto/@makec .
cp ./pbp/main.py .

rm -rf ./pbp/@make-proto
rm -rf ./pbp/das
rm -rf ./pbp/tas*
rm -rf *.png
rm -rf ./pbp/.git
rm -rf ./pbp/attic
rm -rf ./pbp/doc
rm -rf ./pbp/kernel-self
rm ./pbp/api.md
