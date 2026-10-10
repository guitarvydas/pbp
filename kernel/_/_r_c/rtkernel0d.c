#include "pbp.h"

#include "pbp.h"                                       /* line 1 */
#include "pbp.h"

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
Eh* fresh_Eh () {
    Eh *self;
    self = (Eh*)malloc(sizeof(Eh*));
    self->name =  "";                                  /* line 17 */
    self->inq =  queue_fresh()                         /* line 18 */;
    self->outq =  queue_fresh()                        /* line 19 */;
    self->owner =  NULL;                               /* line 20 */
    self->children = list_fresh();                     /* line 21 */
    self->visit_ordering =  queue_fresh()              /* line 22 */;
    self->connections = list_fresh();                  /* line 23 */
    self->handler =  NULL;                             /* line 24 */
    self->finject =  NULL;                             /* line 25 */
    self->reset =  NULL;                               /* line 26 */
    self->instance_data =  NULL;                       /* line 27 *//*  arg needed for probe support  *//* line 28 */
    self->arg =  "";                                   /* line 29 */
    self->state =  "idle";                             /* line 30 */
    self->special =  FALSE;                            /* line 31 *//* line 32 */
    return self;
}
                                                       /* line 33 */
void injector (Eh* eh,Mevent* mevent) {
                                                       /* line 34 */
    (*eh).handler ( eh, mevent)                        /* line 35 *//* line 36 *//* line 37 */}
#include "pbp.h"

Array_of_Str = [ "₀", "₁", "₂", "₃", "₄", "₅", "₆", "₇", "₈", "₉", "₁₀", "₁₁", "₁₂", "₁₃", "₁₄", "₁₅", "₁₆", "₁₇", "₁₈", "₁₉", "₂₀", "₂₁", "₂₂", "₂₃", "₂₄", "₂₅", "₂₆", "₂₇", "₂₈", "₂₉"]/* line 7 */;/* line 8 *//* line 9 */
Str* subscripted_digit (Int n) {
                                                       /* line 10 */
    static digits                                      /* line 11 */
    if ( n >=  0 and  n <=  29):                       /* line 12 */
        return ( (*digits) [ (*n)])                    /* line 13 */
    else:                                              /* line 14 */
        return ( str( "₊") + str ( n)                  /* line 15 */)/* line 16 *//* line 17 *//* line 18 */}

Int =  0                                               /* line 19 */;/* line 20 */
Str* gensymbol (Str* s) {
                                                       /* line 21 */
    static counter                                     /* line 22 */
    Str* name_with_id =  str( s) + subscripted_digit ( counter) /* line 23 */
    counter =  counter+ 1                              /* line 24 */
    return ( name_with_id)                             /* line 25 */;/* line 26 */}
#include "pbp.h"
                                                       /* line 1 */
Datum* fresh_Datum () {
    Datum *self;
    self = (Datum*)malloc(sizeof(Datum*));
    self->v =  NULL;                                   /* line 3 */
    self->clone =  NULL;                               /* line 4 */
    self->reclaim =  NULL;                             /* line 5 *//* line 6 */
    return self;
}
                                                       /* line 7 *//* line 8 */
/*  Mevent passed to a leaf component. */              /* line 9 */
/*  */                                                 /* line 10 */
/*  `port` refers to the name of the incoming or outgoing port of this component. *//* line 11 */
/*  `payload` is the data attached to this mevent. */  /* line 12 */
Mevent* fresh_Mevent () {
    Mevent *self;
    self = (Mevent*)malloc(sizeof(Mevent*));
    self->port =  NULL;                                /* line 14 */
    self->payload =  NULL;                             /* line 15 *//* line 16 */
    return self;
}
                                                       /* line 17 */
Port clone_port (Port s) {
                                                       /* line 18 */
    return (clone_string ( s)                          /* line 19 */)/* line 20 *//* line 21 */}

/*  Utility for making a `Mevent`. Used to safely "seed“ mevents *//* line 22 */
/*  entering the very top of a network. */             /* line 23 */
Mevent* make_mevent (Port port,Datum* datum) {
                                                       /* line 24 */
    Port p = clone_string ( port)                      /* line 25 */
    Mevent*  m =  fresh_Mevent ()                      /* line 26 */;
    (*m).port =  p                                     /* line 27 */
    (*m).payload =   (*datum).clone ()                 /* line 28 */
    return ( m)                                        /* line 29 */;;/* line 30 *//* line 31 */}

/*  Clones a mevent. Primarily used internally for “fanning out“ a mevent to multiple destinations. *//* line 32 */
Mevent* mevent_clone (Mevent* mev) {
                                                       /* line 33 */
    Mevent*  m =  fresh_Mevent ()                      /* line 34 */;
    (*m).port = clone_port (  (*mev).port)             /* line 35 */
    (*m).payload =    (*mev).payload.clone ()          /* line 36 */
    return ( m)                                        /* line 37 */;;/* line 38 *//* line 39 */}

/*  Frees a mevent. */                                 /* line 40 */
void destroy_mevent (Mevent* mev) {
                                                       /* line 41 */
    /*  during debug, dont destroy any mevent, since we want to trace mevents, thus, we need to persist ancestor mevents *//* line 42 */
                                                       /* line 43 *//* line 44 *//* line 45 */}

void destroy_datum (Mevent* mev) {
                                                       /* line 46 */
                                                       /* line 47 *//* line 48 *//* line 49 */}

void destroy_port (Mevent* mev) {
                                                       /* line 50 */
                                                       /* line 51 *//* line 52 *//* line 53 */}

/*  */                                                 /* line 54 */
Str* format_mevent (Mevent* m) {
                                                       /* line 55 */
    if  m ==  NULL:                                    /* line 56 */
        return ( "{}")                                 /* line 57 */
    else:                                              /* line 58 */
        return ( str( "{%5C”") +  str(  (*m).port) +  str( "%5C”:%5C”") +  str(   (*m).payload.v) +  "%5C”}"    /* line 59 */)/* line 60 *//* line 61 */}

Str* format_mevent_raw (Mevent* m) {
                                                       /* line 62 */
    if  m ==  NULL:                                    /* line 63 */
        return ( "")                                   /* line 64 */
    else:                                              /* line 65 */
        return (   (*m).payload.v)                     /* line 66 *//* line 67 *//* line 68 */}
#include "pbp.h"
                                                       /* line 1 */
Dir =  0                                               /* line 2 */;
Dir =  1                                               /* line 3 */;
Dir =  2                                               /* line 4 */;
Dir =  3                                               /* line 5 */;/* line 6 *//* line 7 */
/*  Routing connection for a container component. The `direction` field has *//* line 8 */
/*  no affect on the default mevent routing system _ it is there for debugging *//* line 9 */
/*  purposes, or for reading by other tools. */        /* line 10 *//* line 11 */
Connector* fresh_Connector () {
    Connector *self;
    self = (Connector*)malloc(sizeof(Connector*));
    self->direction =  NULL; /*  down, across, up, through *//* line 13 */
    self->sender =  NULL;                              /* line 14 */
    self->receiver =  NULL;                            /* line 15 *//* line 16 */
    return self;
}
                                                       /* line 17 */
/*  `Sender` is used to "pattern match“ which `Receiver` a mevent should go to, *//* line 18 */
/*  based on component ID (pointer) and port name. */  /* line 19 *//* line 20 */
Sender* fresh_Sender () {
    Sender *self;
    self = (Sender*)malloc(sizeof(Sender*));
    self->name =  NULL;                                /* line 22 */
    self->component =  NULL;                           /* line 23 */
    self->port =  NULL;                                /* line 24 *//* line 25 */
    return self;
}
                                                       /* line 26 *//* line 27 *//* line 28 */
