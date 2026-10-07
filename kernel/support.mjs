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

/// type table based stuff
/// read in the type table, arranged by scope names
/// use the type table during code emission to generate code in a typed language (C in this case)

let scopes = {"_global":{}};
    
function readtypetable (fname) {
    const lines = fs.readFileSync(fname, "utf8").split("\n");

    for (const line of lines) {
	if (line.trim() === "") continue;   // skip blank lines
	const obj = JSON.parse(line);
	if (obj.op === "newscope") {
	    scopes[obj.operand] = {};
	} else if (obj.op === "insert") {
	    scopes[obj.operand.scope][obj.operand.name] = obj.operand;
	}
    }
}

function genscope (pname) {
    return `${parameters [pname].join('/')}`;
}

function genscoperest (pname) {
    let rest = parameters [pname].slice(1);
    return `${rest.join('/')}`;
}

function lookup (scope, id) {
    let descriptor = scopes[scope][id];
    if (null == descriptor) {
	throw `can't find ${id} in ${scope}`;
    }
    return descriptor;
}

function gettypeinfo() {
    readtypetable("typetable.jsn");
}


function getdeclaration(scope, id) {
    let desc = lookup (scope, id);
    return `void* ${id}`;
}

function getmaybederef(deref, scope, id) {
    let desc = lookup (scope, id);
    if (deref === "⊥") {
	return id;
    } else {
	return `(*${id})`;
    }
}

function pderef(s) {
    return `**deref=${s}**`;
}

function pbplog (s) {
    fs.appendFileSync('pbplog.txt', s + "\n");
}

