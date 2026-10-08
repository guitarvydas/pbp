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
