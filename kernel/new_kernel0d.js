import * as fs from 'fs';
import path from 'path';
import execSync from 'child_process';
import 'dotenv/config';
                                                       /* line 1 *//* line 2 */
let  counter =  0;                                     /* line 3 */
let  ticktime =  0;                                    /* line 4 *//* line 5 */
let  digits = [ "₀", "₁", "₂", "₃", "₄", "₅", "₆", "₇", "₈", "₉", "₁₀", "₁₁", "₁₂", "₁₃", "₁₄", "₁₅", "₁₆", "₁₇", "₁₈", "₁₉", "₂₀", "₂₁", "₂₂", "₂₃", "₂₄", "₂₅", "₂₆", "₂₇", "₂₈", "₂₉"];/* line 12 *//* line 13 *//* line 14 */
function gensymbol (s) {                               /* line 15 *//* line 16 */
    let name_with_id =  ( s.toString ()+ subscripted_digit ( counter).toString ()) /* line 17 */;
    counter =  counter+ 1;                             /* line 18 */
    return  name_with_id;                              /* line 19 *//* line 20 *//* line 21 */
}

function subscripted_digit (n) {                       /* line 22 *//* line 23 */
    if (((( n >=  0) && ( n <=  29)))) {               /* line 24 */
      return  digits [ n];                             /* line 25 */
    }
    else {                                             /* line 26 */
      return  ( "₊".toString ()+ `${ n}`.toString ())  /* line 27 */;/* line 28 */
    }                                                  /* line 29 *//* line 30 */
}

class Datum {
  constructor () {                                     /* line 31 */

    this.v =  null;                                    /* line 32 */
    this.clone =  null;                                /* line 33 */
    this.reclaim =  null;                              /* line 34 */
    this.other =  null;/*  reserved for use on per-project basis  *//* line 35 *//* line 36 */
  }
}
                                                       /* line 37 *//* line 38 */
/*  Mevent passed to a leaf component. */              /* line 39 */
/*  */                                                 /* line 40 */
/*  `port` refers to the name of the incoming or outgoing port of this component. *//* line 41 */
/*  `payload` is the data attached to this mevent. */  /* line 42 */
class Mevent {
  constructor () {                                     /* line 43 */

    this.port =  null;                                 /* line 44 */
    this.datum =  null;                                /* line 45 *//* line 46 */
  }
}
                                                       /* line 47 */
function clone_port (s) {                              /* line 48 */
    return clone_string ( s)                           /* line 49 */;/* line 50 *//* line 51 */
}

/*  Utility for making a `Mevent`. Used to safely "seed“ mevents *//* line 52 */
/*  entering the very top of a network. */             /* line 53 */
function make_mevent (port,datum) {                    /* line 54 */
    let p = clone_string ( port)                       /* line 55 */;
    let  m =  new Mevent ();                           /* line 56 */;
    m.port =  p;                                       /* line 57 */
    m.datum =  datum.clone ();                         /* line 58 */
    return  m;                                         /* line 59 *//* line 60 *//* line 61 */
}

/*  Clones a mevent. Primarily used internally for “fanning out“ a mevent to multiple destinations. *//* line 62 */
function mevent_clone (mev) {                          /* line 63 */
    let  m =  new Mevent ();                           /* line 64 */;
    m.port = clone_port ( mev.port)                    /* line 65 */;
    m.datum =  mev.datum.clone ();                     /* line 66 */
    return  m;                                         /* line 67 *//* line 68 *//* line 69 */
}

/*  Frees a mevent. */                                 /* line 70 */
function destroy_mevent (mev) {                        /* line 71 */
    /*  during debug, dont destroy any mevent, since we want to trace mevents, thus, we need to persist ancestor mevents *//* line 72 *//* line 73 *//* line 74 *//* line 75 */
}

function destroy_datum (mev) {                         /* line 76 *//* line 77 *//* line 78 *//* line 79 */
}

function destroy_port (mev) {                          /* line 80 *//* line 81 *//* line 82 *//* line 83 */
}

/*  */                                                 /* line 84 */
function format_mevent (m) {                           /* line 85 */
    if ( m ==  null) {                                 /* line 86 */
      return  "{}";                                    /* line 87 */
    }
    else {                                             /* line 88 */
      return  ( "{%5C”".toString ()+  ( m.port.toString ()+  ( "%5C”:%5C”".toString ()+  ( m.datum.v.toString ()+  "%5C”}".toString ()) .toString ()) .toString ()) .toString ()) /* line 89 */;/* line 90 */
    }                                                  /* line 91 */
}

