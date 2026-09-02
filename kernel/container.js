function create_down_connector (container,proto_conn,connectors,children_by_id) {/* line 1 */
    /*  JSON: {;dir': 0, 'source': {'name': '', 'id': 0}, 'source_port': '', 'target': {'name': 'Echo', 'id': 12}, 'target_port': ''}, *//* line 2 */
    let  connector =  new Connector ();                /* line 3 */;
    connector.direction =  "down";                     /* line 4 */
    connector.sender = mkSender ( container.name, container, proto_conn [ "source_port"])/* line 5 */;
    let target_proto =  proto_conn [ "target"];        /* line 6 */
    let id_proto =  target_proto [ "id"];              /* line 7 */
    let target_component =  children_by_id [id_proto]; /* line 8 */
    if (( target_component ==  null)) {                /* line 9 */
      load_error ( ( "internal error: .Down connection target internal error ".toString ()+ ( proto_conn [ "target"]) [ "name"].toString ()) )/* line 10 */
    }
    else {                                             /* line 11 */
      connector.receiver = mkReceiver ( target_component.name, target_component, proto_conn [ "target_port"], target_component.inq)/* line 12 */;/* line 13 */
    }
    return  connector;                                 /* line 14 *//* line 15 *//* line 16 */
}

function create_across_connector (container,proto_conn,connectors,children_by_id) {/* line 17 */
    let  connector =  new Connector ();                /* line 18 */;
    connector.direction =  "across";                   /* line 19 */
    let source_component =  children_by_id [(( proto_conn [ "source"]) [ "id"])];/* line 20 */
    let target_component =  children_by_id [(( proto_conn [ "target"]) [ "id"])];/* line 21 */
    if ( source_component ==  null) {                  /* line 22 */
      load_error ( ( "internal error: .Across connection source not ok ".toString ()+ ( proto_conn [ "source"]) [ "name"].toString ()) )/* line 23 */
    }
    else {                                             /* line 24 */
      connector.sender = mkSender ( source_component.name, source_component, proto_conn [ "source_port"])/* line 25 */;
      if ( target_component ==  null) {                /* line 26 */
        load_error ( ( "internal error: .Across connection target not ok ".toString ()+ ( proto_conn [ "target"]) [ "name"].toString ()) )/* line 27 */
      }
      else {                                           /* line 28 */
        connector.receiver = mkReceiver ( target_component.name, target_component, proto_conn [ "target_port"], target_component.inq)/* line 29 */;/* line 30 */
      }                                                /* line 31 */
    }
    return  connector;                                 /* line 32 *//* line 33 *//* line 34 */
}

function create_up_connector (container,proto_conn,connectors,children_by_id) {/* line 35 */
    let  connector =  new Connector ();                /* line 36 */;
    connector.direction =  "up";                       /* line 37 */
    let source_component =  children_by_id [(( proto_conn [ "source"]) [ "id"])];/* line 38 */
    if ( source_component ==  null) {                  /* line 39 */
      load_error ( ( "internal error: .Up connection source not ok ".toString ()+ ( proto_conn [ "source"]) [ "name"].toString ()) )/* line 40 */
    }
    else {                                             /* line 41 */
      connector.sender = mkSender ( source_component.name, source_component, proto_conn [ "source_port"])/* line 42 */;
      connector.receiver = mkReceiver ( container.name, container, proto_conn [ "target_port"], container.outq)/* line 43 */;/* line 44 */
    }
    return  connector;                                 /* line 45 *//* line 46 *//* line 47 */
}

function create_through_connector (container,proto_conn,connectors,children_by_id) {/* line 48 */
    let  connector =  new Connector ();                /* line 49 */;
    connector.direction =  "through";                  /* line 50 */
    connector.sender = mkSender ( container.name, container, proto_conn [ "source_port"])/* line 51 */;
    connector.receiver = mkReceiver ( container.name, container, proto_conn [ "target_port"], container.outq)/* line 52 */;
    return  connector;                                 /* line 53 *//* line 54 *//* line 55 */
}
                                                       /* line 56 */
