import * as fs from 'fs';
import path from 'path';
import execSync from 'child_process';
import 'dotenv/config';
                                                       /* line 1 */
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
let digits = [ "₀", "₁", "₂", "₃", "₄", "₅", "₆", "₇", "₈", "₉", "₁₀", "₁₁", "₁₂", "₁₃", "₁₄", "₁₅", "₁₆", "₁₇", "₁₈", "₁₉", "₂₀", "₂₁", "₂₂", "₂₃", "₂₄", "₂₅", "₂₆", "₂₇", "₂₈", "₂₉"];/* line 7 *//* line 8 *//* line 9 */
function subscripted_digit (n) {                       /* line 10 *//* line 11 */
    if (((( n >=  0) && ( n <=  29)))) {               /* line 12 */
      return  digits [ n];                             /* line 13 */
    }
    else {                                             /* line 14 */
      return  ( "₊".toString ()+ `${ n}`.toString ())  /* line 15 */;/* line 16 */
    }                                                  /* line 17 *//* line 18 */
}

let counter =  0;                                      /* line 19 *//* line 20 */
function gensymbol (s) {                               /* line 21 *//* line 22 */
    let name_with_id =  ( s.toString ()+ subscripted_digit ( counter).toString ()) /* line 23 */;
    counter =  counter+ 1;                             /* line 24 */
    return  name_with_id;                              /* line 25 *//* line 26 */
}
/* line 1 */
class Datum {
  constructor () {                                     /* line 2 */

    this.v =  null;                                    /* line 3 */
    this.clone =  null;                                /* line 4 */
    this.reclaim =  null;                              /* line 5 *//* line 6 */
  }
}
                                                       /* line 7 *//* line 8 */
/*  Mevent passed to a leaf component. */              /* line 9 */
/*  */                                                 /* line 10 */
/*  `port` refers to the name of the incoming or outgoing port of this component. *//* line 11 */
/*  `payload` is the data attached to this mevent. */  /* line 12 */
class Mevent {
  constructor () {                                     /* line 13 */

    this.port =  null;                                 /* line 14 */
    this.payload =  null;                              /* line 15 *//* line 16 */
  }
}
                                                       /* line 17 */
function clone_port (s) {                              /* line 18 */
    return clone_string ( s)                           /* line 19 */;/* line 20 *//* line 21 */
}

/*  Utility for making a `Mevent`. Used to safely "seed“ mevents *//* line 22 */
/*  entering the very top of a network. */             /* line 23 */
function make_mevent (port,datum) {                    /* line 24 */
    let p = clone_string ( port)                       /* line 25 */;
    let  m =  new Mevent ();                           /* line 26 */;
    m.port =  p;                                       /* line 27 */
    m.payload =  datum.clone ();                       /* line 28 */
    return  m;                                         /* line 29 *//* line 30 *//* line 31 */
}

/*  Clones a mevent. Primarily used internally for “fanning out“ a mevent to multiple destinations. *//* line 32 */
function mevent_clone (mev) {                          /* line 33 */
    let  m =  new Mevent ();                           /* line 34 */;
    m.port = clone_port ( mev.port)                    /* line 35 */;
    m.payload =  mev.payload.clone ();                 /* line 36 */
    return  m;                                         /* line 37 *//* line 38 *//* line 39 */
}

/*  Frees a mevent. */                                 /* line 40 */
function destroy_mevent (mev) {                        /* line 41 */
    /*  during debug, dont destroy any mevent, since we want to trace mevents, thus, we need to persist ancestor mevents *//* line 42 *//* line 43 *//* line 44 *//* line 45 */
}

function destroy_datum (mev) {                         /* line 46 *//* line 47 *//* line 48 *//* line 49 */
}

function destroy_port (mev) {                          /* line 50 *//* line 51 *//* line 52 *//* line 53 */
}

/*  */                                                 /* line 54 */
function format_mevent (m) {                           /* line 55 */
    if ( m ==  null) {                                 /* line 56 */
      return  "{}";                                    /* line 57 */
    }
    else {                                             /* line 58 */
      return  ( "{%5C”".toString ()+  ( m.port.toString ()+  ( "%5C”:%5C”".toString ()+  ( m.payload.v.toString ()+  "%5C”}".toString ()) .toString ()) .toString ()) .toString ()) /* line 59 */;/* line 60 */
    }                                                  /* line 61 */
}

function format_mevent_raw (m) {                       /* line 62 */
    if ( m ==  null) {                                 /* line 63 */
      return  "";                                      /* line 64 */
    }
    else {                                             /* line 65 */
      return  m.payload.v;                             /* line 66 *//* line 67 */
    }                                                  /* line 68 */
}
/* line 1 */
let enumDown =  0;                                     /* line 2 */
let enumAcross =  1;                                   /* line 3 */
let enumUp =  2;                                       /* line 4 */
let enumThrough =  3;                                  /* line 5 *//* line 6 *//* line 7 */
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
class Component_Registry {
  constructor () {                                     /* line 1 */

    this.templates = {};                               /* line 2 *//* line 3 */
  }
}
                                                       /* line 4 */