function format_mevent_raw (m) {                       /* line 92 */
    if ( m ==  null) {                                 /* line 93 */
      return  "";                                      /* line 94 */
    }
    else {                                             /* line 95 */
      return  m.datum.v;                               /* line 96 *//* line 97 */
    }                                                  /* line 98 *//* line 99 */
}

const  enumDown =  0                                   /* line 100 */;
const  enumAcross =  1                                 /* line 101 */;
const  enumUp =  2                                     /* line 102 */;
const  enumThrough =  3                                /* line 103 */;/* line 104 *//* line 105 */
class Component_Registry {
  constructor () {                                     /* line 106 */

    this.templates = {};                               /* line 107 *//* line 108 */
  }
}
                                                       /* line 109 */
class Template {
  constructor () {                                     /* line 110 */

    this.name =  null;                                 /* line 111 */
    this.container =  null;                            /* line 112 */
    this.instantiator =  null;                         /* line 113 *//* line 114 */
  }
}
                                                       /* line 115 */
/*  Routing connection for a container component. The `direction` field has *//* line 116 */
/*  no affect on the default mevent routing system _ it is there for debugging *//* line 117 */
/*  purposes, or for reading by other tools. */        /* line 118 *//* line 119 */
class Connector {
  constructor () {                                     /* line 120 */

    this.direction =  null;/*  down, across, up, through *//* line 121 */
    this.sender =  null;                               /* line 122 */
    this.receiver =  null;                             /* line 123 *//* line 124 */
  }
}
                                                       /* line 125 */
/*  `Sender` is used to “pattern match“ which `Receiver` a mevent should go to, *//* line 126 */
/*  based on component ID (pointer) and port name. */  /* line 127 *//* line 128 */
class Sender {
  constructor () {                                     /* line 129 */

    this.name =  null;                                 /* line 130 */
    this.component =  null;                            /* line 131 */
    this.port =  null;                                 /* line 132 *//* line 133 */
  }
}
                                                       /* line 134 *//* line 135 *//* line 136 */
/*  `Receiver` is a handle to a destination queue, and a `port` name to assign *//* line 137 */
/*  to incoming mevents to this queue. */              /* line 138 *//* line 139 */
class Receiver {
  constructor () {                                     /* line 140 */

    this.name =  null;                                 /* line 141 */
    this.queue =  null;                                /* line 142 */
    this.port =  null;                                 /* line 143 */
    this.component =  null;                            /* line 144 *//* line 145 */
  }
}
                                                       /* line 146 */
function mkSender (name,component,port) {              /* line 147 */
    let  s =  new Sender ();                           /* line 148 */;
    s.name =  name;                                    /* line 149 */
    s.component =  component;                          /* line 150 */
    s.port =  port;                                    /* line 151 */
    return  s;                                         /* line 152 *//* line 153 *//* line 154 */
}

function mkReceiver (name,component,port,q) {          /* line 155 */
    let  r =  new Receiver ();                         /* line 156 */;
    r.name =  name;                                    /* line 157 */
    r.component =  component;                          /* line 158 */
    r.port =  port;                                    /* line 159 */
    /*  We need a way to determine which queue to target. "Down" and "Across" go to inq, "Up" and "Through" go to outq. *//* line 160 */
    r.queue =  q;                                      /* line 161 */
    return  r;                                         /* line 162 *//* line 163 *//* line 164 */
}
                                                       /* line 165 */
class Component_Registry {
  constructor () {                                     /* line 166 */

    this.templates = {};                               /* line 167 *//* line 168 */
  }
}
                                                       /* line 169 */
class Template {
  constructor () {                                     /* line 170 */

    this.name =  null;                                 /* line 171 */
    this.container =  null;                            /* line 172 */
    this.instantiator =  null;                         /* line 173 *//* line 174 */
  }
}
                                                       /* line 175 */
function mkTemplate (name,template_data,instantiator) {/* line 176 */
    let  templ =  new Template ();                     /* line 177 */;
    templ.name =  name;                                /* line 178 */
    templ.template_data =  template_data;              /* line 179 */
    templ.instantiator =  instantiator;                /* line 180 */
    return  templ;                                     /* line 181 *//* line 182 *//* line 183 */
}

