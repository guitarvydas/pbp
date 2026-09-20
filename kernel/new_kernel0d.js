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
    this.routings =  []                                /* line 26 */;
    this.handler =  null;                              /* line 27 */
    this.reset_instance_data =  null;                  /* line 28 */
    this.finject =  null;                              /* line 29 */
    this.stop =  null;                                 /* line 30 */
    this.instance_data =  null;                        /* line 31 *//*  arg needed for probe support  *//* line 32 */
    this.arg =  "";                                    /* line 33 */
    this.state =  "idle";                              /* line 34 */
    this.special =  false;                             /* line 35 *//*  bootstrap debugging *//* line 36 */
    this.kind =  null;/*  enum { container, leaf, } */ /* line 37 *//* line 38 */
  }
}
let  digits = [ "₀", "₁", "₂", "₃", "₄", "₅", "₆", "₇", "₈", "₉", "₁₀", "₁₁", "₁₂", "₁₃", "₁₄", "₁₅", "₁₆", "₁₇", "₁₈", "₁₉", "₂₀", "₂₁", "₂₂", "₂₃", "₂₄", "₂₅", "₂₆", "₂₇", "₂₈", "₂₉"];/* line 7 *//* line 8 *//* line 9 */
function subscripted_digit (n) {                       /* line 10 *//* line 11 */
    if (((( n >=  0) && ( n <=  29)))) {               /* line 12 */
      return  digits [ n];                             /* line 13 */
    }
    else {                                             /* line 14 */
      return  ( "₊".toString ()+ `${ n}`.toString ())  /* line 15 */;/* line 16 */
    }                                                  /* line 17 *//* line 18 */
}

let  counter =  0;                                     /* line 19 *//* line 20 */
function gensymbol (s) {                               /* line 21 *//* line 22 */
    let name_with_id =  ( s.toString ()+ subscripted_digit ( counter).toString ()) /* line 23 */;
    counter =  counter+ 1;                             /* line 24 */
    return  name_with_id;                              /* line 25 *//* line 26 *//* line 27 */
}
/* line 1 */
class Datum {
  constructor () {                                     /* line 2 */

    this.v =  null;                                    /* line 3 */
    this.clone =  null;                                /* line 4 */
    this.reclaim =  null;                              /* line 5 */
    this.other =  null;/*  reserved for use on per-project basis  *//* line 6 *//* line 7 */
  }
}
                                                       /* line 8 *//* line 9 */
/*  Mevent passed to a leaf component. */              /* line 10 */
/*  */                                                 /* line 11 */
/*  `port` refers to the name of the incoming or outgoing port of this component. *//* line 12 */
/*  `payload` is the data attached to this mevent. */  /* line 13 */
class Mevent {
  constructor () {                                     /* line 14 */

    this.port =  null;                                 /* line 15 */
    this.payload =  null;                              /* line 16 *//* line 17 */
  }
}
                                                       /* line 18 */
function clone_port (s) {                              /* line 19 */
    return clone_string ( s)                           /* line 20 */;/* line 21 *//* line 22 */
}

/*  Utility for making a `Mevent`. Used to safely "seed“ mevents *//* line 23 */
/*  entering the very top of a network. */             /* line 24 */
function make_mevent (port,datum) {                    /* line 25 */
    let p = clone_string ( port)                       /* line 26 */;
    let  m =  new Mevent ();                           /* line 27 */;
    m.port =  p;                                       /* line 28 */
    m.payload =  datum.clone ();                       /* line 29 */
    return  m;                                         /* line 30 *//* line 31 *//* line 32 */
}

/*  Clones a mevent. Primarily used internally for “fanning out“ a mevent to multiple destinations. *//* line 33 */
function mevent_clone (mev) {                          /* line 34 */
    let  m =  new Mevent ();                           /* line 35 */;
    m.port = clone_port ( mev.port)                    /* line 36 */;
    m.payload =  mev.payload.clone ();                 /* line 37 */
    return  m;                                         /* line 38 *//* line 39 *//* line 40 */
}

/*  Frees a mevent. */                                 /* line 41 */
function destroy_mevent (mev) {                        /* line 42 */
    /*  during debug, dont destroy any mevent, since we want to trace mevents, thus, we need to persist ancestor mevents *//* line 43 *//* line 44 *//* line 45 *//* line 46 */
}

function destroy_datum (mev) {                         /* line 47 *//* line 48 *//* line 49 *//* line 50 */
}

