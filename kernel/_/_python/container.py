def create_down_connector (container,proto_conn,connectors,children_by_id):#line 1
    # JSON: {;dir': 0, 'source': {'name': '', 'id': 0}, 'source_port': '', 'target': {'name': 'Echo', 'id': 12}, 'target_port': ''},#line 2
    connector =  Connector ()                          #line 3
    connector.direction =  "down"                      #line 4
    connector.sender = mkSender ( container.name, container, proto_conn [ "source_port"])#line 5
    target_proto =  proto_conn [ "target"]             #line 6
    id_proto =  target_proto [ "id"]                   #line 7
    target_component =  children_by_id [id_proto]      #line 8
    if ( target_component ==  None):                   #line 9
        load_error ( str( "internal error: .Down connection target internal error ") + ( proto_conn [ "target"]) [ "name"] )#line 10
    else:                                              #line 11
        connector.receiver = mkReceiver ( target_component.name, target_component, proto_conn [ "target_port"], target_component.inq)#line 12#line 13
    return  connector                                  #line 14#line 15#line 16

def create_across_connector (container,proto_conn,connectors,children_by_id):#line 17
    connector =  Connector ()                          #line 18
    connector.direction =  "across"                    #line 19
    source_component =  children_by_id [(( proto_conn [ "source"]) [ "id"])]#line 20
    target_component =  children_by_id [(( proto_conn [ "target"]) [ "id"])]#line 21
    if  source_component ==  None:                     #line 22
        load_error ( str( "internal error: .Across connection source not ok ") + ( proto_conn [ "source"]) [ "name"] )#line 23
    else:                                              #line 24
        connector.sender = mkSender ( source_component.name, source_component, proto_conn [ "source_port"])#line 25
        if  target_component ==  None:                 #line 26
            load_error ( str( "internal error: .Across connection target not ok ") + ( proto_conn [ "target"]) [ "name"] )#line 27
        else:                                          #line 28
            connector.receiver = mkReceiver ( target_component.name, target_component, proto_conn [ "target_port"], target_component.inq)#line 29#line 30#line 31
    return  connector                                  #line 32#line 33#line 34

def create_up_connector (container,proto_conn,connectors,children_by_id):#line 35
    connector =  Connector ()                          #line 36
    connector.direction =  "up"                        #line 37
    source_component =  children_by_id [(( proto_conn [ "source"]) [ "id"])]#line 38
    if  source_component ==  None:                     #line 39
        load_error ( str( "internal error: .Up connection source not ok ") + ( proto_conn [ "source"]) [ "name"] )#line 40
    else:                                              #line 41
        connector.sender = mkSender ( source_component.name, source_component, proto_conn [ "source_port"])#line 42
        connector.receiver = mkReceiver ( container.name, container, proto_conn [ "target_port"], container.outq)#line 43#line 44
    return  connector                                  #line 45#line 46#line 47

def create_through_connector (container,proto_conn,connectors,children_by_id):#line 48
    connector =  Connector ()                          #line 49
    connector.direction =  "through"                   #line 50
    connector.sender = mkSender ( container.name, container, proto_conn [ "source_port"])#line 51
    connector.receiver = mkReceiver ( container.name, container, proto_conn [ "target_port"], container.outq)#line 52
    return  connector                                  #line 53#line 54#line 55
                                                       #line 56
def container_instantiator (reg,owner,container_name,desc,arg):#line 57
    global enumDown, enumUp, enumAcross, enumThrough   #line 58
    container = make_container ( container_name, owner)#line 59
    children = []                                      #line 60
    children_by_id = {}
    # not strictly necessary, but, we can remove 1 runtime lookup by "compiling it out“ here#line 61
    # collect children                                 #line 62
    for child_desc in  desc [ "children"]:             #line 63
        child_instance = get_component_instance ( reg, child_desc [ "name"], container)#line 64
        children.append ( child_instance)              #line 65
        id =  child_desc [ "id"]                       #line 66
        children_by_id [id] =  child_instance          #line 67#line 68#line 69
    container.children =  children                     #line 70#line 71
    connectors = []                                    #line 72
    for proto_conn in  desc [ "connections"]:          #line 73
        connector =  Connector ()                      #line 74
        if  proto_conn [ "dir"] ==  enumDown:          #line 75
            connectors.append (create_down_connector ( container, proto_conn, connectors, children_by_id)) #line 76
        elif  proto_conn [ "dir"] ==  enumAcross:      #line 77
            connectors.append (create_across_connector ( container, proto_conn, connectors, children_by_id)) #line 78
        elif  proto_conn [ "dir"] ==  enumUp:          #line 79
            connectors.append (create_up_connector ( container, proto_conn, connectors, children_by_id)) #line 80
        elif  proto_conn [ "dir"] ==  enumThrough:     #line 81
            connectors.append (create_through_connector ( container, proto_conn, connectors, children_by_id)) #line 82#line 83#line 84
    container.connections =  connectors                #line 85
    return  container                                  #line 86#line 87#line 88