function make_component_registry () {                  /* line 184 */
    return  new Component_Registry ();                 /* line 185 */;/* line 186 *//* line 187 */
}

/*  Data for an asyncronous component _ effectively, a function with input *//* line 188 */
/*  and output queues of mevents. */                   /* line 189 */
/*  */                                                 /* line 190 */
/*  Components can either be a user_supplied function (“leaf“), or a “container“ *//* line 191 */
/*  that routes mevents to child components according to a list of connections *//* line 192 */
/*  that serve as a mevent routing table. */           /* line 193 */
/*  */                                                 /* line 194 */
/*  Child components themselves can be leaves or other containers. *//* line 195 */
/*  */                                                 /* line 196 */
/*  `handler` invokes the code that is attached to this component. *//* line 197 */
/*  */                                                 /* line 198 */
/*  `instance_data` is a pointer to instance data that the `leaf_handler` *//* line 199 */
/*  function may want whenever it is invoked again. */ /* line 200 *//* line 201 */
/*  TODO: what is .routings for? (is it a historical artefact that can be removed?)  *//* line 202 *//* line 203 */
/*  Eh_States :: enum { idle, active } */              /* line 204 */
class Eh {
  constructor () {                                     /* line 205 */

    this.name =  "";                                   /* line 206 */
    this.inq =  []                                     /* line 207 */;
    this.outq =  []                                    /* line 208 */;
    this.owner =  null;                                /* line 209 */
    this.children = [];                                /* line 210 */
    this.visit_ordering =  []                          /* line 211 */;
    this.connections = [];                             /* line 212 */
    this.routings =  []                                /* line 213 */;
    this.handler =  null;                              /* line 214 */
    this.reset_instance_data =  null;                  /* line 215 */
    this.finject =  null;                              /* line 216 */
    this.stop =  null;                                 /* line 217 */
    this.instance_data =  null;                        /* line 218 *//*  arg needed for probe support  *//* line 219 */
    this.arg =  "";                                    /* line 220 */
    this.state =  "idle";                              /* line 221 */
    this.special =  false;                             /* line 222 *//*  bootstrap debugging *//* line 223 */
    this.kind =  null;/*  enum { container, leaf, } */ /* line 224 *//* line 225 */
  }
}
                                                       /* line 226 */
let  load_errors =  false;                             /* line 227 */
let  runtime_errors =  false;                          /* line 228 *//* line 229 */
function clone_string (s) {                            /* line 230 */
    return  s;                                         /* line 231 *//* line 232 *//* line 233 */
}

function injector (eh,mevent) {                        /* line 234 */
    eh.handler ( eh, mevent)                           /* line 235 *//* line 236 *//* line 237 */
}
function mkTemplate (name,template_data,instantiator) {/* line 1 */
    let  templ =  new Template ();                     /* line 2 */;
    templ.name =  name;                                /* line 3 */
    templ.template_data =  template_data;              /* line 4 */
    templ.instantiator =  instantiator;                /* line 5 */
    return  templ;                                     /* line 6 *//* line 7 *//* line 8 */
}
                                                       /* line 9 */
/*  convert a little-network to internal form (an object data structure created by json parser) ...  *//* line 10 */
/*  the actual data structure depends on the json parser library used by the target language  *//* line 11 */
/*  the form of the data structure doesn;t matter here, as long as we use lookup operators "@" in this .rt code  *//* line 12 *//* line 13 */
/*  ... by reading the little-net from an external file  *//* line 14 */
function lnet2internal_from_file (container_xml) {     /* line 15 */
    let pathname = process.env.PBPWD                   /* line 16 */;
    let filename =   container_xml                     /* line 17 */;

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
                                                       /* line 18 *//* line 19 *//* line 20 */
}

/*  ... by reading the little-net from an embedded string (an aspect of creating t2t tool code)  *//* line 21 */
function lnet2internal_from_string (lnet) {            /* line 22 */

    return JSON.parse (lnet);
                                                       /* line 23 *//* line 24 *//* line 25 */
}

function delete_decls (d) {                            /* line 26 *//* line 27 *//* line 28 *//* line 29 */
}

