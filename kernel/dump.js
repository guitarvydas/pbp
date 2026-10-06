const fs = require("fs");

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

readtypetable("typetable.jsn");
console.log (scopes);