class Template {
  constructor () {                                     /* line 5 */

    this.name =  null;                                 /* line 6 */
    this.container =  null;                            /* line 7 */
    this.instantiator =  null;                         /* line 8 *//* line 9 */
  }
}
                                                       /* line 10 */
function mkTemplate (name,template_data,instantiator) {/* line 11 */
    let  templ =  new Template ();                     /* line 12 */;
    templ.name =  name;                                /* line 13 */
    templ.template_data =  template_data;              /* line 14 */
    templ.instantiator =  instantiator;                /* line 15 */
    return  templ;                                     /* line 16 *//* line 17 *//* line 18 */
}
                                                       /* line 19 */
/*  convert a little-network to internal form (an object data structure created by json parser) ...  *//* line 20 */
/*  the actual data structure depends on the json parser library used by the target language  *//* line 21 */
/*  the form of the data structure doesn;t matter here, as long as we use lookup operators "@" in this .rt code  *//* line 22 *//* line 23 */
/*  ... by reading the little-net from an external file  *//* line 24 */
function lnet2internal_from_file (container_xml) {     /* line 25 */
    let pathname = process.env.PBPWD                   /* line 26 */;
    let filename =   container_xml                     /* line 27 */;

    let jstr = undefined;
    if (filename == "0") {
    jstr = fs.readFileSync (0, { encoding: 'utf8'});
    } else if (pathname) {
    jstr = fs.readFileSync (`${pathname}/${filename}`, { encoding: 'utf8'});
    } else {
    jstr = fs.readFileSync (`${filename}`, { encoding: 'utf8'});
    }
    if (jstr) {
    return JSON.parse (jstr);
    } else {
    return undefined;
    }
                                                       /* line 28 *//* line 29 *//* line 30 */
}

/*  ... by reading the little-net from an embedded string (an aspect of creating t2t tool code)  *//* line 31 */
function lnet2internal_from_string (lnet) {            /* line 32 */

    return JSON.parse (lnet);
                                                       /* line 33 *//* line 34 *//* line 35 */
}

function make_component_registry () {                  /* line 36 */
    return  new Component_Registry ();                 /* line 37 */;/* line 38 *//* line 39 */
}

function register_component (reg,template) {
    return abstracted_register_component ( reg, template, false);/* line 40 */
}

function register_component_allow_overwriting (reg,template) {
    return abstracted_register_component ( reg, template, true);/* line 41 *//* line 42 */
}

function abstracted_register_component (reg,template,ok_to_overwrite) {/* line 43 */
    let name = mangle_name ( template.name)            /* line 44 */;
    if ((((((( reg!= null) && ( name))) in ( reg.templates))) && ((!  ok_to_overwrite)))) {/* line 45 */
      load_error ( ( "Component /".toString ()+  ( template.name.toString ()+  "/ already declared".toString ()) .toString ()) )/* line 46 */
      return  reg;                                     /* line 47 */
    }
    else {                                             /* line 48 */
      reg.templates [name] =  template;                /* line 49 */
      return  reg;                                     /* line 50 *//* line 51 */
    }                                                  /* line 52 *//* line 53 */
}

function get_component_instance (reg,full_name,owner) {/* line 54 */
    /*  If a part name begins with ":", it is treated as a JIT part and we let the runtime factory generate it on-the-fly (see kernel_external.rt and external.rt) else it is assumed to be a regular AOT part and assumed to have been registered before runtime, so we just pull its template out of the registry and instantiate it.  *//* line 55 */
    /*  ":?<string>" is a probe part that is tagged with <string>  *//* line 56 */
    /*  ":$ <command>" is a shell-out part that sends <command> to the operating system shell  *//* line 57 */
    /*  ":<string>" else, it's just treated as a string part that produces <string> on its output  *//* line 58 */
    let template_name = mangle_name ( full_name)       /* line 59 */;
    if ( ":" ==   full_name[0] ) {                     /* line 60 */
      let instance_name = generate_instance_name ( owner, template_name)/* line 61 */;
      let instance = jit_instantiate ( reg, owner, instance_name, full_name)/* line 62 */;
      return  instance;                                /* line 63 */
    }
    else {                                             /* line 64 */
      if ((( template_name) in ( reg.templates))) {    /* line 65 */
        let template =  reg.templates [template_name]; /* line 66 */
        if (( template ==  null)) {                    /* line 67 */
          load_error ( ( "Registry Error (A): Can't find component /".toString ()+  ( template_name.toString ()+  "/".toString ()) .toString ()) )/* line 68 */
          return  null;                                /* line 69 */
        }
        else {                                         /* line 70 */
          let instance_name = generate_instance_name ( owner, template_name)/* line 71 */;
          let instance =  template.instantiator ( reg, owner, instance_name, template.template_data, "")/* line 72 */;
          return  instance;                            /* line 73 *//* line 74 */
        }
      }
      else {                                           /* line 75 */
        load_error ( ( "Registry Error (B): Can't find component /".toString ()+  ( template_name.toString ()+  "/".toString ()) .toString ()) )/* line 76 */
        return  null;                                  /* line 77 *//* line 78 */
      }                                                /* line 79 */
    }                                                  /* line 80 *//* line 81 */
}