function make_component_registry () {                  /* line 30 */
    return  new Component_Registry ();                 /* line 31 */;/* line 32 *//* line 33 */
}

function register_component (reg,template) {
    return abstracted_register_component ( reg, template, false);/* line 34 */
}

function register_component_allow_overwriting (reg,template) {
    return abstracted_register_component ( reg, template, true);/* line 35 *//* line 36 */
}

function abstracted_register_component (reg,template,ok_to_overwrite) {/* line 37 */
    let name = mangle_name ( template.name)            /* line 38 */;
    if ((((((( reg!= null) && ( name))) in ( reg.templates))) && ((!  ok_to_overwrite)))) {/* line 39 */
      load_error ( ( "Component /".toString ()+  ( template.name.toString ()+  "/ already declared".toString ()) .toString ()) )/* line 40 */
      return  reg;                                     /* line 41 */
    }
    else {                                             /* line 42 */
      reg.templates [name] =  template;                /* line 43 */
      return  reg;                                     /* line 44 *//* line 45 */
    }                                                  /* line 46 *//* line 47 */
}

function get_component_instance (reg,full_name,owner) {/* line 48 */
    /*  If a part name begins with ":", it is treated as a JIT part and we let the runtime factory generate it on-the-fly (see kernel_external.rt and external.rt) else it is assumed to be a regular AOT part and assumed to have been registered before runtime, so we just pull its template out of the registry and instantiate it.  *//* line 49 */
    /*  ":?<string>" is a probe part that is tagged with <string>  *//* line 50 */
    /*  ":$ <command>" is a shell-out part that sends <command> to the operating system shell  *//* line 51 */
    /*  ":<string>" else, it's just treated as a string part that produces <string> on its output  *//* line 52 */
    let template_name = mangle_name ( full_name)       /* line 53 */;
    if ( ":" ==   full_name[0] ) {                     /* line 54 */
      let instance_name = generate_instance_name ( owner, template_name)/* line 55 */;
      let instance = jit_instantiate ( reg, owner, instance_name, full_name)/* line 56 */;
      return  instance;                                /* line 57 */
    }
    else {                                             /* line 58 */
      if ((( template_name) in ( reg.templates))) {    /* line 59 */
        let template =  reg.templates [template_name]; /* line 60 */
        if (( template ==  null)) {                    /* line 61 */
          load_error ( ( "Registry Error (A): Can't find component /".toString ()+  ( template_name.toString ()+  "/".toString ()) .toString ()) )/* line 62 */
          return  null;                                /* line 63 */
        }
        else {                                         /* line 64 */
          let instance_name = generate_instance_name ( owner, template_name)/* line 65 */;
          let instance =  template.instantiator ( reg, owner, instance_name, template.template_data, "")/* line 66 */;
          return  instance;                            /* line 67 *//* line 68 */
        }
      }
      else {                                           /* line 69 */
        load_error ( ( "Registry Error (B): Can't find component /".toString ()+  ( template_name.toString ()+  "/".toString ()) .toString ()) )/* line 70 */
        return  null;                                  /* line 71 *//* line 72 */
      }                                                /* line 73 */
    }                                                  /* line 74 *//* line 75 */
}

function generate_instance_name (owner,template_name) {/* line 76 */
    let owner_name =  "";                              /* line 77 */
    let instance_name =  template_name;                /* line 78 */
    if ( null!= owner) {                               /* line 79 */
      owner_name =  owner.name;                        /* line 80 */
      instance_name =  ( owner_name.toString ()+  ( "▹".toString ()+  template_name.toString ()) .toString ()) /* line 81 */;
    }
    else {                                             /* line 82 */
      instance_name =  template_name;                  /* line 83 *//* line 84 */
    }
    return  instance_name;                             /* line 85 *//* line 86 *//* line 87 */
}

function mangle_name (s) {                             /* line 88 */
    /*  trim name to remove code from Container component names _ deferred until later (or never) *//* line 89 */
    return  s;                                         /* line 90 *//* line 91 */
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
    }                                                  /* line 152 *//* line 153 */
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

function append_routing_descriptor (container,desc) {  /* line 247 */
    container.routings.push ( desc)                    /* line 248 *//* line 249 *//* line 250 */
}
                                                       /* line 251 */
