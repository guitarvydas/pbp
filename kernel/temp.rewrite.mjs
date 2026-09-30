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
    set_return (`[${TopLevel.rwr ().join ('')}\n]`);
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
    set_return (`\n{"name":"${id.rwr ()}", "type":"${functiontype.rwr ()}", "scope:"_global", "kind":"function"},`);
popParameter ("scope");
return exit_rule ("Defn");
},
FunctionType_procedure : function (_tilde_,) {
enter_rule ("FunctionType_procedure");
    set_return (`void`);
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
    set_return (`"name":"${id.rwr ()}", "type":"${Type.rwr ()}", "scope":"${getParameter ("scope")}"`);
return exit_rule ("TypedID");
},
FunctionBody : function (_lb,BodyInnards,_rb,) {
enter_rule ("FunctionBody");
    set_return (`\n${BodyInnards.rwr ().join ('')}`);
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
    set_return (`\nvar ${typedid.rwr ()}${rec.rwr ().join ('')}`);
return exit_rule ("BodyInnards_TypedVar");
},
BodyInnards_other : function (c,rec,) {
enter_rule ("BodyInnards_other");
    set_return (`${rec.rwr ().join ('')}`);
return exit_rule ("BodyInnards_other");
},
Type : function (_at,id,) {
enter_rule ("Type");
    set_return (`${_at.rwr ().join ('')}${id.rwr ()}`);
return exit_rule ("Type");
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