function destroy_port (mev) {                          /* line 51 *//* line 52 *//* line 53 *//* line 54 */
}

/*  */                                                 /* line 55 */
function format_mevent (m) {                           /* line 56 */
    if ( m ==  null) {                                 /* line 57 */
      return  "{}";                                    /* line 58 */
    }
    else {                                             /* line 59 */
      return  ( "{%5C”".toString ()+  ( m.port.toString ()+  ( "%5C”:%5C”".toString ()+  ( m.payload.v.toString ()+  "%5C”}".toString ()) .toString ()) .toString ()) .toString ()) /* line 60 */;/* line 61 */
    }                                                  /* line 62 */
}

function format_mevent_raw (m) {                       /* line 63 */
    if ( m ==  null) {                                 /* line 64 */
      return  "";                                      /* line 65 */
    }
    else {                                             /* line 66 */
      return  m.payload.v;                             /* line 67 *//* line 68 */
    }                                                  /* line 69 */
}
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

function delete_decls (d) {                            /* line 36 *//* line 37 *//* line 38 *//* line 39 */
}

function make_component_registry () {                  /* line 40 */
    return  new Component_Registry ();                 /* line 41 */;/* line 42 *//* line 43 */
}

function register_component (reg,template) {
    return abstracted_register_component ( reg, template, false);/* line 44 */
}

function register_component_allow_overwriting (reg,template) {
    return abstracted_register_component ( reg, template, true);/* line 45 *//* line 46 */
}

function abstracted_register_component (reg,template,ok_to_overwrite) {/* line 47 */
    let name = mangle_name ( template.name)            /* line 48 */;
    if ((((((( reg!= null) && ( name))) in ( reg.templates))) && ((!  ok_to_overwrite)))) {/* line 49 */
      load_error ( ( "Component /".toString ()+  ( template.name.toString ()+  "/ already declared".toString ()) .toString ()) )/* line 50 */
      return  reg;                                     /* line 51 */
    }
    else {                                             /* line 52 */
      reg.templates [name] =  template;                /* line 53 */
      return  reg;                                     /* line 54 *//* line 55 */
    }                                                  /* line 56 *//* line 57 */
}

function get_component_instance (reg,full_name,owner) {/* line 58 */
    /*  If a part name begins with ":", it is treated as a JIT part and we let the runtime factory generate it on-the-fly (see kernel_external.rt and external.rt) else it is assumed to be a regular AOT part and assumed to have been registered before runtime, so we just pull its template out of the registry and instantiate it.  *//* line 59 */
    /*  ":?<string>" is a probe part that is tagged with <string>  *//* line 60 */
    /*  ":$ <command>" is a shell-out part that sends <command> to the operating system shell  *//* line 61 */
    /*  ":<string>" else, it's just treated as a string part that produces <string> on its output  *//* line 62 */
    let template_name = mangle_name ( full_name)       /* line 63 */;
    if ( ":" ==   full_name[0] ) {                     /* line 64 */
      let instance_name = generate_instance_name ( owner, template_name)/* line 65 */;
      let instance = jit_instantiate ( reg, owner, instance_name, full_name)/* line 66 */;
      return  instance;                                /* line 67 */
    }
    else {                                             /* line 68 */
      if ((( template_name) in ( reg.templates))) {    /* line 69 */
        let template =  reg.templates [template_name]; /* line 70 */
        if (( template ==  null)) {                    /* line 71 */
          load_error ( ( "Registry Error (A): Can't find component /".toString ()+  ( template_name.toString ()+  "/".toString ()) .toString ()) )/* line 72 */
          return  null;                                /* line 73 */
        }
        else {                                         /* line 74 */
          let instance_name = generate_instance_name ( owner, template_name)/* line 75 */;
          let instance =  template.instantiator ( reg, owner, instance_name, template.template_data, "")/* line 76 */;
          return  instance;                            /* line 77 *//* line 78 */
        }
      }
      else {                                           /* line 79 */
        load_error ( ( "Registry Error (B): Can't find component /".toString ()+  ( template_name.toString ()+  "/".toString ()) .toString ()) )/* line 80 */
        return  null;                                  /* line 81 *//* line 82 */
      }                                                /* line 83 */
    }                                                  /* line 84 *//* line 85 */
}

