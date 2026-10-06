const fs = require("fs");

const lines = fs.readFileSync("typetable.jsn", "utf8").split("\n");

let scopes = {"_global":{}};

for (const line of lines) {
    if (line.trim() === "") continue;   // skip blank lines
    const obj = JSON.parse(line);
    if (obj.op === "newscope") {
	scopes[obj.operand] = {};
    } else {
    }
}
console.log (scopes);
