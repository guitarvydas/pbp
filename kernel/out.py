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
    global enumDown                                    #line 58
    global enumUp                                      #line 59
    global enumAcross                                  #line 60
    global enumThrough                                 #line 61
    container = make_container ( container_name, owner)#line 62
    children = []                                      #line 63
    children_by_id = {}
    # not strictly necessary, but, we can remove 1 runtime lookup by "compiling it out“ here#line 64
    # collect children                                 #line 65
    for child_desc in  desc [ "children"]:             #line 66
        child_instance = get_component_instance ( reg, child_desc [ "name"], container)#line 67
        children.append ( child_instance)              #line 68
        id =  child_desc [ "id"]                       #line 69
        children_by_id [id] =  child_instance          #line 70#line 71#line 72
    container.children =  children                     #line 73#line 74
    connectors = []                                    #line 75
    for proto_conn in  desc [ "connections"]:          #line 76
        connector =  Connector ()                      #line 77
        if  proto_conn [ "dir"] ==  enumDown:          #line 78
            connectors.append (create_down_connector ( container, proto_conn, connectors, children_by_id)) #line 79
        elif  proto_conn [ "dir"] ==  enumAcross:      #line 80
            connectors.append (create_across_connector ( container, proto_conn, connectors, children_by_id)) #line 81
        elif  proto_conn [ "dir"] ==  enumUp:          #line 82
            connectors.append (create_up_connector ( container, proto_conn, connectors, children_by_id)) #line 83
        elif  proto_conn [ "dir"] ==  enumThrough:     #line 84
            connectors.append (create_through_connector ( container, proto_conn, connectors, children_by_id)) #line 85#line 86#line 87
    container.connections =  connectors                #line 88
    return  container                                  #line 89#line 90#line 91

# The default handler for container components.        #line 92
def container_handler (container,mevent):              #line 93
    route ( container, container, mevent)
    # references to 'self' are replaced by the container during instantiation#line 94
    while any_child_ready ( container):                #line 95
        step_children ( container, mevent)             #line 96#line 97#line 98

# Stop all children. Reset to a known state. Hit the big red button. #line 99
def container_reset (container):                       #line 100
    for child in  container.children:                  #line 101
        child.reset ( child)                           #line 102#line 103

    container.visit_ordering.clear ()                  #line 104

    container.inq.clear ()                             #line 105

    container.outq.clear ()                            #line 106
    container.state =  "idle"                          #line 107#line 108#line 109

# Frees the given container and associated data.       #line 110
def destroy_container (eh):                            #line 111
    pass                                               #line 112#line 113

# Checks if two senders match, by pointer equality and port name matching.#line 114
def sender_eq (s1,s2):                                 #line 115
    same_components = ( s1.component ==  s2.component) #line 116
    same_ports = ( s1.port ==  s2.port)                #line 117
    return  same_components and  same_ports            #line 118#line 119#line 120

# Delivers the given mevent to the receiver of this connector.#line 121#line 122
def deposit (parent,conn,mevent):                      #line 123
    new_mevent = make_mevent ( conn.receiver.port, mevent.payload)#line 124
    push_mevent ( parent, conn.receiver.component, conn.receiver.queue, new_mevent)#line 125#line 126#line 127

def force_tick (parent,eh):                            #line 128
    tick_mev = make_mevent ( ".",new_datum_bang ())    #line 129
    push_mevent ( parent, eh, eh.inq, tick_mev)        #line 130
    return  tick_mev                                   #line 131#line 132#line 133

def push_mevent (parent,receiver,inq,m):               #line 134
    inq.append ( m)                                    #line 135
    if ( receiver.special):                            #line 136
        parent.visit_ordering.appendleft ( receiver)   #line 137
    else:                                              #line 138
        parent.visit_ordering.append ( receiver)       #line 139#line 140#line 141#line 142#line 143

def is_self (child,container):                         #line 144
    # in an earlier version “self“ was denoted as ϕ    #line 145
    return  child ==  container                        #line 146#line 147#line 148

def step_child_once (child,mev):                       #line 149
    if ( ("PBPSTEPPING" in os.environ) ):              #line 150
        print ( str( "-- stepping ❮") +  str( child.name) +  "❯"  , file=sys.stderr)#line 151
                                                       #line 152#line 153
    child.handler ( child, mev)                        #line 154#line 155#line 156

