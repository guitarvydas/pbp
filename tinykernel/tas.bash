#!/bin/bash
set -e
python3 main.py . - "$1.rt" main kernel.drawio.json | node decodeoutput.mjs
if [ -f "out.✗" ]; then cat "out.✗"; exit 1; fi
if [ -f "out.md" ]; then cat "out.md"; fi
mv out.py _/_python/$1.py
mv out.js _/_js/$1.js
mv out.lisp _/_lisp/$1.lisp
mv out.c _/_c/$1.c
