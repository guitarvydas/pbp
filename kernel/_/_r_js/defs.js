import * as fs from 'fs';
import path from 'path';
import execSync from 'child_process';
import 'dotenv/config';
                                                       /* line 1 *//* line 2 */
let  counter =  0;                                     /* line 3 */
let  ticktime =  0;                                    /* line 4 *//* line 5 */
const  enumDown =  0                                   /* line 6 */;
const  enumAcross =  1                                 /* line 7 */;
const  enumUp =  2                                     /* line 8 */;
const  enumThrough =  3                                /* line 9 */;/* line 10 *//* line 11 */
/*  Routing connection for a container component. The `direction` field has *//* line 12 */
/*  no affect on the default mevent routing system _ it is there for debugging *//* line 13 */
/*  purposes, or for reading by other tools. */        /* line 14 *//* line 15 */
class Connector {
  constructor () {                                     /* line 16 */

    this.direction =  null;/*  down, across, up, through *//* line 17 */
    this.sender =  null;                               /* line 18 */
    this.receiver =  null;                             /* line 19 *//* line 20 */
  }
}
                                                       /* line 21 */
/*  `Sender` is used to "pattern match“ which `Receiver` a mevent should go to, *//* line 22 */
/*  based on component ID (pointer) and port name. */  /* line 23 *//* line 24 */
class Sender {
  constructor () {                                     /* line 25 */

    this.name =  null;                                 /* line 26 */
    this.component =  null;                            /* line 27 */
    this.port =  null;                                 /* line 28 *//* line 29 */
  }
}
                                                       /* line 30 *//* line 31 *//* line 32 */
/*  `Receiver` is a handle to a destination queue, and a `port` name to assign *//* line 33 */
/*  to incoming mevents to this queue. */              /* line 34 *//* line 35 */
class Receiver {
  constructor () {                                     /* line 36 */

    this.name =  null;                                 /* line 37 */
    this.queue =  null;                                /* line 38 */
    this.port =  null;                                 /* line 39 */
    this.component =  null;                            /* line 40 *//* line 41 */
  }
}
                                                       /* line 42 */
function mkSender (name,component,port) {              /* line 43 */
    let  s =  new Sender ();                           /* line 44 */;
    s.name =  name;                                    /* line 45 */
    s.component =  component;                          /* line 46 */
    s.port =  port;                                    /* line 47 */
    return  s;                                         /* line 48 *//* line 49 *//* line 50 */
}

function mkReceiver (name,component,port,q) {          /* line 51 */
    let  r =  new Receiver ();                         /* line 52 */;
    r.name =  name;                                    /* line 53 */
    r.component =  component;                          /* line 54 */
    r.port =  port;                                    /* line 55 */
    /*  We need a way to determine which queue to target. "Down" and "Across" go to inq, "Up" and "Through" go to outq. *//* line 56 */
    r.queue =  q;                                      /* line 57 */
    return  r;                                         /* line 58 *//* line 59 *//* line 60 */
}
                                                       /* line 61 */
class Component_Registry {
  constructor () {                                     /* line 62 */

    this.templates = {};                               /* line 63 *//* line 64 */
  }
}
                                                       /* line 65 */
class Template {
  constructor () {                                     /* line 66 */

    this.name =  null;                                 /* line 67 */
    this.container =  null;                            /* line 68 */
    this.instantiator =  null;                         /* line 69 *//* line 70 */
  }
}
                                                       /* line 71 */
function mkTemplate (name,template_data,instantiator) {/* line 72 */
    let  templ =  new Template ();                     /* line 73 */;
    templ.name =  name;                                /* line 74 */
    templ.template_data =  template_data;              /* line 75 */
    templ.instantiator =  instantiator;                /* line 76 */
    return  templ;                                     /* line 77 *//* line 78 *//* line 79 */
}

function make_component_registry () {                  /* line 80 */
    return  new Component_Registry ();                 /* line 81 */;/* line 82 *//* line 83 */
}

/*  Data for an asyncronous component _ effectively, a function with input *//* line 84 */
/*  and output queues of mevents. */                   /* line 85 */
/*  */                                                 /* line 86 */
/*  Components can either be a user_supplied function (“leaf“), or a “container“ *//* line 87 */
/*  that routes mevents to child components according to a list of connections *//* line 88 */
/*  that serve as a mevent routing table. */           /* line 89 */
/*  */                                                 /* line 90 */
/*  Child components themselves can be leaves or other containers. *//* line 91 */
/*  */                                                 /* line 92 */
/*  `handler` invokes the code that is attached to this component. *//* line 93 */
/*  */                                                 /* line 94 */
/*  `instance_data` is a pointer to instance data that the `leaf_handler` *//* line 95 */
/*  function may want whenever it is invoked again. */ /* line 96 *//* line 97 */
/*  TODO: what is .routings for? (is it a historical artefact that can be removed?)  *//* line 98 *//* line 99 */
/*  Eh_States :: enum { idle, active } */              /* line 100 */
class Eh {
  constructor () {                                     /* line 101 */

    this.name =  "";                                   /* line 102 */
    this.inq =  []                                     /* line 103 */;
    this.outq =  []                                    /* line 104 */;
    this.owner =  null;                                /* line 105 */
    this.children = [];                                /* line 106 */
    this.visit_ordering =  []                          /* line 107 */;
    this.connections = [];                             /* line 108 */
    this.routings =  []                                /* line 109 */;
    this.handler =  null;                              /* line 110 */
    this.reset_instance_data =  null;                  /* line 111 */
    this.finject =  null;                              /* line 112 */
    this.stop =  null;                                 /* line 113 */
    this.instance_data =  null;                        /* line 114 *//*  arg needed for probe support  *//* line 115 */
    this.arg =  "";                                    /* line 116 */
    this.state =  "idle";                              /* line 117 */
    this.special =  false;                             /* line 118 *//*  bootstrap debugging *//* line 119 */
    this.kind =  null;/*  enum { container, leaf, } */ /* line 120 *//* line 121 */
  }
}
                                                       /* line 122 */
let  load_errors =  false;                             /* line 123 */
let  runtime_errors =  false;                          /* line 124 *//* line 125 */
function clone_string (s) {                            /* line 126 */
    return  s;                                         /* line 127 *//* line 128 *//* line 129 */
}

function injector (eh,mevent) {                        /* line 130 */
    eh.handler ( eh, mevent)                           /* line 131 *//* line 132 *//* line 133 */
}
