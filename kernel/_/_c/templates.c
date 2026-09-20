typedef struct _Component_Registry {
                                                       /* line 1 */
    templates;                                         /* line 2 *//* line 3 */
} Component_Registry;
Component_Registry fresh_Component_Registry () {
    Component_Registry *self;
    self = (Mevent*)malloc(sizeof(Mevent));
    self->templates = {};                              /* line 2 *//* line 3 */
    return self;
}
                                                       /* line 4 */
void mkTemplate (name,template_data,instantiator) {
                                                       /* line 5 */
    templ =  Template ()                               /* line 6 */
    templ.name =  name                                 /* line 7 */
    templ.template_data =  template_data               /* line 8 */
    templ.instantiator =  instantiator                 /* line 9 */
    return ( templ)                                    /* line 10 */;;;/* line 11 *//* line 12 */}
                                                       /* line 13 */
/*  convert a little-network to internal form (an object data structure created by json parser) ...  *//* line 14 */
/*  the actual data structure depends on the json parser library used by the target language  *//* line 15 */
/*  the form of the data structure doesn;t matter here, as long as we use lookup operators "@" in this .rt code  *//* line 16 *//* line 17 */
/*  ... by reading the little-net from an external file  *//* line 18 */
void lnet2internal_from_file (container_xml) {
                                                       /* line 19 */
    pathname = os.getenv('PBPWD', '<none>')            /* line 20 */
    filename =  os.path.basename ( container_xml)      /* line 21 */
    external
    try:
        fil = open(filename, "r")
        json_data = fil.read()
        routings = json.loads(json_data)
        fil.close ()
        return routings
    except FileNotFoundError:
        print (f"File not found: '{filename}'", file=sys.stderr)
        return None
    except json.JSONDecodeError as e:
        print (f"Error decoding JSON in path /{pathname}/: '{e}'", file=sys.stderr)
        return None
                                                       /* line 22 *//* line 23 *//* line 24 */}

/*  ... by reading the little-net from an embedded string (an aspect of creating t2t tool code)  *//* line 25 */
void lnet2internal_from_string (lnet) {
                                                       /* line 26 */
    external
    try:
        routings = json.loads(lnet)
        return routings
    except json.JSONDecodeError as e:
        print ("Error decoding JSON from string 'lnet': '{e}'")
        return None
                                                       /* line 27 *//* line 28 *//* line 29 */}

void delete_decls (d) {
                                                       /* line 30 */
                                                       /* line 31 *//* line 32 *//* line 33 */}

void make_component_registry () {
                                                       /* line 34 */
    return ( Component_Registry ()                     /* line 35 */)/* line 36 *//* line 37 */}

void register_component (reg,template) {

    return (abstracted_register_component ( reg, template, False))/* line 38 */}

void register_component_allow_overwriting (reg,template) {

    return (abstracted_register_component ( reg, template, True))/* line 39 *//* line 40 */}

void abstracted_register_component (reg,template,ok_to_overwrite) {
                                                       /* line 41 */
    name = mangle_name ( template.name)                /* line 42 */
    if  reg!= NULL and  name in  reg.templates and not  ok_to_overwrite:/* line 43 */
        load_error ( str( "Component /") +  str( template.name) +  "/ already declared"  )/* line 44 */
        return ( reg)                                  /* line 45 */
    else:                                              /* line 46 */
        reg.templates [name] =  template               /* line 47 */
        return ( reg)                                  /* line 48 */;/* line 49 *//* line 50 *//* line 51 */}

void get_component_instance (reg,full_name,owner) {
                                                       /* line 52 */
    /*  If a part name begins with ":", it is treated as a JIT part and we let the runtime factory generate it on-the-fly (see kernel_external.rt and external.rt) else it is assumed to be a regular AOT part and assumed to have been registered before runtime, so we just pull its template out of the registry and instantiate it.  *//* line 53 */
    /*  ":?<string>" is a probe part that is tagged with <string>  *//* line 54 */
    /*  ":$ <command>" is a shell-out part that sends <command> to the operating system shell  *//* line 55 */
    /*  ":<string>" else, it's just treated as a string part that produces <string> on its output  *//* line 56 */
    template_name = mangle_name ( full_name)           /* line 57 */
    if  ":" ==   full_name[0] :                        /* line 58 */
        instance_name = generate_instance_name ( owner, template_name)/* line 59 */
        instance = jit_instantiate ( reg, owner, instance_name, full_name)/* line 60 */
        return ( instance)                             /* line 61 */
    else:                                              /* line 62 */
        if  template_name in  reg.templates:           /* line 63 */
            template =  reg.templates [template_name]  /* line 64 */
            if ( template ==  NULL):                   /* line 65 */
                load_error ( str( "Registry Error (A): Can't find component /") +  str( template_name) +  "/"  )/* line 66 */
                return ( NULL)                         /* line 67 */
            else:                                      /* line 68 */
                instance_name = generate_instance_name ( owner, template_name)/* line 69 */
                instance =  template.instantiator ( reg, owner, instance_name, template.template_data, "")/* line 70 */
                return ( instance)                     /* line 71 *//* line 72 */
        else:                                          /* line 73 */
            load_error ( str( "Registry Error (B): Can't find component /") +  str( template_name) +  "/"  )/* line 74 */
            return ( NULL)                             /* line 75 *//* line 76 *//* line 77 *//* line 78 *//* line 79 */}

void generate_instance_name (owner,template_name) {
                                                       /* line 80 */
    owner_name =  ""                                   /* line 81 */
    instance_name =  template_name                     /* line 82 */
    if  NULL!= owner:                                  /* line 83 */
        owner_name =  owner.name                       /* line 84 */
        instance_name =  str( owner_name) +  str( "▹") +  template_name  /* line 85 */;;
    else:                                              /* line 86 */
        instance_name =  template_name;                /* line 87 *//* line 88 */
    return ( instance_name)                            /* line 89 *//* line 90 *//* line 91 */}

void mangle_name (s) {
                                                       /* line 92 */
    /*  trim name to remove code from Container component names _ deferred until later (or never) *//* line 93 */
    return ( s)                                        /* line 94 *//* line 95 */}
