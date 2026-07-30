# List of Tools
This is a list of the stock PBP and T2T tools that you might want to use. These are all command line tools and scripts that can appear in shell scripts and shell pipelines.

The tools are: 
- `brace_indent` 
- `brace_indent_del` 
- `check-for-html` 
- `checkfailure` 
- `das2json` 
- `del_blank_lines`
- `errgrep`
- `fanout` 
- `include`
- `indent` 
- `output_begin`
- `output_end`
- `output_to_port`
- `pbplog`
- `resetlog`
- `rigid_indent`
- `splitoutputs`
- `t2t`. 

Scripts for building projects are: 
- `@make`
- `@defc`
- `@makec`
- `@testc`.

Scripts to initialize projects are: 
- `@setup-tools.sh
- `@set-maintain.sh`.

See below for descriptions.

I've included the various paths and subdirectores that are found in the `pbp/` directory, for reference

Filenames that have a `*` suffix are shell scripts.

Filenames that have a `/` suffix are subdirectories. Mostly ignorable. The full structure of this directory is set up to maintain the tools. The tool source code and extra fluff are deleted by the `@setup-tools.sh` script. If you want to maintain or tweak the tools, then you need to keep everything and just run the `@setup-maintain.sh` script (instead of running `@setup-tools.sh`).

## @make-proto/
### @make-proto/@make*
The generic project build script. 

This gets copied into your project directory when you run `@setup-tools.sh`. 

Use it by invoking `./@make`. It calls the custom scripts `@defc`, `@makec` and `@testc`. (See below)
### @make-proto/@defc*
Definitions (exported environment variables) that can be edited on a per-project basis.

In typical simple usage, only the last line `export TARGET="<customize this>"` needs to be edited to state the name of your project. For example in the `st` project the line reads `export TARGET="st"`

This gets copied into your project directory when you run `@setup-tools.sh`. 
### @make-proto/@makec*
This gets copied into your project directory when you run `@setup-tools.sh`. 

This is a prototype build script that can be edited on a per-project basis. You can choose 3 kinds of projects to build:
1. default: build a drawware program
2. build a transmogrifier
3. use T2T to transmogrify some text to some other text

The default is option 1. Use this when you create PBP source code using [drawio](drawio.com) called `${TARGET}.drawio`. This option converts the drawing to JSON, then runs the drawing using the (included) Python kernel. The top-most drawing is found on the drawio tab `main`. The PBP program is executed by first injecting "" into the default input pin (called, confusingly ""). This option is akin to building a shell script, except that it provides more powerful ways to interconnect Parts.

If you want to use one of the other options, comment option 1 out and uncomment the appropriate option.

Option 2 builds a transmogrifier ("transpiler") based on the diagram `"${TRANSMOGRIFIER}.drawio"`. Then, it runs the transmogrifier on another diagram called `"{TARGET}.drawio"`. This version can be "called" from other projects as tools for building those projects. 

An example would be the [dtree tool](https://github.com/guitarvydas/dtree) which converts simple decision tree drawings into code snippets. 

A calling project directory contains all of the code for the project, along with a decision tree diagram that it passes to the dtree tool for processing. The decision tree tool converts the decision tree diagram into a snippet of code and deposits the result in the calling project's directory. The calling project includes the generated snippet of code into its own code while building the the main project.

As an example, this process can be seen in the [frish](https://github.com/guitarvydas/frish) project. The frish project implements a forth-like interpreter using Python. For one part of the Python code, a decision tree is used to describe the flow of the Python code. This decision tree is transpiled into Python code and the generated code is included into the rest of the frish project code. The frish project uses a custom `@makec` script to invoke the dtree tool to generate a snippet of Python code, then uses the `m4` program to include generated code into the rest of its code base before building the whole project.

The `@makec` build script for `frish` is
```
#!/bin/bash
set -eE
export dtree_tool=${HOME}/projects/dtree

# create xinterpret.frish from xinterpret.drawio
${dtree_tool}/@make ${dtree_tool} "xinterpret" $(pwd)

