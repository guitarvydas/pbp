import * as fs from 'fs';
import path from 'path';
import execSync from 'child_process';
import 'dotenv/config';
                                                       /* line 1 *//* line 2 */
let  counter =  0;                                     /* line 3 */
let  ticktime =  0;                                    /* line 4 *//* line 5 */
let  digits = [ "₀", "₁", "₂", "₃", "₄", "₅", "₆", "₇", "₈", "₉", "₁₀", "₁₁", "₁₂", "₁₃", "₁₄", "₁₅", "₁₆", "₁₇", "₁₈", "₁₉", "₂₀", "₂₁", "₂₂", "₂₃", "₂₄", "₂₅", "₂₆", "₂₇", "₂₈", "₂₉"];/* line 12 *//* line 13 *//* line 14 */
function gensymbol (s) {                               /* line 15 *//* line 16 */
    let name_with_id =  ( s.toString ()+ subscripted_digit ( counter).toString ()) /* line 17 */;
    counter =  counter+ 1;                             /* line 18 */
    return  name_with_id;                              /* line 19 *//* line 20 *//* line 21 */
}

function subscripted_digit (n) {                       /* line 22 *//* line 23 */
    if (((( n >=  0) && ( n <=  29)))) {               /* line 24 */
      return  digits [ n];                             /* line 25 */
    }
    else {                                             /* line 26 */
      return  ( "₊".toString ()+ `${ n}`.toString ())  /* line 27 */;/* line 28 */
    }                                                  /* line 29 *//* line 30 */
}

class Datum {
  constructor () {                                     /* line 31 */

    this.v =  null;                                    /* line 32 */
    this.clone =  null;                                /* line 33 */
    this.reclaim =  null;                              /* line 34 */
    this.other =  null;/*  reserved for use on per-project basis  *//* line 35 *//* line 36 */
  }
}
                                                       /* line 37 *//* line 38 */
/*  Mevent passed to a leaf component. */              /* line 39 */
/*  */                                                 /* line 40 */
/*  `port` refers to the name of the incoming or outgoing port of this component. *//* line 41 */
/*  `payload` is the data attached to this mevent. */  /* line 42 */
class Mevent {
  constructor () {                                     /* line 43 */

    this.port =  null;                                 /* line 44 */
    this.payload =  null;                              /* line 45 *//* line 46 */
  }
}
                                                       /* line 47 */
function clone_port (s) {                              /* line 48 */
    return clone_string ( s)                           /* line 49 */;/* line 50 *//* line 51 */
}

/*  Utility for making a `Mevent`. Used to safely "seed“ mevents *//* line 52 */
/*  entering the very top of a network. */             /* line 53 */
function make_mevent (port,datum) {                    /* line 54 */
    let p = clone_string ( port)                       /* line 55 */;
    let  m =  new Mevent ();                           /* line 56 */;
    m.port =  p;                                       /* line 57 */
    m.payload =  datum.clone ();                       /* line 58 */
    return  m;                                         /* line 59 *//* line 60 *//* line 61 */
}

/*  Clones a mevent. Primarily used internally for “fanning out“ a mevent to multiple destinations. *//* line 62 */
function mevent_clone (mev) {                          /* line 63 */
    let  m =  new Mevent ();                           /* line 64 */;
    m.port = clone_port ( mev.port)                    /* line 65 */;
    m.payload =  mev.payload.clone ();                 /* line 66 */
    return  m;                                         /* line 67 *//* line 68 *//* line 69 */
}

/*  Frees a mevent. */                                 /* line 70 */
function destroy_mevent (mev) {                        /* line 71 */
    /*  during debug, dont destroy any mevent, since we want to trace mevents, thus, we need to persist ancestor mevents *//* line 72 *//* line 73 *//* line 74 *//* line 75 */
}

function destroy_datum (mev) {                         /* line 76 *//* line 77 *//* line 78 *//* line 79 */
}

function destroy_port (mev) {                          /* line 80 *//* line 81 *//* line 82 *//* line 83 */
}

/*  */                                                 /* line 84 */
function format_mevent (m) {                           /* line 85 */
    if ( m ==  null) {                                 /* line 86 */
      return  "{}";                                    /* line 87 */
    }
    else {                                             /* line 88 */
      return  ( "{%5C”".toString ()+  ( m.port.toString ()+  ( "%5C”:%5C”".toString ()+  ( m.payload.v.toString ()+  "%5C”}".toString ()) .toString ()) .toString ()) .toString ()) /* line 89 */;/* line 90 */
    }                                                  /* line 91 */
}

function format_mevent_raw (m) {                       /* line 92 */
    if ( m ==  null) {                                 /* line 93 */
      return  "";                                      /* line 94 */
    }
    else {                                             /* line 95 */
      return  m.payload.v;                             /* line 96 *//* line 97 */
    }                                                  /* line 98 *//* line 99 */
}

const  enumDown =  0                                   /* line 100 */;
const  enumAcross =  1                                 /* line 101 */;
const  enumUp =  2                                     /* line 102 */;
const  enumThrough =  3                                /* line 103 */;/* line 104 *//* line 105 */
class Component_Registry {
  constructor () {                                     /* line 106 */

    this.templates = {};                               /* line 107 *//* line 108 */
  }
}
                                                       /* line 109 */
/*  Routing connection for a container component. The `direction` field has *//* line 110 */
/*  no affect on the default mevent routing system _ it is there for debugging *//* line 111 */
/*  purposes, or for reading by other tools. */        /* line 112 *//* line 113 */
class Connector {
  constructor () {                                     /* line 114 */

    this.direction =  null;/*  down, across, up, through *//* line 115 */
    this.sender =  null;                               /* line 116 */
    this.receiver =  null;                             /* line 117 *//* line 118 */
  }
}
                                                       /* line 119 */
