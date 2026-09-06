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
internalize {
  text = char+
  char =
    | nl -- nl
    | "“" stringchar* "”" -- string
    | "\"" stringchar* "\"" -- dqstring
    | "⌈" commentchar* "⌉" -- comment
    | "❲" wordchar+ "❳" -- word
    | id -- ident
    | ~"❲" ~"❳" any -- any

  stringchar =
    | "\\" "“" -- beginquote
    | "\\" "”" -- endquote
    | "\\" "\"" -- dqquote
    | "“" stringchar+ "”" -- rec
    | ~"“" ~"”" ~"\"" any -- other

  commentchar =
    | "⌈" commentchar+ "⌉" -- rec
    | ~"⌈" ~"⌉" any -- other

  wordchar =
    | "❲" wordchar+ "❳" -- rec
    | ~"❲" ~"❳" any -- other

  id = (letter | "_") (letter | digit | "_")*
  dq = "\""
  nl = "\n"
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

function part (s, i) {
    let lis = s.split ("⫶");
    let len = lis.length - 1
    let r = []
    let ix = Number (i);
    for (; ix < len ; ix += 3) {
	r.push (`${lis [ix]}`);
    }
    return `${r.join ('')}`;
}

function enspace (arr) {
    // create space-separated args for exec
    return arr;
    //return arr.join (" ");
}

function pycommentify (s) {
    return s;
    //return s.replaceAll (/\n/g, "\n#").replaceAll ("%0A", "%0A#");
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


let _rewrite = {

text : function (c,) {
enter_rule ("text");
    set_return (`¶${c.rwr ().join ('')}`);
return exit_rule ("text");
},
char_nl : function (s,) {
enter_rule ("char_nl");
    set_return (`⎩${getlineinc ()}⎭\n`);
return exit_rule ("char_nl");
},
char_string : function (lq,cs,rq,) {
enter_rule ("char_string");
    set_return (`“${encodews (`${cs.rwr ().join ('')}`,)}”`);
return exit_rule ("char_string");
},
char_dqstring : function (ldq,cs,rdq,) {
enter_rule ("char_dqstring");
    set_return (`“${encodews (`${cs.rwr ().join ('')}`,)}”`);
return exit_rule ("char_dqstring");
},
char_comment : function (lb,cs,rb,) {
enter_rule ("char_comment");
    set_return (`⌈${encodews (`${cs.rwr ().join ('')}`,)}⌉`);
return exit_rule ("char_comment");
},
char_word : function (lb,cs,rb,) {
enter_rule ("char_word");
    set_return (`❲${encodews (`${cs.rwr ().join ('')}`,)}❳`);
return exit_rule ("char_word");
},
char_ident : function (s,) {
enter_rule ("char_ident");
    set_return (`❲${encodews (`${s.rwr ()}`,)}❳`);
return exit_rule ("char_ident");
},
char_any : function (c,) {
enter_rule ("char_any");
    set_return (`${c.rwr ()}`);
return exit_rule ("char_any");
},
stringchar_rec : function (lb,cs,rb,) {
enter_rule ("stringchar_rec");
    set_return (`${lb.rwr ()}${cs.rwr ().join ('')}${rb.rwr ()}`);
return exit_rule ("stringchar_rec");
},
stringchar_beginquote : function (bslash,q,) {
enter_rule ("stringchar_beginquote");
    set_return (`%5C“`);
return exit_rule ("stringchar_beginquote");
},
stringchar_endquote : function (bslash,q,) {
enter_rule ("stringchar_endquote");
    set_return (`%5C”`);
return exit_rule ("stringchar_endquote");
},
stringchar_dqquote : function (bslash,q,) {
enter_rule ("stringchar_dqquote");
    set_return (`%5C”`);
return exit_rule ("stringchar_dqquote");
},
stringchar_other : function (c,) {
enter_rule ("stringchar_other");
    set_return (`${c.rwr ()}`);
return exit_rule ("stringchar_other");
},
commentchar_rec : function (lb,cs,rb,) {
enter_rule ("commentchar_rec");
    set_return (`${lb.rwr ()}${cs.rwr ().join ('')}${rb.rwr ()}`);
return exit_rule ("commentchar_rec");
},
commentchar_other : function (c,) {
enter_rule ("commentchar_other");
    set_return (`${c.rwr ()}`);
return exit_rule ("commentchar_other");
},
wordchar_rec : function (lb,cs,rb,) {
enter_rule ("wordchar_rec");
    set_return (`${lb.rwr ()}${cs.rwr ().join ('')}${rb.rwr ()}`);
return exit_rule ("wordchar_rec");
},
wordchar_other : function (c,) {
enter_rule ("wordchar_other");
    set_return (`${c.rwr ()}`);
return exit_rule ("wordchar_other");
},
id : function (firstc,morecs,) {
enter_rule ("id");
    set_return (`${firstc.rwr ()}${morecs.rwr ().join ('')}`);
return exit_rule ("id");
},
dq : function (c,) {
enter_rule ("dq");
    set_return (`%22`);
return exit_rule ("dq");
},
nl : function (c,) {
enter_rule ("nl");
    set_return (``);
return exit_rule ("nl");
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

