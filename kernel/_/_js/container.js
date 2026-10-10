function create_down_connector (container,proto_conn,connectors,children_by_id) {/* line 1 */
    /*  JSON: {;dir': 0, 'source': {'name': '', 'id': 0}, 'source_port': '', 'target': {'name': 'Echo', 'id': 12}, 'target_port': ''}, *//* line 2 */
    let  connector =  new Connector ();                /* line 3 */;
    connector.direction =  "down";                     /* line 4 */
    connector.sender = mkSender ( container.name, container, proto_conn ["source_port"])/* line 5 */;
    let target_proto =  proto_conn ["target"];         /* line 6 */
    let id_proto =  target_proto ["id"];               /* line 7 */
    let target_component =  children_by_id [id_proto]; /* line 8 */
    if (( target_component ==  null)) {                /* line 9 */
      load_error ( ( "internal error: .Down connection target internal error ".toString ()+ ( proto_conn ["target"]) ["name"].toString ()) )/* line 10 */
    }
    else {                                             /* line 11 */
      connector.receiver = mkReceiver ( target_component.name, target_component, proto_conn ["target_port"], target_component.inq)/* line 12 */;/* line 13 */
    }
    return  connector;                                 /* line 14 *//* line 15 *//* line 16 */
}

function create_across_connector (container,proto_conn,connectors,children_by_id) {/* line 17 */
    let  connector =  new Connector ();                /* line 18 */;
    connector.direction =  "across";                   /* line 19 */
    let sid = ( proto_conn ["source"]) ["id"];         /* line 20 */
    let source_component =  children_by_id [sid];      /* line 21 */
    let tid = ( proto_conn ["target"]) ["id"];         /* line 22 */
    let target_component =  children_by_id [tid];      /* line 23 */
    if ( source_component ==  null) {                  /* line 24 */
      load_error ( ( "internal error: .Across connection source not ok ".toString ()+ ( proto_conn ["source"]) ["name"].toString ()) )/* line 25 */
    }
    else {                                             /* line 26 */
      connector.sender = mkSender ( source_component.name, source_component, proto_conn ["source_port"])/* line 27 */;
      if ( target_component ==  null) {                /* line 28 */
        load_error ( ( "internal error: .Across connection target not ok ".toString ()+ ( proto_conn ["target"]) ["name"].toString ()) )/* line 29 */
      }
      else {                                           /* line 30 */
        connector.receiver = mkReceiver ( target_component.name, target_component, proto_conn ["target_port"], target_component.inq)/* line 31 */;/* line 32 */
      }                                                /* line 33 */
    }
    return  connector;                                 /* line 34 *//* line 35 *//* line 36 */
}

function create_up_connector (container,proto_conn,connectors,children_by_id) {/* line 37 */
    let  connector =  new Connector ();                /* line 38 */;
    connector.direction =  "up";                       /* line 39 */
    let sid = ( proto_conn ["source"]) ["id"];         /* line 40 */
    let source_component =  children_by_id [sid];      /* line 41 */
    if ( source_component ==  null) {                  /* line 42 */
      load_error ( ( "internal error: .Up connection source not ok ".toString ()+ ( proto_conn ["source"]) ["name"].toString ()) )/* line 43 */
    }
    else {                                             /* line 44 */
      connector.sender = mkSender ( source_component.name, source_component, proto_conn ["source_port"])/* line 45 */;
      connector.receiver = mkReceiver ( container.name, container, proto_conn ["target_port"], container.outq)/* line 46 */;/* line 47 */
    }
    return  connector;                                 /* line 48 *//* line 49 *//* line 50 */
}

function create_through_connector (container,proto_conn,connectors,children_by_id) {/* line 51 */
    let  connector =  new Connector ();                /* line 52 */;
    connector.direction =  "through";                  /* line 53 */
    connector.sender = mkSender ( container.name, container, proto_conn ["source_port"])/* line 54 */;
    connector.receiver = mkReceiver ( container.name, container, proto_conn ["target_port"], container.outq)/* line 55 */;
    return  connector;                                 /* line 56 *//* line 57 *//* line 58 */
}
                                                       /* line 59 */
