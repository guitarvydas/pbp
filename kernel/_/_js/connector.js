/* line 1 */
const  enumDown =  0                                   /* line 2 */;
const  enumAcross =  1                                 /* line 3 */;
const  enumUp =  2                                     /* line 4 */;
const  enumThrough =  3                                /* line 5 */;/* line 6 *//* line 7 */
/*  Routing connection for a container component. The `direction` field has *//* line 8 */
/*  no affect on the default mevent routing system _ it is there for debugging *//* line 9 */
/*  purposes, or for reading by other tools. */        /* line 10 *//* line 11 */
class Connector {
  constructor () {                                     /* line 12 */

    this.direction =  null;/*  down, across, up, through *//* line 13 */
    this.sender =  null;                               /* line 14 */
    this.receiver =  null;                             /* line 15 *//* line 16 */
  }
}
                                                       /* line 17 */
/*  `Sender` is used to "pattern match“ which `Receiver` a mevent should go to, *//* line 18 */
/*  based on component ID (pointer) and port name. */  /* line 19 *//* line 20 */
class Sender {
  constructor () {                                     /* line 21 */

    this.name =  null;                                 /* line 22 */
    this.component =  null;                            /* line 23 */
    this.port =  null;                                 /* line 24 *//* line 25 */
  }
}
                                                       /* line 26 *//* line 27 *//* line 28 */
/*  `Receiver` is a handle to a destination queue, and a `port` name to assign *//* line 29 */
/*  to incoming mevents to this queue. */              /* line 30 *//* line 31 */
class Receiver {
  constructor () {                                     /* line 32 */

    this.name =  null;                                 /* line 33 */
    this.queue =  null;                                /* line 34 */
    this.port =  null;                                 /* line 35 */
    this.component =  null;                            /* line 36 *//* line 37 */
  }
}
                                                       /* line 38 */
function mkSender (name,component,port) {              /* line 39 */
    let  s =  new Sender ();                           /* line 40 */;
    s.name =  name;                                    /* line 41 */
    s.component =  component;                          /* line 42 */
    s.port =  port;                                    /* line 43 */
    return  s;                                         /* line 44 *//* line 45 *//* line 46 */
}

function mkReceiver (name,component,port,q) {          /* line 47 */
    let  r =  new Receiver ();                         /* line 48 */;
    r.name =  name;                                    /* line 49 */
    r.component =  component;                          /* line 50 */
    r.port =  port;                                    /* line 51 */
    /*  We need a way to determine which queue to target. "Down" and "Across" go to inq, "Up" and "Through" go to outq. *//* line 52 */
    r.queue =  q;                                      /* line 53 */
    return  r;                                         /* line 54 *//* line 55 */
}
