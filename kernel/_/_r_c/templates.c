#include "pbp.h"

Component_Registry* fresh_Component_Registry () {
    Component_Registry *self;
    self = (Component_Registry*)malloc(sizeof(Component_Registry*));
    self->templates = dict_fresh();                    /* line 2 *//* line 3 */
    return self;
}
                                                       /* line 4 */
Template* fresh_Template () {
    Template *self;
    self = (Template*)malloc(sizeof(Template*));
    self->name =  NULL;                                /* line 6 */
    self->container =  NULL;                           /* line 7 */
    self->instantiator =  NULL;                        /* line 8 *//* line 9 */
    return self;
}
                                                       /* line 10 */
Template* mkTemplate (Str* name,Container* template_data,Finstantiator instantiator) {
                                                       /* line 11 */
    Template*  templ =  fresh_Template ()              /* line 12 */;
    (*templ).name =  name;                             /* line 13 */
    (*templ).template_data =  template_data;           /* line 14 */
    (*templ).instantiator =  instantiator;             /* line 15 */
    return ( templ)                                    /* line 16 */;;;/* line 17 *//* line 18 */}
                                                       /* line 19 */
/*  convert a little-network to internal form (an object data structure created by json parser) ...  *//* line 20 */
/*  the actual data structure depends on the json parser library used by the target language  *//* line 21 */
/*  the form of the data structure doesn;t matter here, as long as we use lookup operators "@" in this .rt code  *//* line 22 *//* line 23 */
/*  ... by reading the little-net from an external file  *//* line 24 */
List_of_Wire* lnet2internal_from_file (Pathname* container_xml) {
                                                       /* line 25 */
    Pathname* pathname = os.getenv('PBPWD', '<none>')  /* line 26 */
    Str* filename =  os.path.basename ( container_xml) /* line 27 */

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
                                                       /* line 28 *//* line 29 *//* line 30 */}

/*  ... by reading the little-net from an embedded string (an aspect of creating t2t tool code)  *//* line 31 */
List_of_Wire* lnet2internal_from_string (Str* lnet) {
                                                       /* line 32 */

    try:
        routings = json.loads(lnet)
        return routings
    except json.JSONDecodeError as e:
        print ("Error decoding JSON from string 'lnet': '{e}'")
        return None
                                                       /* line 33 *//* line 34 *//* line 35 */}

Component_Registry* make_component_registry () {
                                                       /* line 36 */
    return ( fresh_Component_Registry ()               /* line 37 */)/* line 38 *//* line 39 */}

Component_Registry* register_component (Component_Registry* reg,Template* template) {

    return (abstracted_register_component ( reg, template, FALSE))/* line 40 */}

Component_Registry* register_component_allow_overwriting (Component_Registry* reg,Template* template) {

    return (abstracted_register_component ( reg, template, TRUE))/* line 41 *//* line 42 */}

void abstracted_register_component (Component_Registry* reg,Template* template,Bool ok_to_overwrite) {
                                                       /* line 43 */
    Str* name = mangle_name (  (*template).name)       /* line 44 */
    if  reg!= NULL and  name in   (*reg).templates and not  ok_to_overwrite:/* line 45 */
        load_error ( str( "Component /") +  str(  (*template).name) +  "/ already declared"  )/* line 46 */
        return ( reg)                                  /* line 47 */
    else:                                              /* line 48 */
        lookupid (  (*reg).templates, name) =  template;/* line 49 */
        return ( reg)                                  /* line 50 */;/* line 51 *//* line 52 *//* line 53 */}

Part* get_component_instance (Component_Registry* reg,Str* full_name,Container* owner) {
                                                       /* line 54 */
    /*  If a part name begins with ":", it is treated as a JIT part and we let the runtime factory generate it on-the-fly (see kernel_external.rt and external.rt) else it is assumed to be a regular AOT part and assumed to have been registered before runtime, so we just pull its template out of the registry and instantiate it.  *//* line 55 */
    /*  ":?<string>" is a probe part that is tagged with <string>  *//* line 56 */
    /*  ":$ <command>" is a shell-out part that sends <command> to the operating system shell  *//* line 57 */
    /*  ":<string>" else, it's just treated as a string part that produces <string> on its output  *//* line 58 */
    Str* template_name = mangle_name ( full_name)      /* line 59 */
    if  ":" ==   full_name[0] :                        /* line 60 */
        Str* instance_name = generate_instance_name ( owner, template_name)/* line 61 */
        Part* instance = jit_instantiate ( reg, owner, instance_name, full_name)/* line 62 */
        return ( instance)                             /* line 63 */
    else:                                              /* line 64 */
        if  template_name in   (*reg).templates:       /* line 65 */
            Template* template = lookupid (  (*reg).templates, template_name)/* line 66 */
            if ( template ==  NULL):                   /* line 67 */
                load_error ( str( "Registry Error (A): Can't find component /") +  str( template_name) +  "/"  )/* line 68 */
                return ( NULL)                         /* line 69 */
            else:                                      /* line 70 */
                Str* instance_name = generate_instance_name ( owner, template_name)/* line 71 */
                Part* instance =   (*template).instantiator ( reg, owner, instance_name,  (*template).template_data, "")/* line 72 */
                return ( instance)                     /* line 73 *//* line 74 */
        else:                                          /* line 75 */
            load_error ( str( "Registry Error (B): Can't find component /") +  str( template_name) +  "/"  )/* line 76 */
            return ( NULL)                             /* line 77 *//* line 78 *//* line 79 *//* line 80 *//* line 81 */}

Str* generate_instance_name (Container* owner,Str* template_name) {
                                                       /* line 82 */
    Str* owner_name =  ""                              /* line 83 */
    Str* instance_name =  template_name                /* line 84 */
    if  NULL!= owner:                                  /* line 85 */
        owner_name =   (*owner).name;                  /* line 86 */
        instance_name =  str( owner_name) +  str( "▹") +  template_name  /* line 87 */;;;
    else:                                              /* line 88 */
        instance_name =  template_name;;               /* line 89 *//* line 90 */
    return ( instance_name)                            /* line 91 *//* line 92 *//* line 93 */}

Str* mangle_name (Str* s) {
                                                       /* line 94 */
    /*  trim name to remove code from Container component names _ deferred until later (or never) *//* line 95 */
    return ( s)                                        /* line 96 *//* line 97 */}
