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

main : function (topLevel,) {
enter_rule ("main");
    set_return (`[${topLevel.rwr ().join ('')}]`);
return exit_rule ("main");
},
topLevel_defn : function (x,) {
enter_rule ("topLevel_defn");
    set_return (`${x.rwr ()}`);
return exit_rule ("topLevel_defn");
},
topLevel_line : function (x,) {
enter_rule ("topLevel_line");
    set_return (`${x.rwr ()}`);
return exit_rule ("topLevel_line");
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
defvar : function (ws1,_2,ws2,id,ws3,_6,ws4,exp,ws5,) {
enter_rule ("defvar");
    set_return (`\n${ws2.rwr ()}${_2.rwr ()}${ws2.rwr ()}${id.rwr ()}${ws3.rwr ()}${_6.rwr ()}${ws4.rwr ()}${exp.rwr ()}${ws5.rwr ()}`);
return exit_rule ("defvar");
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
exp : function (cs,) {
enter_rule ("exp");
    set_return (`${cs.rwr ().join ('')}`);
return exit_rule ("exp");
},
expchar : function (c,) {
enter_rule ("expchar");
    set_return (`${c.rwr ()}`);
return exit_rule ("expchar");
},
spaces : function (cs,) {
enter_rule ("spaces");
    set_return (`${cs.rwr ().join ('')}`);
return exit_rule ("spaces");
},
_terminal: function () { return this.sourceString; },
_iter: function (...children) { return children.map(c => c.rwr ()); }
}