function container_instantiator (reg,owner,container_name,desc,arg) {/* line 60 *//* line 61 *//* line 62 *//* line 63 *//* line 64 */
    let container = make_container ( container_name, owner)/* line 65 */;
    let children = [];                                 /* line 66 */
    let children_by_id = {};
    /*  not strictly necessary, but, we can remove 1 runtime lookup by "compiling it out“ here *//* line 67 */
    /*  collect children */                            /* line 68 */
    for (let child_desc of  desc ["children"]) {       /* line 69 */
      let child_instance = get_component_instance ( reg, child_desc ["name"], container)/* line 70 */;
      children.push ( child_instance)                  /* line 71 */
      let id =  child_desc ["id"];                     /* line 72 */
      children_by_id [id] =  child_instance;           /* line 73 *//* line 74 *//* line 75 */
    }
    container.children =  children;                    /* line 76 *//* line 77 */
    let connectors = [];                               /* line 78 */
    for (let proto_conn of  desc ["connections"]) {    /* line 79 */
      let  connector =  new Connector ();              /* line 80 */;
      if ( proto_conn ["dir"] ==  enumDown) {          /* line 81 */
        connectors.push (create_down_connector ( container, proto_conn, connectors, children_by_id)) /* line 82 */
      }
      else if ( proto_conn ["dir"] ==  enumAcross) {   /* line 83 */
        connectors.push (create_across_connector ( container, proto_conn, connectors, children_by_id)) /* line 84 */
      }
      else if ( proto_conn ["dir"] ==  enumUp) {       /* line 85 */
        connectors.push (create_up_connector ( container, proto_conn, connectors, children_by_id)) /* line 86 */
      }
      else if ( proto_conn ["dir"] ==  enumThrough) {  /* line 87 */
        connectors.push (create_through_connector ( container, proto_conn, connectors, children_by_id)) /* line 88 *//* line 89 */
      }                                                /* line 90 */
    }
    container.connections =  connectors;               /* line 91 */
    return  container;                                 /* line 92 *//* line 93 *//* line 94 */
}

/*  The default handler for container components. */   /* line 95 */
function container_handler (container,mevent) {        /* line 96 */
    route ( container, container, mevent)
    /*  references to 'self' are replaced by the container during instantiation *//* line 97 */
    while (any_child_ready ( container)) {             /* line 98 */
      step_children ( container, mevent)               /* line 99 */
    }                                                  /* line 100 *//* line 101 */
}

/*  Stop all children. Reset to a known state. Hit the big red button.  *//* line 102 */
function container_reset (container) {                 /* line 103 */
    for (let child of  container.children) {           /* line 104 */
      child.reset ( child)                             /* line 105 *//* line 106 */
    }

    container.visit_ordering = [];                     /* line 107 */

    container.inq = [];                                /* line 108 */

    container.outq = [];                               /* line 109 */
    container.state =  "idle";                         /* line 110 *//* line 111 *//* line 112 */
}

/*  Frees the given container and associated data. */  /* line 113 */
function destroy_container (eh) {                      /* line 114 *//* line 115 *//* line 116 */
}

/*  Checks if two senders match, by pointer equality and port name matching. *//* line 117 */
function sender_eq (s1,s2) {                           /* line 118 */
    let same_components = ( s1.component ==  s2.component);/* line 119 */
    let same_ports = ( s1.port ==  s2.port);           /* line 120 */
    return (( same_components) && ( same_ports));      /* line 121 *//* line 122 *//* line 123 */
}

/*  Delivers the given mevent to the receiver of this connector. *//* line 124 *//* line 125 */
function deposit (parent,conn,mevent) {                /* line 126 */
    let new_mevent = make_mevent ( conn.receiver.port, mevent.payload)/* line 127 */;
    push_mevent ( parent, conn.receiver.component, conn.receiver.queue, new_mevent)/* line 128 *//* line 129 *//* line 130 */
}

function force_tick (parent,eh) {                      /* line 131 */
    let tick_mev = make_mevent ( ".",new_datum_bang ())/* line 132 */;
    push_mevent ( parent, eh, eh.inq, tick_mev)        /* line 133 */
    return  tick_mev;                                  /* line 134 *//* line 135 *//* line 136 */
}

