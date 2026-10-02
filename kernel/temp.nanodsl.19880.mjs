'use strict'

import * as ohm from 'ohm-js';
import * as process from 'process';

const verbose = !!process.env.T2TVERBOSE;

function top (stack) { let v = stack.pop (); stack.push (v); return v; }

function set_top (stack, v) { stack.pop (); stack.push (v); return v; }

let return_value_stack = [];
let rule_name_stack = [];
let depth_prefix = ' ';

function enter_rule (name) {
    if (verbose) {
	pbplog (`${depth_prefix}enter ${name}`);
	depth_prefix += ' ';
    }
    return_value_stack.push ("");
    rule_name_stack.push (name);
}

function set_return (v) {
    set_top (return_value_stack, v);
}

function exit_rule (name) {
    if (verbose) {
	depth_prefix = depth_prefix.substr (1);
	pbplog (`${depth_prefix}exit ${name}`);
    }
    rule_name_stack.pop ();
    return return_value_stack.pop ()
}

const grammar = String.raw`
typeExtractor2 {
  Main = TopLevel+
  TopLevel =
    | Defn -- defn
    | DefObj -- obj
    | Defvar -- defvar
    | Line -- line
  Defn = "defn"  id "≡" FunctionType Formals FunctionBody
  DefObj = "defobj" id ObjBody
  Defvar = "defvar" id "⇐" Exp line?
  FunctionType =
    | "~" -- procedure
    | Type -- returnvalue
  Formals = "(" TypedParamComma ")"
  TypedParamComma = TypedID ","? TypedParamComma?
  TypedID = id "≡" Type
  ObjBody = FunctionBody
  FunctionBody = "{" BodyInnards? "}"
  BodyInnards =
    | "{" BodyInnards? "}" BodyInnards? -- brace
    | "(" BodyInnards? ")" BodyInnards? -- parenthesis
    | "[" BodyInnards? "]" BodyInnards? -- bracket
    | TypedID BodyInnards?            -- TypedVar
    | ~"}" ~")" ~"]" any  BodyInnards? -- other
  Type =
    | "@" id -- pointer
    | id     -- plain
  id = (alnum | "_")+
  Line = "#line" digit+
}
`;

let args = {};
function resetArgs () {
    args = {};
}
function memoArg (name, accessorString) {
    args [name] = accessorString;
};
function fetchArg (name) {
    return args [name];
}

function encodews (s) { return encodequotes (encodeURIComponent (s)); }