function container_instantiator (reg,owner,container_name,desc,arg) {/* line 57 *//* line 58 */
    let container = make_container ( container_name, owner)/* line 59 */;
    let children = [];                                 /* line 60 */
    let children_by_id = {};
    /*  not strictly necessary, but, we can remove 1 runtime lookup by "compiling it out“ here *//* line 61 */
    /*  collect children */                            /* line 62 */
    for (let child_desc of  desc [ "children"]) {      /* line 63 */
      let child_instance = get_component_instance ( reg, child_desc [ "name"], container)/* line 64 */;
      children.push ( child_instance)                  /* line 65 */
      let id =  child_desc [ "id"];                    /* line 66 */
      children_by_id [id] =  child_instance;           /* line 67 *//* line 68 *//* line 69 */
    }
    container.children =  children;                    /* line 70 *//* line 71 */
    let connectors = [];                               /* line 72 */
    for (let proto_conn of  desc [ "connections"]) {   /* line 73 */
      let  connector =  new Connector ();              /* line 74 */;
      if ( proto_conn [ "dir"] ==  enumDown) {         /* line 75 */
        connectors.push (create_down_connector ( container, proto_conn, connectors, children_by_id)) /* line 76 */
      }
      else if ( proto_conn [ "dir"] ==  enumAcross) {  /* line 77 */
        connectors.push (create_across_connector ( container, proto_conn, connectors, children_by_id)) /* line 78 */
      }
      else if ( proto_conn [ "dir"] ==  enumUp) {      /* line 79 */
        connectors.push (create_up_connector ( container, proto_conn, connectors, children_by_id)) /* line 80 */
      }
      else if ( proto_conn [ "dir"] ==  enumThrough) { /* line 81 */
        connectors.push (create_through_connector ( container, proto_conn, connectors, children_by_id)) /* line 82 *//* line 83 */
      }                                                /* line 84 */
    }
    container.connections =  connectors;               /* line 85 */
    return  container;                                 /* line 86 *//* line 87 *//* line 88 */
}

/*  The default handler for container components. */   /* line 89 */
function container_handler (container,mevent) {        /* line 90 */
    route ( container, container, mevent)
    /*  references to 'self' are replaced by the container during instantiation *//* line 91 */
    while (any_child_ready ( container)) {             /* line 92 */
      step_children ( container, mevent)               /* line 93 */
    }                                                  /* line 94 *//* line 95 */
}

/*  Stop all children. Reset to a known state. Hit the big red button.  *//* line 96 */
function container_reset_children (container) {        /* line 97 */
    for (let child of  container.children) {           /* line 98 */
      child.stop ( child)                              /* line 99 *//* line 100 */
    }

    container.visit_ordering = [];                     /* line 101 */

    container.routings = [];                           /* line 102 */

    container.inq = [];                                /* line 103 */

    container.outq = [];                               /* line 104 */
    container.state =  "idle";                         /* line 105 *//* line 106 *//* line 107 */
}

/*  Frees the given container and associated data. */  /* line 108 */
function destroy_container (eh) {                      /* line 109 *//* line 110 *//* line 111 */
}

/*  Checks if two senders match, by pointer equality and port name matching. *//* line 112 */
function sender_eq (s1,s2) {                           /* line 113 */
    let same_components = ( s1.component ==  s2.component);/* line 114 */
    let same_ports = ( s1.port ==  s2.port);           /* line 115 */
    return (( same_components) && ( same_ports));      /* line 116 *//* line 117 *//* line 118 */
}

/*  Delivers the given mevent to the receiver of this connector. *//* line 119 *//* line 120 */
function deposit (parent,conn,mevent) {                /* line 121 */
    let new_mevent = make_mevent ( conn.receiver.port, mevent.datum)/* line 122 */;
    push_mevent ( parent, conn.receiver.component, conn.receiver.queue, new_mevent)/* line 123 *//* line 124 *//* line 125 */
}

function force_tick (parent,eh) {                      /* line 126 */
    let tick_mev = make_mevent ( ".",new_datum_bang ())/* line 127 */;
    push_mevent ( parent, eh, eh.inq, tick_mev)        /* line 128 */
    return  tick_mev;                                  /* line 129 *//* line 130 *//* line 131 */
}