function push_mevent (parent,receiver,inq,m) {         /* line 137 */
    inq.push ( m)                                      /* line 138 */
    if (( receiver.special)) {                         /* line 139 */
      parent.visit_ordering.unshift ( receiver)        /* line 140 */
    }
    else {                                             /* line 141 */
      parent.visit_ordering.push ( receiver)           /* line 142 *//* line 143 */
    }                                                  /* line 144 *//* line 145 *//* line 146 */
}

function is_self (child,container) {                   /* line 147 */
    /*  in an earlier version “self“ was denoted as ϕ *//* line 148 */
    return  child ==  container;                       /* line 149 *//* line 150 *//* line 151 */
}

function step_child_once (child,mev) {                 /* line 152 */
    if (( (typeof process.env.PBPSTEPPING !== "undefined") )) {/* line 153 */
      console.error ( ( "-- stepping ❮".toString ()+  ( child.name.toString ()+  "❯".toString ()) .toString ()) );/* line 154 */
                                                       /* line 155 *//* line 156 */
    }
    child.handler ( child, mev)                        /* line 157 *//* line 158 *//* line 159 */
}

function step_children (container,causingMevent) {     /* line 160 */
    container.state =  "idle";                         /* line 161 *//* line 162 */
    /*  phase 1 - loop through children and process inputs or children that not "idle"  *//* line 163 */
    for (let child of   container.visit_ordering) {    /* line 164 */
      /*  child = container represents self, skip it *//* line 165 */
      if (((! (is_self ( child, container))))) {       /* line 166 */
        if (((! ((0=== child.inq.length))))) {         /* line 167 */
          let mev =  child.inq.shift ()                /* line 168 */;
          step_child_once ( child, mev)                /* line 169 *//* line 170 */
          destroy_mevent ( mev)                        /* line 171 */
        }
        else {                                         /* line 172 */
          if ( child.state ==  "idle") {               /* line 173 *//* line 174 */
          }
          else {                                       /* line 175 */
            let mev = force_tick ( container, child)   /* line 176 */;
            step_child_once ( child, mev)              /* line 177 */
            destroy_mevent ( mev)                      /* line 178 *//* line 179 */
          }                                            /* line 180 */
        }                                              /* line 181 */
      }                                                /* line 182 */
    }

    container.visit_ordering = [];                     /* line 183 *//* line 184 */
    /*  phase 2 - loop through children and route their outputs to appropriate receiver queues based on .connections  *//* line 185 */
    for (let child of  container.children) {           /* line 186 */
      if ( child.state ==  "active") {                 /* line 187 */
        /*  if child remains active, then the container must remain active and must propagate “ticks“ to child *//* line 188 */
        container.state =  "active";                   /* line 189 *//* line 190 */
      }                                                /* line 191 */
      while (((! ((0=== child.outq.length))))) {       /* line 192 */
        let mev =  child.outq.shift ()                 /* line 193 */;
        route ( container, child, mev)                 /* line 194 */
        destroy_mevent ( mev)                          /* line 195 *//* line 196 */
      }                                                /* line 197 */
    }                                                  /* line 198 *//* line 199 */
}

function attempt_tick (parent,eh) {                    /* line 200 */
    if ( eh.state!= "idle") {                          /* line 201 */
      force_tick ( parent, eh)                         /* line 202 *//* line 203 */
    }                                                  /* line 204 *//* line 205 */
}

function is_tick (mev) {                               /* line 206 */
    return  "." ==  mev.port
    /*  assume that any mevent that is sent to port "." is a tick  *//* line 207 */;/* line 208 *//* line 209 */
}

