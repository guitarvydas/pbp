#include "pbp.h"

Wire* create_down_connector (Container* container,Wire_Proto* proto_conn,List_of_Wire* connectors,Table_by_ID_of_Part* children_by_id) {
                                                       /* line 1 */
    /*  JSON: {;dir': 0, 'source': {'name': '', 'id': 0}, 'source_port': '', 'target': {'name': 'Echo', 'id': 12}, 'target_port': ''}, *//* line 2 */
    Wire*  connector =  fresh_Connector ()             /* line 3 */;
    (*connector).direction =  counted("down");         /* line 4 */
    (*connector).sender = mkSender (  (*container).name, container,lookupstring ( proto_conn, counted("source_port")))/* line 5 */;
    Part* target_proto = lookupstring ( proto_conn, counted("target"))/* line 6 */
    ID id_proto = lookupstring ( target_proto, counted("id"))/* line 7 */
    Part* target_component = lookupid ( children_by_id, id_proto)/* line 8 */
    if ( target_component ==  NULL):                   /* line 9 */
        load_error ( str( counted("internal_error:_.Down_connection_target_internal_error_")) + lookupstring ((lookupstring ( proto_conn, counted("target"))), counted("name")) )/* line 10 */
    else:                                              /* line 11 */
        (*connector).receiver = mkReceiver (  (*target_component).name, target_component,lookupstring ( proto_conn, counted("target_port")),  (*target_component).inq)/* line 12 */;;/* line 13 */
    return ( connector)                                /* line 14 */;;/* line 15 *//* line 16 */}

Wire* create_across_connector (Container* container,Wire_Proto* proto_conn,List_of_Wire* connectors,Table_by_ID_of_Part* children_by_id) {
                                                       /* line 17 */
    Wire*  connector =  fresh_Connector ()             /* line 18 */;
    (*connector).direction =  counted("across");       /* line 19 */
    Str sid = lookupstring ((lookupstring ( proto_conn, counted("source"))), counted("id"))/* line 20 */
    Part* source_component = lookupid ( children_by_id, sid)/* line 21 */
    Str tid = lookupstring ((lookupstring ( proto_conn, counted("target"))), counted("id"))/* line 22 */
    Part* target_component = lookupid ( children_by_id, tid)/* line 23 */
    if  source_component ==  NULL:                     /* line 24 */
        load_error ( str( counted("internal_error:_.Across_connection_source_not_ok_")) + lookupstring ((lookupstring ( proto_conn, counted("source"))), counted("name")) )/* line 25 */
    else:                                              /* line 26 */
        (*connector).sender = mkSender (  (*source_component).name, source_component,lookupstring ( proto_conn, counted("source_port")))/* line 27 */;
        if  target_component ==  NULL:                 /* line 28 */
            load_error ( str( counted("internal_error:_.Across_connection_target_not_ok_")) + lookupstring ((lookupstring ( proto_conn, counted("target"))), counted("name")) )/* line 29 */
        else:                                          /* line 30 */
            (*connector).receiver = mkReceiver (  (*target_component).name, target_component,lookupstring ( proto_conn, counted("target_port")),  (*target_component).inq)/* line 31 */;;/* line 32 */;/* line 33 */
    return ( connector)                                /* line 34 */;/* line 35 *//* line 36 */}

Wire* create_up_connector (Container* container,Wire_Proto* proto_conn,List_of_Wire* connectors,Table_by_ID_of_Part* children_by_id) {
                                                       /* line 37 */
    Wire*  connector =  fresh_Connector ()             /* line 38 */;
    (*connector).direction =  counted("up");           /* line 39 */
    Str sid = lookupstring ((lookupstring ( proto_conn, counted("source"))), counted("id"))/* line 40 */
    Part* source_component = lookupid ( children_by_id, sid)/* line 41 */
    if  source_component ==  NULL:                     /* line 42 */
        load_error ( str( counted("internal_error:_.Up_connection_source_not_ok_")) + lookupstring ((lookupstring ( proto_conn, counted("source"))), counted("name")) )/* line 43 */
    else:                                              /* line 44 */
        (*connector).sender = mkSender (  (*source_component).name, source_component,lookupstring ( proto_conn, counted("source_port")))/* line 45 */;
        (*connector).receiver = mkReceiver (  (*container).name, container,lookupstring ( proto_conn, counted("target_port")),  (*container).outq)/* line 46 */;;;/* line 47 */
    return ( connector)                                /* line 48 */;/* line 49 *//* line 50 */}