/*  `Receiver` is a handle to a destination queue, and a `port` name to assign *//* line 29 */
/*  to incoming mevents to this queue. */              /* line 30 *//* line 31 */
Receiver* fresh_Receiver () {
    Receiver *self;
    self = (Receiver*)malloc(sizeof(Receiver*));
    self->name =  NULL;                                /* line 33 */
    self->queue =  NULL;                               /* line 34 */
    self->port =  NULL;                                /* line 35 */
    self->component =  NULL;                           /* line 36 *//* line 37 */
    return self;
}
                                                       /* line 38 */
Sender* mkSender (Str* name,Eh* component,Port port) {
                                                       /* line 39 */
    Sender*  s =  fresh_Sender ()                      /* line 40 */;
    (*s).name =  name                                  /* line 41 */
    (*s).component =  component                        /* line 42 */
    (*s).port =  port                                  /* line 43 */
    return ( s)                                        /* line 44 */;;;/* line 45 *//* line 46 */}

Receiver* mkReceiver (Str* name,Eh* component,Port port,Queue* q) {
                                                       /* line 47 */
    Receiver*  r =  fresh_Receiver ()                  /* line 48 */;
    (*r).name =  name                                  /* line 49 */
    (*r).component =  component                        /* line 50 */
    (*r).port =  port                                  /* line 51 */
    /*  We need a way to determine which queue to target. "Down" and "Across" go to inq, "Up" and "Through" go to outq. *//* line 52 */
    (*r).queue =  q                                    /* line 53 */
    return ( r)                                        /* line 54 */;;;;/* line 55 */}
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
    (*templ).name =  name                              /* line 13 */
    (*templ).template_data =  template_data            /* line 14 */
    (*templ).instantiator =  instantiator              /* line 15 */
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
        (*reg).templates [name] =  template            /* line 49 */
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
        PART* instance = jit_instantiate ( reg, owner, instance_name, full_name)/* line 62 */
        return ( instance)                             /* line 63 */
    else:                                              /* line 64 */
        if  template_name in   (*reg).templates:       /* line 65 */
            Template* template =   (*reg).templates [template_name]/* line 66 */
            if ( template ==  NULL):                   /* line 67 */
                load_error ( str( "Registry Error (A): Can't find component /") +  str( template_name) +  "/"  )/* line 68 */
                return ( NULL)                         /* line 69 */
            else:                                      /* line 70 */
                Str* instance_name = generate_instance_name ( owner, template_name)/* line 71 */
                PART* instance =   (*template).instantiator ( reg, owner, instance_name,  (*template).template_data, "")/* line 72 */
                return ( instance)                     /* line 73 *//* line 74 */
        else:                                          /* line 75 */
            load_error ( str( "Registry Error (B): Can't find component /") +  str( template_name) +  "/"  )/* line 76 */
            return ( NULL)                             /* line 77 *//* line 78 *//* line 79 *//* line 80 *//* line 81 */}

Str* generate_instance_name (Container* owner,Str* template_name) {
                                                       /* line 82 */
    Str* owner_name =  ""                              /* line 83 */
    Str* instance_name =  template_name                /* line 84 */
    if  NULL!= owner:                                  /* line 85 */
        owner_name =   (*owner).name                   /* line 86 */
        instance_name =  str( owner_name) +  str( "▹") +  template_name  /* line 87 */;;
    else:                                              /* line 88 */
        instance_name =  template_name;                /* line 89 *//* line 90 */
    return ( instance_name)                            /* line 91 *//* line 92 *//* line 93 */}

Str* mangle_name (Str* s) {
                                                       /* line 94 */
    /*  trim name to remove code from Container component names _ deferred until later (or never) *//* line 95 */
    return ( s)                                        /* line 96 *//* line 97 */}
#include "pbp.h"

Wire* create_down_connector (Container* container,Wire_Proto* proto_conn,List_of_Wire* connectors,Table_by_ID_of_Part* children_by_id) {
                                                       /* line 1 */
    /*  JSON: {;dir': 0, 'source': {'name': '', 'id': 0}, 'source_port': '', 'target': {'name': 'Echo', 'id': 12}, 'target_port': ''}, *//* line 2 */
    Wire*  connector =  fresh_Connector ()             /* line 3 */;
    (*connector).direction =  "down"                   /* line 4 */
    (*connector).sender = mkSender (  (*container).name, container, (*proto_conn) [ "source_port"])/* line 5 */
    Part* target_proto =  (*proto_conn) [ "target"]    /* line 6 */
    ID id_proto =  (*target_proto) [ "id"]             /* line 7 */
    Part* target_component =  (*children_by_id) [id_proto]/* line 8 */
    if ( target_component ==  NULL):                   /* line 9 */
        load_error ( str( "internal error: .Down connection target internal error ") + ( (*proto_conn) [ "target"]) [ "name"] )/* line 10 */
    else:                                              /* line 11 */
        (*connector).receiver = mkReceiver (  (*target_component).name, target_component, (*proto_conn) [ "target_port"],  (*target_component).inq)/* line 12 */;/* line 13 */
    return ( connector)                                /* line 14 */;;/* line 15 *//* line 16 */}

Wire* create_across_connector (Container* container,Wire_Proto* proto_conn,List_of_Wire* connectors,Table_by_ID_of_Part* children_by_id) {
                                                       /* line 17 */
    Wire*  connector =  fresh_Connector ()             /* line 18 */;
    (*connector).direction =  "across"                 /* line 19 */
    Part* source_component =  (*children_by_id) [(( (*proto_conn) [ "source"]) [ "id"])]/* line 20 */
    Part* target_component =  (*children_by_id) [(( (*proto_conn) [ "target"]) [ "id"])]/* line 21 */
    if  source_component ==  NULL:                     /* line 22 */
        load_error ( str( "internal error: .Across connection source not ok ") + ( (*proto_conn) [ "source"]) [ "name"] )/* line 23 */
    else:                                              /* line 24 */
        (*connector).sender = mkSender (  (*source_component).name, source_component, (*proto_conn) [ "source_port"])/* line 25 */
        if  target_component ==  NULL:                 /* line 26 */
            load_error ( str( "internal error: .Across connection target not ok ") + ( (*proto_conn) [ "target"]) [ "name"] )/* line 27 */
        else:                                          /* line 28 */
            (*connector).receiver = mkReceiver (  (*target_component).name, target_component, (*proto_conn) [ "target_port"],  (*target_component).inq)/* line 29 */;/* line 30 */;/* line 31 */
    return ( connector)                                /* line 32 */;/* line 33 *//* line 34 */}

Wire* create_up_connector (Container* container,Wire_Proto* proto_conn,List_of_Wire* connectors,Table_by_ID_of_Part* children_by_id) {
                                                       /* line 35 */
    Wire*  connector =  fresh_Connector ()             /* line 36 */;
    (*connector).direction =  "up"                     /* line 37 */
    Part* source_component =  (*children_by_id) [(( (*proto_conn) [ "source"]) [ "id"])]/* line 38 */
    if  source_component ==  NULL:                     /* line 39 */
        load_error ( str( "internal error: .Up connection source not ok ") + ( (*proto_conn) [ "source"]) [ "name"] )/* line 40 */
    else:                                              /* line 41 */
        (*connector).sender = mkSender (  (*source_component).name, source_component, (*proto_conn) [ "source_port"])/* line 42 */
        (*connector).receiver = mkReceiver (  (*container).name, container, (*proto_conn) [ "target_port"],  (*container).outq)/* line 43 */;;/* line 44 */
    return ( connector)                                /* line 45 */;/* line 46 *//* line 47 */}

