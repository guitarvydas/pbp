/*  Data for an asyncronous component _ effectively, a function with input *//* line 1 */
/*  and output queues of mevents. */                   /* line 2 */
/*  */                                                 /* line 3 */
/*  Components can either be a user_supplied function ("leaf“), or a “container“ *//* line 4 */
/*  that routes mevents to child components according to a list of connections *//* line 5 */
/*  that serve as a mevent routing table. */           /* line 6 */
/*  */                                                 /* line 7 */
/*  Child components themselves can be leaves or other containers. *//* line 8 */
/*  */                                                 /* line 9 */
/*  `handler` invokes the code that is attached to this component. *//* line 10 */
/*  */                                                 /* line 11 */
/*  `instance_data` is a pointer to instance data that the `leaf_handler` *//* line 12 */
/*  function may want whenever it is invoked again. */ /* line 13 *//* line 14 */
/*  TODO: what is .routings for? (is it a historical artefact that can be removed?)  *//* line 15 *//* line 16 */
/*  Eh_States :: enum { idle, active } */              /* line 17 */
class Eh {
  constructor () {                                     /* line 18 */

    this.name =  "";                                   /* line 19 */
    this.inq =  []                                     /* line 20 */;
    this.outq =  []                                    /* line 21 */;
    this.owner =  null;                                /* line 22 */
    this.children = [];                                /* line 23 */
    this.visit_ordering =  []                          /* line 24 */;
    this.connections = [];                             /* line 25 */
    this.handler =  null;                              /* line 26 */
    this.reset_instance_data =  null;                  /* line 27 */
    this.finject =  null;                              /* line 28 */
    this.stop =  null;                                 /* line 29 */
    this.instance_data =  null;                        /* line 30 *//*  arg needed for probe support  *//* line 31 */
    this.arg =  "";                                    /* line 32 */
    this.state =  "idle";                              /* line 33 */
    this.special =  false;                             /* line 34 *//*  bootstrap debugging *//* line 35 */
    this.kind =  null;/*  enum { container, leaf, } */ /* line 36 *//* line 37 */
  }
}
                                                       /* line 38 */
function injector (eh,mevent) {                        /* line 39 */
    eh.handler ( eh, mevent)                           /* line 40 *//* line 41 *//* line 42 */
}