function generate_instance_name (owner,template_name) {/* line 86 */
    let owner_name =  "";                              /* line 87 */
    let instance_name =  template_name;                /* line 88 */
    if ( null!= owner) {                               /* line 89 */
      owner_name =  owner.name;                        /* line 90 */
      instance_name =  ( owner_name.toString ()+  ( "▹".toString ()+  template_name.toString ()) .toString ()) /* line 91 */;
    }
    else {                                             /* line 92 */
      instance_name =  template_name;                  /* line 93 *//* line 94 */
    }
    return  instance_name;                             /* line 95 *//* line 96 *//* line 97 */
}

function mangle_name (s) {                             /* line 98 */
    /*  trim name to remove code from Container component names _ deferred until later (or never) *//* line 99 */
    return  s;                                         /* line 100 *//* line 101 */
}
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
    eh.finject =  inject_mevent;                       /* line 260 */
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
    eh.finject =  inject_mevent;                       /* line 14 */
    eh.stop =  leaf_reset;                             /* line 15 */
    eh.instance_data =  instance_data;                 /* line 16 */
    eh.arg =  arg;                                     /* line 17 */
    eh.state =  "idle";                                /* line 18 */
    eh.kind =  "leaf";                                 /* line 19 */
    return  eh;                                        /* line 20 *//* line 21 *//* line 22 */
}

