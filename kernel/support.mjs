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

function fetchscopeobject (scope) {
    // scope is a string of scope names with '/' separating the fields
    let scopelist = scope.split('/');
    let dict = {};
    for (const name of scopelist) {
	dict = scopes[name];
	if (!dict) {
	    throw `scope ${name} not found in ${scopes}`;
	}
    }
    return dict;
}

function rmub(s) {
    // remove unicode brackets around idents (if any)
    return s.replace("❲","").replace("❳","");
}
    
function readtypetable (fname) {
    const lines = rmub(fs.readFileSync(fname, "utf8")).split("\n");

    for (const line of lines) {
	if (line.trim() === "") continue;   // skip blank lines
	const obj = JSON.parse(line);
	if (obj.op === "newscope") {
	    scopes[obj.operand] = {};
	} else if (obj.op === "insert") {
	    let scope = fetchscopeobject(obj.operand.scope);
	    scope[obj.operand.name] = obj.operand;
	}
    }
}


function gettypeinfo() {
    readtypetable("typetable.jsn");
}


function genscope (pname) {
    return `${parameters [pname].join('/')}`;
}

function genscoperest (pname) {
    let rest = parameters [pname].slice(0, -1);
    return `${rest.join('/')}`;
}

function lookup (scope, ubid) {
    let sc = fetchscopeobject(scope);
    let id = rmub(ubid);
    let descriptor = sc[id];
    if (!descriptor) {
	throw `can't find "${id}" in "${scope}"`;
    }
    if (descriptor.lookup) {
	return lookup (rmub(descriptor.lookup.scope), rmub(descriptor.lookup.name));
    } else {
	return descriptor;
    }
}


function getdeclaration(n, scope, id) {
    pbplog (`getdeclaration(${n}, "${scope}", "${id}")`);
    let desc = lookup (rmub(scope), rmub(id));
    return `void* ${id}`;
}

function getmaybederef(deref, scope, id) {
    pbplog (`getmaybederef(${deref}, "${scope}", "${id}")`);
    let desc = lookup (rmub(scope), rmub(id));
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

