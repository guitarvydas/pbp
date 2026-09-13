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
    let new_mevent = make_mevent ( conn.receiver.port, mevent.payload)/* line 122 */;
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
    child.handler ( child, mev)                        /* line 152 *//* line 153 *//* line 154 */
}

function step_children (container,causingMevent) {     /* line 155 */
    container.state =  "idle";                         /* line 156 *//* line 157 */
    /*  phase 1 - loop through children and process inputs or children that not "idle"  *//* line 158 */
    for (let child of   container.visit_ordering) {    /* line 159 */
      /*  child = container represents self, skip it *//* line 160 */
      if (((! (is_self ( child, container))))) {       /* line 161 */
        if (((! ((0=== child.inq.length))))) {         /* line 162 */
          let mev =  child.inq.shift ()                /* line 163 */;
          step_child_once ( child, mev)                /* line 164 *//* line 165 */
          destroy_mevent ( mev)                        /* line 166 */
        }
        else {                                         /* line 167 */
          if ( child.state ==  "idle") {               /* line 168 *//* line 169 */
          }
          else {                                       /* line 170 */
            let mev = force_tick ( container, child)   /* line 171 */;
            step_child_once ( child, mev)              /* line 172 */
            destroy_mevent ( mev)                      /* line 173 *//* line 174 */
          }                                            /* line 175 */
        }                                              /* line 176 */
      }                                                /* line 177 */
    }

    container.visit_ordering = [];                     /* line 178 *//* line 179 */
    /*  phase 2 - loop through children and route their outputs to appropriate receiver queues based on .connections  *//* line 180 */
    for (let child of  container.children) {           /* line 181 */
      if ( child.state ==  "active") {                 /* line 182 */
        /*  if child remains active, then the container must remain active and must propagate “ticks“ to child *//* line 183 */
        container.state =  "active";                   /* line 184 *//* line 185 */
      }                                                /* line 186 */
      while (((! ((0=== child.outq.length))))) {       /* line 187 */
        let mev =  child.outq.shift ()                 /* line 188 */;
        route ( container, child, mev)                 /* line 189 */
        destroy_mevent ( mev)                          /* line 190 *//* line 191 */
      }                                                /* line 192 */
    }                                                  /* line 193 *//* line 194 */
}

function attempt_tick (parent,eh) {                    /* line 195 */
    if ( eh.state!= "idle") {                          /* line 196 */
      force_tick ( parent, eh)                         /* line 197 *//* line 198 */
    }                                                  /* line 199 *//* line 200 */
}

function is_tick (mev) {                               /* line 201 */
    return  "." ==  mev.port
    /*  assume that any mevent that is sent to port "." is a tick  *//* line 202 */;/* line 203 *//* line 204 */
}

/*  Routes a single mevent to all matching destinations, according to *//* line 205 */
/*  the container's connection network. */             /* line 206 *//* line 207 */
function route (container,from_component,mevent) {     /* line 208 */
    let  was_sent =  false;
    /*  for checking that output went somewhere (at least during bootstrap) *//* line 209 */
    let  fromname =  "";                               /* line 210 *//* line 211 */
    ticktime =  ticktime+ 1;                           /* line 212 */
    if (is_tick ( mevent)) {                           /* line 213 */
      for (let child of  container.children) {         /* line 214 */
        attempt_tick ( container, child)               /* line 215 */
      }
      was_sent =  true;                                /* line 216 */
    }
    else {                                             /* line 217 */
      if (((! (is_self ( from_component, container))))) {/* line 218 */
        fromname =  from_component.name;               /* line 219 *//* line 220 */
      }
      let from_sender = mkSender ( fromname, from_component, mevent.port)/* line 221 */;/* line 222 */
      for (let connector of  container.connections) {  /* line 223 */
        if (sender_eq ( from_sender, connector.sender)) {/* line 224 */
          deposit ( container, connector, mevent)      /* line 225 */
          was_sent =  true;                            /* line 226 *//* line 227 */
        }                                              /* line 228 */
      }                                                /* line 229 */
    }
    if ((! ( was_sent))) {                             /* line 230 */
      console.error ( "internal error" + ": " +  ( container.name.toString ()+  ( ": mevent on port '".toString ()+  ( mevent.port.toString ()+  ( "' from ".toString ()+  ( fromname.toString ()+  " dropped on floor...".toString ()) .toString ()) .toString ()) .toString ()) .toString ()) )/* line 231 *//* line 232 */
    }                                                  /* line 233 *//* line 234 */
}

function any_child_ready (container) {                 /* line 235 */
    for (let child of  container.children) {           /* line 236 */
      if (child_is_ready ( child)) {                   /* line 237 */
        return  true;                                  /* line 238 *//* line 239 */
      }                                                /* line 240 */
    }
    return  false;                                     /* line 241 *//* line 242 *//* line 243 */
}

function child_is_ready (eh) {                         /* line 244 */
    return ((((((((! ((0=== eh.outq.length))))) || (((! ((0=== eh.inq.length))))))) || (( eh.state!= "idle")))) || ((any_child_ready ( eh))));/* line 245 *//* line 246 *//* line 247 */
}

function append_routing_descriptor (container,desc) {  /* line 248 */
    container.routings.push ( desc)                    /* line 249 *//* line 250 *//* line 251 */
}
                                                       /* line 252 */
/*  Creates a component that acts as a container. It is the same as a `Eh` instance *//* line 253 */
/*  whose handler function is `container_handler`. */  /* line 254 */
function make_container (name,owner) {                 /* line 255 */
    let  eh =  new Eh ();                              /* line 256 */;
    eh.name =  name;                                   /* line 257 */
    eh.owner =  owner;                                 /* line 258 */
    eh.handler =  container_handler;                   /* line 259 */
    eh.finject =  injector;                            /* line 260 */
    eh.stop =  container_reset_children;               /* line 261 */
    eh.state =  "idle";                                /* line 262 */
    eh.kind =  "container";                            /* line 263 */
    return  eh;                                        /* line 264 *//* line 265 *//* line 266 */
}

/*  Sends a mevent on the given `port` with `data`, placing it on the output *//* line 267 */
/*  of the given component. */                         /* line 268 *//* line 269 */
function send (eh,port,obj,causingMevent) {            /* line 270 */
    let  d =  new Datum ();                            /* line 271 */;
    d.v =  obj;                                        /* line 272 */
    d.clone =  function () {return obj_clone ( d)      /* line 273 */;};
    d.reclaim =  null;                                 /* line 274 */
    let mev = make_mevent ( port, d)                   /* line 275 */;
    put_output ( eh, mev)                              /* line 276 *//* line 277 *//* line 278 */
}

function forward (eh,port,mev) {                       /* line 279 */
    let fwdmev = make_mevent ( port, mev.payload)      /* line 280 */;
    put_output ( eh, fwdmev)                           /* line 281 *//* line 282 *//* line 283 */
}

function inject_mevent (eh,mev) {                      /* line 284 */
    eh.finject ( eh, mev)                              /* line 285 *//* line 286 *//* line 287 */
}

function set_active (eh) {                             /* line 288 */
    eh.state =  "active";                              /* line 289 *//* line 290 *//* line 291 */
}

function set_idle (eh) {                               /* line 292 */
    eh.state =  "idle";                                /* line 293 *//* line 294 *//* line 295 */
}

function put_output (eh,mev) {                         /* line 296 */
    eh.outq.push ( mev)                                /* line 297 *//* line 298 *//* line 299 */
}

function obj_clone (obj) {                             /* line 300 */
    return  obj;                                       /* line 301 *//* line 302 */
}
