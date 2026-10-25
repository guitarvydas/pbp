/*  Directions of wires. This section is the only place that knows how a direction is represented *//* line 1 */
/*  (here: strings). To use integers, change the four constants and direction_label; nothing else changes. *//* line 2 */
let directionDown =  "down";                           /* line 3 */
let directionAcross =  "across";                       /* line 4 */
let directionUp =  "up";                               /* line 5 */
let directionThrough =  "through";                     /* line 6 *//* line 7 */
/*  d is the number used for a direction in the diagram JSON: 0 down, 1 across, 2 up, 3 through. *//* line 8 */
/*  Returns ϕ for any other number. */                 /* line 9 */
function direction_of_json (d) {                       /* line 10 *//* line 11 *//* line 12 *//* line 13 *//* line 14 */
    if ( d ==  0) {                                    /* line 15 */
      return  directionDown;                           /* line 16 */
    }
    else if ( d ==  1) {                               /* line 17 */
      return  directionAcross;                         /* line 18 */
    }
    else if ( d ==  2) {                               /* line 19 */
      return  directionUp;                             /* line 20 */
    }
    else if ( d ==  3) {                               /* line 21 */
      return  directionThrough;                        /* line 22 */
    }
    else {                                             /* line 23 */
      return  null;                                    /* line 24 *//* line 25 */
    }                                                  /* line 26 *//* line 27 */
}

/*  direction   sender                   receiver */   /* line 28 */
/*  down        the container            a child */    /* line 29 */
/*  across      a child                  a child */    /* line 30 */
/*  up          a child                  the container *//* line 31 */
/*  through     the container            the container *//* line 32 */
function sender_is_container (direction) {             /* line 33 *//* line 34 *//* line 35 */
    return (( direction ==  directionDown) || ( direction ==  directionThrough));/* line 36 *//* line 37 *//* line 38 */
}

function receiver_is_container (direction) {           /* line 39 *//* line 40 *//* line 41 */
    return (( direction ==  directionUp) || ( direction ==  directionThrough));/* line 42 *//* line 43 *//* line 44 */
}

/*  a name for messages and debugging */               /* line 45 */
function direction_label (direction) {                 /* line 46 */
    return  direction;                                 /* line 47 *//* line 48 *//* line 49 */
}

/*  Routing connection (wire) for a container. The `direction` field has no effect *//* line 50 */
/*  on routing; it is there for debugging, or for reading by other tools. *//* line 51 */
class Connector {
  constructor () {                                     /* line 52 */

    this.direction =  null;/*  down, across, up, through *//* line 53 */
    this.sender =  null;                               /* line 54 */
    this.receiver =  null;                             /* line 55 *//* line 56 */
  }
}
                                                       /* line 57 */
/*  `Sender` is matched on the part (identity) and the port name. *//* line 58 */
class Sender {
  constructor () {                                     /* line 59 */

    this.component =  null;                            /* line 60 */
    this.port =  null;                                 /* line 61 *//* line 62 */
  }
}
                                                       /* line 63 */
/*  `Receiver` is the part and port that a mevent is delivered to. *//* line 64 */
/*  When the receiver is the container itself (up and through wires) the port is an output gate. *//* line 65 */
class Receiver {
  constructor () {                                     /* line 66 */

    this.component =  null;                            /* line 67 */
    this.port =  null;                                 /* line 68 *//* line 69 */
  }
}
                                                       /* line 70 */
function mkSender (component,port) {                   /* line 71 */
    let s =  new Sender ();                            /* line 72 */;
    s.component =  component;                          /* line 73 */
    s.port =  port;                                    /* line 74 */
    return  s;                                         /* line 75 *//* line 76 *//* line 77 */
}

function mkReceiver (component,port) {                 /* line 78 */
    let r =  new Receiver ();                          /* line 79 */;
    r.component =  component;                          /* line 80 */
    r.port =  port;                                    /* line 81 */
    return  r;                                         /* line 82 *//* line 83 *//* line 84 */
}

function mkConnector (direction,sender,receiver) {     /* line 85 */
    let c =  new Connector ();                         /* line 86 */;
    c.direction =  direction;                          /* line 87 */
    c.sender =  sender;                                /* line 88 */
    c.receiver =  receiver;                            /* line 89 */
    return  c;                                         /* line 90 *//* line 91 *//* line 92 */
}

/*  JSON: {;dir': 0, 'source': {'name': '', 'id': 0}, 'source_port': '', 'target': {'name': 'Echo', 'id': 12}, 'target_port': ''}, *//* line 93 */
/*  The sender port is proto_conn@"source_port" and the receiver port is proto_conn@"target_port". *//* line 94 */
/*  A child sender is proto_conn@"source", a child receiver is proto_conn@"target". *//* line 95 */
/*  Returns ϕ if the wire can not be made. */          /* line 96 */
function create_connector (container,proto_conn,children_by_id) {/* line 97 */
    let direction = direction_of_json ( proto_conn ["dir"])/* line 98 */;
    if ( direction ==  null) {                         /* line 99 */
      load_error ( ( "unknown wire direction ".toString ()+ `${ proto_conn ["dir"]}`.toString ()) )/* line 100 */
      return  null;                                    /* line 101 *//* line 102 */
    }
    let sender_component =  container;                 /* line 103 */
    if ((! (sender_is_container ( direction)))) {      /* line 104 */
      let sid = ( proto_conn ["source"]) ["id"];       /* line 105 */
      sender_component =  children_by_id [sid];        /* line 106 *//* line 107 */
    }
    let receiver_component =  container;               /* line 108 */
    if ((! (receiver_is_container ( direction)))) {    /* line 109 */
      let tid = ( proto_conn ["target"]) ["id"];       /* line 110 */
      receiver_component =  children_by_id [tid];      /* line 111 *//* line 112 */
    }
    if ((( sender_component ==  null) || ( receiver_component ==  null))) {/* line 113 */
      load_error ( (direction_label ( direction).toString ()+  " wire: no such source or target".toString ()) )/* line 114 */
      return  null;                                    /* line 115 *//* line 116 */
    }
    return mkConnector ( direction,mkSender ( sender_component, proto_conn ["source_port"]),mkReceiver ( receiver_component, proto_conn ["target_port"]))/* line 119 */;/* line 120 */
}
