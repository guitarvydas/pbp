#!/bin/bash
set -e
python3 main.py . - "$1.rt" main kernel.drawio.json
#python3 main.py . - "$1.rt" main kernel.drawio.json | node decodeoutput.mjs
exit 0
cat out.md
mv out.py _python/$1.py
mv out.js _js/$1.js
mv out.lisp _lisp/$1.lisp
mv out.c _c/$1.c