Wire* create_through_connector (Container* container,Wire_Proto* proto_conn,List_of_Wire* connectors,Table_by_ID_of_Part* children_by_id) {
                                                       /* line 51 */
    Wire*  connector =  fresh_Connector ()             /* line 52 */;
    (*connector).direction =  counted("through");      /* line 53 */
    (*connector).sender = mkSender (  (*container).name, container,lookupstring ( proto_conn, counted("source_port")))/* line 54 */;
    (*connector).receiver = mkReceiver (  (*container).name, container,lookupstring ( proto_conn, counted("target_port")),  (*container).outq)/* line 55 */;
    return ( connector)                                /* line 56 */;;;/* line 57 *//* line 58 */}
                                                       /* line 59 */
Container* container_instantiator (Component_Registry* reg,Container* owner,Str* container_name,Template* desc,Str* arg) {
                                                       /* line 60 */
    static enumDown                                    /* line 61 */
    static enumUp                                      /* line 62 */
    static enumAcross                                  /* line 63 */
    static enumThrough                                 /* line 64 */
    Container* container = make_container ( container_name, owner)/* line 65 */
    List_of_PartI* children = list_fresh()             /* line 66 */
    List_of_PartI* children_by_id = dict_fresh()
    /*  not strictly necessary, but, we can remove 1 runtime lookup by "compiling it out“ here *//* line 67 */
    /*  collect children */                            /* line 68 */
    for child_desc in lookupstring ( desc, counted("children")):/* line 69 */
        PartI* child_instance = get_component_instance ( reg,lookupstring ( child_desc, counted("name")), container)/* line 70 */
        children.append ( child_instance)              /* line 71 */
        Str* id = lookupstring ( child_desc, counted("id"))/* line 72 */
        lookupid ( children_by_id, id) =  child_instance;/* line 73 *//* line 74 */;/* line 75 */
    (*container).children =  children;                 /* line 76 *//* line 77 */
    List_of_WireI* connectors = list_fresh()           /* line 78 */
    for proto_conn in lookupstring ( desc, counted("connections")):/* line 79 */
        WireI*  connector =  fresh_Connector ()        /* line 80 */;
        if lookupstring ( proto_conn, counted("dir")) ==  enumDown:/* line 81 */
            connectors.append (create_down_connector ( container, proto_conn, connectors, children_by_id)) /* line 82 */
        elif lookupstring ( proto_conn, counted("dir")) ==  enumAcross:/* line 83 */
            connectors.append (create_across_connector ( container, proto_conn, connectors, children_by_id)) /* line 84 */
        elif lookupstring ( proto_conn, counted("dir")) ==  enumUp:/* line 85 */
            connectors.append (create_up_connector ( container, proto_conn, connectors, children_by_id)) /* line 86 */
        elif lookupstring ( proto_conn, counted("dir")) ==  enumThrough:/* line 87 */
            connectors.append (create_through_connector ( container, proto_conn, connectors, children_by_id)) /* line 88 *//* line 89 *//* line 90 */
    (*container).connections =  connectors;            /* line 91 */
    return ( container)                                /* line 92 */;;/* line 93 *//* line 94 */}

/*  The default handler for container components. */   /* line 95 */
void container_handler (Container* container,Mevent* mevent) {
                                                       /* line 96 */
    route ( container, container, mevent)
    /*  references to 'self' are replaced by the container during instantiation *//* line 97 */
    while any_child_ready ( container):                /* line 98 */
        step_children ( container, mevent)             /* line 99 *//* line 100 *//* line 101 */}

/*  Stop all children. Reset to a known state. Hit the big red button.  *//* line 102 */
void container_reset (Container* container) {
                                                       /* line 103 */
    for child in   (*container).children:              /* line 104 */
        (*child).reset ( child)                        /* line 105 *//* line 106 */

    queue_clear(  (*container).visit_ordering)         /* line 107 */

    queue_clear(  (*container).inq)                    /* line 108 */

    queue_clear(  (*container).outq)                   /* line 109 */
    (*container).state =  counted("idle");;            /* line 110 *//* line 111 *//* line 112 */}

/*  Frees the given container and associated data. */  /* line 113 */
void destroy_container (Part* eh) {
                                                       /* line 114 */
                                                       /* line 115 *//* line 116 */}

/*  Checks if two senders match, by pointer equality and port name matching. *//* line 117 */
BOOL sender_eq (Part* s1,Part* s2) {
                                                       /* line 118 */
    BOOL same_components = (  (*s1).component ==   (*s2).component)/* line 119 */
    BOOL same_ports = (  (*s1).port ==   (*s2).port)   /* line 120 */
    return ( same_components and  same_ports)          /* line 121 *//* line 122 *//* line 123 */}

