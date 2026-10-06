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
/*  Eh_States :: enum { idle, active } */              /* line 15 */
class Eh {
  constructor () {                                     /* line 16 */

    this.name =  "";                                   /* line 17 */
    this.inq =  []                                     /* line 18 */;
    this.outq =  []                                    /* line 19 */;
    this.owner =  null;                                /* line 20 */
    this.children = [];                                /* line 21 */
    this.visit_ordering =  []                          /* line 22 */;
    this.connections = [];                             /* line 23 */
    this.handler =  null;                              /* line 24 */
    this.finject =  null;                              /* line 25 */
    this.reset =  null;                                /* line 26 */
    this.instance_data =  null;                        /* line 27 *//*  arg needed for probe support  *//* line 28 */
    this.arg =  "";                                    /* line 29 */
    this.state =  "idle";                              /* line 30 */
    this.special =  false;                             /* line 31 *//* line 32 */
  }
}
                                                       /* line 33 */
function injector (eh,mevent) {                        /* line 34 */
    eh.handler ( eh, mevent)                           /* line 35 *//* line 36 *//* line 37 */
}