Wire* create_through_connector (Container* container,Wire_Proto* proto_conn,List_of_Wire* connectors,Table_by_ID_of_Part* children_by_id) {
                                                       /* line 48 */
    Wire*  connector =  fresh_Connector ()             /* line 49 */;
    (*connector).direction =  "through"                /* line 50 */
    (*connector).sender = mkSender (  (*container).name, container, (*proto_conn) [ "source_port"])/* line 51 */
    (*connector).receiver = mkReceiver (  (*container).name, container, (*proto_conn) [ "target_port"],  (*container).outq)/* line 52 */
    return ( connector)                                /* line 53 */;;;/* line 54 *//* line 55 */}
                                                       /* line 56 */
Container* container_instantiator (Component_Registry* reg,Container* owner,Str* container_name,Template* desc,Str* arg) {
                                                       /* line 57 */
    static enumDown                                    /* line 58 */
    static enumUp                                      /* line 59 */
    static enumAcross                                  /* line 60 */
    static enumThrough                                 /* line 61 */
    Container* container = make_container ( container_name, owner)/* line 62 */
    List_of_PartI* children = list_fresh()             /* line 63 */
    List_of_PartI* children_by_id = dict_fresh()
    /*  not strictly necessary, but, we can remove 1 runtime lookup by "compiling it out“ here *//* line 64 */
    /*  collect children */                            /* line 65 */
    for child_desc in  (*desc) [ "children"]:          /* line 66 */
        PartI* child_instance = get_component_instance ( reg, (*child_desc) [ "name"], container)/* line 67 */
        children.append ( child_instance)              /* line 68 */
        Str* id =  (*child_desc) [ "id"]               /* line 69 */
        (*children_by_id) [id] =  child_instance       /* line 70 *//* line 71 */;/* line 72 */
    (*container).children =  children                  /* line 73 *//* line 74 */
    List_of_WireI* connectors = list_fresh()           /* line 75 */
    for proto_conn in  (*desc) [ "connections"]:       /* line 76 */
        WireI*  connector =  fresh_Connector ()        /* line 77 */;
        if  (*proto_conn) [ "dir"] ==  enumDown:       /* line 78 */
            connectors.append (create_down_connector ( container, proto_conn, connectors, children_by_id)) /* line 79 */
        elif  (*proto_conn) [ "dir"] ==  enumAcross:   /* line 80 */
            connectors.append (create_across_connector ( container, proto_conn, connectors, children_by_id)) /* line 81 */
        elif  (*proto_conn) [ "dir"] ==  enumUp:       /* line 82 */
            connectors.append (create_up_connector ( container, proto_conn, connectors, children_by_id)) /* line 83 */
        elif  (*proto_conn) [ "dir"] ==  enumThrough:  /* line 84 */
            connectors.append (create_through_connector ( container, proto_conn, connectors, children_by_id)) /* line 85 *//* line 86 *//* line 87 */
    (*container).connections =  connectors             /* line 88 */
    return ( container)                                /* line 89 */;;/* line 90 *//* line 91 */}

/*  The default handler for container components. */   /* line 92 */
void container_handler (Container* container,Mevent* mevent) {
                                                       /* line 93 */
    route ( container, container, mevent)
    /*  references to 'self' are replaced by the container during instantiation *//* line 94 */
    while any_child_ready ( container):                /* line 95 */
        step_children ( container, mevent)             /* line 96 *//* line 97 *//* line 98 */}

/*  Stop all children. Reset to a known state. Hit the big red button.  *//* line 99 */
void container_reset (Container* container) {
                                                       /* line 100 */
    for child in   (*container).children:              /* line 101 */
        (*child).reset ( child)                        /* line 102 *//* line 103 */

    queue_clear(  (*container).visit_ordering)         /* line 104 */

    queue_clear(  (*container).inq)                    /* line 105 */

    queue_clear(  (*container).outq)                   /* line 106 */
    (*container).state =  "idle";                      /* line 107 *//* line 108 *//* line 109 */}

/*  Frees the given container and associated data. */  /* line 110 */
void destroy_container (Part* eh) {
                                                       /* line 111 */
                                                       /* line 112 *//* line 113 */}

/*  Checks if two senders match, by pointer equality and port name matching. *//* line 114 */
BOOL sender_eq (Part* s1,Part* s2) {
                                                       /* line 115 */
    BOOL same_components = (  (*s1).component ==   (*s2).component)/* line 116 */
    BOOL same_ports = (  (*s1).port ==   (*s2).port)   /* line 117 */
    return ( same_components and  same_ports)          /* line 118 *//* line 119 *//* line 120 */}

/*  Delivers the given mevent to the receiver of this connector. *//* line 121 *//* line 122 */
void deposit (Container* parent,Wire* conn,Mevent* mevent) {
                                                       /* line 123 */
    Mevent* new_mevent = make_mevent (   (*conn).receiver.port,  (*mevent).payload)/* line 124 */
    push_mevent ( parent,   (*conn).receiver.component,   (*conn).receiver.queue, new_mevent)/* line 125 *//* line 126 *//* line 127 */}

void force_tick (Container* parent,Part* eh) {
                                                       /* line 128 */
    Mevent* tick_mev = make_mevent ( ".",new_datum_bang ())/* line 129 */
    push_mevent ( parent, eh,  (*eh).inq, tick_mev)    /* line 130 */
    return ( tick_mev)                                 /* line 131 *//* line 132 *//* line 133 */}

void push_mevent (Container* parent,Part* receiver,Queue* inq,Mevent* m) {
                                                       /* line 134 */
    inq.append ( m)                                    /* line 135 */
    if (  (*receiver).special):                        /* line 136 */
        (*parent).visit_ordering.appendleft ( receiver)/* line 137 */
    else:                                              /* line 138 */
        (*parent).visit_ordering.append ( receiver)    /* line 139 *//* line 140 *//* line 141 *//* line 142 *//* line 143 */}

Bool is_self (Part* child,Container* container) {
                                                       /* line 144 */
    /*  in an earlier version “self“ was denoted as ϕ *//* line 145 */
    return ( child ==  container)                      /* line 146 *//* line 147 *//* line 148 */}

void step_child_once (Part* child,Mevent* mev) {
                                                       /* line 149 */
    if ( ("PBPSTEPPING" in os.environ) ):              /* line 150 */
        print ( str( "-- stepping ❮") +  str(  (*child).name) +  "❯"  , file=sys.stderr)/* line 151 */
                                                       /* line 152 *//* line 153 */
    (*child).handler ( child, mev)                     /* line 154 *//* line 155 *//* line 156 */}