function generate_instance_name (owner,template_name) {/* line 82 */
    let owner_name =  "";                              /* line 83 */
    let instance_name =  template_name;                /* line 84 */
    if ( null!= owner) {                               /* line 85 */
      owner_name =  owner.name;                        /* line 86 */
      instance_name =  ( owner_name.toString ()+  ( "▹".toString ()+  template_name.toString ()) .toString ()) /* line 87 */;
    }
    else {                                             /* line 88 */
      instance_name =  template_name;                  /* line 89 *//* line 90 */
    }
    return  instance_name;                             /* line 91 *//* line 92 *//* line 93 */
}

function mangle_name (s) {                             /* line 94 */
    /*  trim name to remove code from Container component names _ deferred until later (or never) *//* line 95 */
    return  s;                                         /* line 96 *//* line 97 */
}
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
/*  Creates a new leaf component out of a handler function, and a data parameter *//* line 1 */
/*  that will be passed back to your handler when called. *//* line 2 *//* line 3 */
function make_leaf (name,owner,instance_data,arg,handler,reset_handler) {/* line 4 */
    let  eh =  new Eh ();                              /* line 5 */;
    let  nm =  "";                                     /* line 6 */
    if ( null!= owner) {                               /* line 7 */
      nm =  owner.name;                                /* line 8 *//* line 9 */
    }
    eh.name =  ( nm.toString ()+  ( "▹".toString ()+  name.toString ()) .toString ()) /* line 10 */;
    eh.owner =  owner;                                 /* line 11 */
    eh.handler =  handler;                             /* line 12 */
    eh.reset_handler =  reset_handler;                 /* line 13 */
    eh.finject =  injector;                            /* line 14 */
    eh.reset =  leaf_reset;                            /* line 15 */
    eh.instance_data =  instance_data;                 /* line 16 */
    eh.arg =  arg;                                     /* line 17 */
    eh.state =  "idle";                                /* line 18 */
    return  eh;                                        /* line 19 *//* line 20 *//* line 21 */
}

/*  Reset Leaf part to a known, idle state. Hit the big red button.  *//* line 22 */
function leaf_reset (part) {                           /* line 23 */

    part.inq = [];                                     /* line 24 */

    part.outq = [];                                    /* line 25 */
    if (( part.reset_handler!= null)) {                /* line 26 */
      part.reset_handler ( part)                       /* line 27 *//* line 28 */
    }
    part.state =  "idle";                              /* line 29 *//* line 30 */
}
/*  (This used to be called `external` due to historical reasons). This has evolved into 2 kinds of Leaf parts: AOT and JIT (statically generated before runtime, vs. dynamically generated at runtime). If a part name begins with ;:', it is treated specially as a JIT part, else the part is assumed to have been pre-loaded into the register in the regular way.  *//* line 1 *//* line 2 */
function jit_instantiate (reg,owner,name,arg) {        /* line 3 */
    let name_with_id = gensymbol ( name)               /* line 4 */;
    let  inst = make_leaf ( name_with_id, owner, null, arg, handle_jit, null)/* line 5 */;
    let  firstc =  name [ 1];                          /* line 6 */
    if (( firstc!= "$")) {                             /* line 7 */
      /*  probes get to go to the front of the line  *//* line 8 */
      inst.special =  true;                            /* line 9 *//* line 10 */
    }
    return  inst;                                      /* line 11 *//* line 12 *//* line 13 */
}

function handle_jit (eh,mev) {                         /* line 14 */
    let s =  eh.arg;                                   /* line 15 */
    let  firstc =  s [ 1];                             /* line 16 */
    if ( firstc ==  "$") {                             /* line 17 */
      shell_out_handler ( eh,    s.substring (1) .substring (1) .substring (1) , mev)/* line 18 */
    }
    else if ( firstc ==  "?") {                        /* line 19 */
      probe_handler ( eh,  s.substring (1) , mev)      /* line 20 */
    }
    else {                                             /* line 21 */
      /*  just a string, send it out  */               /* line 22 */
      send ( eh, "",  s.substring (1) , mev)           /* line 23 *//* line 24 */
    }                                                  /* line 25 *//* line 26 */
}

function probe_handler (eh,tag,mev) {                  /* line 27 *//* line 28 */
    let s =  mev.payload.v;                            /* line 29 */
    console.error ( "Info" + ": " +  ( "  @".toString ()+  (`${ ticktime}`.toString ()+  ( "  ".toString ()+  ( "probe ".toString ()+  ( eh.name.toString ()+  ( ": ".toString ()+ `${ s}`.toString ()) .toString ()) .toString ()) .toString ()) .toString ()) .toString ()) )/* line 37 *//* line 38 *//* line 39 */
}

