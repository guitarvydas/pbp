#!/bin/bash
set -e
python3 regression_test_main.py . - "$1.rt" main kernel.drawio.json | node decodeoutput.mjs
cat out.md
mv out.py "_python/$1.py"
mv out.js "_js/$1.js"
mv out.lisp "_lisp/$1.lisp"
