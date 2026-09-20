#!/bin/bash
set -e
echo "1"
node das2json.mjs kernel.drawio
echo "2"
#python3 regression_test_main.py . - "$1.rt" main kernel.drawio.json | node decodeoutput.mjs
python3 regression_test_main.py . - "$1.rt" main kernel.drawio.json >output.json
ls -l output.json
echo "3"
if [ -f "out.md" ]; then cat "out.md"; fi
mv out.py "_/_r_python/$1.py"
mv out.js "_/_r_js/$1.js"
mv out.lisp "_/_r_lisp/$1.lisp"
mv out.c "_/_r_c/$1.c"
