#!/bin/bash
set -e
python3 extractor_main.py . - "$1.rt" main ${TARGET}.drawio.json | node decodeoutput.mjs
if [ -f "out.✗" ]; then cat "out.✗"; exit 1; fi
if [ -f "out.md" ]; then cat "out.md"; fi
mv "out.ty" "$1.ty"