function shell_out_handler (eh,cmd,mev) {              /* line 40 */
    let s =  mev.payload.v;                            /* line 41 */
    let  ret =  null;                                  /* line 42 */
    let  rc =  null;                                   /* line 43 */
    let  stdout =  null;                               /* line 44 */
    let  stderr =  null;                               /* line 45 */
    let  command =  cmd;                               /* line 46 */
    let  pbpRoot = process.env.PBP                     /* line 47 */;
    if ( pbpRoot!= "") {                               /* line 48 */
      command =  command.replaceAll ( "_/",  ( pbpRoot.toString ()+  "/".toString ()) )/* line 51 */;/* line 52 */
    }
    if (( (typeof process.env.PBPSHELLOUT !== "undefined") )) {/* line 53 */
      console.error ( ( "- --- shell-out: ".toString ()+  command.toString ()) );/* line 54 */
                                                       /* line 55 *//* line 56 */
    }

    stdout = execSync(`${ command} ${ s}`, { encoding: 'utf-8' });
    ret = true;
                                                       /* line 57 */
    if ( rc ==  0) {                                   /* line 58 */
      send ( eh, "", ( stdout.toString ()+  stderr.toString ()) , mev)/* line 59 */
    }
    else {                                             /* line 60 */
      send ( eh, "✗", ( stdout.toString ()+  stderr.toString ()) , mev)/* line 61 *//* line 62 */
    }                                                  /* line 63 *//* line 64 */
}
function clone_string (s) {                            /* line 1 */
    return  s;                                         /* line 2 *//* line 3 *//* line 4 */
}
                                                       /* line 5 */
function trash_instantiate (reg,owner,name,template_data,arg) {/* line 6 */
    let name_with_id = gensymbol ( "trash")            /* line 7 */;
    return make_leaf ( name_with_id, owner, null, "", trash_handler, null)/* line 8 */;/* line 9 *//* line 10 */
}

function trash_handler (eh,mev) {                      /* line 11 */
    /*  to appease dumped_on_floor checker */          /* line 12 *//* line 13 *//* line 14 */
}

class TwoMevents {
  constructor () {                                     /* line 15 */

    this.firstmev =  null;                             /* line 16 */
    this.secondmev =  null;                            /* line 17 *//* line 18 */
  }
}
                                                       /* line 19 */
/*  Deracer_States :: enum { idle, waitingForFirstmev, waitingForSecondmev } *//* line 20 */
class Deracer_Instance_Data {
  constructor () {                                     /* line 21 */

    this.state =  null;                                /* line 22 */
    this.buffer =  null;                               /* line 23 *//* line 24 */
  }
}
                                                       /* line 25 */
function reclaim_Buffers_from_heap (inst) {            /* line 26 *//* line 27 *//* line 28 *//* line 29 */
}

function deracer_reset_handler (eh) {                  /* line 30 */
    let  inst =  eh.instance_data;                     /* line 31 */
    inst.state =  "idle";                              /* line 32 */
    inst.buffer =  new TwoMevents ();                  /* line 33 */;/* line 34 *//* line 35 */
}

function deracer_instantiate (reg,owner,name,template_data,arg) {/* line 36 */
    let name_with_id = gensymbol ( "deracer")          /* line 37 */;
    let  inst =  new Deracer_Instance_Data ();         /* line 38 */;
    inst.state =  "idle";                              /* line 39 */
    inst.buffer =  new TwoMevents ();                  /* line 40 */;
    let eh = make_leaf ( name_with_id, owner, inst, "", deracer_handler, deracer_reset_handler)/* line 41 */;
    return  eh;                                        /* line 42 *//* line 43 *//* line 44 */
}

function send_firstmev_then_secondmev (eh,inst) {      /* line 45 */
    forward ( eh, "1", inst.buffer.firstmev)           /* line 46 */
    forward ( eh, "2", inst.buffer.secondmev)          /* line 47 */
    reclaim_Buffers_from_heap ( inst)                  /* line 48 *//* line 49 *//* line 50 */
}