/*  Delivers the given mevent to the receiver of this connector. *//* line 124 *//* line 125 */
void deposit (Container* parent,Wire* conn,Mevent* mevent) {
                                                       /* line 126 */
    Mevent* new_mevent = make_mevent (   (*conn).receiver.port,  (*mevent).payload)/* line 127 */
    push_mevent ( parent,   (*conn).receiver.component,   (*conn).receiver.queue, new_mevent)/* line 128 *//* line 129 *//* line 130 */}

void force_tick (Container* parent,Part* eh) {
                                                       /* line 131 */
    Mevent* tick_mev = make_mevent ( counted("."),new_datum_bang ())/* line 132 */
    push_mevent ( parent, eh,  (*eh).inq, tick_mev)    /* line 133 */
    return ( tick_mev)                                 /* line 134 *//* line 135 *//* line 136 */}

void push_mevent (Container* parent,Part* receiver,Queue* inq,Mevent* m) {
                                                       /* line 137 */
    inq.append ( m)                                    /* line 138 */
    if (  (*receiver).special):                        /* line 139 */
        (*parent).visit_ordering.appendleft ( receiver)/* line 140 */
    else:                                              /* line 141 */
        (*parent).visit_ordering.append ( receiver)    /* line 142 *//* line 143 *//* line 144 *//* line 145 *//* line 146 */}

Bool is_self (Part* child,Container* container) {
                                                       /* line 147 */
    /*  in an earlier version “self“ was denoted as ϕ *//* line 148 */
    return ( child ==  container)                      /* line 149 *//* line 150 *//* line 151 */}

void step_child_once (Part* child,Mevent* mev) {
                                                       /* line 152 */
    if ( ("PBPSTEPPING" in os.environ) ):              /* line 153 */
        print ( str( counted("--_stepping_❮")) +  str(  (*child).name) +  counted("❯")  , file=sys.stderr)/* line 154 */
                                                       /* line 155 *//* line 156 */
    (*child).handler ( child, mev)                     /* line 157 *//* line 158 *//* line 159 */}

void step_children (Container* container,Mevent* causingMevent) {
                                                       /* line 160 */
    (*container).state =  counted("idle");             /* line 161 *//* line 162 */
    /*  phase 1 - loop through children and process inputs or children that not "idle"  *//* line 163 */
    for child in  list (  (*container).visit_ordering):/* line 164 */
        /*  child = container represents self, skip it *//* line 165 */
        if (not (is_self ( child, container))):        /* line 166 */
            if (not ((0==len(  (*child).inq)))):       /* line 167 */
                Mevent* mev =   (*child).inq.popleft ()/* line 168 */
                step_child_once ( child, mev)          /* line 169 *//* line 170 */
                destroy_mevent ( mev)                  /* line 171 */
            else:                                      /* line 172 */
                if   (*child).state ==  counted("idle"):/* line 173 */
                                                       /* line 174 */
                else:                                  /* line 175 */
                    Mevent* mev = force_tick ( container, child)/* line 176 */
                    step_child_once ( child, mev)      /* line 177 */
                    destroy_mevent ( mev)              /* line 178 *//* line 179 *//* line 180 *//* line 181 *//* line 182 */

    queue_clear(  (*container).visit_ordering)         /* line 183 *//* line 184 */
    /*  phase 2 - loop through children and route their outputs to appropriate receiver queues based on .connections  *//* line 185 */
    for child in   (*container).children:              /* line 186 */
        if   (*child).state ==  counted("active"):     /* line 187 */
            /*  if child remains active, then the container must remain active and must propagate “ticks“ to child *//* line 188 */
            (*container).state =  counted("active");;  /* line 189 *//* line 190 *//* line 191 */
        while (not ((0==len(  (*child).outq)))):       /* line 192 */
            Mevent* mev =   (*child).outq.popleft ()   /* line 193 */
            route ( container, child, mev)             /* line 194 */
            destroy_mevent ( mev)                      /* line 195 *//* line 196 *//* line 197 */;/* line 198 *//* line 199 */}

void attempt_tick (Container* parent,Part* eh) {
                                                       /* line 200 */
    if   (*eh).state!= counted("idle"):                /* line 201 */
        force_tick ( parent, eh)                       /* line 202 *//* line 203 *//* line 204 *//* line 205 */}

Bool is_tick (Mevent* mev) {
                                                       /* line 206 */
    return ( counted(".") ==   (*mev).port)
    /*  assume that any mevent that is sent to port "." is a tick  *//* line 207 *//* line 208 *//* line 209 */}

