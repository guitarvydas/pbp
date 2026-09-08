#!/bin/bash
set -e
python3 regression_test_main.py . - "$1.rt" main kernel.drawio.json | node decodeoutput.mjs
if [ -f "out.md" ]; then cat "out.md"; fi
mv out.py "_r_python/$1.py"
mv out.js "_r_js/$1.js"
mv out.lisp "_r_lisp/$1.lisp"
mv out.c "_r_c/$1.c"