/*  `Sender` is used to “pattern match“ which `Receiver` a mevent should go to, *//* line 120 */
/*  based on component ID (pointer) and port name. */  /* line 121 *//* line 122 */
class Sender {
  constructor () {                                     /* line 123 */

    this.name =  null;                                 /* line 124 */
    this.component =  null;                            /* line 125 */
    this.port =  null;                                 /* line 126 *//* line 127 */
  }
}
                                                       /* line 128 *//* line 129 *//* line 130 */
/*  `Receiver` is a handle to a destination queue, and a `port` name to assign *//* line 131 */
/*  to incoming mevents to this queue. */              /* line 132 *//* line 133 */
class Receiver {
  constructor () {                                     /* line 134 */

    this.name =  null;                                 /* line 135 */
    this.queue =  null;                                /* line 136 */
    this.port =  null;                                 /* line 137 */
    this.component =  null;                            /* line 138 *//* line 139 */
  }
}
                                                       /* line 140 */
function mkSender (name,component,port) {              /* line 141 */
    let  s =  new Sender ();                           /* line 142 */;
    s.name =  name;                                    /* line 143 */
    s.component =  component;                          /* line 144 */
    s.port =  port;                                    /* line 145 */
    return  s;                                         /* line 146 *//* line 147 *//* line 148 */
}

function mkReceiver (name,component,port,q) {          /* line 149 */
    let  r =  new Receiver ();                         /* line 150 */;
    r.name =  name;                                    /* line 151 */
    r.component =  component;                          /* line 152 */
    r.port =  port;                                    /* line 153 */
    /*  We need a way to determine which queue to target. "Down" and "Across" go to inq, "Up" and "Through" go to outq. *//* line 154 */
    r.queue =  q;                                      /* line 155 */
    return  r;                                         /* line 156 *//* line 157 *//* line 158 */
}
                                                       /* line 159 */
class Component_Registry {
  constructor () {                                     /* line 160 */

    this.templates = {};                               /* line 161 *//* line 162 */
  }
}
                                                       /* line 163 */
class Template {
  constructor () {                                     /* line 164 */

    this.name =  null;                                 /* line 165 */
    this.container =  null;                            /* line 166 */
    this.instantiator =  null;                         /* line 167 *//* line 168 */
  }
}
                                                       /* line 169 */
function mkTemplate (name,template_data,instantiator) {/* line 170 */
    let  templ =  new Template ();                     /* line 171 */;
    templ.name =  name;                                /* line 172 */
    templ.template_data =  template_data;              /* line 173 */
    templ.instantiator =  instantiator;                /* line 174 */
    return  templ;                                     /* line 175 *//* line 176 *//* line 177 */
}

function make_component_registry () {                  /* line 178 */
    return  new Component_Registry ();                 /* line 179 */;/* line 180 *//* line 181 */
}

/*  Data for an asyncronous component _ effectively, a function with input *//* line 182 */
/*  and output queues of mevents. */                   /* line 183 */
/*  */                                                 /* line 184 */
/*  Components can either be a user_supplied function (“leaf“), or a “container“ *//* line 185 */
/*  that routes mevents to child components according to a list of connections *//* line 186 */
/*  that serve as a mevent routing table. */           /* line 187 */
/*  */                                                 /* line 188 */
/*  Child components themselves can be leaves or other containers. *//* line 189 */
/*  */                                                 /* line 190 */
/*  `handler` invokes the code that is attached to this component. *//* line 191 */
/*  */                                                 /* line 192 */
/*  `instance_data` is a pointer to instance data that the `leaf_handler` *//* line 193 */
/*  function may want whenever it is invoked again. */ /* line 194 *//* line 195 */
/*  TODO: what is .routings for? (is it a historical artefact that can be removed?)  *//* line 196 *//* line 197 */
/*  Eh_States :: enum { idle, active } */              /* line 198 */
class Eh {
  constructor () {                                     /* line 199 */

    this.name =  "";                                   /* line 200 */
    this.inq =  []                                     /* line 201 */;
    this.outq =  []                                    /* line 202 */;
    this.owner =  null;                                /* line 203 */
    this.children = [];                                /* line 204 */
    this.visit_ordering =  []                          /* line 205 */;
    this.connections = [];                             /* line 206 */
    this.routings =  []                                /* line 207 */;
    this.handler =  null;                              /* line 208 */
    this.reset_instance_data =  null;                  /* line 209 */
    this.finject =  null;                              /* line 210 */
    this.stop =  null;                                 /* line 211 */
    this.instance_data =  null;                        /* line 212 *//*  arg needed for probe support  *//* line 213 */
    this.arg =  "";                                    /* line 214 */
    this.state =  "idle";                              /* line 215 */
    this.special =  false;                             /* line 216 *//*  bootstrap debugging *//* line 217 */
    this.kind =  null;/*  enum { container, leaf, } */ /* line 218 *//* line 219 */
  }
}
                                                       /* line 220 */
let  load_errors =  false;                             /* line 221 */
let  runtime_errors =  false;                          /* line 222 *//* line 223 */
function clone_string (s) {                            /* line 224 */
    return  s;                                         /* line 225 *//* line 226 *//* line 227 */
}

function injector (eh,mevent) {                        /* line 228 */
    eh.handler ( eh, mevent)                           /* line 229 *//* line 230 *//* line 231 */
}