def step_children (container,causingMevent):           #line 157
    container.state =  "idle"                          #line 158#line 159
    # phase 1 - loop through children and process inputs or children that not "idle" #line 160
    for child in  list ( container.visit_ordering):    #line 161
        # child = container represents self, skip it   #line 162
        if (not (is_self ( child, container))):        #line 163
            if (not ((0==len( child.inq)))):           #line 164
                mev =  child.inq.popleft ()            #line 165
                step_child_once ( child, mev)          #line 166#line 167
                destroy_mevent ( mev)                  #line 168
            else:                                      #line 169
                if  child.state ==  "idle":            #line 170
                    pass                               #line 171
                else:                                  #line 172
                    mev = force_tick ( container, child)#line 173
                    step_child_once ( child, mev)      #line 174
                    destroy_mevent ( mev)              #line 175#line 176#line 177#line 178#line 179

    container.visit_ordering.clear ()                  #line 180#line 181
    # phase 2 - loop through children and route their outputs to appropriate receiver queues based on .connections #line 182
    for child in  container.children:                  #line 183
        if  child.state ==  "active":                  #line 184
            # if child remains active, then the container must remain active and must propagate “ticks“ to child#line 185
            container.state =  "active"                #line 186#line 187#line 188
        while (not ((0==len( child.outq)))):           #line 189
            mev =  child.outq.popleft ()               #line 190
            route ( container, child, mev)             #line 191
            destroy_mevent ( mev)                      #line 192#line 193#line 194#line 195#line 196

def attempt_tick (parent,eh):                          #line 197
    if  eh.state!= "idle":                             #line 198
        force_tick ( parent, eh)                       #line 199#line 200#line 201#line 202

def is_tick (mev):                                     #line 203
    return  "." ==  mev.port
    # assume that any mevent that is sent to port "." is a tick #line 204#line 205#line 206

# Routes a single mevent to all matching destinations, according to#line 207
# the container's connection network.                  #line 208#line 209
def route (container,from_component,mevent):           #line 210
    was_sent =  False
    # for checking that output went somewhere (at least during bootstrap)#line 211
    fromname =  ""                                     #line 212
    global ticktime                                    #line 213
    ticktime =  ticktime+ 1                            #line 214
    if is_tick ( mevent):                              #line 215
        for child in  container.children:              #line 216
            attempt_tick ( container, child)           #line 217
        was_sent =  True                               #line 218
    else:                                              #line 219
        if (not (is_self ( from_component, container))):#line 220
            fromname =  from_component.name            #line 221#line 222
        from_sender = mkSender ( fromname, from_component, mevent.port)#line 223#line 224
        for connector in  container.connections:       #line 225
            if sender_eq ( from_sender, connector.sender):#line 226
                deposit ( container, connector, mevent)#line 227
                was_sent =  True                       #line 228#line 229#line 230#line 231
    if not ( was_sent):                                #line 232
        live_update ( "internal error",  str( container.name) +  str( ": mevent on port '") +  str( mevent.port) +  str( "' from ") +  str( fromname) +  " dropped on floor..."     )#line 233#line 234#line 235#line 236

def any_child_ready (container):                       #line 237
    for child in  container.children:                  #line 238
        if child_is_ready ( child):                    #line 239
            return  True                               #line 240#line 241#line 242
    return  False                                      #line 243#line 244#line 245

def child_is_ready (eh):                               #line 246
    return (not ((0==len( eh.outq)))) or (not ((0==len( eh.inq)))) or ( eh.state!= "idle") or (any_child_ready ( eh))#line 247#line 248#line 249
                                                       #line 250
# Creates a component that acts as a container. It is the same as a `Eh` instance#line 251
# whose handler function is `container_handler`.       #line 252
def make_container (name,owner):                       #line 253
    eh =  Eh ()                                        #line 254
    eh.name =  name                                    #line 255
    eh.owner =  owner                                  #line 256
    eh.handler =  container_handler                    #line 257
    eh.finject =  injector                             #line 258
    eh.reset =  container_reset                        #line 259
    eh.state =  "idle"                                 #line 260
    eh.kind =  "container"                             #line 261
    return  eh                                         #line 262#line 263#line 264

# Sends a mevent on the given `port` with `data`, placing it on the output#line 265
# of the given component.                              #line 266#line 267
def send (eh,port,obj,causingMevent):                  #line 268
    d =  Datum ()                                      #line 269
    d.v =  obj                                         #line 270
    d.clone =  obj_clone                               #line 271
    d.reclaim =  None                                  #line 272
    mev = make_mevent ( port, d)                       #line 273
    put_output ( eh, mev)                              #line 274#line 275#line 276

def forward (eh,port,mev):                             #line 277
    fwdmev = make_mevent ( port, mev.payload)          #line 278
    put_output ( eh, fwdmev)                           #line 279#line 280#line 281

def inject_mevent (eh,mev):                            #line 282
    eh.finject ( eh, mev)                              #line 283#line 284#line 285

def set_active (eh):                                   #line 286
    eh.state =  "active"                               #line 287#line 288#line 289

def set_idle (eh):                                     #line 290
    eh.state =  "idle"                                 #line 291#line 292#line 293

def put_output (eh,mev):                               #line 294
    eh.outq.append ( mev)                              #line 295#line 296#line 297

def obj_clone (obj):                                   #line 298
    return  obj                                        #line 299#line 300
