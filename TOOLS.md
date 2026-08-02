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
First time initialization for maintainers. If you just want to use the tools, see `@setup-tools.sh` instead.

Keeps everything and runs the first-time initialization, including npm install.

The pbp directory contains `.git/` to allow it to be a stand-alone git repository
## @setup-tools.sh*
First time initialization for users of the tools. 

This is intended to be the main way to incorporate the tools into a new project, i.e. the default way to set up a new project that will use the tools.

It creates a `./pbp` subdirectory within the current directory (i.e. a new project directory) and copies certain files up into the current directory and gets rid of a lot of files that aren't needed for just using the tools, like the generator for kernel code, etc. 

The `.pbp/git/` subdirectory is deleted, so that it doesn't interfere with the git repository for the new project. The new project repository should be made to include the `./pbp/` directory (`git add pbp`) so that all changes to the tools (if any) are local to the new project. Trying to generalize the tools so that they work in every project and can be shared across projects results in lots of extra, unnecessary work.
## api.md
Documentation of the PBP api for Container and Leaf parts
## attic/
ignore - contains stuff that I couldn't imagine throwing away. 

Ideally, this directory should become empty and should not even exist.
## brace_indent*
Source code indentation (pretty-printing) tool that uses braces (`{` and `}`) instead of unicode symbols as in the `indent` tool.

Written in node.js, but works on source code for any language.

The default tool is `indent`. This tool (`brace_indent`) is for special cases.
## brace_indent_del*
Like `brace_indent` but also removes the braces.

This is for special use-cases. Use `indent` by default.
## check-for-html*
Checks for any left-over HTML (e.g. `span`) commands in the incoming source. Returns a shell error code if any left-overs are present.

Meant to be used in a pipeline to check source code generated from diagrams.

This is needed as a stop-gap measure. We're using drawio as a drawing editor for now, because we don't have a proper DaS (Diagrams as Syntax) code editor. Drawio will sometimes insert various bits of HTML into text if we forget to disable the `word wrap` and `formatted text` options in drawio. 

If you get an error thrown by this tool, disable the formatting options in drawio (on a per-figure basis) and try again.

The drawing transmogrifier uses text on the drawings as names of parts. HTML in part names will confuse the transmogrifier and typically cause part-not-found errors.

Some time in the future, we'll build a proper drawing source code editor, or, we'll find a way to disable all formatting in draw.io by default, but, not yet.
## checkfailure*
PBP drawware must produce the file `out.👍👎`.

If that file contains "fail" then it outputs the file `out.txt` (if it exists) and returns a shell error code to stop any further processing.

Again, this is a stop-gap measure to allow PBP drawware (such as transpilers and transmogrifiers) to work in conjunction with shell scripts and pipelines, "as is".
## das/
## das2json*
Takes one command-line arg - the name of a `<name>.drawio` file.

Converts the file to `<name>.drawio.json`, discarding graphics-only information (i.e. most of the info - noise). Emits each tab on the drawio diagram as a JSON object
- {
	- `"name": "..."`,
	- `"children": [ {"name": "...", "id" : NN}, {"name": "...", "id" : NN}, ... {"name": "...", "id" : NN} ]`
	- `"connections": [{ "dir": 1, "source": { "name": "...", "id": NN },"source_port": "...","target_port": "...", "target": { "name": "...", "id": NN "" }, ... }`,
	- `"file": "..."`
- }

where `children` is a JSON array of children template names and unique ids,
where `connections` is a JSON array of wires between one source and one target object
where `"dir"` is
- 0 for a down connection (from parent input port to child input port)
- 1 for an across connection (from child output port to child input port)
- 2 for an up connection (from child output port to parent output port)
- 3 for a through connection (from parent input port to parent output port)

For down connections, the source is not specified and is inferred to be "self". Only the source_port is specified.

For up connections, the target is not specified and is inferred to be "self". Only the target_port is specified.
## del_blank_lines*
A pipeline filter that deletes blank lines from the input. Used mainly for human-readability while debugging.
## doc/
Contains the document "semantics.pdf" and various files containing drawings included in that document.
## errgrep*
At present, semantics error messages are inserted by PBP transmogrifiers into intermediate code with the prefix `>>>`.