void step_children (Container* container,Mevent* causingMevent) {
                                                       /* line 157 */
    (*container).state =  "idle"                       /* line 158 *//* line 159 */
    /*  phase 1 - loop through children and process inputs or children that not "idle"  *//* line 160 */
    for child in  list (  (*container).visit_ordering):/* line 161 */
        /*  child = container represents self, skip it *//* line 162 */
        if (not (is_self ( child, container))):        /* line 163 */
            if (not ((0==len(  (*child).inq)))):       /* line 164 */
                Mevent* mev =   (*child).inq.popleft ()/* line 165 */
                step_child_once ( child, mev)          /* line 166 *//* line 167 */
                destroy_mevent ( mev)                  /* line 168 */
            else:                                      /* line 169 */
                if   (*child).state ==  "idle":        /* line 170 */
                                                       /* line 171 */
                else:                                  /* line 172 */
                    Mevent* mev = force_tick ( container, child)/* line 173 */
                    step_child_once ( child, mev)      /* line 174 */
                    destroy_mevent ( mev)              /* line 175 *//* line 176 *//* line 177 *//* line 178 *//* line 179 */

    queue_clear(  (*container).visit_ordering)         /* line 180 *//* line 181 */
    /*  phase 2 - loop through children and route their outputs to appropriate receiver queues based on .connections  *//* line 182 */
    for child in   (*container).children:              /* line 183 */
        if   (*child).state ==  "active":              /* line 184 */
            /*  if child remains active, then the container must remain active and must propagate “ticks“ to child *//* line 185 */
            (*container).state =  "active";            /* line 186 *//* line 187 *//* line 188 */
        while (not ((0==len(  (*child).outq)))):       /* line 189 */
            Mevent* mev =   (*child).outq.popleft ()   /* line 190 */
            route ( container, child, mev)             /* line 191 */
            destroy_mevent ( mev)                      /* line 192 *//* line 193 *//* line 194 */;/* line 195 *//* line 196 */}

void attempt_tick (Container* parent,Part* eh) {
                                                       /* line 197 */
    if   (*eh).state!= "idle":                         /* line 198 */
        force_tick ( parent, eh)                       /* line 199 *//* line 200 *//* line 201 *//* line 202 */}

Bool is_tick (Mevent* mev) {
                                                       /* line 203 */
    return ( "." ==   (*mev).port)
    /*  assume that any mevent that is sent to port "." is a tick  *//* line 204 *//* line 205 *//* line 206 */}

/*  Routes a single mevent to all matching destinations, according to *//* line 207 */
/*  the container's connection network. */             /* line 208 *//* line 209 */
void route (Container* container,Part* from_component,Mevent* mevent) {
                                                       /* line 210 */
    Bool  was_sent =  FALSE;
    /*  for checking that output went somewhere (at least during bootstrap) *//* line 211 */
    Str*  fromname =  "";                              /* line 212 */
    static ticktime                                    /* line 213 */
    ticktime =  ticktime+ 1                            /* line 214 */
    if is_tick ( mevent):                              /* line 215 */
        for child in   (*container).children:          /* line 216 */
            attempt_tick ( container, child)           /* line 217 */
        was_sent =  TRUE;                              /* line 218 */
    else:                                              /* line 219 */
        if (not (is_self ( from_component, container))):/* line 220 */
            fromname =   (*from_component).name;       /* line 221 *//* line 222 */
        Sender* from_sender = mkSender ( fromname, from_component,  (*mevent).port)/* line 223 *//* line 224 */
        for connector in   (*container).connections:   /* line 225 */
            if sender_eq ( from_sender,  (*connector).sender):/* line 226 */
                deposit ( container, connector, mevent)/* line 227 */
                was_sent =  TRUE;                      /* line 228 *//* line 229 *//* line 230 *//* line 231 */
    if not ( was_sent):                                /* line 232 */
        live_update ( "internal error",  str(  (*container).name) +  str( ": mevent on port '") +  str(  (*mevent).port) +  str( "' from ") +  str( fromname) +  " dropped on floor..."     )/* line 233 *//* line 234 */;/* line 235 *//* line 236 */}

Bool any_child_ready (Container* container) {
                                                       /* line 237 */
    for child in   (*container).children:              /* line 238 */
        if child_is_ready ( child):                    /* line 239 */
            return ( TRUE)                             /* line 240 *//* line 241 *//* line 242 */
    return ( FALSE)                                    /* line 243 *//* line 244 *//* line 245 */}

Bool child_is_ready (Part* eh) {
                                                       /* line 246 */
    return ((not ((0==len(  (*eh).outq)))) or (not ((0==len(  (*eh).inq)))) or (  (*eh).state!= "idle") or (any_child_ready ( eh)))/* line 247 *//* line 248 *//* line 249 */}
                                                       /* line 250 */
/*  Creates a component that acts as a container. It is the same as a `Eh` instance *//* line 251 */
/*  whose handler function is `container_handler`. */  /* line 252 */
Container* make_container (Str* name,Container* owner) {
                                                       /* line 253 */
    Container*  eh =  fresh_Eh ()                      /* line 254 */;
    (*eh).name =  name                                 /* line 255 */
    (*eh).owner =  owner                               /* line 256 */
    (*eh).handler =  container_handler                 /* line 257 */
    (*eh).finject =  injector                          /* line 258 */
    (*eh).reset =  container_reset                     /* line 259 */
    (*eh).state =  "idle"                              /* line 260 */
    (*eh).kind =  "container"                          /* line 261 */
    return ( eh)                                       /* line 262 */;;;;;;;/* line 263 *//* line 264 */}

/*  Sends a mevent on the given `port` with `data`, placing it on the output *//* line 265 */
/*  of the given component. */                         /* line 266 *//* line 267 */
void send (Part* eh,Port port,Part* obj,Mevent* causingMevent) {
                                                       /* line 268 */
    Payload*  d =  fresh_Datum ()                      /* line 269 */;
    (*d).v =  obj                                      /* line 270 */
    (*d).clone =  lambda : obj_clone ( d)              /* line 271 */
    (*d).reclaim =  NULL                               /* line 272 */
    Mevent* mev = make_mevent ( port, d)               /* line 273 */
    put_output ( eh, mev)                              /* line 274 */;;;/* line 275 *//* line 276 */}

void forward (Part* eh,Port port,Mevent* mev) {
                                                       /* line 277 */
    Mevent* fwdmev = make_mevent ( port,  (*mev).payload)/* line 278 */
    put_output ( eh, fwdmev)                           /* line 279 *//* line 280 *//* line 281 */}

Mevent* inject_mevent (Part* eh,Mevent* mev) {
                                                       /* line 282 */
    (*eh).finject ( eh, mev)                           /* line 283 *//* line 284 *//* line 285 */}

void set_active (Part* eh) {
                                                       /* line 286 */
    (*eh).state =  "active";                           /* line 287 *//* line 288 *//* line 289 */}

void set_idle (Part* eh) {
                                                       /* line 290 */
    (*eh).state =  "idle";                             /* line 291 *//* line 292 *//* line 293 */}

void put_output (Part* eh,Mevent* mev) {
                                                       /* line 294 */
    (*eh).outq.append ( mev)                           /* line 295 *//* line 296 *//* line 297 */}

Payload* obj_clone (Payload* obj) {
                                                       /* line 298 */
    return ( obj)                                      /* line 299 *//* line 300 */}
#include "pbp.h"

/*  Creates a new leaf component out of a handler function, and a data parameter *//* line 1 */
/*  that will be passed back to your handler when called. *//* line 2 *//* line 3 */
Leaf* make_leaf (Str* name,Container* owner,any* instance_data,Str* arg,Fhandler handler,Freset reset_handler) {
                                                       /* line 4 */
    Leaf*  eh =  fresh_Eh ()                           /* line 5 */;
    Part*  nm =  "";                                   /* line 6 */
    if  NULL!= owner:                                  /* line 7 */
        nm =   (*owner).name;                          /* line 8 *//* line 9 */
    (*eh).name =  str( nm) +  str( "▹") +  name        /* line 10 */
    (*eh).owner =  owner                               /* line 11 */
    (*eh).handler =  handler                           /* line 12 */
    (*eh).reset_handler =  reset_handler               /* line 13 */
    (*eh).finject =  injector                          /* line 14 */
    (*eh).reset =  leaf_reset                          /* line 15 */
    (*eh).instance_data =  instance_data               /* line 16 */
    (*eh).arg =  arg                                   /* line 17 */
    (*eh).state =  "idle"                              /* line 18 */
    return ( eh)                                       /* line 19 */;;;;;;;;;/* line 20 *//* line 21 */}