function deracer_handler (eh,mev) {                    /* line 51 */
    let  inst =  eh.instance_data;                     /* line 52 */
    if ( inst.state ==  "idle") {                      /* line 53 */
      if ( "1" ==  mev.port) {                         /* line 54 */
        inst.buffer.firstmev =  mev;                   /* line 55 */
        inst.state =  "waitingForSecondmev";           /* line 56 */
      }
      else if ( "2" ==  mev.port) {                    /* line 57 */
        inst.buffer.secondmev =  mev;                  /* line 58 */
        inst.state =  "waitingForFirstmev";            /* line 59 */
      }
      else {                                           /* line 60 */
        runtime_error ( ( "bad mev.port (case A) for deracer ".toString ()+  mev.port.toString ()) )/* line 61 *//* line 62 */
      }
    }
    else if ( inst.state ==  "waitingForFirstmev") {   /* line 63 */
      if ( "1" ==  mev.port) {                         /* line 64 */
        inst.buffer.firstmev =  mev;                   /* line 65 */
        send_firstmev_then_secondmev ( eh, inst)       /* line 66 */
        inst.state =  "idle";                          /* line 67 */
      }
      else {                                           /* line 68 */
        runtime_error ( ( "deracer: waiting for 1 but got [".toString ()+  ( mev.port.toString ()+  "] (case B)".toString ()) .toString ()) )/* line 69 *//* line 70 */
      }
    }
    else if ( inst.state ==  "waitingForSecondmev") {  /* line 71 */
      if ( "2" ==  mev.port) {                         /* line 72 */
        inst.buffer.secondmev =  mev;                  /* line 73 */
        send_firstmev_then_secondmev ( eh, inst)       /* line 74 */
        inst.state =  "idle";                          /* line 75 */
      }
      else {                                           /* line 76 */
        runtime_error ( ( "deracer: waiting for 2 but got [".toString ()+  ( mev.port.toString ()+  "] (case C)".toString ()) .toString ()) )/* line 77 *//* line 78 */
      }
    }
    else {                                             /* line 79 */
      runtime_error ( "bad state for deracer {eh.state}")/* line 80 *//* line 81 */
    }                                                  /* line 82 *//* line 83 */
}

function low_level_read_text_file_instantiate (reg,owner,name,template_data,arg) {/* line 84 */
    let name_with_id = gensymbol ( "Low Level Read Text File")/* line 85 */;
    return make_leaf ( name_with_id, owner, null, "", low_level_read_text_file_handler, null)/* line 86 */;/* line 87 *//* line 88 */
}

function low_level_read_text_file_handler (eh,mev) {   /* line 89 */
    let fname =  mev.payload.v;                        /* line 90 */

    if (fname == "0") {
    data = fs.readFileSync (0, { encoding: 'utf8'});
    } else {
    data = fs.readFileSync (fname, { encoding: 'utf8'});
    }
    if (data) {
      send_string (eh, "", data, mev);
    } else {
      send_string (eh, "✗", `read error on file '${fname}'`, mev);
    }
                                                       /* line 91 *//* line 92 *//* line 93 */
}

function ensure_string_datum_instantiate (reg,owner,name,template_data,arg) {/* line 94 */
    let name_with_id = gensymbol ( "Ensure String Datum")/* line 95 */;
    return make_leaf ( name_with_id, owner, null, "", ensure_string_datum_handler, null)/* line 96 */;/* line 97 *//* line 98 */
}

function ensure_string_datum_handler (eh,mev) {        /* line 99 */
    if ( "string" ==  mev.payload.kind ()) {           /* line 100 */
      forward ( eh, "", mev)                           /* line 101 */
    }
    else {                                             /* line 102 */
      let emev =  ( "*** ensure: type error (expected a string payload) but got ".toString ()+  mev.payload.toString ()) /* line 103 */;
      send ( eh, "✗", emev, mev)                       /* line 104 *//* line 105 */
    }                                                  /* line 106 *//* line 107 */
}

class Syncfilewrite_Data {
  constructor () {                                     /* line 108 */

    this.filename =  "";                               /* line 109 *//* line 110 */
  }
}
                                                       /* line 111 */
function syncfilewrite_reset_handler (eh) {            /* line 112 */
    eh.instance_data =  new Syncfilewrite_Data ();     /* line 113 */;/* line 114 *//* line 115 */
}

/*  temp copy for bootstrap, sends "done“ (error during bootstrap if not wired) *//* line 116 */
function syncfilewrite_instantiate (reg,owner,name,template_data,arg) {/* line 117 */
    let name_with_id = gensymbol ( "syncfilewrite")    /* line 118 */;
    let inst =  new Syncfilewrite_Data ();             /* line 119 */;
    return make_leaf ( name_with_id, owner, inst, "", syncfilewrite_handler, syncfilewrite_reset_handler)/* line 120 */;/* line 121 *//* line 122 */
}

function syncfilewrite_handler (eh,mev) {              /* line 123 */
    let  inst =  eh.instance_data;                     /* line 124 */
    if ( "filename" ==  mev.port) {                    /* line 125 */
      inst.filename =  mev.payload.v;                  /* line 126 */
    }
    else if ( "input" ==  mev.port) {                  /* line 127 */
      let contents =  mev.payload.v;                   /* line 128 */
      let  f = open ( inst.filename, "w")              /* line 129 */;
      if ( f!= null) {                                 /* line 130 */
        f.write ( mev.payload.v)                       /* line 131 */
        f.close ()                                     /* line 132 */
        send ( eh, "done",new_datum_bang (), mev)      /* line 133 */
      }
      else {                                           /* line 134 */
        send ( eh, "✗", ( "open error on file ".toString ()+  inst.filename.toString ()) , mev)/* line 135 *//* line 136 */
      }                                                /* line 137 */
    }                                                  /* line 138 *//* line 139 */
}

class StringConcat_Instance_Data {
  constructor () {                                     /* line 140 */

    this.buffer1 =  null;                              /* line 141 */
    this.buffer2 =  null;                              /* line 142 *//* line 143 */
  }
}
                                                       /* line 144 */
