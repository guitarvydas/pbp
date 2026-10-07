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
    pushParameter ("scope", `_global`);
    set_return (`${TopLevel.rwr ().join ('')}`);
popParameter ("scope");
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
TopLevel_defvar : function (x,) {
enter_rule ("TopLevel_defvar");
    set_return (`${x.rwr ()}`);
return exit_rule ("TopLevel_defvar");
},
Defn : function (_defn_,id,_eq_,functiontype,Formals,FunctionBody,) {
enter_rule ("Defn");
    pushParameter ("scope", `${id.rwr ()}`);
    set_return (`\n{"op":"newscope", "operand":"${id.rwr ()}"}\n{"op":"insert", "operand":{"name":"${id.rwr ()}", ${functiontype.rwr ()}, "scope":"${genscoperest (`scope`,)}" }}${Formals.rwr ()}${FunctionBody.rwr ()}`);
popParameter ("scope");
return exit_rule ("Defn");
},
DefObj : function (_defobj_,id,ObjBody,) {
enter_rule ("DefObj");
    pushParameter ("scope", `${id.rwr ()}`);
    set_return (`\n{"op":"newscope", "operand":"${id.rwr ()}"}\n{"op":"insert", "operand":{"name":"${id.rwr ()}", "indir":1, "type":"obj", "scope":"${genscoperest (`scope`,)}", "kind":"obj"}}${ObjBody.rwr ()}`);
popParameter ("scope");
return exit_rule ("DefObj");
},
DefVar : function (_1,id,_3,ty,) {
enter_rule ("DefVar");
    set_return (`\n{"op":"insert", "operand":{"name":"${id.rwr ()}", ${ty.rwr ()}, "scope":"${genscope (`scope`,)}", "kind":"variable"}}`);
return exit_rule ("DefVar");
},
FunctionType_procedure : function (_tilde_,) {
enter_rule ("FunctionType_procedure");
    set_return (`"indir":0, "type":"void", "kind":"proc"`);
return exit_rule ("FunctionType_procedure");
},
FunctionType_returnvalue : function (ty,) {
enter_rule ("FunctionType_returnvalue");
    set_return (`${ty.rwr ()}, "kind":"retproc"`);
return exit_rule ("FunctionType_returnvalue");
},
Formals : function (_lp,typedvarcomma,_rp,) {
enter_rule ("Formals");
    set_return (`${typedvarcomma.rwr ()}`);
return exit_rule ("Formals");
},
TypedParamComma : function (typedID,_comma,typedparamcomma,) {
enter_rule ("TypedParamComma");
    set_return (`\n{"op":"insert", "operand":{${typedID.rwr ()},"kind":"parameter"}}${typedparamcomma.rwr ().join ('')}`);
return exit_rule ("TypedParamComma");
},
TypedID : function (id,_eq,Type,) {
enter_rule ("TypedID");
    set_return (`"name":"${id.rwr ()}", ${Type.rwr ()}, "scope":"${genscope (`scope`,)}"`);
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
    set_return (`\n{"op":"insert", "operand":{${typedid.rwr ()}, "kind":"variable"}}${rec.rwr ().join ('')}`);
return exit_rule ("BodyInnards_TypedVar");
},
BodyInnards_other : function (c,rec,) {
enter_rule ("BodyInnards_other");
    set_return (`${rec.rwr ().join ('')}`);
return exit_rule ("BodyInnards_other");
},
Type_pointer : function (_at,id,) {
enter_rule ("Type_pointer");
    set_return (`"indir":2, "type":"${id.rwr ()}"`);
return exit_rule ("Type_pointer");
},
Type_pointerany : function (_at,) {
enter_rule ("Type_pointerany");
    set_return (`"indir":2, "type":"any"`);
return exit_rule ("Type_pointerany");
},
Type_plain : function (id,) {
enter_rule ("Type_plain");
    set_return (`"indir":1, "type":"${id.rwr ()}"`);
return exit_rule ("Type_plain");
},
id : function (c,cs,) {
enter_rule ("id");
    set_return (`${c.rwr ()}${cs.rwr ().join ('')}`);
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
