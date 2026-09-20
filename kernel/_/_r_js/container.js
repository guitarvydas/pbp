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

    container.inq = [];                                /* line 102 */

    container.outq = [];                               /* line 103 */
    container.state =  "idle";                         /* line 104 *//* line 105 *//* line 106 */
}

/*  Frees the given container and associated data. */  /* line 107 */
function destroy_container (eh) {                      /* line 108 *//* line 109 *//* line 110 */
}

/*  Checks if two senders match, by pointer equality and port name matching. *//* line 111 */
function sender_eq (s1,s2) {                           /* line 112 */
    let same_components = ( s1.component ==  s2.component);/* line 113 */
    let same_ports = ( s1.port ==  s2.port);           /* line 114 */
    return (( same_components) && ( same_ports));      /* line 115 *//* line 116 *//* line 117 */
}

/*  Delivers the given mevent to the receiver of this connector. *//* line 118 *//* line 119 */
function deposit (parent,conn,mevent) {                /* line 120 */
    let new_mevent = make_mevent ( conn.receiver.port, mevent.payload)/* line 121 */;
    push_mevent ( parent, conn.receiver.component, conn.receiver.queue, new_mevent)/* line 122 *//* line 123 *//* line 124 */
}

function force_tick (parent,eh) {                      /* line 125 */
    let tick_mev = make_mevent ( ".",new_datum_bang ())/* line 126 */;
    push_mevent ( parent, eh, eh.inq, tick_mev)        /* line 127 */
    return  tick_mev;                                  /* line 128 *//* line 129 *//* line 130 */
}

function push_mevent (parent,receiver,inq,m) {         /* line 131 */
    inq.push ( m)                                      /* line 132 */
    if (( receiver.special)) {                         /* line 133 */
      parent.visit_ordering.unshift ( receiver)        /* line 134 */
    }
    else {                                             /* line 135 */
      parent.visit_ordering.push ( receiver)           /* line 136 *//* line 137 */
    }                                                  /* line 138 *//* line 139 *//* line 140 */
}

function is_self (child,container) {                   /* line 141 */
    /*  in an earlier version “self“ was denoted as ϕ *//* line 142 */
    return  child ==  container;                       /* line 143 *//* line 144 *//* line 145 */
}

function step_child_once (child,mev) {                 /* line 146 */
    if (( (typeof process.env.PBPSTEPPING !== "undefined") )) {/* line 147 */
      console.error ( ( "-- stepping ❮".toString ()+  ( child.name.toString ()+  "❯".toString ()) .toString ()) );/* line 148 */
                                                       /* line 149 *//* line 150 */
    }
    child.handler ( child, mev)                        /* line 151 *//* line 152 *//* line 153 */
}

function step_children (container,causingMevent) {     /* line 154 */
    container.state =  "idle";                         /* line 155 *//* line 156 */
    /*  phase 1 - loop through children and process inputs or children that not "idle"  *//* line 157 */
    for (let child of   container.visit_ordering) {    /* line 158 */
      /*  child = container represents self, skip it *//* line 159 */
      if (((! (is_self ( child, container))))) {       /* line 160 */
        if (((! ((0=== child.inq.length))))) {         /* line 161 */
          let mev =  child.inq.shift ()                /* line 162 */;
          step_child_once ( child, mev)                /* line 163 *//* line 164 */
          destroy_mevent ( mev)                        /* line 165 */
        }
        else {                                         /* line 166 */
          if ( child.state ==  "idle") {               /* line 167 *//* line 168 */
          }
          else {                                       /* line 169 */
            let mev = force_tick ( container, child)   /* line 170 */;
            step_child_once ( child, mev)              /* line 171 */
            destroy_mevent ( mev)                      /* line 172 *//* line 173 */
          }                                            /* line 174 */
        }                                              /* line 175 */
      }                                                /* line 176 */
    }

    container.visit_ordering = [];                     /* line 177 *//* line 178 */
    /*  phase 2 - loop through children and route their outputs to appropriate receiver queues based on .connections  *//* line 179 */
    for (let child of  container.children) {           /* line 180 */
      if ( child.state ==  "active") {                 /* line 181 */
        /*  if child remains active, then the container must remain active and must propagate “ticks“ to child *//* line 182 */
        container.state =  "active";                   /* line 183 *//* line 184 */
      }                                                /* line 185 */
      while (((! ((0=== child.outq.length))))) {       /* line 186 */
        let mev =  child.outq.shift ()                 /* line 187 */;
        route ( container, child, mev)                 /* line 188 */
        destroy_mevent ( mev)                          /* line 189 *//* line 190 */
      }                                                /* line 191 */
    }                                                  /* line 192 *//* line 193 */
}

function attempt_tick (parent,eh) {                    /* line 194 */
    if ( eh.state!= "idle") {                          /* line 195 */
      force_tick ( parent, eh)                         /* line 196 *//* line 197 */
    }                                                  /* line 198 *//* line 199 */
}

