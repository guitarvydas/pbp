#!/bin/bash
set -e
python3 regression_test_main.py . - "$1.rt" main kernel.drawio.json | node decodeoutput.mjs
cat out.md
mv out.py "$1.py"
mv out.js "$1.js"
mv out.lisp "$1.lisp"
