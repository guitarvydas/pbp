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