/*  Reset Leaf part to a known, idle state. Hit the big red button.  *//* line 23 */
function leaf_reset (part) {                           /* line 24 */

    part.inq = [];                                     /* line 25 */

    part.outq = [];                                    /* line 26 */
    if (( part.reset_handler!= null)) {                /* line 27 */
      part.reset_handler ( part)                       /* line 28 *//* line 29 */
    }
    part.state =  "idle";                              /* line 30 *//* line 31 */
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

function probe_handler (eh,tag,mev) {                  /* line 27 */
    let s =  mev.payload.v;                            /* line 28 */
    console.error ( "Info" + ": " +  ( "  @".toString ()+  (`${ ticktime}`.toString ()+  ( "  ".toString ()+  ( "probe ".toString ()+  ( eh.name.toString ()+  ( ": ".toString ()+ `${ s}`.toString ()) .toString ()) .toString ()) .toString ()) .toString ()) .toString ()) )/* line 36 *//* line 37 *//* line 38 */
}

function shell_out_handler (eh,cmd,mev) {              /* line 39 */
    let s =  mev.payload.v;                            /* line 40 */
    let  ret =  null;                                  /* line 41 */
    let  rc =  null;                                   /* line 42 */
    let  stdout =  null;                               /* line 43 */
    let  stderr =  null;                               /* line 44 */
    let  command =  cmd;                               /* line 45 */
    let  pbpRoot = process.env.PBP                     /* line 46 */;
    if ( pbpRoot!= "") {                               /* line 47 */
      command =  command.replaceAll ( "_/",  ( pbpRoot.toString ()+  "/".toString ()) )/* line 50 */;/* line 51 */
    }
    if (( (typeof process.env.PBPSHELLOUT !== "undefined") )) {/* line 52 */
      console.error ( ( "- --- shell-out: ".toString ()+  command.toString ()) );/* line 53 */
                                                       /* line 54 *//* line 55 */
    }

    stdout = execSync(`${ command} ${ s}`, { encoding: 'utf-8' });
    ret = true;
                                                       /* line 56 */
    if ( rc ==  0) {                                   /* line 57 */
      send ( eh, "", ( stdout.toString ()+  stderr.toString ()) , mev)/* line 58 */
    }
    else {                                             /* line 59 */
      send ( eh, "✗", ( stdout.toString ()+  stderr.toString ()) , mev)/* line 60 *//* line 61 */
    }                                                  /* line 62 *//* line 63 */
}
/* line 1 */
function trash_instantiate (reg,owner,name,template_data,arg) {/* line 2 */
    let name_with_id = gensymbol ( "trash")            /* line 3 */;
    return make_leaf ( name_with_id, owner, null, "", trash_handler, null)/* line 4 */;/* line 5 *//* line 6 */
}

function trash_handler (eh,mev) {                      /* line 7 */
    /*  to appease dumped_on_floor checker */          /* line 8 *//* line 9 *//* line 10 */
}

class TwoMevents {
  constructor () {                                     /* line 11 */

    this.firstmev =  null;                             /* line 12 */
    this.secondmev =  null;                            /* line 13 *//* line 14 */
  }
}
                                                       /* line 15 */
/*  Deracer_States :: enum { idle, waitingForFirstmev, waitingForSecondmev } *//* line 16 */
class Deracer_Instance_Data {
  constructor () {                                     /* line 17 */

    this.state =  null;                                /* line 18 */
    this.buffer =  null;                               /* line 19 *//* line 20 */
  }
}
                                                       /* line 21 */
function reclaim_Buffers_from_heap (inst) {            /* line 22 *//* line 23 *//* line 24 *//* line 25 */
}

function deracer_reset_handler (eh) {                  /* line 26 */
    let  inst =  eh.instance_data;                     /* line 27 */
    inst.state =  "idle";                              /* line 28 */
    inst.buffer =  new TwoMevents ();                  /* line 29 */;/* line 30 *//* line 31 */
}

function deracer_instantiate (reg,owner,name,template_data,arg) {/* line 32 */
    let name_with_id = gensymbol ( "deracer")          /* line 33 */;
    let  inst =  new Deracer_Instance_Data ();         /* line 34 */;
    inst.state =  "idle";                              /* line 35 */
    inst.buffer =  new TwoMevents ();                  /* line 36 */;
    let eh = make_leaf ( name_with_id, owner, inst, "", deracer_handler, deracer_reset_handler)/* line 37 */;
    return  eh;                                        /* line 38 *//* line 39 *//* line 40 */
}

function send_firstmev_then_secondmev (eh,inst) {      /* line 41 */
    forward ( eh, "1", inst.buffer.firstmev)           /* line 42 */
    forward ( eh, "2", inst.buffer.secondmev)          /* line 43 */
    reclaim_Buffers_from_heap ( inst)                  /* line 44 *//* line 45 *//* line 46 */
}

function deracer_handler (eh,mev) {                    /* line 47 */
    let  inst =  eh.instance_data;                     /* line 48 */
    if ( inst.state ==  "idle") {                      /* line 49 */
      if ( "1" ==  mev.port) {                         /* line 50 */
        inst.buffer.firstmev =  mev;                   /* line 51 */
        inst.state =  "waitingForSecondmev";           /* line 52 */
      }
      else if ( "2" ==  mev.port) {                    /* line 53 */
        inst.buffer.secondmev =  mev;                  /* line 54 */
        inst.state =  "waitingForFirstmev";            /* line 55 */
      }
      else {                                           /* line 56 */
        runtime_error ( ( "bad mev.port (case A) for deracer ".toString ()+  mev.port.toString ()) )/* line 57 *//* line 58 */
      }
    }
    else if ( inst.state ==  "waitingForFirstmev") {   /* line 59 */
      if ( "1" ==  mev.port) {                         /* line 60 */
        inst.buffer.firstmev =  mev;                   /* line 61 */
        send_firstmev_then_secondmev ( eh, inst)       /* line 62 */
        inst.state =  "idle";                          /* line 63 */
      }
      else {                                           /* line 64 */
        runtime_error ( ( "deracer: waiting for 1 but got [".toString ()+  ( mev.port.toString ()+  "] (case B)".toString ()) .toString ()) )/* line 65 *//* line 66 */
      }
    }
    else if ( inst.state ==  "waitingForSecondmev") {  /* line 67 */
      if ( "2" ==  mev.port) {                         /* line 68 */
        inst.buffer.secondmev =  mev;                  /* line 69 */
        send_firstmev_then_secondmev ( eh, inst)       /* line 70 */
        inst.state =  "idle";                          /* line 71 */
      }
      else {                                           /* line 72 */
        runtime_error ( ( "deracer: waiting for 2 but got [".toString ()+  ( mev.port.toString ()+  "] (case C)".toString ()) .toString ()) )/* line 73 *//* line 74 */
      }
    }
    else {                                             /* line 75 */
      runtime_error ( "bad state for deracer {eh.state}")/* line 76 *//* line 77 */
    }                                                  /* line 78 *//* line 79 */
}

function low_level_read_text_file_instantiate (reg,owner,name,template_data,arg) {/* line 80 */
    let name_with_id = gensymbol ( "Low Level Read Text File")/* line 81 */;
    return make_leaf ( name_with_id, owner, null, "", low_level_read_text_file_handler, null)/* line 82 */;/* line 83 *//* line 84 */
}

function low_level_read_text_file_handler (eh,mev) {   /* line 85 */
    let fname =  mev.payload.v;                        /* line 86 */

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
                                                       /* line 87 *//* line 88 *//* line 89 */
}

function ensure_string_datum_instantiate (reg,owner,name,template_data,arg) {/* line 90 */
    let name_with_id = gensymbol ( "Ensure String Datum")/* line 91 */;
    return make_leaf ( name_with_id, owner, null, "", ensure_string_datum_handler, null)/* line 92 */;/* line 93 *//* line 94 */
}

function ensure_string_datum_handler (eh,mev) {        /* line 95 */
    if ( "string" ==  mev.payload.kind ()) {           /* line 96 */
      forward ( eh, "", mev)                           /* line 97 */
    }
    else {                                             /* line 98 */
      let emev =  ( "*** ensure: type error (expected a string payload) but got ".toString ()+  mev.payload.toString ()) /* line 99 */;
      send ( eh, "✗", emev, mev)                       /* line 100 *//* line 101 */
    }                                                  /* line 102 *//* line 103 */
}

class Syncfilewrite_Data {
  constructor () {                                     /* line 104 */

    this.filename =  "";                               /* line 105 *//* line 106 */
  }
}
                                                       /* line 107 */
function syncfilewrite_reset_handler (eh) {            /* line 108 */
    eh.instance_data =  new Syncfilewrite_Data ();     /* line 109 */;/* line 110 *//* line 111 */
}

/*  temp copy for bootstrap, sends "done“ (error during bootstrap if not wired) *//* line 112 */
function syncfilewrite_instantiate (reg,owner,name,template_data,arg) {/* line 113 */
    let name_with_id = gensymbol ( "syncfilewrite")    /* line 114 */;
    let inst =  new Syncfilewrite_Data ();             /* line 115 */;
    return make_leaf ( name_with_id, owner, inst, "", syncfilewrite_handler, syncfilewrite_reset_handler)/* line 116 */;/* line 117 *//* line 118 */
}

function syncfilewrite_handler (eh,mev) {              /* line 119 */
    let  inst =  eh.instance_data;                     /* line 120 */
    if ( "filename" ==  mev.port) {                    /* line 121 */
      inst.filename =  mev.payload.v;                  /* line 122 */
    }
    else if ( "input" ==  mev.port) {                  /* line 123 */
      let contents =  mev.payload.v;                   /* line 124 */
      let  f = open ( inst.filename, "w")              /* line 125 */;
      if ( f!= null) {                                 /* line 126 */
        f.write ( mev.payload.v)                       /* line 127 */
        f.close ()                                     /* line 128 */
        send ( eh, "done",new_datum_bang (), mev)      /* line 129 */
      }
      else {                                           /* line 130 */
        send ( eh, "✗", ( "open error on file ".toString ()+  inst.filename.toString ()) , mev)/* line 131 *//* line 132 */
      }                                                /* line 133 */
    }                                                  /* line 134 *//* line 135 */
}

class StringConcat_Instance_Data {
  constructor () {                                     /* line 136 */

    this.buffer1 =  null;                              /* line 137 */
    this.buffer2 =  null;                              /* line 138 *//* line 139 */
  }
}
                                                       /* line 140 */
function stringconcat_reset_handler (eh) {             /* line 141 */
    let  inst =  eh.instance_data;                     /* line 142 */
    inst.buffer1 =  null;                              /* line 143 */
    inst.buffer2 =  null;                              /* line 144 *//* line 145 *//* line 146 */
}

function stringconcat_instantiate (reg,owner,name,template_data,arg) {/* line 147 */
    let name_with_id = gensymbol ( "stringconcat")     /* line 148 */;
    let instp =  new StringConcat_Instance_Data ();    /* line 149 */;
    return make_leaf ( name_with_id, owner, instp, "", stringconcat_handler, stringconcat_reset_handler)/* line 150 */;/* line 151 *//* line 152 */
}

function stringconcat_handler (eh,mev) {               /* line 153 */
    let  inst =  eh.instance_data;                     /* line 154 */
    if ( "1" ==  mev.port) {                           /* line 155 */
      inst.buffer1 = clone_string ( mev.payload.v)     /* line 156 */;
      maybe_stringconcat ( eh, inst, mev)              /* line 157 */
    }
    else if ( "2" ==  mev.port) {                      /* line 158 */
      inst.buffer2 = clone_string ( mev.payload.v)     /* line 159 */;
      maybe_stringconcat ( eh, inst, mev)              /* line 160 */
    }
    else if ( "reset" ==  mev.port) {                  /* line 161 */
      inst.buffer1 =  null;                            /* line 162 */
      inst.buffer2 =  null;                            /* line 163 */
    }
    else {                                             /* line 164 */
      runtime_error ( ( "bad mev.port for stringconcat: ".toString ()+  mev.port.toString ()) )/* line 165 *//* line 166 */
    }                                                  /* line 167 *//* line 168 */
}

function maybe_stringconcat (eh,inst,mev) {            /* line 169 */
    if ((( inst.buffer1!= null) && ( inst.buffer2!= null))) {/* line 170 */
      let  concatenated_string =  "";                  /* line 171 */
      if ( 0 == ( inst.buffer1.length)) {              /* line 172 */
        concatenated_string =  inst.buffer2;           /* line 173 */
      }
      else if ( 0 == ( inst.buffer2.length)) {         /* line 174 */
        concatenated_string =  inst.buffer1;           /* line 175 */
      }
      else {                                           /* line 176 */
        concatenated_string =  inst.buffer1+ inst.buffer2;/* line 177 *//* line 178 */
      }
      send ( eh, "", concatenated_string, mev)         /* line 179 */
      inst.buffer1 =  null;                            /* line 180 */
      inst.buffer2 =  null;                            /* line 181 *//* line 182 */
    }                                                  /* line 183 *//* line 184 */
}

/*  */                                                 /* line 185 *//* line 186 */
function string_constant_instantiate (reg,owner,name,template_data,arg) {/* line 187 *//* line 188 */
    let name_with_id = gensymbol ( "strconst")         /* line 189 */;
    let  s =  template_data;                           /* line 190 */
    if ( projectRoot!= "") {                           /* line 191 */
      s =  s.replaceAll ( "_00_",  projectRoot)        /* line 192 */;/* line 193 */
    }
    return make_leaf ( name_with_id, owner, s, "", string_constant_handler, null)/* line 194 */;/* line 195 *//* line 196 */
}

function string_constant_handler (eh,mev) {            /* line 197 */
    let s =  eh.instance_data;                         /* line 198 */
    send ( eh, "", s, mev)                             /* line 199 *//* line 200 *//* line 201 */
}

function fakepipename_instantiate (reg,owner,name,template_data,arg) {/* line 202 */
    let instance_name = gensymbol ( "fakepipe")        /* line 203 */;
    return make_leaf ( instance_name, owner, null, "", fakepipename_handler, null)/* line 204 */;/* line 205 *//* line 206 */
}

let  rand =  0;                                        /* line 207 *//* line 208 */
function fakepipename_handler (eh,mev) {               /* line 209 *//* line 210 */
    rand =  rand+ 1;
    /*  not very random, but good enough _ ;rand' must be unique within a single run *//* line 211 */
    send ( eh, "", ( "/tmp/fakepipe".toString ()+  rand.toString ()) , mev)/* line 212 *//* line 213 *//* line 214 */
}
                                                       /* line 215 */
class Switch1star_Instance_Data {
  constructor () {                                     /* line 216 */

    this.state =  "1";                                 /* line 217 *//* line 218 */
  }
}
                                                       /* line 219 */
function switch1star_reset_handler (eh) {              /* line 220 */
    let  inst =  eh.instance_data;                     /* line 221 */
    inst =  new Switch1star_Instance_Data ();          /* line 222 */;/* line 223 *//* line 224 */
}

function switch1star_instantiate (reg,owner,name,template_data,arg) {/* line 225 */
    let name_with_id = gensymbol ( "switch1*")         /* line 226 */;
    let instp =  new Switch1star_Instance_Data ();     /* line 227 */;
    return make_leaf ( name_with_id, owner, instp, "", switch1star_handler, switch1star_reset_handler)/* line 228 */;/* line 229 *//* line 230 */
}

function switch1star_handler (eh,mev) {                /* line 231 */
    let  inst =  eh.instance_data;                     /* line 232 */
    let whichOutput =  inst.state;                     /* line 233 */
    if ( "" ==  mev.port) {                            /* line 234 */
      if ( "1" ==  whichOutput) {                      /* line 235 */
        forward ( eh, "1", mev)                        /* line 236 */
        inst.state =  "*";                             /* line 237 */
      }
      else if ( "*" ==  whichOutput) {                 /* line 238 */
        forward ( eh, "*", mev)                        /* line 239 */
      }
      else {                                           /* line 240 */
        send ( eh, "✗", "internal error bad state in switch1*", mev)/* line 241 *//* line 242 */
      }
    }
    else if ( "reset" ==  mev.port) {                  /* line 243 */
      inst.state =  "1";                               /* line 244 */
    }
    else {                                             /* line 245 */
      send ( eh, "✗", "internal error bad mevent for switch1*", mev)/* line 246 *//* line 247 */
    }                                                  /* line 248 *//* line 249 */
}

class StringAccumulator {
  constructor () {                                     /* line 250 */

    this.s =  "";                                      /* line 251 *//* line 252 */
  }
}
                                                       /* line 253 */
function strcatstar_reset_handler (eh) {               /* line 254 */
    eh.instance_data =  new StringAccumulator ();      /* line 255 */;/* line 256 *//* line 257 */
}

function strcatstar_instantiate (reg,owner,name,template_data,arg) {/* line 258 */
    let name_with_id = gensymbol ( "String Concat *")  /* line 259 */;
    let instp =  new StringAccumulator ();             /* line 260 */;
    return make_leaf ( name_with_id, owner, instp, "", strcatstar_handler, strcatstar_reset_handler)/* line 261 */;/* line 262 *//* line 263 */
}

function strcatstar_handler (eh,mev) {                 /* line 264 */
    let  accum =  eh.instance_data;                    /* line 265 */
    if ( "" ==  mev.port) {                            /* line 266 */
      accum.s =  ( accum.s.toString ()+  mev.payload.v.toString ()) /* line 267 */;
    }
    else if ( "fini" ==  mev.port) {                   /* line 268 */
      send ( eh, "", accum.s, mev)                     /* line 269 */
    }
    else {                                             /* line 270 */
      send ( eh, "✗", "internal error bad mevent for String Concat *", mev)/* line 271 *//* line 272 */
    }                                                  /* line 273 *//* line 274 */
}

function stop_instantiate (reg,owner,name,template_data,arg) {/* line 275 */
    let name_with_id = gensymbol ( "Stop")             /* line 276 */;
    let inst =  null;                                  /* line 277 */
    return make_leaf ( name_with_id, owner, inst, "", stop_handler, null)/* line 278 */;/* line 279 *//* line 280 */
}

function stop_handler (eh,mev) {                       /* line 281 */
    let  inst =  eh.instance_data;                     /* line 282 */
    let  parent =  eh.owner;                           /* line 283 */
    let  s =  ( "   !!! stopping: '".toString ()+  ( parent.name.toString ()+  "'".toString ()) .toString ()) /* line 284 */;
    console.error ( s);                                /* line 285 */
                                                       /* line 286 */
    parent.stop ( parent)                              /* line 287 */
    send ( eh, "", mev.payload.v, mev)                 /* line 288 *//* line 289 *//* line 290 */
}

/*  all of the the built_in leaves are listed here */  /* line 291 */
/*  future: refactor this such that programmers can pick and choose which (lumps of) builtins are used in a specific project *//* line 292 *//* line 293 */
function initialize_stock_components (reg) {           /* line 294 */
    register_component ( reg,mkTemplate ( "1then2", null, deracer_instantiate))/* line 295 */
    register_component ( reg,mkTemplate ( "1→2", null, deracer_instantiate))/* line 296 */
    register_component ( reg,mkTemplate ( "trash", null, trash_instantiate))/* line 297 */
    register_component ( reg,mkTemplate ( "🗑️", null, trash_instantiate))/* line 298 */
    register_component ( reg,mkTemplate ( "🚫", null, stop_instantiate))/* line 299 *//* line 300 *//* line 301 */
    register_component ( reg,mkTemplate ( "Read Text File", null, low_level_read_text_file_instantiate))/* line 302 */
    register_component ( reg,mkTemplate ( "Ensure String Datum", null, ensure_string_datum_instantiate))/* line 303 *//* line 304 */
    register_component ( reg,mkTemplate ( "syncfilewrite", null, syncfilewrite_instantiate))/* line 305 */
    register_component ( reg,mkTemplate ( "String Concat", null, stringconcat_instantiate))/* line 306 */
    register_component ( reg,mkTemplate ( "switch1*", null, switch1star_instantiate))/* line 307 */
    register_component ( reg,mkTemplate ( "String Concat *", null, strcatstar_instantiate))/* line 308 */
    /*  for fakepipe */                                /* line 309 */
    register_component ( reg,mkTemplate ( "fakepipename", null, fakepipename_instantiate))/* line 310 *//* line 311 *//* line 312 */
}
let  load_errors =  true;                              /* line 1 */
let  runtime_errors =  true;                           /* line 2 *//* line 3 */
function load_error (s) {                              /* line 4 *//* line 5 */
    console.error ( s);                                /* line 6 */
                                                       /* line 7 */
    load_errors =  true;                               /* line 8 *//* line 9 *//* line 10 */
}

function runtime_error (s) {                           /* line 11 *//* line 12 */
    console.error ( s);                                /* line 13 */
    process.exit (1)                                   /* line 14 */
    runtime_errors =  true;                            /* line 15 *//* line 16 *//* line 17 */
}
                                                       /* line 18 */
function initialize_component_palette_from_files (diagram_source_files) {/* line 19 */
    let  reg = make_component_registry ();             /* line 20 */
    for (let diagram_source of  diagram_source_files) {/* line 21 */
      let all_containers_within_single_file = lnet2internal_from_file ( diagram_source)/* line 22 */;
      for (let container of  all_containers_within_single_file) {/* line 23 */
        register_component ( reg,mkTemplate ( container [ "name"], container, container_instantiator))/* line 24 *//* line 25 */
      }                                                /* line 26 */
    }
    initialize_stock_components ( reg)                 /* line 27 */
    return  reg;                                       /* line 28 *//* line 29 *//* line 30 */
}

function initialize_component_palette_from_string (lnet) {/* line 31 */
    let  reg = make_component_registry ();             /* line 32 */
    let all_containers = lnet2internal_from_string ( lnet)/* line 33 */;
    for (let container of  all_containers) {           /* line 34 */
      register_component ( reg,mkTemplate ( container [ "name"], container, container_instantiator))/* line 35 *//* line 36 */
    }
    initialize_stock_components ( reg)                 /* line 37 */
    return  reg;                                       /* line 38 *//* line 39 */
}

function initialize_from_files (diagram_names) {       /* line 40 */
    let arg =  null;                                   /* line 41 */
    let palette = initialize_component_palette_from_files ( diagram_names)/* line 42 */;
    return [ palette,[ diagram_names, arg]];           /* line 43 *//* line 44 *//* line 45 */
}

function initialize_from_string () {                   /* line 46 */
    let arg =  null;                                   /* line 47 */
    let palette = initialize_component_palette_from_string ();/* line 48 */
    return [ palette,[ null, arg]];                    /* line 49 *//* line 50 *//* line 51 */
}

function start (arg,part_name,palette,env) {           /* line 52 */
    let part = start_bare ( part_name, palette, env)   /* line 53 */;
    inject ( part, "", arg)                            /* line 54 */
    finalize ( part)                                   /* line 55 *//* line 56 *//* line 57 */
}

function start_bare (part_name,palette,env) {          /* line 58 */
    let diagram_names =  env [ 0];                     /* line 59 */
    /*  get entrypoint container */                    /* line 60 */
    let  part = get_component_instance ( palette, part_name, null)/* line 61 */;
    if ( null ==  part) {                              /* line 62 */
      load_error ( ( "Couldn;t find container with page name /".toString ()+  ( part_name.toString ()+  ( "/ in files ".toString ()+  (`${ diagram_names}`.toString ()+  " (check tab names, or disable compression?)".toString ()) .toString ()) .toString ()) .toString ()) )/* line 66 *//* line 67 */
    }
    return  part;                                      /* line 68 *//* line 69 *//* line 70 */
}

function inject (part,port,payload) {                  /* line 71 */
    if ((!  load_errors)) {                            /* line 72 */
      let  d =  new Datum ();                          /* line 73 */;
      d.v =  payload;                                  /* line 74 */
      d.clone =  function () {return obj_clone ( d)    /* line 75 */;};
      d.reclaim =  null;                               /* line 76 */
      let  mev = make_mevent ( port, d)                /* line 77 */;
      inject_mevent ( part, mev)                       /* line 78 */
    }
    else {                                             /* line 79 */
      process.exit (1)                                 /* line 80 *//* line 81 */
    }                                                  /* line 82 *//* line 83 */
}

function finalize (part) {                             /* line 84 */
    console.log (JSON.stringify ( part.outq.map(item => ({ [item.port]: item.datum.v })), null, 2));/* line 85 *//* line 86 *//* line 87 */
}

function new_datum_bang () {                           /* line 88 */
    let  d =  new Datum ();                            /* line 89 */;
    d.v =  "!";                                        /* line 90 */
    d.clone =  function () {return obj_clone ( d)      /* line 91 */;};
    d.reclaim =  null;                                 /* line 92 */
    return  d                                          /* line 93 *//* line 94 */;
}
