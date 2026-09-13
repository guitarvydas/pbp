import * as fs from 'fs';
import path from 'path';
import execSync from 'child_process';
import 'dotenv/config';
                                                       /* line 1 *//* line 2 */
let  counter =  0;                                     /* line 3 */
let  ticktime =  0;                                    /* line 4 *//* line 5 *//* line 6 */
class Component_Registry {
  constructor () {                                     /* line 7 */

    this.templates = {};                               /* line 8 *//* line 9 */
  }
}
                                                       /* line 10 */
class Template {
  constructor () {                                     /* line 11 */

    this.name =  null;                                 /* line 12 */
    this.container =  null;                            /* line 13 */
    this.instantiator =  null;                         /* line 14 *//* line 15 */
  }
}
                                                       /* line 16 */
function mkTemplate (name,template_data,instantiator) {/* line 17 */
    let  templ =  new Template ();                     /* line 18 */;
    templ.name =  name;                                /* line 19 */
    templ.template_data =  template_data;              /* line 20 */
    templ.instantiator =  instantiator;                /* line 21 */
    return  templ;                                     /* line 22 *//* line 23 *//* line 24 */
}

function make_component_registry () {                  /* line 25 */
    return  new Component_Registry ();                 /* line 26 */;/* line 27 *//* line 28 */
}

/*  Data for an asyncronous component _ effectively, a function with input *//* line 29 */
/*  and output queues of mevents. */                   /* line 30 */
/*  */                                                 /* line 31 */
/*  Components can either be a user_supplied function ("leaf“), or a “container“ *//* line 32 */
/*  that routes mevents to child components according to a list of connections *//* line 33 */
/*  that serve as a mevent routing table. */           /* line 34 */
/*  */                                                 /* line 35 */
/*  Child components themselves can be leaves or other containers. *//* line 36 */
/*  */                                                 /* line 37 */
/*  `handler` invokes the code that is attached to this component. *//* line 38 */
/*  */                                                 /* line 39 */
/*  `instance_data` is a pointer to instance data that the `leaf_handler` *//* line 40 */
/*  function may want whenever it is invoked again. */ /* line 41 *//* line 42 */
/*  TODO: what is .routings for? (is it a historical artefact that can be removed?)  *//* line 43 *//* line 44 */
/*  Eh_States :: enum { idle, active } */              /* line 45 */
class Eh {
  constructor () {                                     /* line 46 */

    this.name =  "";                                   /* line 47 */
    this.inq =  []                                     /* line 48 */;
    this.outq =  []                                    /* line 49 */;
    this.owner =  null;                                /* line 50 */
    this.children = [];                                /* line 51 */
    this.visit_ordering =  []                          /* line 52 */;
    this.connections = [];                             /* line 53 */
    this.routings =  []                                /* line 54 */;
    this.handler =  null;                              /* line 55 */
    this.reset_instance_data =  null;                  /* line 56 */
    this.finject =  null;                              /* line 57 */
    this.stop =  null;                                 /* line 58 */
    this.instance_data =  null;                        /* line 59 *//*  arg needed for probe support  *//* line 60 */
    this.arg =  "";                                    /* line 61 */
    this.state =  "idle";                              /* line 62 */
    this.special =  false;                             /* line 63 *//*  bootstrap debugging *//* line 64 */
    this.kind =  null;/*  enum { container, leaf, } */ /* line 65 *//* line 66 */
  }
}
                                                       /* line 67 */
let  load_errors =  false;                             /* line 68 */
let  runtime_errors =  false;                          /* line 69 *//* line 70 */
function clone_string (s) {                            /* line 71 */
    return  s;                                         /* line 72 *//* line 73 *//* line 74 */
}

function injector (eh,mevent) {                        /* line 75 */
    eh.handler ( eh, mevent)                           /* line 76 *//* line 77 *//* line 78 */
}