/*  Reset Leaf part to a known, idle state. Hit the big red button.  *//* line 22 */
void leaf_reset (Leaf* part) {
                                                       /* line 23 */

    queue_clear(  (*part).inq)                         /* line 24 */

    queue_clear(  (*part).outq)                        /* line 25 */
    if (  (*part).reset_handler!= NULL):               /* line 26 */
        (*part).reset_handler ( part)                  /* line 27 *//* line 28 */
    (*part).state =  "idle";                           /* line 29 *//* line 30 */}
#include "pbp.h"

/*  (This used to be called `external` due to historical reasons). This has evolved into 2 kinds of Leaf parts: AOT and JIT (statically generated before runtime, vs. dynamically generated at runtime). If a part name begins with ;:', it is treated specially as a JIT part, else the part is assumed to have been pre-loaded into the register in the regular way.  *//* line 1 *//* line 2 */
Part* jit_instantiate (Component_Registry* reg,Container* owner,Str* name,Str* arg) {
                                                       /* line 3 */
    Str* name_with_id = gensymbol ( name)              /* line 4 */
    Part*  inst = make_leaf ( name_with_id, owner, NULL, arg, handle_jit, NULL)/* line 5 */;
    Char  firstc =  (*name) [ 1];                      /* line 6 */
    if ( firstc!= "$"):                                /* line 7 */
        /*  probes get to go to the front of the line  *//* line 8 */
        (*inst).special =  TRUE;                       /* line 9 *//* line 10 */
    return ( inst)                                     /* line 11 *//* line 12 *//* line 13 */}

void handle_jit (Part* eh,Mevent* mev) {
                                                       /* line 14 */
    Str* s =   (*eh).arg                               /* line 15 */
    Char  firstc =  (*s) [ 1];                         /* line 16 */
    if  firstc ==  "$":                                /* line 17 */
        shell_out_handler ( eh,    s[1:] [1:] [1:] , mev)/* line 18 */
    elif  firstc ==  "?":                              /* line 19 */
        probe_handler ( eh,  s[1:] , mev)              /* line 20 */
    else:                                              /* line 21 */
        /*  just a string, send it out  */             /* line 22 */
        send ( eh, "",  s[1:] , mev)                   /* line 23 *//* line 24 *//* line 25 *//* line 26 */}

void probe_handler (Part* eh,Port tag,Mevent* mev) {
                                                       /* line 27 */
    static ticktime                                    /* line 28 */
    Str* s =    (*mev).payload.v                       /* line 29 */
    live_update ( "Info",  str( "  @") +  str(str ( ticktime)) +  str( "  ") +  str( "probe ") +  str(  (*eh).name) +  str( ": ") + str ( s)      )/* line 37 *//* line 38 *//* line 39 */}

void shell_out_handler (Part* eh,Port cmd,Mevent* mev) {
                                                       /* line 40 */
    Str* s =    (*mev).payload.v                       /* line 41 */
    Int  ret =  NULL;                                  /* line 42 */
    Int  rc =  NULL;                                   /* line 43 */
    Str*  stdout =  NULL;                              /* line 44 */
    Str*  stderr =  NULL;                              /* line 45 */
    Str*  command =  cmd;                              /* line 46 */
    Pathname*  pbpRoot = os.getenv('PBP', '<none>')    /* line 47 */;
    if  pbpRoot!= "":                                  /* line 48 */
        command = re.sub ( "_/",  str( pbpRoot) +  "/" ,  command)/* line 51 */;/* line 52 */
    if ( ("PBPSHELLUT" in os.environ) ):               /* line 53 */
        print ( str( "- --- shell-out: ") +  command , file=sys.stderr)/* line 54 */
                                                       /* line 55 *//* line 56 */

    try:
        with tempfile.NamedTemporaryFile(mode='w', suffix='.txt', delete=False) as tmp:
            tmp.write( s)
            tmp_path = tmp.name
        try:
            with open(tmp_path, 'r') as stdin_file:
                ret = subprocess.run(
                shlex.split( command),
                stdin=stdin_file,
                text=True,
                capture_output=True
                )
        finally:
            os.unlink(tmp_path)
        rc = ret.returncode
        stdout = ret.stdout.strip()
        stderr = ret.stderr.strip()
    except Exception as e:
        rc = 1
        stdout = ''
        stderr = str(e)
                                                       /* line 57 */
    if  rc ==  0:                                      /* line 58 */
        send ( eh, "", str( stdout) +  stderr , mev)   /* line 59 */
    else:                                              /* line 60 */
        send ( eh, "✗", str( stdout) +  stderr , mev)  /* line 61 *//* line 62 *//* line 63 *//* line 64 */}
#include "pbp.h"

Str* clone_string (Str* s) {
                                                       /* line 1 */
    return ( s)                                        /* line 2 *//* line 3 *//* line 4 */}
                                                       /* line 5 */
Leaf* trash_instantiate (Component_Registry* reg,Container* owner,Str* name,Template* template_data,Str* arg) {
                                                       /* line 6 */
    Str* name_with_id = gensymbol ( "trash")           /* line 7 */
    return (make_leaf ( name_with_id, owner, NULL, "", trash_handler, NULL)/* line 8 */)/* line 9 *//* line 10 */}

void trash_handler (Part* eh,Mevent* mev) {
                                                       /* line 11 */
    /*  to appease dumped_on_floor checker */          /* line 12 */
                                                       /* line 13 *//* line 14 */}

TwoMevents* fresh_TwoMevents () {
    TwoMevents *self;
    self = (TwoMevents*)malloc(sizeof(TwoMevents*));
    self->firstmev =  NULL;                            /* line 16 */
    self->secondmev =  NULL;                           /* line 17 *//* line 18 */
    return self;
}
                                                       /* line 19 */
/*  Deracer_States :: enum { idle, waitingForFirstmev, waitingForSecondmev } *//* line 20 */
Deracer_Instance_Data* fresh_Deracer_Instance_Data () {
    Deracer_Instance_Data *self;
    self = (Deracer_Instance_Data*)malloc(sizeof(Deracer_Instance_Data*));
    self->state =  NULL;                               /* line 22 */
    self->buffer =  NULL;                              /* line 23 *//* line 24 */
    return self;
}
                                                       /* line 25 */
void reclaim_Buffers_from_heap (Leaf* inst) {
                                                       /* line 26 */
                                                       /* line 27 *//* line 28 *//* line 29 */}

void deracer_reset_handler (Leaf* eh) {
                                                       /* line 30 */
    Deracer_Instance_Data*  inst =   (*eh).instance_data;/* line 31 */
    (*inst).state =  "idle"                            /* line 32 */
    (*inst).buffer =  fresh_TwoMevents ()              /* line 33 */;;/* line 34 *//* line 35 */}

Leaf* deracer_instantiate (Component_Registry* reg,Container* owner,Str* name,Ignored template_data,Ignored arg) {
                                                       /* line 36 */
    Str* name_with_id = gensymbol ( "deracer")         /* line 37 */
    Deracer_Instance_Data*  inst =  fresh_Deracer_Instance_Data ()/* line 38 */;
    (*inst).state =  "idle"                            /* line 39 */
    (*inst).buffer =  fresh_TwoMevents ()              /* line 40 */
    Leaf* eh = make_leaf ( name_with_id, owner, inst, "", deracer_handler, deracer_reset_handler)/* line 41 */
    return ( eh)                                       /* line 42 */;;/* line 43 *//* line 44 */}