function stringconcat_reset_handler (eh) {             /* line 145 */
    let  inst =  eh.instance_data;                     /* line 146 */
    inst.buffer1 =  null;                              /* line 147 */
    inst.buffer2 =  null;                              /* line 148 *//* line 149 *//* line 150 */
}

function stringconcat_instantiate (reg,owner,name,template_data,arg) {/* line 151 */
    let name_with_id = gensymbol ( "stringconcat")     /* line 152 */;
    let instp =  new StringConcat_Instance_Data ();    /* line 153 */;
    return make_leaf ( name_with_id, owner, instp, "", stringconcat_handler, stringconcat_reset_handler)/* line 154 */;/* line 155 *//* line 156 */
}

function stringconcat_handler (eh,mev) {               /* line 157 */
    let  inst =  eh.instance_data;                     /* line 158 */
    if ( "1" ==  mev.port) {                           /* line 159 */
      inst.buffer1 = clone_string ( mev.payload.v)     /* line 160 */;
      maybe_stringconcat ( eh, inst, mev)              /* line 161 */
    }
    else if ( "2" ==  mev.port) {                      /* line 162 */
      inst.buffer2 = clone_string ( mev.payload.v)     /* line 163 */;
      maybe_stringconcat ( eh, inst, mev)              /* line 164 */
    }
    else if ( "reset" ==  mev.port) {                  /* line 165 */
      inst.buffer1 =  null;                            /* line 166 */
      inst.buffer2 =  null;                            /* line 167 */
    }
    else {                                             /* line 168 */
      runtime_error ( ( "bad mev.port for stringconcat: ".toString ()+  mev.port.toString ()) )/* line 169 *//* line 170 */
    }                                                  /* line 171 *//* line 172 */
}

function maybe_stringconcat (eh,inst,mev) {            /* line 173 */
    if ((( inst.buffer1!= null) && ( inst.buffer2!= null))) {/* line 174 */
      let  concatenated_string =  "";                  /* line 175 */
      if ( 0 == ( inst.buffer1.length)) {              /* line 176 */
        concatenated_string =  inst.buffer2;           /* line 177 */
      }
      else if ( 0 == ( inst.buffer2.length)) {         /* line 178 */
        concatenated_string =  inst.buffer1;           /* line 179 */
      }
      else {                                           /* line 180 */
        concatenated_string =  inst.buffer1+ inst.buffer2;/* line 181 *//* line 182 */
      }
      send ( eh, "", concatenated_string, mev)         /* line 183 */
      inst.buffer1 =  null;                            /* line 184 */
      inst.buffer2 =  null;                            /* line 185 *//* line 186 */
    }                                                  /* line 187 *//* line 188 */
}

/*  */                                                 /* line 189 *//* line 190 */
let projectRoot =  ".";                                /* line 191 *//* line 192 */
function string_constant_instantiate (reg,owner,name,template_data,arg) {/* line 193 *//* line 194 */
    let name_with_id = gensymbol ( "strconst")         /* line 195 */;
    let  s =  template_data;                           /* line 196 */
    if ( projectRoot!= "") {                           /* line 197 */
      s =  s.replaceAll ( "_00_",  projectRoot)        /* line 198 */;/* line 199 */
    }
    return make_leaf ( name_with_id, owner, s, "", string_constant_handler, null)/* line 200 */;/* line 201 *//* line 202 */
}

function string_constant_handler (eh,mev) {            /* line 203 */
    let s =  eh.instance_data;                         /* line 204 */
    send ( eh, "", s, mev)                             /* line 205 *//* line 206 *//* line 207 */
}

function fakepipename_instantiate (reg,owner,name,template_data,arg) {/* line 208 */
    let instance_name = gensymbol ( "fakepipe")        /* line 209 */;
    return make_leaf ( instance_name, owner, null, "", fakepipename_handler, null)/* line 210 */;/* line 211 *//* line 212 */
}

let rand =  0;                                         /* line 213 *//* line 214 */
function fakepipename_handler (eh,mev) {               /* line 215 *//* line 216 */
    rand =  rand+ 1;
    /*  not very random, but good enough _ ;rand' must be unique within a single run *//* line 217 */
    send ( eh, "", ( "/tmp/fakepipe".toString ()+  rand.toString ()) , mev)/* line 218 *//* line 219 *//* line 220 */
}
                                                       /* line 221 */
class Switch1star_Instance_Data {
  constructor () {                                     /* line 222 */

    this.state =  "1";                                 /* line 223 *//* line 224 */
  }
}
                                                       /* line 225 */
function switch1star_reset_handler (eh) {              /* line 226 */
    let  inst =  eh.instance_data;                     /* line 227 */
    inst =  new Switch1star_Instance_Data ();          /* line 228 */;/* line 229 *//* line 230 */
}