`Errgrep` is a pipeline filter that detects the presence of such error messages. If found, it displays the messages and halts further processing (by returning a shell error code).
## fanout*
A shell pipeline filter that sends `stdin` to `stdout` _AND_ to `stderr`.

Essentially a half-measure to cover for the fact that shell pipelines don't actually support reasonable fan-out.

Shell pipes do not provide fan-out. When any receiver consumes an output from a pipe, that item is gone and cannot also be delivered to other receivers.

Fan-out is essential to creating software architectures that are more interesting than those inspired by functional-only thinking.

This is done better in the PBP kernel. But, if you insist on using shell pipelines, this is one way to achieve actual fan-out.
## include*
Shell pipeline filter that inserts included files based on syntax such as:
```
    ⊕⟨xinterpret.frish⟩
    ⊕⟨../shared/macros.frish⟩
```

Note that the include prefix `⊕` and brackets `⟨` `⟩` are unicode characters.

The shell command `m4` can also be used to perform file includes, but `m4` also does many other kinds of things.

This simpler version of `include` was written in Python by Claude 4.
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
(see `output_to_port`)
## output_end*
(see `output_to_port`)
## output_to_port*
Attempt to simulate PBP-like mevent outputs to ports, represented in final form as a JSON array, using shell commands.

One uses `output_to_port ...name... < ...value...` during a run of a parts based program, where `...name...` is a string representing the output port name, and `...value...` is the payload as a string.

Multiple calls to `output_to_port` are allowed and are converted to ordered JSON key/value pairs, as is done by the PBP kernel.

The beginning and end of the run of a parts based program must be bracketed by
- `output_begin`
- `output_end`

You only need to use these if you are using shell commands and pipelines to simulate parts based programming. You can ignore all of this if you just use the PBP system and kernel.
## doc/pbp-lifecycle.drawio
source for `doc/pbp-lifecycle.drawio.png`
## doc/pbp-lifecycle.drawio.png
sketch of the PBP life cycle.
## pbplog*
Appends a log message to `pbplog.txt` in the current working directory (`${CALLERPATH}/pbplog.txt`)
## README.md
main README.md for the PBP repository
## resetlog*
Clears `pbplog.txt` in the current directory (`${CALLERPATH}/pbplog.txt`)
## rigid_indent*
DEPRECATED

Prepends the given command line argument to each line of `stdin`.
## splitoutputs*
Pipeline filter than unwinds PBP outputs from JSON key/value form into a separate, ordered file for each key.

The name of the files is `out` with a suffix that is the key, e.g. `out.py`.

Useful for writing transmogrifiers which create a single output for each target language, e.g. `out.py`, `out.js`, `out.lisp`, etc.
## t2t*
The main driver command for text-to-text transmogrification.

This needs a grammar specification and a rewrite specification. It inhales source text from `stdout` and produces transmogrified text on `stdout`.

When invoked with only one command line argument, the argument is assumed to be the name of the grammar and the rewrite files. `.ohm` and `.rwr` suffixes are appended to the name to specify the two required specifications.

When invoked with two command line arguments, each argument is a full pathname of a file. The first argument is the grammar file, the second argument is the rewrite file.

Example:
- `t2t xyz` is the same as `t2t xyz.ohm xyz.rwr`
- `t2t xyz.ohm pqrxyz.rwr` cannot be written using only one argument.

To create a T2T Part using a PBP drawing, use shell-out syntax for the part name, for example: `:$ t2t xyz`. (See the `make_leaf (Leaf or LJIT part)` syntax section in `README.md`)
## t2td/
### Tool Use
To simply use T2T, you only need to keep the subdirectory `t2td/lib/` which contains scripts and snippets of code necessary to build the RWR portion of T2T and to run T2T on your code.
### Ohm Grammar Specification
see `ohmjs.org`
### Rewrite Specification
see `t2td/doc/rwr/RWR Spec.pdf`
### Maintenance
[TBD] - the rest of the stuff in `t2td/` to be documented at a later date.
## tas/
ignore
## tas-bloated/
ignore
## TOOLS.md
this file