function is_tick (mev) {                               /* line 200 */
    return  "." ==  mev.port
    /*  assume that any mevent that is sent to port "." is a tick  *//* line 201 */;/* line 202 *//* line 203 */
}

/*  Routes a single mevent to all matching destinations, according to *//* line 204 */
/*  the container's connection network. */             /* line 205 *//* line 206 */
function route (container,from_component,mevent) {     /* line 207 */
    let  was_sent =  false;
    /*  for checking that output went somewhere (at least during bootstrap) *//* line 208 */
    let  fromname =  "";                               /* line 209 *//* line 210 */
    ticktime =  ticktime+ 1;                           /* line 211 */
    if (is_tick ( mevent)) {                           /* line 212 */
      for (let child of  container.children) {         /* line 213 */
        attempt_tick ( container, child)               /* line 214 */
      }
      was_sent =  true;                                /* line 215 */
    }
    else {                                             /* line 216 */
      if (((! (is_self ( from_component, container))))) {/* line 217 */
        fromname =  from_component.name;               /* line 218 *//* line 219 */
      }
      let from_sender = mkSender ( fromname, from_component, mevent.port)/* line 220 */;/* line 221 */
      for (let connector of  container.connections) {  /* line 222 */
        if (sender_eq ( from_sender, connector.sender)) {/* line 223 */
          deposit ( container, connector, mevent)      /* line 224 */
          was_sent =  true;                            /* line 225 *//* line 226 */
        }                                              /* line 227 */
      }                                                /* line 228 */
    }
    if ((! ( was_sent))) {                             /* line 229 */
      console.error ( "internal error" + ": " +  ( container.name.toString ()+  ( ": mevent on port '".toString ()+  ( mevent.port.toString ()+  ( "' from ".toString ()+  ( fromname.toString ()+  " dropped on floor...".toString ()) .toString ()) .toString ()) .toString ()) .toString ()) )/* line 230 *//* line 231 */
    }                                                  /* line 232 *//* line 233 */
}

function any_child_ready (container) {                 /* line 234 */
    for (let child of  container.children) {           /* line 235 */
      if (child_is_ready ( child)) {                   /* line 236 */
        return  true;                                  /* line 237 *//* line 238 */
      }                                                /* line 239 */
    }
    return  false;                                     /* line 240 *//* line 241 *//* line 242 */
}

function child_is_ready (eh) {                         /* line 243 */
    return ((((((((! ((0=== eh.outq.length))))) || (((! ((0=== eh.inq.length))))))) || (( eh.state!= "idle")))) || ((any_child_ready ( eh))));/* line 244 *//* line 245 *//* line 246 */
}
                                                       /* line 247 */
/*  Creates a component that acts as a container. It is the same as a `Eh` instance *//* line 248 */
/*  whose handler function is `container_handler`. */  /* line 249 */
function make_container (name,owner) {                 /* line 250 */
    let  eh =  new Eh ();                              /* line 251 */;
    eh.name =  name;                                   /* line 252 */
    eh.owner =  owner;                                 /* line 253 */
    eh.handler =  container_handler;                   /* line 254 */
    eh.finject =  injector;                            /* line 255 */
    eh.stop =  container_reset_children;               /* line 256 */
    eh.state =  "idle";                                /* line 257 */
    eh.kind =  "container";                            /* line 258 */
    return  eh;                                        /* line 259 *//* line 260 *//* line 261 */
}

/*  Sends a mevent on the given `port` with `data`, placing it on the output *//* line 262 */
/*  of the given component. */                         /* line 263 *//* line 264 */
function send (eh,port,obj,causingMevent) {            /* line 265 */
    let  d =  new Datum ();                            /* line 266 */;
    d.v =  obj;                                        /* line 267 */
    d.clone =  function () {return obj_clone ( d)      /* line 268 */;};
    d.reclaim =  null;                                 /* line 269 */
    let mev = make_mevent ( port, d)                   /* line 270 */;
    put_output ( eh, mev)                              /* line 271 *//* line 272 *//* line 273 */
}

function forward (eh,port,mev) {                       /* line 274 */
    let fwdmev = make_mevent ( port, mev.payload)      /* line 275 */;
    put_output ( eh, fwdmev)                           /* line 276 *//* line 277 *//* line 278 */
}

function inject_mevent (eh,mev) {                      /* line 279 */
    eh.finject ( eh, mev)                              /* line 280 *//* line 281 *//* line 282 */
}

function set_active (eh) {                             /* line 283 */
    eh.state =  "active";                              /* line 284 *//* line 285 *//* line 286 */
}

function set_idle (eh) {                               /* line 287 */
    eh.state =  "idle";                                /* line 288 *//* line 289 *//* line 290 */
}

function put_output (eh,mev) {                         /* line 291 */
    eh.outq.push ( mev)                                /* line 292 *//* line 293 *//* line 294 */
}

function obj_clone (obj) {                             /* line 295 */
    return  obj;                                       /* line 296 *//* line 297 */
}
