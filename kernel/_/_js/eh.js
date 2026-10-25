/*  Runtime data of a Part. */                         /* line 1 */
/*  Parts are either leaves (a user-supplied handler) or containers (a handler that routes mevents to children). *//* line 2 */
/*  `handler` is called as handler (eh, mev). */       /* line 3 */
/*  `instance_data` is private to the leaf; the handler finds it again each time it is invoked. *//* line 4 */
/*  `visit_queue` (containers only) holds the children that have been sent a mevent, once for each mevent. *//* line 5 */
/*  `tick` is a function (eh) that continues the processing of an active part. make_container sets it to container_tick; the programmer supplies it to make_leaf. *//* line 6 */
/*  `reset` is a function (eh) that puts the part back into a known state. make_container sets it to container_reset; the programmer supplies it to make_leaf. *//* line 7 */
/*  Eh_States :: enum { idle, active } */              /* line 8 */
class Eh {
  constructor () {                                     /* line 9 */

    this.name =  "";                                   /* line 10 */
    this.inq =  []                                     /* line 11 */;
    this.outq =  []                                    /* line 12 */;
    this.owner =  null;                                /* line 13 */
    this.children = [];                                /* line 14 */
    this.visit_queue =  []                             /* line 15 */;
    this.connections = [];                             /* line 16 */
    this.handler =  null;                              /* line 17 */
    this.tick =  null;                                 /* line 18 */
    this.reset =  null;                                /* line 19 */
    this.instance_data =  null;                        /* line 20 */
    this.arg =  "";                                    /* line 21 */
    this.state =  "idle";                              /* line 22 */
    this.vip =  false;                                 /* line 23 *//* line 24 */
  }
}