/*  Creates a component that acts as a container. It is the same as a `Eh` instance *//* line 252 */
/*  whose handler function is `container_handler`. */  /* line 253 */
function make_container (name,owner) {                 /* line 254 */
    let  eh =  new Eh ();                              /* line 255 */;
    eh.name =  name;                                   /* line 256 */
    eh.owner =  owner;                                 /* line 257 */
    eh.handler =  container_handler;                   /* line 258 */
    eh.finject =  injector;                            /* line 259 */
    eh.stop =  container_reset_children;               /* line 260 */
    eh.state =  "idle";                                /* line 261 */
    eh.kind =  "container";                            /* line 262 */
    return  eh;                                        /* line 263 *//* line 264 *//* line 265 */
}

/*  Sends a mevent on the given `port` with `data`, placing it on the output *//* line 266 */
/*  of the given component. */                         /* line 267 *//* line 268 */
function send (eh,port,obj,causingMevent) {            /* line 269 */
    let  d =  new Datum ();                            /* line 270 */;
    d.v =  obj;                                        /* line 271 */
    d.clone =  function () {return obj_clone ( d)      /* line 272 */;};
    d.reclaim =  null;                                 /* line 273 */
    let mev = make_mevent ( port, d)                   /* line 274 */;
    put_output ( eh, mev)                              /* line 275 *//* line 276 *//* line 277 */
}

function forward (eh,port,mev) {                       /* line 278 */
    let fwdmev = make_mevent ( port, mev.datum)        /* line 279 */;
    put_output ( eh, fwdmev)                           /* line 280 *//* line 281 *//* line 282 */
}

function inject_mevent (eh,mev) {                      /* line 283 */
    eh.finject ( eh, mev)                              /* line 284 *//* line 285 *//* line 286 */
}

function set_active (eh) {                             /* line 287 */
    eh.state =  "active";                              /* line 288 *//* line 289 *//* line 290 */
}

function set_idle (eh) {                               /* line 291 */
    eh.state =  "idle";                                /* line 292 *//* line 293 *//* line 294 */
}

function put_output (eh,mev) {                         /* line 295 */
    eh.outq.push ( mev)                                /* line 296 *//* line 297 *//* line 298 */
}

function obj_clone (obj) {                             /* line 299 */
    return  obj;                                       /* line 300 *//* line 301 */
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
    let s =  mev.datum.v;                              /* line 28 */
    console.error ( "Info" + ": " +  ( "  @".toString ()+  (`${ ticktime}`.toString ()+  ( "  ".toString ()+  ( "probe ".toString ()+  ( eh.name.toString ()+  ( ": ".toString ()+ `${ s}`.toString ()) .toString ()) .toString ()) .toString ()) .toString ()) .toString ()) )/* line 36 *//* line 37 *//* line 38 */
}