void send_firstmev_then_secondmev (Leaf* eh,Deracer_Instance_Data* inst) {
                                                       /* line 45 */
    forward ( eh, "1",   (*inst).buffer.firstmev)      /* line 46 */
    forward ( eh, "2",   (*inst).buffer.secondmev)     /* line 47 */
    reclaim_Buffers_from_heap ( inst)                  /* line 48 *//* line 49 *//* line 50 */}

void deracer_handler (Leaf* eh,Mevent* mev) {
                                                       /* line 51 */
    Deracer_Instance_Data*  inst =   (*eh).instance_data;/* line 52 */
    if   (*inst).state ==  "idle":                     /* line 53 */
        if  "1" ==   (*mev).port:                      /* line 54 */
            (*inst).buffer.firstmev =  mev             /* line 55 */
            (*inst).state =  "waitingForSecondmev";    /* line 56 */;
        elif  "2" ==   (*mev).port:                    /* line 57 */
            (*inst).buffer.secondmev =  mev            /* line 58 */
            (*inst).state =  "waitingForFirstmev";     /* line 59 */;
        else:                                          /* line 60 */
            runtime_error ( str( "bad mev.port (case A) for deracer ") +   (*mev).port )/* line 61 *//* line 62 */
    elif   (*inst).state ==  "waitingForFirstmev":     /* line 63 */
        if  "1" ==   (*mev).port:                      /* line 64 */
            (*inst).buffer.firstmev =  mev             /* line 65 */
            send_firstmev_then_secondmev ( eh, inst)   /* line 66 */
            (*inst).state =  "idle";                   /* line 67 */;
        else:                                          /* line 68 */
            runtime_error ( str( "deracer: waiting for 1 but got [") +  str(  (*mev).port) +  "] (case B)"  )/* line 69 *//* line 70 */
    elif   (*inst).state ==  "waitingForSecondmev":    /* line 71 */
        if  "2" ==   (*mev).port:                      /* line 72 */
            (*inst).buffer.secondmev =  mev            /* line 73 */
            send_firstmev_then_secondmev ( eh, inst)   /* line 74 */
            (*inst).state =  "idle";                   /* line 75 */;
        else:                                          /* line 76 */
            runtime_error ( str( "deracer: waiting for 2 but got [") +  str(  (*mev).port) +  "] (case C)"  )/* line 77 *//* line 78 */
    else:                                              /* line 79 */
        runtime_error ( "bad state for deracer {eh.state}")/* line 80 *//* line 81 *//* line 82 *//* line 83 */}

Leaf* low_level_read_text_file_instantiate (Component_Registry* reg,Container* owner,Str* name,Template* template_data,Str* arg) {
                                                       /* line 84 */
    Str* name_with_id = gensymbol ( "Low Level Read Text File")/* line 85 */
    return (make_leaf ( name_with_id, owner, NULL, "", low_level_read_text_file_handler, NULL)/* line 86 */)/* line 87 *//* line 88 */}

void low_level_read_text_file_handler (Leaf* eh,Mevent* mev) {
                                                       /* line 89 */
    Payload* fname =    (*mev).payload.v               /* line 90 */

    try:
        f = open (fname)
    except Exception as e:
        f = None
    if f != None:
        data = f.read ()
        if data!= None:
            send (eh, "", data, mev)
        else:
            send (eh, "✗", f"read error on file '{fname}'", mev)
        f.close ()
    else:
        send (eh, "✗", f"open error on file '{fname}'", mev)
                                                       /* line 91 *//* line 92 *//* line 93 */}

Leaf* ensure_string_datum_instantiate (Component_Registry* reg,Container* owner,Str* name,Ignored template_data,Ignored arg) {
                                                       /* line 94 */
    Str* name_with_id = gensymbol ( "Ensure String Datum")/* line 95 */
    return (make_leaf ( name_with_id, owner, NULL, "", ensure_string_datum_handler, NULL)/* line 96 */)/* line 97 *//* line 98 */}

void ensure_string_datum_handler (Leaf* eh,Mevent* mev) {
                                                       /* line 99 */
    if  "string" ==    (*mev).payload.kind ():         /* line 100 */
        forward ( eh, "", mev)                         /* line 101 */
    else:                                              /* line 102 */
        Str* emev =  str( "*** ensure: type error (expected a string payload) but got ") +   (*mev).payload /* line 103 */
        send ( eh, "✗", emev, mev)                     /* line 104 *//* line 105 *//* line 106 *//* line 107 */}

Syncfilewrite_Data* fresh_Syncfilewrite_Data () {
    Syncfilewrite_Data *self;
    self = (Syncfilewrite_Data*)malloc(sizeof(Syncfilewrite_Data*));
    self->filename =  "";                              /* line 109 *//* line 110 */
    return self;
}
                                                       /* line 111 */
void syncfilewrite_reset_handler (Leaf* eh) {
                                                       /* line 112 */
    (*eh).instance_data =  fresh_Syncfilewrite_Data () /* line 113 */;/* line 114 *//* line 115 */}

/*  temp copy for bootstrap, sends "done“ (error during bootstrap if not wired) *//* line 116 */
Leaf* syncfilewrite_instantiate (Component_Registry* reg,Container* owner,Str* name,Ignored template_data,Ignored arg) {
                                                       /* line 117 */
    Str* name_with_id = gensymbol ( "syncfilewrite")   /* line 118 */
    SyncFilewrite_Data* inst =  fresh_Syncfilewrite_Data ()/* line 119 */
    return (make_leaf ( name_with_id, owner, inst, "", syncfilewrite_handler, syncfilewrite_reset_handler)/* line 120 */)/* line 121 *//* line 122 */}

void syncfilewrite_handler (Leaf* eh,Mevent* mev) {
                                                       /* line 123 */
    Synfilewrite_Data*  inst =   (*eh).instance_data;  /* line 124 */
    if  "filename" ==   (*mev).port:                   /* line 125 */
        (*inst).filename =    (*mev).payload.v;        /* line 126 */
    elif  "input" ==   (*mev).port:                    /* line 127 */
        Payload* contents =    (*mev).payload.v        /* line 128 */
        FileDescriptor*  f = open (  (*inst).filename, "w")/* line 129 */;
        if  f!= NULL:                                  /* line 130 */
            (*f).write (   (*mev).payload.v)           /* line 131 */
            (*f).close ()                              /* line 132 */
            send ( eh, "done",new_datum_bang (), mev)  /* line 133 */
        else:                                          /* line 134 */
            send ( eh, "✗", str( "open error on file ") +   (*inst).filename , mev)/* line 135 *//* line 136 *//* line 137 *//* line 138 *//* line 139 */}

StringConcat_Instance_Data* fresh_StringConcat_Instance_Data () {
    StringConcat_Instance_Data *self;
    self = (StringConcat_Instance_Data*)malloc(sizeof(StringConcat_Instance_Data*));
    self->buffer1 =  NULL;                             /* line 141 */
    self->buffer2 =  NULL;                             /* line 142 *//* line 143 */
    return self;
}
                                                       /* line 144 */
void stringconcat_reset_handler (Leaf* eh) {
                                                       /* line 145 */
    StringConcat_Instance_Data*  inst =   (*eh).instance_data;/* line 146 */
    (*inst).buffer1 =  NULL                            /* line 147 */
    (*inst).buffer2 =  NULL;                           /* line 148 */;/* line 149 *//* line 150 */}