# insert generated code (`xinterpret.frish`) into forthish.frish
${PBP}/include forthish.frish.inc | tr -d '\r' > forthish.frish

# transmogrify `forthish.frish` into `forthish.py`
${PBP}/das2json frish.drawio
export PBPSTEPPING="1"
python main.py forthish.frish main frish.drawio.json | ${PBP}/splitoutputs
mv out.1.py forthish.py
```


The line `${dtree_tool}/@make ${dtree_tool} "xinterpret" $(pwd)` invokes dtree's `@make` script with 3 command line arguments which causes the `@make` script act as a subordinate tool. The tool - dtree - ingests the file `xinterpret.drawio` and spits out `xinterpret.py` (both of these files are in the `frish` project directory). Then, the line `${PBP}/include forthish.frish.inc | tr -d '\r' > forthish.frish` includes the generated code into the rest of the `forthish.frish` code, which then is used to build the main project - which generates `forthish.py` - using the subsequent lines in the script.

When `./@make` is invoked with no command line arguments, the build happens in place and `./@defc`, `./@makec` and `./@testc` are invoked. I call this a "top level" build. This is meant for building and maintaining and testing the tool. This is akin to building a compiler. The input and output for testing the tool are specified by `@testc`.

When `./@make` is invoked with three command line arguments, it becomes a use of the tool where the input and output for using the tool are directed to be in another project's directory. The three arguments are:
1. callee's working dir
2. caller's working dir
3. basename (no suffix) of the caller's file to be processed

and these paths are exported as environment variables
1. ${TOOLWD}
2. ${CALLERPATH}
3. ${TARGET}.

This is akin to building and maintaining a compiler vs. using the compiler, except that instead of creating a single binary for the compiler tool, the tool is made up of several scripts and binaries residing in the tool's directory. The tool is a bundle of scripts and programs. This makes the process of building a tool easier - you don't need to figure out how to bundle everything into a single package.

This works because all paths are explicitly specified. Other systems, like Apple's bundles, Python's environment and npm's packages probably solve the same kind of problem. This version uses only standard shell scripts and exported environment variables.

### @make-proto/@testc*
This is a script that tests a tool using various test files that are typically found in the tool's directory (or subdirectories). The test files are not used at all when the tool is used by another project.

This gets copied into your project directory when you run `@setup-tools.sh`. 

## @setup-init.sh*
Don't use this script directly. It is invoked by `./@setup-tools.sh` and `./@setup-maintain.sh`.

## @setup-maintain.sh*
## @setup-tools.sh*
## api.md
## attic/
ignore
## brace_indent*

## brace_indent_del*
## check-for-html*
## checkfailure*
## das/
## das2json*
## del_blank_lines*
## doc/
## errgrep*
## fanout*
## include*
## indent*
Indenter for generated Python code. Input contains unicode symbols for indent `⤷` and outdent `⤶`, output is Python friendly indented code using 4 spaces as indentation.

Use this as the tail end of a pipeline to fixup generated meta-python code.

The assumption is that the T2T tool has been used with `.rwr` rewrite rules that embed indent and outdent characters in the right places.

A (complicated) example of the use of indent and outdent can be seen in `./kernel/emitPython.rwr`.

## kernel/
ignore - contains the source code for the PBP kernel along with generated kernels in Python, Javascript and Common Lisp

## kernel-self/
ignore - used when rebuilding the PBP kernel

## main.py
prototype `main.py` for use in PBP projects 
- you need to modify this for your specific needs for each specific project
- an example of a modified `main.py` can be found in the [pbp helloworld repository](https://github.com/guitarvydas/pbp-helloworldpy)
- "modifying" means adding a line or two of code - this is not hard to do (generalization is overkill that leads to unneccesary complexity, so I favour making small edits like this instead of over-generalizing project builds)

## output_begin*
## output_end*
## output_to_port*
## pbp-lifecycle.drawio
## pbp-lifecycle.drawio.png
## pbplog*
## README.md
## resetlog*
## rigid_indent*
## splitoutputs*
## t2t*
## t2td/
## tas/
ignore
## tas-bloated/
ignore
## TOOLS.md
