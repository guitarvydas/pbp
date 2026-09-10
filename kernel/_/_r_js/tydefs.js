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
/*  Routing connection for a container component. The `direction` field has *//* line 106 */
/*  no affect on the default mevent routing system _ it is there for debugging *//* line 107 */
/*  purposes, or for reading by other tools. */        /* line 108 *//* line 109 */
class Connector {
  constructor () {                                     /* line 110 */

    this.direction =  null;/*  down, across, up, through *//* line 111 */
    this.sender =  null;                               /* line 112 */
    this.receiver =  null;                             /* line 113 *//* line 114 */
  }
}
                                                       /* line 115 */
/*  `Sender` is used to “pattern match“ which `Receiver` a mevent should go to, *//* line 116 */
/*  based on component ID (pointer) and port name. */  /* line 117 *//* line 118 */
class Sender {
  constructor () {                                     /* line 119 */

    this.name =  null;                                 /* line 120 */
    this.component =  null;                            /* line 121 */
    this.port =  null;                                 /* line 122 *//* line 123 */
  }
}
                                                       /* line 124 *//* line 125 *//* line 126 */
/*  `Receiver` is a handle to a destination queue, and a `port` name to assign *//* line 127 */
/*  to incoming mevents to this queue. */              /* line 128 *//* line 129 */
class Receiver {
  constructor () {                                     /* line 130 */

    this.name =  null;                                 /* line 131 */
    this.queue =  null;                                /* line 132 */
    this.port =  null;                                 /* line 133 */
    this.component =  null;                            /* line 134 *//* line 135 */
  }
}
                                                       /* line 136 */
function mkSender (name,component,port) {              /* line 137 */
    let  s =  new Sender ();                           /* line 138 */;
    s.name =  name;                                    /* line 139 */
    s.component =  component;                          /* line 140 */
    s.port =  port;                                    /* line 141 */
    return  s;                                         /* line 142 *//* line 143 *//* line 144 */
}

function mkReceiver (name,component,port,q) {          /* line 145 */
    let  r =  new Receiver ();                         /* line 146 */;
    r.name =  name;                                    /* line 147 */
    r.component =  component;                          /* line 148 */
    r.port =  port;                                    /* line 149 */
    /*  We need a way to determine which queue to target. "Down" and "Across" go to inq, "Up" and "Through" go to outq. *//* line 150 */
    r.queue =  q;                                      /* line 151 */
    return  r;                                         /* line 152 *//* line 153 *//* line 154 */
}
                                                       /* line 155 */
class Component_Registry {
  constructor () {                                     /* line 156 */

    this.templates = {};                               /* line 157 *//* line 158 */
  }
}
                                                       /* line 159 */
class Template {
  constructor () {                                     /* line 160 */

    this.name =  null;                                 /* line 161 */
    this.container =  null;                            /* line 162 */
    this.instantiator =  null;                         /* line 163 *//* line 164 */
  }
}
                                                       /* line 165 */
function mkTemplate (name,template_data,instantiator) {/* line 166 */
    let  templ =  new Template ();                     /* line 167 */;
    templ.name =  name;                                /* line 168 */
    templ.template_data =  template_data;              /* line 169 */
    templ.instantiator =  instantiator;                /* line 170 */
    return  templ;                                     /* line 171 *//* line 172 *//* line 173 */
}

function make_component_registry () {                  /* line 174 */
    return  new Component_Registry ();                 /* line 175 */;/* line 176 *//* line 177 */
}

/*  Data for an asyncronous component _ effectively, a function with input *//* line 178 */
/*  and output queues of mevents. */                   /* line 179 */
/*  */                                                 /* line 180 */
/*  Components can either be a user_supplied function (“leaf“), or a “container“ *//* line 181 */
/*  that routes mevents to child components according to a list of connections *//* line 182 */
/*  that serve as a mevent routing table. */           /* line 183 */
/*  */                                                 /* line 184 */
/*  Child components themselves can be leaves or other containers. *//* line 185 */
/*  */                                                 /* line 186 */
/*  `handler` invokes the code that is attached to this component. *//* line 187 */
/*  */                                                 /* line 188 */
/*  `instance_data` is a pointer to instance data that the `leaf_handler` *//* line 189 */
/*  function may want whenever it is invoked again. */ /* line 190 *//* line 191 */
/*  TODO: what is .routings for? (is it a historical artefact that can be removed?)  *//* line 192 *//* line 193 */
/*  Eh_States :: enum { idle, active } */              /* line 194 */
class Eh {
  constructor () {                                     /* line 195 */

    this.name =  "";                                   /* line 196 */
    this.inq =  []                                     /* line 197 */;
    this.outq =  []                                    /* line 198 */;
    this.owner =  null;                                /* line 199 */
    this.children = [];                                /* line 200 */
    this.visit_ordering =  []                          /* line 201 */;
    this.connections = [];                             /* line 202 */
    this.routings =  []                                /* line 203 */;
    this.handler =  null;                              /* line 204 */
    this.reset_instance_data =  null;                  /* line 205 */
    this.finject =  null;                              /* line 206 */
    this.stop =  null;                                 /* line 207 */
    this.instance_data =  null;                        /* line 208 *//*  arg needed for probe support  *//* line 209 */
    this.arg =  "";                                    /* line 210 */
    this.state =  "idle";                              /* line 211 */
    this.special =  false;                             /* line 212 *//*  bootstrap debugging *//* line 213 */
    this.kind =  null;/*  enum { container, leaf, } */ /* line 214 *//* line 215 */
  }
}
                                                       /* line 216 */
let  load_errors =  false;                             /* line 217 */
let  runtime_errors =  false;                          /* line 218 *//* line 219 */
function clone_string (s) {                            /* line 220 */
    return  s;                                         /* line 221 *//* line 222 *//* line 223 */
}

function injector (eh,mevent) {                        /* line 224 */
    eh.handler ( eh, mevent)                           /* line 225 *//* line 226 *//* line 227 */
}