Leaf* stringconcat_instantiate (Component_Registry* reg,Container* owner,Str* name,Ignored template_data,Ignored arg) {
                                                       /* line 151 */
    Str* name_with_id = gensymbol ( "stringconcat")    /* line 152 */
    StringConcat_Instance_Data* instp =  fresh_StringConcat_Instance_Data ()/* line 153 */
    return (make_leaf ( name_with_id, owner, instp, "", stringconcat_handler, stringconcat_reset_handler)/* line 154 */)/* line 155 *//* line 156 */}

void stringconcat_handler (Leaf* eh,Mevent* mev) {
                                                       /* line 157 */
    StringConcat_Instance_Data*  inst =   (*eh).instance_data;/* line 158 */
    if  "1" ==   (*mev).port:                          /* line 159 */
        (*inst).buffer1 = clone_string (   (*mev).payload.v)/* line 160 */
        maybe_stringconcat ( eh, inst, mev)            /* line 161 */;
    elif  "2" ==   (*mev).port:                        /* line 162 */
        (*inst).buffer2 = clone_string (   (*mev).payload.v)/* line 163 */
        maybe_stringconcat ( eh, inst, mev)            /* line 164 */;
    elif  "reset" ==   (*mev).port:                    /* line 165 */
        (*inst).buffer1 =  NULL                        /* line 166 */
        (*inst).buffer2 =  NULL;                       /* line 167 */;
    else:                                              /* line 168 */
        runtime_error ( str( "bad mev.port for stringconcat: ") +   (*mev).port )/* line 169 *//* line 170 *//* line 171 *//* line 172 */}

void maybe_stringconcat (Leaf* eh,StringConcat_Instance_Dat* inst,Mevent* mev) {
                                                       /* line 173 */
    if   (*inst).buffer1!= NULL and   (*inst).buffer2!= NULL:/* line 174 */
        Str*  concatenated_string =  "";               /* line 175 */
        if  0 == len (  (*inst).buffer1):              /* line 176 */
            concatenated_string =   (*inst).buffer2;   /* line 177 */
        elif  0 == len (  (*inst).buffer2):            /* line 178 */
            concatenated_string =   (*inst).buffer1;   /* line 179 */
        else:                                          /* line 180 */
            concatenated_string =   (*inst).buffer1+  (*inst).buffer2;/* line 181 *//* line 182 */
        send ( eh, "", concatenated_string, mev)       /* line 183 */
        (*inst).buffer1 =  NULL                        /* line 184 */
        (*inst).buffer2 =  NULL;                       /* line 185 */;/* line 186 *//* line 187 *//* line 188 */}

/*  */                                                 /* line 189 *//* line 190 */
Str* =  "."                                            /* line 191 */;/* line 192 */
Leaf* string_constant_instantiate (Component_Registry* reg,Container* owner,Str* name,Ignored template_data,Ignored arg) {
                                                       /* line 193 */
    static projectRoot                                 /* line 194 */
    Str* name_with_id = gensymbol ( "strconst")        /* line 195 */
    Str*  s =  template_data;                          /* line 196 */
    if  projectRoot!= "":                              /* line 197 */
        s = re.sub ( "_00_",  projectRoot,  s)         /* line 198 */;/* line 199 */
    return (make_leaf ( name_with_id, owner, s, "", string_constant_handler, NULL)/* line 200 */)/* line 201 *//* line 202 */}

void string_constant_handler (Leaf* eh,Mevent* mev) {
                                                       /* line 203 */
    Str* s =   (*eh).instance_data                     /* line 204 */
    send ( eh, "", s, mev)                             /* line 205 *//* line 206 *//* line 207 */}

Leaf* fakepipename_instantiate (Component_Registry* reg,Container* owner,Str* name,Ignored template_data,Ignored arg) {
                                                       /* line 208 */
    Str* instance_name = gensymbol ( "fakepipe")       /* line 209 */
    return (make_leaf ( instance_name, owner, NULL, "", fakepipename_handler, NULL)/* line 210 */)/* line 211 *//* line 212 */}

Number =  0                                            /* line 213 */;/* line 214 */
void fakepipename_handler (Leaf* eh,Mevent* mev) {
                                                       /* line 215 */
    static rand                                        /* line 216 */
    rand =  rand+ 1
    /*  not very random, but good enough _ ;rand' must be unique within a single run *//* line 217 */
    send ( eh, "", str( "/tmp/fakepipe") +  rand , mev)/* line 218 */;/* line 219 *//* line 220 */}
                                                       /* line 221 */
Switch1star_Instance_Data* fresh_Switch1star_Instance_Data () {
    Switch1star_Instance_Data *self;
    self = (Switch1star_Instance_Data*)malloc(sizeof(Switch1star_Instance_Data*));
    self->state =  "1";                                /* line 223 *//* line 224 */
    return self;
}
                                                       /* line 225 */
void switch1star_reset_handler (Leaf* eh) {
                                                       /* line 226 */
    Switch1star_Instance_Data*  inst =   (*eh).instance_data;/* line 227 */
    inst =  fresh_Switch1star_Instance_Data ()         /* line 228 */;/* line 229 *//* line 230 */}

Leaf* switch1star_instantiate (Component_Registry* reg,Container* owner,Str* name,Ignored template_data,Ignored arg) {
                                                       /* line 231 */
    Str* name_with_id = gensymbol ( "switch1*")        /* line 232 */
    Switch1star_Instance_Data* instp =  fresh_Switch1star_Instance_Data ()/* line 233 */
    return (make_leaf ( name_with_id, owner, instp, "", switch1star_handler, switch1star_reset_handler)/* line 234 */)/* line 235 *//* line 236 */}

void switch1star_handler (Leaf* eh,Mevent* mev) {
                                                       /* line 237 */
    Switch1star_Instance_Data*  inst =   (*eh).instance_data;/* line 238 */
    Str* whichOutput =   (*inst).state                 /* line 239 */
    if  "" ==   (*mev).port:                           /* line 240 */
        if  "1" ==  whichOutput:                       /* line 241 */
            forward ( eh, "1", mev)                    /* line 242 */
            (*inst).state =  "*";                      /* line 243 */
        elif  "*" ==  whichOutput:                     /* line 244 */
            forward ( eh, "*", mev)                    /* line 245 */
        else:                                          /* line 246 */
            send ( eh, "✗", "internal error bad state in switch1*", mev)/* line 247 *//* line 248 */
    elif  "reset" ==   (*mev).port:                    /* line 249 */
        (*inst).state =  "1";                          /* line 250 */
    else:                                              /* line 251 */
        send ( eh, "✗", "internal error bad mevent for switch1*", mev)/* line 252 *//* line 253 *//* line 254 *//* line 255 */}

StringAccumulator* fresh_StringAccumulator () {
    StringAccumulator *self;
    self = (StringAccumulator*)malloc(sizeof(StringAccumulator*));
    self->s =  "";                                     /* line 257 *//* line 258 */
    return self;
}
                                                       /* line 259 */
void strcatstar_reset_handler (Leaf* eh) {
                                                       /* line 260 */
    (*eh).instance_data =  fresh_StringAccumulator ()  /* line 261 */;/* line 262 *//* line 263 */}

Leaf* strcatstar_instantiate (Component_Registry* reg,Container* owner,Str* name,Ignored template_data,Ignored arg) {
                                                       /* line 264 */
    Str* name_with_id = gensymbol ( "String Concat *") /* line 265 */
    Switch1star_Instance_Data* instp =  fresh_StringAccumulator ()/* line 266 */
    return (make_leaf ( name_with_id, owner, instp, "", strcatstar_handler, strcatstar_reset_handler)/* line 267 */)/* line 268 *//* line 269 */}

