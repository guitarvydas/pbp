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
semantics {

  Main = ParaMark TopLevel+
  TopLevel =
    | Defvar -- defvar
    | Defconst -- defconst
    | Defn -- defn
    | Defobj -- defobj
    | Import -- import
    | External -- external
    | comment line? -- comment
    | line -- line

   Defvar = kw<"defvar"> Lval "⇐" Exp line?
   Defconst = kw<"defconst"> Lval "≡" Exp line?
   Defn = kw<"defn"> ident Formals StatementBlock line?
   Defobj = kw<"defobj"> ident ObjFormals line? "{" line? InitStatement+ "}" line?
   Import = kw<"import"> ident line?

   StatementBlock = line? "{" line? Rec_Statement line? "}" line?

   Rec_Statement = line? R_Statement line?
   R_Statement =
     | comment Rec_Statement? -- comment
     | External Rec_Statement? -- external
     | Deftemp -- deftemp
     | Defsynonym -- defsynonym     
     | kw<"global"> ident CommaIdent* Rec_Statement? -- globals
     | IfStatement  -- if
     | kw<"pass"> Rec_Statement? -- pass
     | kw<"return"> ReturnExp -- return
     | ForStatement -- for
     | WhileStatement  -- while
     | Assignment -- assignment
     | Lval Rec_Statement? -- call
     | line Rec_Statement? -- line
   CommaIdent = Comma ident

   External = "#" ident "(" ExpComma* ")" line?

   Deftemp = kw<"deftemp"> Lval "⇐" Exp Rec_Statement?
   Defsynonym =
     | ident "≡" Exp Rec_Statement? -- legal
     | Lval "≡" Exp Rec_Statement? -- illegal

   InitStatement = "•" ident "⇐" Exp (comment | line)*

   IfStatement = kw<"if"> Exp StatementBlock ElifStatement* ElseStatement? Rec_Statement?
   ElifStatement = kw<"elif"> Exp StatementBlock
   ElseStatement = kw<"else"> StatementBlock

   ForStatement = kw<"for"> ident kw<"in"> Exp StatementBlock Rec_Statement?
   WhileStatement = kw<"while"> Exp StatementBlock Rec_Statement?

   Assignment = 
     | "[" LvalComma+ "]" "⇐" Exp Rec_Statement? -- multiple
     | Lval "⇐" Exp Rec_Statement? -- single

   LvalComma = Lval Comma?

    ReturnExp =
      | "[" ExpComma+ "]" Rec_Statement? -- multiple
      | Exp Rec_Statement? -- single

    ExpComma = Exp Comma?
    
    Exp =  BooleanAndOrIn

    BooleanAndOrIn =
      | BooleanAndOrIn andOrIn BooleanExp -- andOrIn
      | BooleanExp -- default
      
    BooleanExp =
      | BooleanExp boolNeq BooleanNot -- boolopneq
      | BooleanExp boolOp BooleanNot -- boolop
      | BooleanNot -- basic

    BooleanNot =
      | kw<"not"> BooleanExp -- not
      | AddExp -- basic

    AddExp =
      | AddExp "+" MulExp  -- plus
      | AddExp "-" MulExp  -- minus
      | MulExp -- basic

    MulExp =
      | MulExp "*" ExpExp  -- times
      | MulExp "/" ExpExp  -- divide
      | ExpExp -- basic

    ExpExp =
      | Primary "^" ExpExp  -- power
      | Primary -- basic

    Primary =
      | Primary "@" ident -- lookupident
      | Primary "@" Primary -- lookup
      | Primary "." ident -- fieldident
      | Primary "." Primary -- field
      | Primary "[" Exp "]" -- index
      | Primary "[" digit+ ":" "]" -- nthslice
      | ident Actuals -- identcall
      | Primary Actuals -- call
      | Atom -- atom

    Atom =
      | "[" "]" -- emptylistconst
      | "{" "}" -- emptydict
      | "(" Exp ")" -- paren
      | "[" line? PrimaryComma+ line? "]" -- listconst
      | "{" line? PairComma+ line? "}" -- dict
      | lambda LambdaFormals? ":" Exp -- lambda
      | phi -- phi
      | "⊤" -- true
      | "⊥" -- false
      | "↪︎" ident -- subr
      | External -- external
      | kw<"range"> "(" Exp ")" -- range
      | string -- string
      | number -- number
      | ident -- ident


    PrimaryComma = Primary Comma?
    PairComma = Pair Comma?
    
    Lval = Exp

    Formals =
      | "(" ")" -- noformals
      | "(" FormalComma* ")" -- withformals
    ObjFormals =
      | "(" ")" -- noformals
      | "(" FormalComma* ")" -- withformals
    LambdaFormals =
      | "(" ")" -- noformals
      | "(" FormalComma* ")" -- withformals

    Formal = ident
       
    FormalComma = Formal Comma?
    
    Actuals = 
      | "(" ")" -- noactuals
      | "(" ActualComma* ")" line? -- actuals

   Actual = ParamName? Exp
   ActualComma = comment? Actual Comma? line?

   ParamName = ident "∷"

    number =
      | digit* "." digit+  -- fract
      | digit+             -- whole

    Pair = string ":" Exp Comma?
  

  andOrIn = (kw<"and"> | kw<"or"> | kw<"in">)
  boolOp = (boolEq | boolNeq | "<=" | ">=" | ">" | "<")
  boolEq = "="
  boolNeq = "!="

  string = "“" stringchar* "”"
  stringchar = 
    | "“" stringchar* "”" -- rec
    | ~"“" ~"”" any -- other

    keyword = (
        kw<"defconst">
      | kw<"deftemp">
      | kw<"defobj">
      | kw<"defvar">
      | kw<"defn">
      | kw<"useglobal">
      | kw<"pass">
      | kw<"return">
      | kw<"if">
      | kw<"elif">
      | kw<"else">
      | kw<"and">
      | kw<"or">
      | kw<"in">
      | kw<"not">
      | kw<"range">
      | kw<"while">
      | kw<"import">
      | kw<"as">
      | lambda
      | phi
      )
      
  lambda = ("λ" | kw<"%CE%BB">)
  phi = ("ϕ" | kw<"%CF%95">)

  kw<s> = "❲" s "❳"
  ident  = ~keyword "❲" idchar+ "❳"
  idchar =
    | "❲" idchar+ "❳" -- rec
    | ~"❲" ~"❳" any -- other

  comment = "⌈" commentchar* "⌉"
  commentchar = 
    | "⌈" commentchar* "⌉" -- rec
    | ~"⌈" ~"⌉" any -- other

  errorMessage = "‽" errorchar* "⸘"
  errorchar = 
    | "‽" errorchar* "⸘" -- rec
    | ~"‽" ~"⸘" any -- other

  eh = ident
  fname = ident
  msg = ident
  ok = port
  err = port
  port = string
  
  line = "⎩" (~"⎩" ~"⎭" any)* "⎭"

  Comma = line? "," line?
  ParaMark = "¶"
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

Main : function (Para,TopLevel,) {
enter_rule ("Main");
    set_return (`${Para.rwr ()}${TopLevel.rwr ().join ('')}`);
return exit_rule ("Main");
},
TopLevel_defvar : function (Defvar,) {
enter_rule ("TopLevel_defvar");
    set_return (`${Defvar.rwr ()}`);
return exit_rule ("TopLevel_defvar");
},
TopLevel_defconst : function (Defconst,) {
enter_rule ("TopLevel_defconst");
    set_return (`${Defconst.rwr ()}`);
return exit_rule ("TopLevel_defconst");
},
TopLevel_defn : function (Defn,) {
enter_rule ("TopLevel_defn");
    set_return (`${Defn.rwr ()}`);
return exit_rule ("TopLevel_defn");
},
TopLevel_defobj : function (Defobj,) {
enter_rule ("TopLevel_defobj");
    set_return (`${Defobj.rwr ()}`);
return exit_rule ("TopLevel_defobj");
},
TopLevel_import : function (Import,) {
enter_rule ("TopLevel_import");
    set_return (`${Import.rwr ()}`);
return exit_rule ("TopLevel_import");
},
TopLevel_external : function (x,) {
enter_rule ("TopLevel_external");
    set_return (`${x.rwr ()}`);
return exit_rule ("TopLevel_external");
},
TopLevel_comment : function (comment,line,) {
enter_rule ("TopLevel_comment");
    set_return (`${comment.rwr ()}${line.rwr ().join ('')}\n`);
return exit_rule ("TopLevel_comment");
},
TopLevel_line : function (line,) {
enter_rule ("TopLevel_line");
    set_return (`${line.rwr ()}`);
return exit_rule ("TopLevel_line");
},
Defvar : function (_2,Lval,_3,Exp,line,) {
enter_rule ("Defvar");
    set_return (`${_2.rwr ()}${Lval.rwr ()}${_3.rwr ()}${Exp.rwr ()}${line.rwr ().join ('')}`);
return exit_rule ("Defvar");
},
Defconst : function (_5,Lval,_6,Exp,line,) {
enter_rule ("Defconst");
    set_return (`${_5.rwr ()}${Lval.rwr ()}${_6.rwr ()}${Exp.rwr ()}${line.rwr ().join ('')}`);
return exit_rule ("Defconst");
},
Defn : function (_8,ident,Formals,StatementBlock,line,) {
enter_rule ("Defn");
    set_return (`${_8.rwr ()}${ident.rwr ()}${Formals.rwr ()}${StatementBlock.rwr ()}${line.rwr ().join ('')}`);
return exit_rule ("Defn");
},
Defobj : function (_10,ident,ObjFormals,line1,_11,line2,InitStatement,_12,line3,) {
enter_rule ("Defobj");
    set_return (`${_10.rwr ()}${ident.rwr ()}${ObjFormals.rwr ()}${line1.rwr ().join ('')}${_11.rwr ()}${line2.rwr ().join ('')}${InitStatement.rwr ().join ('')}${_12.rwr ()}${line3.rwr ().join ('')}`);
return exit_rule ("Defobj");
},
Import : function (_14,ident,line,) {
enter_rule ("Import");
    set_return (`${_14.rwr ()}${ident.rwr ()}${line.rwr ().join ('')}`);
return exit_rule ("Import");
},
StatementBlock : function (line1,lb,line2,Rec_Statement,line3,rb,line4,) {
enter_rule ("StatementBlock");
    set_return (`${line1.rwr ().join ('')}${lb.rwr ()}${line2.rwr ().join ('')}${Rec_Statement.rwr ()}${line3.rwr ().join ('')}${rb.rwr ()}${line4.rwr ().join ('')}`);
return exit_rule ("StatementBlock");
},
Rec_Statement : function (line1,R_Statement,line2,) {
enter_rule ("Rec_Statement");
    set_return (`${line1.rwr ().join ('')}${R_Statement.rwr ()}${line2.rwr ().join ('')}`);
return exit_rule ("Rec_Statement");
},
R_Statement_comment : function (comment,Rec_Statement,) {
enter_rule ("R_Statement_comment");
    set_return (`${comment.rwr ()}\n${Rec_Statement.rwr ().join ('')}`);
return exit_rule ("R_Statement_comment");
},
R_Statement_external : function (x,rec,) {
enter_rule ("R_Statement_external");
    set_return (`${x.rwr ()}${rec.rwr ().join ('')}`);
return exit_rule ("R_Statement_external");
},
R_Statement_deftemp : function (Deftemp,) {
enter_rule ("R_Statement_deftemp");
    set_return (`${Deftemp.rwr ()}`);
return exit_rule ("R_Statement_deftemp");
},
R_Statement_defsynonym : function (Defsynonym,) {
enter_rule ("R_Statement_defsynonym");
    set_return (`${Defsynonym.rwr ()}`);
return exit_rule ("R_Statement_defsynonym");
},
R_Statement_globals : function (_18,ident,CommaIdent,Rec_Statement,) {
enter_rule ("R_Statement_globals");
    set_return (`${_18.rwr ()}${ident.rwr ()}${CommaIdent.rwr ().join ('')}${Rec_Statement.rwr ().join ('')}`);
return exit_rule ("R_Statement_globals");
},
R_Statement_if : function (IfStatement,) {
enter_rule ("R_Statement_if");
    set_return (`${IfStatement.rwr ()}`);
return exit_rule ("R_Statement_if");
},
R_Statement_pass : function (_20,Rec_Statement,) {
enter_rule ("R_Statement_pass");
    set_return (`${_20.rwr ()}${Rec_Statement.rwr ().join ('')}`);
return exit_rule ("R_Statement_pass");
},
R_Statement_return : function (_22,ReturnExp,) {
enter_rule ("R_Statement_return");
    set_return (`${_22.rwr ()}${ReturnExp.rwr ()}`);
return exit_rule ("R_Statement_return");
},
R_Statement_for : function (ForStatement,) {
enter_rule ("R_Statement_for");
    set_return (`${ForStatement.rwr ()}`);
return exit_rule ("R_Statement_for");
},
R_Statement_while : function (WhileStatement,) {
enter_rule ("R_Statement_while");
    set_return (`${WhileStatement.rwr ()}`);
return exit_rule ("R_Statement_while");
},
R_Statement_assignment : function (Assignment,) {
enter_rule ("R_Statement_assignment");
    set_return (`${Assignment.rwr ()}`);
return exit_rule ("R_Statement_assignment");
},
R_Statement_call : function (Lval,Rec_Statement,) {
enter_rule ("R_Statement_call");
    set_return (`${Lval.rwr ()}${Rec_Statement.rwr ().join ('')}`);
return exit_rule ("R_Statement_call");
},
R_Statement_line : function (line,rec,) {
enter_rule ("R_Statement_line");
    set_return (`${line.rwr ()}${rec.rwr ().join ('')}`);
return exit_rule ("R_Statement_line");
},
CommaIdent : function (_23,ident,) {
enter_rule ("CommaIdent");
    set_return (`${_23.rwr ()}${ident.rwr ()}`);
return exit_rule ("CommaIdent");
},
External : function (_octothorpe,id,lpar,expcomma,rpar,line,) {
enter_rule ("External");
    set_return (`${_octothorpe.rwr ()}${id.rwr ()}${lpar.rwr ()}${expcomma.rwr ().join ('')}${rpar.rwr ()}${line.rwr ().join ('')}`);
return exit_rule ("External");
},
Deftemp : function (_37,Lval,_38,Exp,Rec_Statement,) {
enter_rule ("Deftemp");
    set_return (`${_37.rwr ()}${Lval.rwr ()}${_38.rwr ()}${Exp.rwr ()}${Rec_Statement.rwr ().join ('')}`);
return exit_rule ("Deftemp");
},
Defsynonym_legal : function (id,_39,Exp,Rec_Statement,) {
enter_rule ("Defsynonym_legal");
    set_return (`${id.rwr ()}${_39.rwr ()}${Exp.rwr ()}${Rec_Statement.rwr ().join ('')}`);
return exit_rule ("Defsynonym_legal");
},
Defsynonym_illegal : function (e,_39,Exp,Rec_Statement,) {
enter_rule ("Defsynonym_illegal");
    set_return (`${e.rwr ()} ⎝ error - LHS must be a single identifier for defsynonym ⎠ ${_39.rwr ()}${Exp.rwr ()}${Rec_Statement.rwr ().join ('')} `);
return exit_rule ("Defsynonym_illegal");
},
InitStatement : function (_40,ident,_41,Exp,fluff,) {
enter_rule ("InitStatement");
    set_return (`${_40.rwr ()}${ident.rwr ()}${_41.rwr ()}${Exp.rwr ()}${fluff.rwr ().join ('')}`);
return exit_rule ("InitStatement");
},
IfStatement : function (_43,Exp,StatementBlock,ElifStatement,ElseStatement,Rec_Statement,) {
enter_rule ("IfStatement");
    set_return (`${_43.rwr ()}${Exp.rwr ()}${StatementBlock.rwr ()}${ElifStatement.rwr ().join ('')}${ElseStatement.rwr ().join ('')}${Rec_Statement.rwr ().join ('')}`);
return exit_rule ("IfStatement");
},
ElifStatement : function (_45,Exp,StatementBlock,) {
enter_rule ("ElifStatement");
    set_return (`${_45.rwr ()}${Exp.rwr ()}${StatementBlock.rwr ()}`);
return exit_rule ("ElifStatement");
},
ElseStatement : function (_47,StatementBlock,) {
enter_rule ("ElseStatement");
    set_return (`${_47.rwr ()}${StatementBlock.rwr ()}`);
return exit_rule ("ElseStatement");
},
ForStatement : function (_49,ident,_51,Exp,StatementBlock,Rec_Statement,) {
enter_rule ("ForStatement");
    set_return (`${_49.rwr ()}${ident.rwr ()}${_51.rwr ()}${Exp.rwr ()}${StatementBlock.rwr ()}${Rec_Statement.rwr ().join ('')}`);
return exit_rule ("ForStatement");
},
WhileStatement : function (_53,Exp,StatementBlock,Rec_Statement,) {
enter_rule ("WhileStatement");
    set_return (`${_53.rwr ()}${Exp.rwr ()}${StatementBlock.rwr ()}${Rec_Statement.rwr ().join ('')}`);
return exit_rule ("WhileStatement");
},
Assignment_multiple : function (_54,LvalComma,_55,_56,Exp,Rec_Statement,) {
enter_rule ("Assignment_multiple");
    set_return (`${_54.rwr ()}${LvalComma.rwr ().join ('')}${_55.rwr ()}${_56.rwr ()}${Exp.rwr ()}${Rec_Statement.rwr ().join ('')}`);
return exit_rule ("Assignment_multiple");
},
Assignment_single : function (Lval,_57,Exp,Rec_Statement,) {
enter_rule ("Assignment_single");
    set_return (`${Lval.rwr ()}${_57.rwr ()}${Exp.rwr ()}${Rec_Statement.rwr ().join ('')}`);
return exit_rule ("Assignment_single");
},
LvalComma : function (Lval,Comma,) {
enter_rule ("LvalComma");
    set_return (`${Lval.rwr ()}${Comma.rwr ().join ('')}`);
return exit_rule ("LvalComma");
},
ReturnExp_multiple : function (_59,ExpComma,_60,Rec_Statement,) {
enter_rule ("ReturnExp_multiple");
    set_return (`${_59.rwr ()}${ExpComma.rwr ().join ('')}${_60.rwr ()}${Rec_Statement.rwr ().join ('')}`);
return exit_rule ("ReturnExp_multiple");
},
ReturnExp_single : function (Exp,Rec_Statement,) {
enter_rule ("ReturnExp_single");
    set_return (`${Exp.rwr ()}${Rec_Statement.rwr ().join ('')}`);
return exit_rule ("ReturnExp_single");
},
ExpComma : function (Exp,Comma,) {
enter_rule ("ExpComma");
    set_return (`${Exp.rwr ()}${Comma.rwr ().join ('')}`);
return exit_rule ("ExpComma");
},
Exp : function (BooleanAndOrIn,) {
enter_rule ("Exp");
    set_return (`${BooleanAndOrIn.rwr ()}`);
return exit_rule ("Exp");
},
BooleanAndOrIn_andOrIn : function (BooleanAndOrIn,andOrIn,BooleanExp,) {
enter_rule ("BooleanAndOrIn_andOrIn");
    set_return (`${BooleanAndOrIn.rwr ()}${andOrIn.rwr ()}${BooleanExp.rwr ()}`);
return exit_rule ("BooleanAndOrIn_andOrIn");
},
BooleanAndOrIn_default : function (BooleanExp,) {
enter_rule ("BooleanAndOrIn_default");
    set_return (`${BooleanExp.rwr ()}`);
return exit_rule ("BooleanAndOrIn_default");
},
BooleanExp_boolopneq : function (BooleanExp,boolNeq,BooleanNot,) {
enter_rule ("BooleanExp_boolopneq");
    set_return (`${BooleanExp.rwr ()}${boolNeq.rwr ()}${BooleanNot.rwr ()}`);
return exit_rule ("BooleanExp_boolopneq");
},
BooleanExp_boolop : function (BooleanExp,boolOp,BooleanNot,) {
enter_rule ("BooleanExp_boolop");
    set_return (`${BooleanExp.rwr ()}${boolOp.rwr ()}${BooleanNot.rwr ()}`);
return exit_rule ("BooleanExp_boolop");
},
BooleanExp_basic : function (BooleanNot,) {
enter_rule ("BooleanExp_basic");
    set_return (`${BooleanNot.rwr ()}`);
return exit_rule ("BooleanExp_basic");
},
BooleanNot_not : function (_63,BooleanExp,) {
enter_rule ("BooleanNot_not");
    set_return (`${_63.rwr ()}${BooleanExp.rwr ()}`);
return exit_rule ("BooleanNot_not");
},
BooleanNot_basic : function (AddExp,) {
enter_rule ("BooleanNot_basic");
    set_return (`${AddExp.rwr ()}`);
return exit_rule ("BooleanNot_basic");
},
AddExp_plus : function (AddExp,_64,MulExp,) {
enter_rule ("AddExp_plus");
    set_return (`${AddExp.rwr ()}${_64.rwr ()}${MulExp.rwr ()}`);
return exit_rule ("AddExp_plus");
},
AddExp_minus : function (AddExp,_65,MulExp,) {
enter_rule ("AddExp_minus");
    set_return (`${AddExp.rwr ()}${_65.rwr ()}${MulExp.rwr ()}`);
return exit_rule ("AddExp_minus");
},
AddExp_basic : function (MulExp,) {
enter_rule ("AddExp_basic");
    set_return (`${MulExp.rwr ()}`);
return exit_rule ("AddExp_basic");
},
MulExp_times : function (MulExp,_66,ExpExp,) {
enter_rule ("MulExp_times");
    set_return (`${MulExp.rwr ()}${_66.rwr ()}${ExpExp.rwr ()}`);
return exit_rule ("MulExp_times");
},
MulExp_divide : function (MulExp,_67,ExpExp,) {
enter_rule ("MulExp_divide");
    set_return (`${MulExp.rwr ()}${_67.rwr ()}${ExpExp.rwr ()}`);
return exit_rule ("MulExp_divide");
},
MulExp_basic : function (ExpExp,) {
enter_rule ("MulExp_basic");
    set_return (`${ExpExp.rwr ()}`);
return exit_rule ("MulExp_basic");
},
ExpExp_power : function (Primary,_68,ExpExp,) {
enter_rule ("ExpExp_power");
    set_return (`${Primary.rwr ()}${_68.rwr ()}${ExpExp.rwr ()}`);
return exit_rule ("ExpExp_power");
},
ExpExp_basic : function (Primary,) {
enter_rule ("ExpExp_basic");
    set_return (`${Primary.rwr ()}`);
return exit_rule ("ExpExp_basic");
},
Primary_lookupident : function (Primary,_69,ident,) {
enter_rule ("Primary_lookupident");
    set_return (`${Primary.rwr ()}${_69.rwr ()}${ident.rwr ()}`);
return exit_rule ("Primary_lookupident");
},
Primary_lookup : function (Primary,_70,ePrimary,) {
enter_rule ("Primary_lookup");
    set_return (`${Primary.rwr ()}${_70.rwr ()}${ePrimary.rwr ()}`);
return exit_rule ("Primary_lookup");
},
Primary_fieldident : function (Primary,_71,e,) {
enter_rule ("Primary_fieldident");
    set_return (`${Primary.rwr ()}${_71.rwr ()}${e.rwr ()}`);
return exit_rule ("Primary_fieldident");
},
Primary_field : function (Primary,_71,e,) {
enter_rule ("Primary_field");
    set_return (`${Primary.rwr ()}${_71.rwr ()}${e.rwr ()}`);
return exit_rule ("Primary_field");
},
Primary_index : function (Primary,_72,Exp,_73,) {
enter_rule ("Primary_index");
    set_return (`${Primary.rwr ()}${_72.rwr ()}${Exp.rwr ()}${_73.rwr ()}`);
return exit_rule ("Primary_index");
},
Primary_nthslice : function (Primary,_74,digit,_75,_76,) {
enter_rule ("Primary_nthslice");
    set_return (`${Primary.rwr ()}${_74.rwr ()}${digit.rwr ().join ('')}${_75.rwr ()}${_76.rwr ()}`);
return exit_rule ("Primary_nthslice");
},
Primary_identcall : function (ident,Actuals,) {
enter_rule ("Primary_identcall");
    set_return (`${ident.rwr ()}${Actuals.rwr ()}`);
return exit_rule ("Primary_identcall");
},
Primary_call : function (Primary,Actuals,) {
enter_rule ("Primary_call");
    set_return (`${Primary.rwr ()}${Actuals.rwr ()}`);
return exit_rule ("Primary_call");
},
Primary_atom : function (Atom,) {
enter_rule ("Primary_atom");
    set_return (`${Atom.rwr ()}`);
return exit_rule ("Primary_atom");
},
Atom_emptylistconst : function (_77,_78,) {
enter_rule ("Atom_emptylistconst");
    set_return (`${_77.rwr ()}${_78.rwr ()}`);
return exit_rule ("Atom_emptylistconst");
},
Atom_emptydict : function (_79,_80,) {
enter_rule ("Atom_emptydict");
    set_return (`${_79.rwr ()}${_80.rwr ()}`);
return exit_rule ("Atom_emptydict");
},
Atom_paren : function (_81,Exp,_82,) {
enter_rule ("Atom_paren");
    set_return (`${_81.rwr ()}${Exp.rwr ()}${_82.rwr ()}`);
return exit_rule ("Atom_paren");
},
Atom_listconst : function (lb,line1,PrimaryComma,line2,rb,) {
enter_rule ("Atom_listconst");
    set_return (`${lb.rwr ()}${line1.rwr ().join ('')}${PrimaryComma.rwr ().join ('')}${rb.rwr ()}${line2.rwr ().join ('')}`);
return exit_rule ("Atom_listconst");
},
Atom_dict : function (lb,line1,PairComma,line2,rb,) {
enter_rule ("Atom_dict");
    set_return (`${lb.rwr ()}${line1.rwr ().join ('')}${PairComma.rwr ().join ('')}${line2.rwr ().join ('')}${rb.rwr ()}`);
return exit_rule ("Atom_dict");
},
Atom_lambda : function (lambda,LambdaFormals,_87,Exp,) {
enter_rule ("Atom_lambda");
    set_return (`${lambda.rwr ()}${LambdaFormals.rwr ().join ('')}${_87.rwr ()}${Exp.rwr ()}`);
return exit_rule ("Atom_lambda");
},
Atom_phi : function (phi,) {
enter_rule ("Atom_phi");
    set_return (`${phi.rwr ()}`);
return exit_rule ("Atom_phi");
},
Atom_true : function (_119,) {
enter_rule ("Atom_true");
    set_return (`${_119.rwr ()}`);
return exit_rule ("Atom_true");
},
Atom_false : function (_120,) {
enter_rule ("Atom_false");
    set_return (`${_120.rwr ()}`);
return exit_rule ("Atom_false");
},
Atom_external : function (m,) {
enter_rule ("Atom_external");
    set_return (`${m.rwr ()}`);
return exit_rule ("Atom_external");
},
Atom_subr : function (_subrref,ident,) {
enter_rule ("Atom_subr");
    set_return (`${_subrref.rwr ()}${ident.rwr ()}`);
return exit_rule ("Atom_subr");
},
Atom_range : function (_122,_123,Exp,_124,) {
enter_rule ("Atom_range");
    set_return (`${_122.rwr ()}${_123.rwr ()}${Exp.rwr ()}${_124.rwr ()}`);
return exit_rule ("Atom_range");
},
Atom_string : function (string,) {
enter_rule ("Atom_string");
    set_return (`${string.rwr ()}`);
return exit_rule ("Atom_string");
},
Atom_number : function (number,) {
enter_rule ("Atom_number");
    set_return (`${number.rwr ()}`);
return exit_rule ("Atom_number");
},
Atom_ident : function (ident,) {
enter_rule ("Atom_ident");
    set_return (`${ident.rwr ()}`);
return exit_rule ("Atom_ident");
},
PrimaryComma : function (Primary,comma,) {
enter_rule ("PrimaryComma");
    set_return (`${Primary.rwr ()}${comma.rwr ().join ('')}`);
return exit_rule ("PrimaryComma");
},
PairComma : function (Pair,_126,) {
enter_rule ("PairComma");
    set_return (`${Pair.rwr ()}${_126.rwr ().join ('')}`);
return exit_rule ("PairComma");
},
Lval : function (Exp,) {
enter_rule ("Lval");
    set_return (`${Exp.rwr ()}`);
return exit_rule ("Lval");
},
Formals_noformals : function (_127,_128,) {
enter_rule ("Formals_noformals");
    set_return (`${_127.rwr ()}${_128.rwr ()}`);
return exit_rule ("Formals_noformals");
},
Formals_withformals : function (_129,FormalComma,_130,) {
enter_rule ("Formals_withformals");
    set_return (`${_129.rwr ()}${FormalComma.rwr ().join ('')}${_130.rwr ()}`);
return exit_rule ("Formals_withformals");
},
ObjFormals_noformals : function (_131,_132,) {
enter_rule ("ObjFormals_noformals");
    set_return (`${_131.rwr ()}${_132.rwr ()}`);
return exit_rule ("ObjFormals_noformals");
},
ObjFormals_withformals : function (_133,FormalComma,_134,) {
enter_rule ("ObjFormals_withformals");
    set_return (`${_133.rwr ()}${FormalComma.rwr ().join ('')}${_134.rwr ()}`);
return exit_rule ("ObjFormals_withformals");
},
LambdaFormals_noformals : function (_135,_136,) {
enter_rule ("LambdaFormals_noformals");
    set_return (`${_135.rwr ()}${_136.rwr ()}`);
return exit_rule ("LambdaFormals_noformals");
},
LambdaFormals_withformals : function (_137,FormalComma,_138,) {
enter_rule ("LambdaFormals_withformals");
    set_return (`${_137.rwr ()}${FormalComma.rwr ().join ('')}${_138.rwr ()}`);
return exit_rule ("LambdaFormals_withformals");
},
Formal : function (ident,) {
enter_rule ("Formal");
    set_return (`${ident.rwr ()}`);
return exit_rule ("Formal");
},
FormalComma : function (Formal,comma,) {
enter_rule ("FormalComma");
    set_return (`${Formal.rwr ()}${comma.rwr ().join ('')}`);
return exit_rule ("FormalComma");
},
Actuals_noactuals : function (_141,_142,) {
enter_rule ("Actuals_noactuals");
    set_return (`${_141.rwr ()}${_142.rwr ()}`);
return exit_rule ("Actuals_noactuals");
},
Actuals_actuals : function (_143,ActualComma,_144,line,) {
enter_rule ("Actuals_actuals");
    set_return (`${_143.rwr ()}${ActualComma.rwr ().join ('')}${_144.rwr ()}${line.rwr ().join ('')}`);
return exit_rule ("Actuals_actuals");
},
Actual : function (ParamName,Exp,) {
enter_rule ("Actual");
    set_return (`${ParamName.rwr ().join ('')}${Exp.rwr ()}`);
return exit_rule ("Actual");
},
ActualComma : function (comment,Actual,comma,line,) {
enter_rule ("ActualComma");
    set_return (`${comment.rwr ().join ('')}${Actual.rwr ()}${comma.rwr ().join ('')}${line.rwr ().join ('')}`);
return exit_rule ("ActualComma");
},
ParamName : function (ident,_146,) {
enter_rule ("ParamName");
    set_return (`${ident.rwr ()}${_146.rwr ()}`);
return exit_rule ("ParamName");
},
number_fract : function (digit,_147,denominatordigit,) {
enter_rule ("number_fract");
    set_return (`${digit.rwr ().join ('')}${_147.rwr ()}${denominatordigit.rwr ().join ('')}`);
return exit_rule ("number_fract");
},
number_whole : function (digit,) {
enter_rule ("number_whole");
    set_return (`${digit.rwr ().join ('')}`);
return exit_rule ("number_whole");
},
Pair : function (string,_148,Exp,_149,) {
enter_rule ("Pair");
    set_return (`${string.rwr ()}${_148.rwr ()}${Exp.rwr ()}${_149.rwr ().join ('')}`);
return exit_rule ("Pair");
},
andOrIn : function (_156,) {
enter_rule ("andOrIn");
    set_return (`${_156.rwr ()}`);
return exit_rule ("andOrIn");
},
boolOp : function (_161,) {
enter_rule ("boolOp");
    set_return (`${_161.rwr ()}`);
return exit_rule ("boolOp");
},
boolEq : function (_162,) {
enter_rule ("boolEq");
    set_return (`${_162.rwr ()}`);
return exit_rule ("boolEq");
},
boolNeq : function (_163,) {
enter_rule ("boolNeq");
    set_return (`${_163.rwr ()}`);
return exit_rule ("boolNeq");
},
string : function (_164,stringchar,_165,) {
enter_rule ("string");
    set_return (`${_164.rwr ()}${stringchar.rwr ().join ('')}${_165.rwr ()}`);
return exit_rule ("string");
},
stringchar_rec : function (_166,stringchar,_167,) {
enter_rule ("stringchar_rec");
    set_return (`${_166.rwr ()}${stringchar.rwr ().join ('')}${_167.rwr ()}`);
return exit_rule ("stringchar_rec");
},
stringchar_other : function (any,) {
enter_rule ("stringchar_other");
    set_return (`${any.rwr ()}`);
return exit_rule ("stringchar_other");
},
keyword : function (_222,) {
enter_rule ("keyword");
    set_return (`${_222.rwr ()}`);
return exit_rule ("keyword");
},
lambda : function (_226,) {
enter_rule ("lambda");
    set_return (`${_226.rwr ()}`);
return exit_rule ("lambda");
},
phi : function (_230,) {
enter_rule ("phi");
    set_return (`${_230.rwr ()}`);
return exit_rule ("phi");
},
kw : function (_231,s,_232,) {
enter_rule ("kw");
    set_return (`${_231.rwr ()}${s.rwr ()}${_232.rwr ()}`);
return exit_rule ("kw");
},
ident : function (_233,idchar,_234,) {
enter_rule ("ident");
    set_return (`${_233.rwr ()}${idchar.rwr ().join ('')}${_234.rwr ()}`);
return exit_rule ("ident");
},
idchar_rec : function (_235,idchar,_236,) {
enter_rule ("idchar_rec");
    set_return (`${_235.rwr ()}${idchar.rwr ().join ('')}${_236.rwr ()}`);
return exit_rule ("idchar_rec");
},
idchar_other : function (any,) {
enter_rule ("idchar_other");
    set_return (`${any.rwr ()}`);
return exit_rule ("idchar_other");
},
comment : function (_239,commentchar,_240,) {
enter_rule ("comment");
    set_return (`${_239.rwr ()}${commentchar.rwr ().join ('')}${_240.rwr ()}\n`);
return exit_rule ("comment");
},
commentchar_rec : function (_241,commentchar,_242,) {
enter_rule ("commentchar_rec");
    set_return (`${_241.rwr ()}${commentchar.rwr ().join ('')}${_242.rwr ()}`);
return exit_rule ("commentchar_rec");
},
commentchar_other : function (any,) {
enter_rule ("commentchar_other");
    set_return (`${any.rwr ()}`);
return exit_rule ("commentchar_other");
},
eh : function (ident,) {
enter_rule ("eh");
    set_return (`${ident.rwr ()}`);
return exit_rule ("eh");
},
fname : function (ident,) {
enter_rule ("fname");
    set_return (`${ident.rwr ()}`);
return exit_rule ("fname");
},
msg : function (ident,) {
enter_rule ("msg");
    set_return (`${ident.rwr ()}`);
return exit_rule ("msg");
},
ok : function (port,) {
enter_rule ("ok");
    set_return (`${port.rwr ()}`);
return exit_rule ("ok");
},
err : function (port,) {
enter_rule ("err");
    set_return (`${port.rwr ()}`);
return exit_rule ("err");
},
port : function (string,) {
enter_rule ("port");
    set_return (`${string.rwr ()}`);
return exit_rule ("port");
},
line : function (lb,cs,rb,) {
enter_rule ("line");
    set_return (`${lb.rwr ()}${cs.rwr ().join ('')}${rb.rwr ()}`);
return exit_rule ("line");
},
Comma : function (line1,_comma,line2,) {
enter_rule ("Comma");
    set_return (`${line1.rwr ().join ('')}${_comma.rwr ()}${line2.rwr ().join ('')}`);
return exit_rule ("Comma");
},
ParaMark : function (c,) {
enter_rule ("ParaMark");
    set_return (`${c.rwr ()}`);
return exit_rule ("ParaMark");
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