# The default handler for container components.        #line 89
def container_handler (container,mevent):              #line 90
    route ( container, container, mevent)
    # references to 'self' are replaced by the container during instantiation#line 91
    while any_child_ready ( container):                #line 92
        step_children ( container, mevent)             #line 93#line 94#line 95

# Stop all children. Reset to a known state. Hit the big red button. #line 96
def container_reset_children (container):              #line 97
    for child in  container.children:                  #line 98
        child.stop ( child)                            #line 99#line 100

    container.visit_ordering.clear ()                  #line 101

    container.inq.clear ()                             #line 102

    container.outq.clear ()                            #line 103
    container.state =  "idle"                          #line 104#line 105#line 106

# Frees the given container and associated data.       #line 107
def destroy_container (eh):                            #line 108
    pass                                               #line 109#line 110

# Checks if two senders match, by pointer equality and port name matching.#line 111
def sender_eq (s1,s2):                                 #line 112
    same_components = ( s1.component ==  s2.component) #line 113
    same_ports = ( s1.port ==  s2.port)                #line 114
    return  same_components and  same_ports            #line 115#line 116#line 117

# Delivers the given mevent to the receiver of this connector.#line 118#line 119
def deposit (parent,conn,mevent):                      #line 120
    new_mevent = make_mevent ( conn.receiver.port, mevent.payload)#line 121
    push_mevent ( parent, conn.receiver.component, conn.receiver.queue, new_mevent)#line 122#line 123#line 124

def force_tick (parent,eh):                            #line 125
    tick_mev = make_mevent ( ".",new_datum_bang ())    #line 126
    push_mevent ( parent, eh, eh.inq, tick_mev)        #line 127
    return  tick_mev                                   #line 128#line 129#line 130

def push_mevent (parent,receiver,inq,m):               #line 131
    inq.append ( m)                                    #line 132
    if ( receiver.special):                            #line 133
        parent.visit_ordering.appendleft ( receiver)   #line 134
    else:                                              #line 135
        parent.visit_ordering.append ( receiver)       #line 136#line 137#line 138#line 139#line 140

def is_self (child,container):                         #line 141
    # in an earlier version “self“ was denoted as ϕ    #line 142
    return  child ==  container                        #line 143#line 144#line 145

def step_child_once (child,mev):                       #line 146
    if ( ("PBPSTEPPING" in os.environ) ):              #line 147
        print ( str( "-- stepping ❮") +  str( child.name) +  "❯"  , file=sys.stderr)#line 148
                                                       #line 149#line 150
    child.handler ( child, mev)                        #line 151#line 152#line 153

def step_children (container,causingMevent):           #line 154
    container.state =  "idle"                          #line 155#line 156
    # phase 1 - loop through children and process inputs or children that not "idle" #line 157
    for child in  list ( container.visit_ordering):    #line 158
        # child = container represents self, skip it   #line 159
        if (not (is_self ( child, container))):        #line 160
            if (not ((0==len( child.inq)))):           #line 161
                mev =  child.inq.popleft ()            #line 162
                step_child_once ( child, mev)          #line 163#line 164
                destroy_mevent ( mev)                  #line 165
            else:                                      #line 166
                if  child.state ==  "idle":            #line 167
                    pass                               #line 168
                else:                                  #line 169
                    mev = force_tick ( container, child)#line 170
                    step_child_once ( child, mev)      #line 171
                    destroy_mevent ( mev)              #line 172#line 173#line 174#line 175#line 176

    container.visit_ordering.clear ()                  #line 177#line 178
    # phase 2 - loop through children and route their outputs to appropriate receiver queues based on .connections #line 179
    for child in  container.children:                  #line 180
        if  child.state ==  "active":                  #line 181
            # if child remains active, then the container must remain active and must propagate “ticks“ to child#line 182
            container.state =  "active"                #line 183#line 184#line 185
        while (not ((0==len( child.outq)))):           #line 186
            mev =  child.outq.popleft ()               #line 187
            route ( container, child, mev)             #line 188
            destroy_mevent ( mev)                      #line 189#line 190#line 191#line 192#line 193

