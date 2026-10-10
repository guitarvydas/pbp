#include "pbp.h"

Bool =  FALSE                                          /* line 1 */;
Bool =  FALSE                                          /* line 2 */;
Int =  0                                               /* line 3 */;/* line 4 */
void load_error (Str s) {
                                                       /* line 5 */
    static load_errors                                 /* line 6 */
    print ( s, file=sys.stderr)                        /* line 7 */
                                                       /* line 8 */
    load_errors =  TRUE;;                              /* line 9 *//* line 10 *//* line 11 */}

void runtime_error (Str s) {
                                                       /* line 12 */
    static runtime_errors                              /* line 13 */
    print ( s, file=sys.stderr)                        /* line 14 */
    exit (1)                                           /* line 15 */
    runtime_errors =  TRUE;;                           /* line 16 *//* line 17 *//* line 18 */}
                                                       /* line 19 */
Component_Registry* initialize_component_palette_from_files (List_of_Pathname* diagram_source_files) {
                                                       /* line 20 */
    Component_Registry*  reg = make_component_registry ();/* line 21 */
    for diagram_source in  diagram_source_files:       /* line 22 */
        List_of_Container* all_containers_within_single_file = lnet2internal_from_file ( diagram_source)/* line 23 */
        for container in  all_containers_within_single_file:/* line 24 */
            register_component ( reg,mkTemplate (lookupstring ( container, "name"), container, container_instantiator))/* line 25 *//* line 26 *//* line 27 */
    initialize_stock_components ( reg)                 /* line 28 */
    return ( reg)                                      /* line 29 *//* line 30 *//* line 31 */}

void initialize_component_palette_from_string (JSONStr* lnet) {
                                                       /* line 32 */
    Component_Registry*  reg = make_component_registry ();/* line 33 */
    List_of_Container* all_containers = lnet2internal_from_string ( lnet)/* line 34 */
    for container in  all_containers:                  /* line 35 */
        register_component ( reg,mkTemplate (lookupstring ( container, "name"), container, container_instantiator))/* line 36 *//* line 37 */
    initialize_stock_components ( reg)                 /* line 38 */
    return ( reg)                                      /* line 39 *//* line 40 */}

Tuple_Palette_DiagramNames_ArgStr* initialize_from_files (List_of_DiagramsName* diagram_names) {
                                                       /* line 41 */
    Str arg =  NULL                                    /* line 42 */
    Component_Registry* palette = initialize_component_palette_from_files ( diagram_names)/* line 43 */
    return [ palette,[ diagram_names, arg]]            /* line 44 *//* line 45 *//* line 46 */}

Tuple_Palette_DiagramNames_ArgStr* initialize_from_string () {
                                                       /* line 47 */
    Str arg =  NULL                                    /* line 48 */
    Component_Registry* palette = initialize_component_palette_from_string ()/* line 49 */
    return [ palette,[ NULL, arg]]                     /* line 50 *//* line 51 *//* line 52 */}

void start (Str arg,Str part_name,Component_Registry* palette,Tuple_Palette_DiagramNames_ArgStr* env) {
                                                       /* line 53 */
    Part* part = start_bare ( part_name, palette, env) /* line 54 */
    inject ( part, "", arg)                            /* line 55 */
    finalize ( part)                                   /* line 56 *//* line 57 *//* line 58 */}

Part* start_bare (Str part_name,Component_Registry* palette,Tuple_Palette_DiagramNames_ArgStr* env) {
                                                       /* line 59 */
    List_of_DiagramsName* diagram_names =  (*env) [ 0] /* line 60 */
    /*  get entrypoint container */                    /* line 61 */
    Part*  part = get_component_instance ( palette, part_name, NULL)/* line 62 */;
    if  NULL ==  part:                                 /* line 63 */
        load_error ( str( "Couldn;t find container with page name /") +  str( part_name) +  str( "/ in files ") +  str(str ( diagram_names)) +  " (check tab names, or disable compression?)"    )/* line 67 *//* line 68 */
    return ( part)                                     /* line 69 *//* line 70 *//* line 71 */}

void inject (Part* part,Port port,Payload* payload) {
                                                       /* line 72 */
    static load_errors                                 /* line 73 */
    if not  load_errors:                               /* line 74 */
        Datum*  d =  fresh_Datum ()                    /* line 75 */;
        (*d).v =  payload;                             /* line 76 */
        (*d).clone =  lambda : obj_clone ( d)          /* line 77 */;
        (*d).reclaim =  NULL;                          /* line 78 */
        Mevent*  mev = make_mevent ( port, d)          /* line 79 */;
        inject_mevent ( part, mev)                     /* line 80 */;;;
    else:                                              /* line 81 */
        exit (1)                                       /* line 82 *//* line 83 *//* line 84 *//* line 85 */}

void finalize (Part* part) {
                                                       /* line 86 */
    print (deque_to_json (  (*part).outq))             /* line 87 *//* line 88 *//* line 89 */}

Datum* new_datum_bang () {
                                                       /* line 90 */
    Datum*  d =  fresh_Datum ()                        /* line 91 */;
    (*d).v =  "!";                                     /* line 92 */
    (*d).clone =  lambda : obj_clone ( d)              /* line 93 */;
    (*d).reclaim =  NULL;                              /* line 94 */
    return ( d)                                        /* line 95 *//* line 96 */;;;}