/*  Routes a single mevent to all matching destinations, according to *//* line 210 */
/*  the container's connection network. */             /* line 211 *//* line 212 */
void route (Container* container,Part* from_component,Mevent* mevent) {
                                                       /* line 213 */
    Bool  was_sent =  FALSE;
    /*  for checking that output went somewhere (at least during bootstrap) *//* line 214 */
    Str*  fromname =  counted("");                     /* line 215 */
    static ticktime                                    /* line 216 */
    ticktime =  ticktime+ 1;                           /* line 217 */
    if is_tick ( mevent):                              /* line 218 */
        for child in   (*container).children:          /* line 219 */
            attempt_tick ( container, child)           /* line 220 */
        was_sent =  TRUE;;                             /* line 221 */
    else:                                              /* line 222 */
        if (not (is_self ( from_component, container))):/* line 223 */
            fromname =   (*from_component).name;;      /* line 224 *//* line 225 */
        Sender* from_sender = mkSender ( fromname, from_component,  (*mevent).port)/* line 226 *//* line 227 */
        for connector in   (*container).connections:   /* line 228 */
            if sender_eq ( from_sender,  (*connector).sender):/* line 229 */
                deposit ( container, connector, mevent)/* line 230 */
                was_sent =  TRUE;;                     /* line 231 *//* line 232 *//* line 233 *//* line 234 */
    if not ( was_sent):                                /* line 235 */
        live_update ( counted("internal_error"),  str(  (*container).name) +  str( counted(":_mevent_on_port_'")) +  str(  (*mevent).port) +  str( counted("'_from_")) +  str( fromname) +  counted("_dropped_on_floor...")     )/* line 236 *//* line 237 */;/* line 238 *//* line 239 */}

Bool any_child_ready (Container* container) {
                                                       /* line 240 */
    for child in   (*container).children:              /* line 241 */
        if child_is_ready ( child):                    /* line 242 */
            return ( TRUE)                             /* line 243 *//* line 244 *//* line 245 */
    return ( FALSE)                                    /* line 246 *//* line 247 *//* line 248 */}

Bool child_is_ready (Part* eh) {
                                                       /* line 249 */
    return ((not ((0==len(  (*eh).outq)))) or (not ((0==len(  (*eh).inq)))) or (  (*eh).state!= counted("idle")) or (any_child_ready ( eh)))/* line 250 *//* line 251 *//* line 252 */}
                                                       /* line 253 */
/*  Creates a component that acts as a container. It is the same as a `Eh` instance *//* line 254 */
/*  whose handler function is `container_handler`. */  /* line 255 */
Container* make_container (Str* name,Container* owner) {
                                                       /* line 256 */
    Container*  eh =  fresh_Eh ()                      /* line 257 */;
    (*eh).name =  name;                                /* line 258 */
    (*eh).owner =  owner;                              /* line 259 */
    (*eh).handler =  container_handler;                /* line 260 */
    (*eh).finject =  injector;                         /* line 261 */
    (*eh).reset =  container_reset;                    /* line 262 */
    (*eh).state =  counted("idle");                    /* line 263 */
    (*eh).kind =  counted("container");                /* line 264 */
    return ( eh)                                       /* line 265 */;;;;;;;/* line 266 *//* line 267 */}

/*  Sends a mevent on the given `port` with `data`, placing it on the output *//* line 268 */
/*  of the given component. */                         /* line 269 *//* line 270 */
void send (Part* eh,Port port,Part* obj,Mevent* causingMevent) {
                                                       /* line 271 */
    Payload*  d =  fresh_Datum ()                      /* line 272 */;
    (*d).v =  obj;                                     /* line 273 */
    (*d).clone =  lambda : obj_clone ( d)              /* line 274 */;
    (*d).reclaim =  NULL;                              /* line 275 */
    Mevent* mev = make_mevent ( port, d)               /* line 276 */
    put_output ( eh, mev)                              /* line 277 */;;;/* line 278 *//* line 279 */}

void forward (Part* eh,Port port,Mevent* mev) {
                                                       /* line 280 */
    Mevent* fwdmev = make_mevent ( port,  (*mev).payload)/* line 281 */
    put_output ( eh, fwdmev)                           /* line 282 *//* line 283 *//* line 284 */}

Mevent* inject_mevent (Part* eh,Mevent* mev) {
                                                       /* line 285 */
    (*eh).finject ( eh, mev)                           /* line 286 *//* line 287 *//* line 288 */}

void set_active (Part* eh) {
                                                       /* line 289 */
    (*eh).state =  counted("active");;                 /* line 290 *//* line 291 *//* line 292 */}

void set_idle (Part* eh) {
                                                       /* line 293 */
    (*eh).state =  counted("idle");;                   /* line 294 *//* line 295 *//* line 296 */}

void put_output (Part* eh,Mevent* mev) {
                                                       /* line 297 */
    (*eh).outq.append ( mev)                           /* line 298 *//* line 299 *//* line 300 */}

Payload* obj_clone (Payload* obj) {
                                                       /* line 301 */
    return ( obj)                                      /* line 302 *//* line 303 */}