function encodequotes (s) { 
    let rs = s.replace (/"/g, '%22').replace (/'/g, '%27');
    return rs;
}

let linenumber = 0;
function getlineinc () {
    linenumber += 1;
    return `${linenumber}`;
}

function enspace (arr) {
    // create space-separated args for exec
    return arr;
    //return arr.join (" ");
}

// In Javascript:
// s is a string containing a two-level list.
// The top level items are separated by "⫶".
// Each inner item contains sub-items separated by "◦".
// The top level list always contains a trailing "⫶", resulting in an empty final top level item.
// example: s = "aaa◦bbb⫶ccc◦ddd⫶"
// Function `first(s)` .joins('') every first sub-item of every inner item.
// Function `second(s)` .joins('') every second sub-item of every inner item.
function first(s) {
  return s.split('⫶').slice(0, -1).map(item => item.split('◦')[0]).join('');
}

function second(s) {
  return s.split('⫶').slice(0, -1).map(item => item.split('◦')[1]).join('');
}

let parameters = {};
function pushParameter (name, v) {
    if (!parameters [name]) {
        parameters [name] = [];
    }
    parameters [name].push (v);
}
function popParameter (name) {
    parameters [name].pop ();
}
function getParameter (name) {
    let top = parameters [name].pop ();
    parameters [name].push (top);
    return top;
}

parameters ["scope"] = [];

let _rewrite = {

Main : function (TopLevel,) {
enter_rule ("Main");
    set_return (`[${TopLevel.rwr ().join ('')}]`);
return exit_rule ("Main");
},
TopLevel_defn : function (x,) {
enter_rule ("TopLevel_defn");
    set_return (`${x.rwr ()}`);
return exit_rule ("TopLevel_defn");
},
TopLevel_line : function (x,) {
enter_rule ("TopLevel_line");
    set_return (`${x.rwr ()}`);
return exit_rule ("TopLevel_line");
},
Defn : function (_defn_,id,_eq_,functiontype,Formals,FunctionBody,) {
enter_rule ("Defn");
    pushParameter ("scope", `${id.rwr ()}`);
    set_return (`\n{"name":"${id.rwr ()}", ${functiontype.rwr ()}, "scope:"_global", "kind":"function"},${Formals.rwr ()}${FunctionBody.rwr ()}`);
popParameter ("scope");
return exit_rule ("Defn");
},
DefObj : function (_defobj_,id,ObjBody,) {
enter_rule ("DefObj");
    pushParameter ("scope", `${id.rwr ()}`);
    set_return (`\n{"name":"${id.rwr ()}", "pointer":false, "type":"obj", "scope:"_global", "kind":"obj"},${ObjBody.rwr ()}`);
popParameter ("scope");
return exit_rule ("DefObj");
},
FunctionType_procedure : function (_tilde_,) {
enter_rule ("FunctionType_procedure");
    set_return (`"pointer":false, "type":"void"`);
return exit_rule ("FunctionType_procedure");
},
FunctionType_returnvalue : function (ty,) {
enter_rule ("FunctionType_returnvalue");
    set_return (`${ty.rwr ()}`);
return exit_rule ("FunctionType_returnvalue");
},
Formals : function (_lp,typedvarcomma,_rp,) {
enter_rule ("Formals");
    set_return (`${typedvarcomma.rwr ()}`);
return exit_rule ("Formals");
},
TypedParamComma : function (typedID,_comma,typedparamcomma,) {
enter_rule ("TypedParamComma");
    set_return (`\n{${typedID.rwr ()},"kind":"parameter"},${typedparamcomma.rwr ().join ('')}`);
return exit_rule ("TypedParamComma");
},
TypedID : function (id,_eq,Type,) {
enter_rule ("TypedID");
    set_return (`"name":"${id.rwr ()}", ${Type.rwr ()}, "scope":"${getParameter ("scope")}"`);
return exit_rule ("TypedID");
},
FunctionBody : function (_lb,BodyInnards,_rb,) {
enter_rule ("FunctionBody");
    set_return (`${BodyInnards.rwr ().join ('')}`);
return exit_rule ("FunctionBody");
},
BodyInnards_brace : function (_l,BodyInnards,_r,rec,) {
enter_rule ("BodyInnards_brace");
    set_return (`${BodyInnards.rwr ().join ('')}${rec.rwr ().join ('')}`);
return exit_rule ("BodyInnards_brace");
},
BodyInnards_parenthesis : function (_l,BodyInnards,_r,rec,) {
enter_rule ("BodyInnards_parenthesis");
    set_return (`${BodyInnards.rwr ().join ('')}${rec.rwr ().join ('')}`);
return exit_rule ("BodyInnards_parenthesis");
},
BodyInnards_bracket : function (_l,BodyInnards,_r,rec,) {
enter_rule ("BodyInnards_bracket");
    set_return (`${BodyInnards.rwr ().join ('')}${rec.rwr ().join ('')}`);
return exit_rule ("BodyInnards_bracket");
},
BodyInnards_TypedVar : function (typedid,rec,) {
enter_rule ("BodyInnards_TypedVar");
    set_return (`\n{${typedid.rwr ()}, "kind":"variable"},${rec.rwr ().join ('')}`);
return exit_rule ("BodyInnards_TypedVar");
},
BodyInnards_other : function (c,rec,) {
enter_rule ("BodyInnards_other");
    set_return (`${rec.rwr ().join ('')}`);
return exit_rule ("BodyInnards_other");
},
Type_pointer : function (_at,id,) {
enter_rule ("Type_pointer");
    set_return (`"pointer":true, "type":"${id.rwr ()}"`);
return exit_rule ("Type_pointer");
},
Type_plain : function (id,) {
enter_rule ("Type_plain");
    set_return (`"pointer":false", type:"${id.rwr ()}"`);
return exit_rule ("Type_plain");
},
id : function (cs,) {
enter_rule ("id");
    set_return (`${cs.rwr ().join ('')}`);
return exit_rule ("id");
},
Line : function (_line,digit,) {
enter_rule ("Line");
    set_return (``);
return exit_rule ("Line");
},
_terminal: function () { return this.sourceString; },
_iter: function (...children) { return children.map(c => c.rwr ()); }
}
import * as fs from 'fs';

let terminated = false;

function xbreak () {
    terminated = true;
    return '';
}

function xcontinue () {
    terminated = false;
    return '';
}
    
function is_terminated () {
    return terminated;
}
function expand (src, parser) {
    let cst = parser.match (src);
    if (cst.failed ()) {
	//th  row Error (`${cst.message}\ngrammar=${grammarname (grammar)}\nsrc=\n${src}`);
	throw Error (cst.message);
    }
    let sem = parser.createSemantics ();
    sem.addOperation ('rwr', _rewrite);
    return sem (cst).rwr ();
}

function grammarname (s) {
    let n = s.search (/{/);
    return s.substr (0, n).replaceAll (/\n/g,'').trim ();
}

try {
    const argv = process.argv.slice(2);
    let srcFilename = argv[0];
    if ('-' == srcFilename) { srcFilename = 0 }
    let src = fs.readFileSync(srcFilename, 'utf-8');
    try {
	let parser = ohm.grammar (grammar);
	let s = src;
	xcontinue ();
	while (! is_terminated ()) {
	    xbreak ();
	    s = expand (s, parser);
	}
	console.log (s);
	process.exit (0);
    } catch (e) {
	//console.error (`${e}\nargv=${argv}\ngrammar=${grammarname (grammar)}\src=\n${src}`);
	console.error (`${e}\n\ngrammar = "${grammarname (grammar)}\n"`);
	process.exit (1);
    }
} catch (e) {
    console.error (`${e}\n\ngrammar = "${grammarname (grammar)}"\n`);
    process.exit (1);
}

