# Generate 0D Kernel

![back end diagram](kernel-Back-End.drawio.png)

- source code for the kernel is written in `.rt` format (similar to Python, but with braces instead of indentation)
- generates a kernel in Python, in Javascript and in Common Lisp
- source code is written in `kernel.drawio` (Containers) and `*.rt` (Leaves)

# Usage:

## command line
`./@make`
## regression test
`./@make` runs `./@testc` which re-builds kernel0d.py using the newly-built kernel0d.py. If the two `.py` kernels are the same, the regression version is copied to `kernel0d.py` after saving the previous version of `kernel0d.py` with  `cp kernel0d.py "_$(date +%Y%m%d_%H%M%S)_kernel0d.py"`.

If there are differences, an error message is displayed and the previous `kernel0d.py` is left in place. The intent being, that the new kernel code will be fixed and recompiled using another run of `./@make`.
# Notes

`:$ cmd arg1 arg2 ...` is a shell-out
`:?xyz` is a probe with name "xyz"
`:sss` is a string `sss`

I removed the special case code for handling shell-outs in the kernel and replaced it with a part. See [kernel_external.rt](https://github.com/guitarvydas/pbp-dev/blob/dev/kernel/kernel_external.rt) for implementation details

Instantiation of ":" parts is special-cased in 0d.rt which calls external_instantiate in external.rt.

---

An attempt to clean out this directory has been made, but there might still be some left-over files from earlier testing.

---

The Javascript and Common Lisp versions of the kernel haven't been tested lately (they were tested earlier on). Various `.lisp` support files for the kernel are included in this directory.
