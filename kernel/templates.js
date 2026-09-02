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