function push_mevent (parent,receiver,inq,m) {         /* line 132 */
    inq.push ( m)                                      /* line 133 */
    if (( receiver.special)) {                         /* line 134 */
      parent.visit_ordering.unshift ( receiver)        /* line 135 */
    }
    else {                                             /* line 136 */
      parent.visit_ordering.push ( receiver)           /* line 137 *//* line 138 */
    }                                                  /* line 139 *//* line 140 *//* line 141 */
}

function is_self (child,container) {                   /* line 142 */
    /*  in an earlier version “self“ was denoted as ϕ *//* line 143 */
    return  child ==  container;                       /* line 144 *//* line 145 *//* line 146 */
}

function step_child_once (child,mev) {                 /* line 147 */
    if (( (typeof process.env.PBPSTEPPING !== "undefined") )) {/* line 148 */
      console.error ( ( "-- stepping ❮".toString ()+  ( child.name.toString ()+  "❯".toString ()) .toString ()) );/* line 149 */
                                                       /* line 150 *//* line 151 */
    }
    let before_state =  child.state;                   /* line 152 */
    child.handler ( child, mev)                        /* line 153 */
    let after_state =  child.state;                    /* line 154 */
    return [(( before_state ==  "idle") && ( after_state!= "idle")),(( before_state!= "idle") && ( after_state!= "idle")),(( before_state!= "idle") && ( after_state ==  "idle"))];/* line 157 *//* line 158 *//* line 159 */
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

function append_routing_descriptor (container,desc) {  /* line 253 */
    container.routings.push ( desc)                    /* line 254 *//* line 255 *//* line 256 */
}

function injector (eh,mevent) {                        /* line 257 */
    eh.handler ( eh, mevent)                           /* line 258 *//* line 259 *//* line 260 */
}
                                                       /* line 261 */
/*  Creates a component that acts as a container. It is the same as a `Eh` instance *//* line 262 */
/*  whose handler function is `container_handler`. */  /* line 263 */
function make_container (name,owner) {                 /* line 264 */
    let  eh =  new Eh ();                              /* line 265 */;
    eh.name =  name;                                   /* line 266 */
    eh.owner =  owner;                                 /* line 267 */
    eh.handler =  container_handler;                   /* line 268 */
    eh.finject =  injector;                            /* line 269 */
    eh.stop =  container_reset_children;               /* line 270 */
    eh.state =  "idle";                                /* line 271 */
    eh.kind =  "container";                            /* line 272 */
    return  eh;                                        /* line 273 *//* line 274 *//* line 275 */
}

/*  Sends a mevent on the given `port` with `data`, placing it on the output *//* line 276 */
/*  of the given component. */                         /* line 277 *//* line 278 */
function send (eh,port,obj,causingMevent) {            /* line 279 */
    let  d =  new Datum ();                            /* line 280 */;
    d.v =  obj;                                        /* line 281 */
    d.clone =  function () {return obj_clone ( d)      /* line 282 */;};
    d.reclaim =  null;                                 /* line 283 */
    let mev = make_mevent ( port, d)                   /* line 284 */;
    put_output ( eh, mev)                              /* line 285 *//* line 286 *//* line 287 */
}

function forward (eh,port,mev) {                       /* line 288 */
    let fwdmev = make_mevent ( port, mev.datum)        /* line 289 */;
    put_output ( eh, fwdmev)                           /* line 290 *//* line 291 *//* line 292 */
}

function inject_mevent (eh,mev) {                      /* line 293 */
    eh.finject ( eh, mev)                              /* line 294 *//* line 295 *//* line 296 */
}

function set_active (eh) {                             /* line 297 */
    eh.state =  "active";                              /* line 298 *//* line 299 *//* line 300 */
}

function set_idle (eh) {                               /* line 301 */
    eh.state =  "idle";                                /* line 302 *//* line 303 *//* line 304 */
}

function put_output (eh,mev) {                         /* line 305 */
    eh.outq.push ( mev)                                /* line 306 *//* line 307 *//* line 308 */
}

function obj_clone (obj) {                             /* line 309 */
    return  obj;                                       /* line 310 *//* line 311 */
}