function shell_out_handler (eh,cmd,mev) {              /* line 39 */
    let s =  mev.datum.v;                              /* line 40 */
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
    let fname =  mev.datum.v;                          /* line 86 */

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
    if ( "string" ==  mev.datum.kind ()) {             /* line 96 */
      forward ( eh, "", mev)                           /* line 97 */
    }
    else {                                             /* line 98 */
      let emev =  ( "*** ensure: type error (expected a string datum) but got ".toString ()+  mev.datum.toString ()) /* line 99 */;
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
      inst.filename =  mev.datum.v;                    /* line 122 */
    }
    else if ( "input" ==  mev.port) {                  /* line 123 */
      let contents =  mev.datum.v;                     /* line 124 */
      let  f = open ( inst.filename, "w")              /* line 125 */;
      if ( f!= null) {                                 /* line 126 */
        f.write ( mev.datum.v)                         /* line 127 */
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
      inst.buffer1 = clone_string ( mev.datum.v)       /* line 156 */;
      maybe_stringconcat ( eh, inst, mev)              /* line 157 */
    }
    else if ( "2" ==  mev.port) {                      /* line 158 */
      inst.buffer2 = clone_string ( mev.datum.v)       /* line 159 */;
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
      accum.s =  ( accum.s.toString ()+  mev.datum.v.toString ()) /* line 267 */;
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
    send ( eh, "", mev.datum.v, mev)                   /* line 288 *//* line 289 *//* line 290 */
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
/* line 1 */
function load_error (s) {                              /* line 2 *//* line 3 */
    console.error ( s);                                /* line 4 */
                                                       /* line 5 */
    load_errors =  true;                               /* line 6 *//* line 7 *//* line 8 */
}

function runtime_error (s) {                           /* line 9 *//* line 10 */
    console.error ( s);                                /* line 11 */
    process.exit (1)                                   /* line 12 */
    runtime_errors =  true;                            /* line 13 *//* line 14 *//* line 15 */
}
                                                       /* line 16 */
function initialize_component_palette_from_files (diagram_source_files) {/* line 17 */
    let  reg = make_component_registry ();             /* line 18 */
    for (let diagram_source of  diagram_source_files) {/* line 19 */
      let all_containers_within_single_file = lnet2internal_from_file ( diagram_source)/* line 20 */;
      for (let container of  all_containers_within_single_file) {/* line 21 */
        register_component ( reg,mkTemplate ( container [ "name"], container, container_instantiator))/* line 22 *//* line 23 */
      }                                                /* line 24 */
    }
    initialize_stock_components ( reg)                 /* line 25 */
    return  reg;                                       /* line 26 *//* line 27 *//* line 28 */
}

function initialize_component_palette_from_string (lnet) {/* line 29 */
    let  reg = make_component_registry ();             /* line 30 */
    let all_containers = lnet2internal_from_string ( lnet)/* line 31 */;
    for (let container of  all_containers) {           /* line 32 */
      register_component ( reg,mkTemplate ( container [ "name"], container, container_instantiator))/* line 33 *//* line 34 */
    }
    initialize_stock_components ( reg)                 /* line 35 */
    return  reg;                                       /* line 36 *//* line 37 */
}

function initialize_from_files (diagram_names) {       /* line 38 */
    let arg =  null;                                   /* line 39 */
    let palette = initialize_component_palette_from_files ( diagram_names)/* line 40 */;
    return [ palette,[ diagram_names, arg]];           /* line 41 *//* line 42 *//* line 43 */
}

function initialize_from_string () {                   /* line 44 */
    let arg =  null;                                   /* line 45 */
    let palette = initialize_component_palette_from_string ();/* line 46 */
    return [ palette,[ null, arg]];                    /* line 47 *//* line 48 *//* line 49 */
}

function start (arg,part_name,palette,env) {           /* line 50 */
    let part = start_bare ( part_name, palette, env)   /* line 51 */;
    inject ( part, "", arg)                            /* line 52 */
    finalize ( part)                                   /* line 53 *//* line 54 *//* line 55 */
}

function start_bare (part_name,palette,env) {          /* line 56 */
    let diagram_names =  env [ 0];                     /* line 57 */
    /*  get entrypoint container */                    /* line 58 */
    let  part = get_component_instance ( palette, part_name, null)/* line 59 */;
    if ( null ==  part) {                              /* line 60 */
      load_error ( ( "Couldn;t find container with page name /".toString ()+  ( part_name.toString ()+  ( "/ in files ".toString ()+  (`${ diagram_names}`.toString ()+  " (check tab names, or disable compression?)".toString ()) .toString ()) .toString ()) .toString ()) )/* line 64 *//* line 65 */
    }
    return  part;                                      /* line 66 *//* line 67 *//* line 68 */
}

function inject (part,port,payload) {                  /* line 69 */
    if ((!  load_errors)) {                            /* line 70 */
      let  d =  new Datum ();                          /* line 71 */;
      d.v =  payload;                                  /* line 72 */
      d.clone =  function () {return obj_clone ( d)    /* line 73 */;};
      d.reclaim =  null;                               /* line 74 */
      let  mev = make_mevent ( port, d)                /* line 75 */;
      inject_mevent ( part, mev)                       /* line 76 */
    }
    else {                                             /* line 77 */
      process.exit (1)                                 /* line 78 *//* line 79 */
    }                                                  /* line 80 *//* line 81 */
}

function finalize (part) {                             /* line 82 */
    console.log (JSON.stringify ( part.outq.map(item => ({ [item.port]: item.datum.v })), null, 2));/* line 83 *//* line 84 *//* line 85 */
}

function new_datum_bang () {                           /* line 86 */
    let  d =  new Datum ();                            /* line 87 */;
    d.v =  "!";                                        /* line 88 */
    d.clone =  function () {return obj_clone ( d)      /* line 89 */;};
    d.reclaim =  null;                                 /* line 90 */
    return  d                                          /* line 91 *//* line 92 */;
}