def attempt_tick (parent,eh):                          #line 194
    if  eh.state!= "idle":                             #line 195
        force_tick ( parent, eh)                       #line 196#line 197#line 198#line 199

def is_tick (mev):                                     #line 200
    return  "." ==  mev.port
    # assume that any mevent that is sent to port "." is a tick #line 201#line 202#line 203

# Routes a single mevent to all matching destinations, according to#line 204
# the container's connection network.                  #line 205#line 206
def route (container,from_component,mevent):           #line 207
    was_sent =  False
    # for checking that output went somewhere (at least during bootstrap)#line 208
    fromname =  ""                                     #line 209
    global ticktime                                    #line 210
    ticktime =  ticktime+ 1                            #line 211
    if is_tick ( mevent):                              #line 212
        for child in  container.children:              #line 213
            attempt_tick ( container, child)           #line 214
        was_sent =  True                               #line 215
    else:                                              #line 216
        if (not (is_self ( from_component, container))):#line 217
            fromname =  from_component.name            #line 218#line 219
        from_sender = mkSender ( fromname, from_component, mevent.port)#line 220#line 221
        for connector in  container.connections:       #line 222
            if sender_eq ( from_sender, connector.sender):#line 223
                deposit ( container, connector, mevent)#line 224
                was_sent =  True                       #line 225#line 226#line 227#line 228
    if not ( was_sent):                                #line 229
        live_update ( "internal error",  str( container.name) +  str( ": mevent on port '") +  str( mevent.port) +  str( "' from ") +  str( fromname) +  " dropped on floor..."     )#line 230#line 231#line 232#line 233

def any_child_ready (container):                       #line 234
    for child in  container.children:                  #line 235
        if child_is_ready ( child):                    #line 236
            return  True                               #line 237#line 238#line 239
    return  False                                      #line 240#line 241#line 242

def child_is_ready (eh):                               #line 243
    return (not ((0==len( eh.outq)))) or (not ((0==len( eh.inq)))) or ( eh.state!= "idle") or (any_child_ready ( eh))#line 244#line 245#line 246
                                                       #line 247
# Creates a component that acts as a container. It is the same as a `Eh` instance#line 248
# whose handler function is `container_handler`.       #line 249
def make_container (name,owner):                       #line 250
    eh =  Eh ()                                        #line 251
    eh.name =  name                                    #line 252
    eh.owner =  owner                                  #line 253
    eh.handler =  container_handler                    #line 254
    eh.finject =  injector                             #line 255
    eh.stop =  container_reset_children                #line 256
    eh.state =  "idle"                                 #line 257
    eh.kind =  "container"                             #line 258
    return  eh                                         #line 259#line 260#line 261

# Sends a mevent on the given `port` with `data`, placing it on the output#line 262
# of the given component.                              #line 263#line 264
def send (eh,port,obj,causingMevent):                  #line 265
    d =  Datum ()                                      #line 266
    d.v =  obj                                         #line 267
    d.clone =  lambda : obj_clone ( d)                 #line 268
    d.reclaim =  None                                  #line 269
    mev = make_mevent ( port, d)                       #line 270
    put_output ( eh, mev)                              #line 271#line 272#line 273

def forward (eh,port,mev):                             #line 274
    fwdmev = make_mevent ( port, mev.payload)          #line 275
    put_output ( eh, fwdmev)                           #line 276#line 277#line 278

def inject_mevent (eh,mev):                            #line 279
    eh.finject ( eh, mev)                              #line 280#line 281#line 282

def set_active (eh):                                   #line 283
    eh.state =  "active"                               #line 284#line 285#line 286

def set_idle (eh):                                     #line 287
    eh.state =  "idle"                                 #line 288#line 289#line 290

def put_output (eh,mev):                               #line 291
    eh.outq.append ( mev)                              #line 292#line 293#line 294

def obj_clone (obj):                                   #line 295
    return  obj                                        #line 296#line 297