function switch1star_instantiate (reg,owner,name,template_data,arg) {/* line 231 */
    let name_with_id = gensymbol ( "switch1*")         /* line 232 */;
    let instp =  new Switch1star_Instance_Data ();     /* line 233 */;
    return make_leaf ( name_with_id, owner, instp, "", switch1star_handler, switch1star_reset_handler)/* line 234 */;/* line 235 *//* line 236 */
}

function switch1star_handler (eh,mev) {                /* line 237 */
    let  inst =  eh.instance_data;                     /* line 238 */
    let whichOutput =  inst.state;                     /* line 239 */
    if ( "" ==  mev.port) {                            /* line 240 */
      if ( "1" ==  whichOutput) {                      /* line 241 */
        forward ( eh, "1", mev)                        /* line 242 */
        inst.state =  "*";                             /* line 243 */
      }
      else if ( "*" ==  whichOutput) {                 /* line 244 */
        forward ( eh, "*", mev)                        /* line 245 */
      }
      else {                                           /* line 246 */
        send ( eh, "✗", "internal error bad state in switch1*", mev)/* line 247 *//* line 248 */
      }
    }
    else if ( "reset" ==  mev.port) {                  /* line 249 */
      inst.state =  "1";                               /* line 250 */
    }
    else {                                             /* line 251 */
      send ( eh, "✗", "internal error bad mevent for switch1*", mev)/* line 252 *//* line 253 */
    }                                                  /* line 254 *//* line 255 */
}

class StringAccumulator {
  constructor () {                                     /* line 256 */

    this.s =  "";                                      /* line 257 *//* line 258 */
  }
}
                                                       /* line 259 */
function strcatstar_reset_handler (eh) {               /* line 260 */
    eh.instance_data =  new StringAccumulator ();      /* line 261 */;/* line 262 *//* line 263 */
}

function strcatstar_instantiate (reg,owner,name,template_data,arg) {/* line 264 */
    let name_with_id = gensymbol ( "String Concat *")  /* line 265 */;
    let instp =  new StringAccumulator ();             /* line 266 */;
    return make_leaf ( name_with_id, owner, instp, "", strcatstar_handler, strcatstar_reset_handler)/* line 267 */;/* line 268 *//* line 269 */
}

function strcatstar_handler (eh,mev) {                 /* line 270 */
    let  accum =  eh.instance_data;                    /* line 271 */
    if ( "" ==  mev.port) {                            /* line 272 */
      accum.s =  ( accum.s.toString ()+  mev.payload.v.toString ()) /* line 273 */;
    }
    else if ( "fini" ==  mev.port) {                   /* line 274 */
      send ( eh, "", accum.s, mev)                     /* line 275 */
    }
    else {                                             /* line 276 */
      send ( eh, "✗", "internal error bad mevent for String Concat *", mev)/* line 277 *//* line 278 */
    }                                                  /* line 279 *//* line 280 */
}

function stop_instantiate (reg,owner,name,template_data,arg) {/* line 281 */
    let name_with_id = gensymbol ( "Stop")             /* line 282 */;
    let inst =  null;                                  /* line 283 */
    return make_leaf ( name_with_id, owner, inst, "", stop_handler, null)/* line 284 */;/* line 285 *//* line 286 */
}

function stop_handler (eh,mev) {                       /* line 287 */
    let  inst =  eh.instance_data;                     /* line 288 */
    let  parent =  eh.owner;                           /* line 289 */
    let  s =  ( "   !!! stopping: '".toString ()+  ( parent.name.toString ()+  "'".toString ()) .toString ()) /* line 290 */;
    console.error ( s);                                /* line 291 */
                                                       /* line 292 */
    parent.reset ( parent)                             /* line 293 */
    send ( eh, "", mev.payload.v, mev)                 /* line 294 *//* line 295 *//* line 296 */
}

/*  all of the the built_in leaves are listed here */  /* line 297 */
/*  future: refactor this such that programmers can pick and choose which (lumps of) builtins are used in a specific project *//* line 298 *//* line 299 */
function initialize_stock_components (reg) {           /* line 300 */
    register_component ( reg,mkTemplate ( "1then2", null, deracer_instantiate))/* line 301 */
    register_component ( reg,mkTemplate ( "1→2", null, deracer_instantiate))/* line 302 */
    register_component ( reg,mkTemplate ( "trash", null, trash_instantiate))/* line 303 */
    register_component ( reg,mkTemplate ( "🗑️", null, trash_instantiate))/* line 304 */
    register_component ( reg,mkTemplate ( "🚫", null, stop_instantiate))/* line 305 *//* line 306 *//* line 307 */
    register_component ( reg,mkTemplate ( "Read Text File", null, low_level_read_text_file_instantiate))/* line 308 */
    register_component ( reg,mkTemplate ( "Ensure String Datum", null, ensure_string_datum_instantiate))/* line 309 *//* line 310 */
    register_component ( reg,mkTemplate ( "syncfilewrite", null, syncfilewrite_instantiate))/* line 311 */
    register_component ( reg,mkTemplate ( "String Concat", null, stringconcat_instantiate))/* line 312 */
    register_component ( reg,mkTemplate ( "switch1*", null, switch1star_instantiate))/* line 313 */
    register_component ( reg,mkTemplate ( "String Concat *", null, strcatstar_instantiate))/* line 314 */
    /*  for fakepipe */                                /* line 315 */
    register_component ( reg,mkTemplate ( "fakepipename", null, fakepipename_instantiate))/* line 316 *//* line 317 *//* line 318 */
}
let load_errors =  false;                              /* line 1 */
let runtime_errors =  false;                           /* line 2 */
let ticktime =  0;                                     /* line 3 *//* line 4 */
function load_error (s) {                              /* line 5 *//* line 6 */
    console.error ( s);                                /* line 7 */
                                                       /* line 8 */
    load_errors =  true;                               /* line 9 *//* line 10 *//* line 11 */
}