/*  Routes a single mevent to all matching destinations, according to *//* line 210 */
/*  the container's connection network. */             /* line 211 *//* line 212 */
function route (container,from_component,mevent) {     /* line 213 */
    let  was_sent =  false;
    /*  for checking that output went somewhere (at least during bootstrap) *//* line 214 */
    let  fromname =  "";                               /* line 215 *//* line 216 */
    ticktime =  ticktime+ 1;                           /* line 217 */
    if (is_tick ( mevent)) {                           /* line 218 */
      for (let child of  container.children) {         /* line 219 */
        attempt_tick ( container, child)               /* line 220 */
      }
      was_sent =  true;                                /* line 221 */
    }
    else {                                             /* line 222 */
      if (((! (is_self ( from_component, container))))) {/* line 223 */
        fromname =  from_component.name;               /* line 224 *//* line 225 */
      }
      let from_sender = mkSender ( fromname, from_component, mevent.port)/* line 226 */;/* line 227 */
      for (let connector of  container.connections) {  /* line 228 */
        if (sender_eq ( from_sender, connector.sender)) {/* line 229 */
          deposit ( container, connector, mevent)      /* line 230 */
          was_sent =  true;                            /* line 231 *//* line 232 */
        }                                              /* line 233 */
      }                                                /* line 234 */
    }
    if ((! ( was_sent))) {                             /* line 235 */
      console.error ( "internal error" + ": " +  ( container.name.toString ()+  ( ": mevent on port '".toString ()+  ( mevent.port.toString ()+  ( "' from ".toString ()+  ( fromname.toString ()+  " dropped on floor...".toString ()) .toString ()) .toString ()) .toString ()) .toString ()) )/* line 236 *//* line 237 */
    }                                                  /* line 238 *//* line 239 */
}

function any_child_ready (container) {                 /* line 240 */
    for (let child of  container.children) {           /* line 241 */
      if (child_is_ready ( child)) {                   /* line 242 */
        return  true;                                  /* line 243 *//* line 244 */
      }                                                /* line 245 */
    }
    return  false;                                     /* line 246 *//* line 247 *//* line 248 */
}

function child_is_ready (eh) {                         /* line 249 */
    return ((((((((! ((0=== eh.outq.length))))) || (((! ((0=== eh.inq.length))))))) || (( eh.state!= "idle")))) || ((any_child_ready ( eh))));/* line 250 *//* line 251 *//* line 252 */
}
                                                       /* line 253 */
/*  Creates a component that acts as a container. It is the same as a `Eh` instance *//* line 254 */
/*  whose handler function is `container_handler`. */  /* line 255 */
function make_container (name,owner) {                 /* line 256 */
    let  eh =  new Eh ();                              /* line 257 */;
    eh.name =  name;                                   /* line 258 */
    eh.owner =  owner;                                 /* line 259 */
    eh.handler =  container_handler;                   /* line 260 */
    eh.finject =  injector;                            /* line 261 */
    eh.reset =  container_reset;                       /* line 262 */
    eh.state =  "idle";                                /* line 263 */
    eh.kind =  "container";                            /* line 264 */
    return  eh;                                        /* line 265 *//* line 266 *//* line 267 */
}

/*  Sends a mevent on the given `port` with `data`, placing it on the output *//* line 268 */
/*  of the given component. */                         /* line 269 *//* line 270 */
function send (eh,port,obj,causingMevent) {            /* line 271 */
    let  d =  new Datum ();                            /* line 272 */;
    d.v =  obj;                                        /* line 273 */
    d.clone =  function () {return obj_clone ( d)      /* line 274 */;};
    d.reclaim =  null;                                 /* line 275 */
    let mev = make_mevent ( port, d)                   /* line 276 */;
    put_output ( eh, mev)                              /* line 277 *//* line 278 *//* line 279 */
}

function forward (eh,port,mev) {                       /* line 280 */
    let fwdmev = make_mevent ( port, mev.payload)      /* line 281 */;
    put_output ( eh, fwdmev)                           /* line 282 *//* line 283 *//* line 284 */
}

function inject_mevent (eh,mev) {                      /* line 285 */
    eh.finject ( eh, mev)                              /* line 286 *//* line 287 *//* line 288 */
}

function set_active (eh) {                             /* line 289 */
    eh.state =  "active";                              /* line 290 *//* line 291 *//* line 292 */
}

function set_idle (eh) {                               /* line 293 */
    eh.state =  "idle";                                /* line 294 *//* line 295 *//* line 296 */
}

function put_output (eh,mev) {                         /* line 297 */
    eh.outq.push ( mev)                                /* line 298 *//* line 299 *//* line 300 */
}

function obj_clone (obj) {                             /* line 301 */
    return  obj;                                       /* line 302 *//* line 303 */
}