void strcatstar_handler (Leaf* eh,Mevent* mev) {
                                                       /* line 270 */
    Switch1star_Instance_Data*  accum =   (*eh).instance_data;/* line 271 */
    if  "" ==   (*mev).port:                           /* line 272 */
        (*accum).s =  str(  (*accum).s) +    (*mev).payload.v /* line 273 */;
    elif  "fini" ==   (*mev).port:                     /* line 274 */
        send ( eh, "",  (*accum).s, mev)               /* line 275 */
    else:                                              /* line 276 */
        send ( eh, "✗", "internal error bad mevent for String Concat *", mev)/* line 277 *//* line 278 *//* line 279 *//* line 280 */}

Leaf* stop_instantiate (Component_Registry* reg,Container* owner,Str* name,Ignored template_data,Ignored arg) {
                                                       /* line 281 */
    Str* name_with_id = gensymbol ( "Stop")            /* line 282 */
    Switch1star_Instance_Data* inst =  NULL            /* line 283 */
    return (make_leaf ( name_with_id, owner, inst, "", stop_handler, NULL)/* line 284 */)/* line 285 *//* line 286 */}

void stop_handler (Leaf* eh,Mevent* mev) {
                                                       /* line 287 */
    any*  inst =   (*eh).instance_data;                /* line 288 */
    Container*  parent =   (*eh).owner;                /* line 289 */
    Str*  s =  str( "   !!! stopping: '") +  str(  (*parent).name) +  "'"  /* line 290 */;
    print ( s, file=sys.stderr)                        /* line 291 */
                                                       /* line 292 */
    (*parent).reset ( parent)                          /* line 293 */
    send ( eh, "",   (*mev).payload.v, mev)            /* line 294 *//* line 295 *//* line 296 */}

/*  all of the the built_in leaves are listed here */  /* line 297 */
/*  future: refactor this such that programmers can pick and choose which (lumps of) builtins are used in a specific project *//* line 298 *//* line 299 */
void initialize_stock_components (Component_Registry* reg) {
                                                       /* line 300 */
    register_component ( reg,mkTemplate ( "1then2", NULL, deracer_instantiate))/* line 301 */
    register_component ( reg,mkTemplate ( "1→2", NULL, deracer_instantiate))/* line 302 */
    register_component ( reg,mkTemplate ( "trash", NULL, trash_instantiate))/* line 303 */
    register_component ( reg,mkTemplate ( "🗑️", NULL, trash_instantiate))/* line 304 */
    register_component ( reg,mkTemplate ( "🚫", NULL, stop_instantiate))/* line 305 *//* line 306 *//* line 307 */
    register_component ( reg,mkTemplate ( "Read Text File", NULL, low_level_read_text_file_instantiate))/* line 308 */
    register_component ( reg,mkTemplate ( "Ensure String Datum", NULL, ensure_string_datum_instantiate))/* line 309 *//* line 310 */
    register_component ( reg,mkTemplate ( "syncfilewrite", NULL, syncfilewrite_instantiate))/* line 311 */
    register_component ( reg,mkTemplate ( "String Concat", NULL, stringconcat_instantiate))/* line 312 */
    register_component ( reg,mkTemplate ( "switch1*", NULL, switch1star_instantiate))/* line 313 */
    register_component ( reg,mkTemplate ( "String Concat *", NULL, strcatstar_instantiate))/* line 314 */
    /*  for fakepipe */                                /* line 315 */
    register_component ( reg,mkTemplate ( "fakepipename", NULL, fakepipename_instantiate))/* line 316 *//* line 317 *//* line 318 */}
#include "pbp.h"

Bool =  FALSE                                          /* line 1 */;
Bool =  FALSE                                          /* line 2 */;
Int =  0                                               /* line 3 */;/* line 4 */
void load_error (Str* s) {
                                                       /* line 5 */
    static load_errors                                 /* line 6 */
    print ( s, file=sys.stderr)                        /* line 7 */
                                                       /* line 8 */
    load_errors =  TRUE;                               /* line 9 *//* line 10 *//* line 11 */}

void runtime_error (Str* s) {
                                                       /* line 12 */
    static runtime_errors                              /* line 13 */
    print ( s, file=sys.stderr)                        /* line 14 */
    exit (1)                                           /* line 15 */
    runtime_errors =  TRUE;                            /* line 16 *//* line 17 *//* line 18 */}
                                                       /* line 19 */
Component_Registry* initialize_component_palette_from_files (List_of_Pathname* diagram_source_files) {
                                                       /* line 20 */
    Component_Registry*  reg = make_component_registry ();/* line 21 */
    for diagram_source in  diagram_source_files:       /* line 22 */
        List_of_Container* all_containers_within_single_file = lnet2internal_from_file ( diagram_source)/* line 23 */
        for container in  all_containers_within_single_file:/* line 24 */
            register_component ( reg,mkTemplate ( (*container) [ "name"], container, container_instantiator))/* line 25 *//* line 26 *//* line 27 */
    initialize_stock_components ( reg)                 /* line 28 */
    return ( reg)                                      /* line 29 *//* line 30 *//* line 31 */}

void initialize_component_palette_from_string (JSONStr* lnet) {
                                                       /* line 32 */
    Component_Registry*  reg = make_component_registry ();/* line 33 */
    List_of_Container* all_containers = lnet2internal_from_string ( lnet)/* line 34 */
    for container in  all_containers:                  /* line 35 */
        register_component ( reg,mkTemplate ( (*container) [ "name"], container, container_instantiator))/* line 36 *//* line 37 */
    initialize_stock_components ( reg)                 /* line 38 */
    return ( reg)                                      /* line 39 *//* line 40 */}

Tuple_Palette_DiagramNames_ArgStr* initialize_from_files (List_of_DiagramsName* diagram_names) {
                                                       /* line 41 */
    Str* arg =  NULL                                   /* line 42 */
    Component_Registry* palette = initialize_component_palette_from_files ( diagram_names)/* line 43 */
    return [ palette,[ diagram_names, arg]]            /* line 44 *//* line 45 *//* line 46 */}

Tuple_Palette_DiagramNames_ArgStr* initialize_from_string () {
                                                       /* line 47 */
    Str* arg =  NULL                                   /* line 48 */
    Component_Registry* palette = initialize_component_palette_from_string ()/* line 49 */
    return [ palette,[ NULL, arg]]                     /* line 50 *//* line 51 *//* line 52 */}

void start (Str* arg,Str* part_name,Component_Registry* palette,Tuple_Palette_DiagramNames_ArgStr* env) {
                                                       /* line 53 */
    Part* part = start_bare ( part_name, palette, env) /* line 54 */
    inject ( part, "", arg)                            /* line 55 */
    finalize ( part)                                   /* line 56 *//* line 57 *//* line 58 */}

Part* start_bare (Str* part_name,Component_Registry* palette,Tuple_Palette_DiagramNames_ArgStr* env) {
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
        (*d).v =  payload                              /* line 76 */
        (*d).clone =  lambda : obj_clone ( d)          /* line 77 */
        (*d).reclaim =  NULL                           /* line 78 */
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
    (*d).v =  "!"                                      /* line 92 */
    (*d).clone =  lambda : obj_clone ( d)              /* line 93 */
    (*d).reclaim =  NULL                               /* line 94 */
    return ( d)                                        /* line 95 *//* line 96 */;;;}