function runtime_error (s) {                           /* line 12 *//* line 13 */
    console.error ( s);                                /* line 14 */
    process.exit (1)                                   /* line 15 */
    runtime_errors =  true;                            /* line 16 *//* line 17 *//* line 18 */
}
                                                       /* line 19 */
function initialize_component_palette_from_files (diagram_source_files) {/* line 20 */
    let  reg = make_component_registry ();             /* line 21 */
    for (let diagram_source of  diagram_source_files) {/* line 22 */
      let all_containers_within_single_file = lnet2internal_from_file ( diagram_source)/* line 23 */;
      for (let container of  all_containers_within_single_file) {/* line 24 */
        register_component ( reg,mkTemplate ( container ["name"], container, container_instantiator))/* line 25 *//* line 26 */
      }                                                /* line 27 */
    }
    initialize_stock_components ( reg)                 /* line 28 */
    return  reg;                                       /* line 29 *//* line 30 *//* line 31 */
}

function initialize_component_palette_from_string (lnet) {/* line 32 */
    let  reg = make_component_registry ();             /* line 33 */
    let all_containers = lnet2internal_from_string ( lnet)/* line 34 */;
    for (let container of  all_containers) {           /* line 35 */
      register_component ( reg,mkTemplate ( container ["name"], container, container_instantiator))/* line 36 *//* line 37 */
    }
    initialize_stock_components ( reg)                 /* line 38 */
    return  reg;                                       /* line 39 *//* line 40 */
}

function initialize_from_files (diagram_names) {       /* line 41 */
    let arg =  null;                                   /* line 42 */
    let palette = initialize_component_palette_from_files ( diagram_names)/* line 43 */;
    return [ palette,[ diagram_names, arg]];           /* line 44 *//* line 45 *//* line 46 */
}

function initialize_from_string () {                   /* line 47 */
    let arg =  null;                                   /* line 48 */
    let palette = initialize_component_palette_from_string ();/* line 49 */
    return [ palette,[ null, arg]];                    /* line 50 *//* line 51 *//* line 52 */
}

function start (arg,part_name,palette,env) {           /* line 53 */
    let part = start_bare ( part_name, palette, env)   /* line 54 */;
    inject ( part, "", arg)                            /* line 55 */
    finalize ( part)                                   /* line 56 *//* line 57 *//* line 58 */
}

function start_bare (part_name,palette,env) {          /* line 59 */
    let diagram_names =  env [ 0];                     /* line 60 */
    /*  get entrypoint container */                    /* line 61 */
    let  part = get_component_instance ( palette, part_name, null)/* line 62 */;
    if ( null ==  part) {                              /* line 63 */
      load_error ( ( "Couldn;t find container with page name /".toString ()+  ( part_name.toString ()+  ( "/ in files ".toString ()+  (`${ diagram_names}`.toString ()+  " (check tab names, or disable compression?)".toString ()) .toString ()) .toString ()) .toString ()) )/* line 67 *//* line 68 */
    }
    return  part;                                      /* line 69 *//* line 70 *//* line 71 */
}

function inject (part,port,payload) {                  /* line 72 *//* line 73 */
    if ((!  load_errors)) {                            /* line 74 */
      let  d =  new Datum ();                          /* line 75 */;
      d.v =  payload;                                  /* line 76 */
      d.clone =  function () {return obj_clone ( d)    /* line 77 */;};
      d.reclaim =  null;                               /* line 78 */
      let  mev = make_mevent ( port, d)                /* line 79 */;
      inject_mevent ( part, mev)                       /* line 80 */
    }
    else {                                             /* line 81 */
      process.exit (1)                                 /* line 82 *//* line 83 */
    }                                                  /* line 84 *//* line 85 */
}

function finalize (part) {                             /* line 86 */
    console.log (JSON.stringify ( part.outq.map(item => ({ [item.port]: item.datum.v })), null, 2));/* line 87 *//* line 88 *//* line 89 */
}

function new_datum_bang () {                           /* line 90 */
    let  d =  new Datum ();                            /* line 91 */;
    d.v =  "!";                                        /* line 92 */
    d.clone =  function () {return obj_clone ( d)      /* line 93 */;};
    d.reclaim =  null;                                 /* line 94 */
    return  d                                          /* line 95 *//* line 96 */;
}
