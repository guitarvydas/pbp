#
import sys
import re
import subprocess
import tempfile
import shlex
import os
import json
from collections import deque
import socket
import struct
import base64
import hashlib
import random
from repl import live_update

def deque_to_json(d):
    # """
    # Convert a deque of Mevent objects to a JSON string, preserving order.
    # Each Mevent object is converted to a dict with a single key (from Mevent.key)
    # containing the payload as its value.

    # Args:
    #     d: The deque of Mevent objects to convert

    # Returns:
    #     A JSON string representation of the deque
    # """
    # # Convert deque to list of objects where each mevent's key contains its payload
    ordered_list = [{mev.port: "" if mev.payload.v is None else mev.payload.v} for mev in d]

    # # Convert to JSON with indentation for readability
    return json.dumps(ordered_list, indent=2)


                                                       #line 1
# Data for an asyncronous component _ effectively, a function with input#line 1
# and output queues of mevents.                        #line 2
#                                                      #line 3
# Components can either be a user_supplied function ("leaf“), or a “container“#line 4
# that routes mevents to child components according to a list of connections#line 5
# that serve as a mevent routing table.                #line 6
#                                                      #line 7
# Child components themselves can be leaves or other containers.#line 8
#                                                      #line 9
# `handler` invokes the code that is attached to this component.#line 10
#                                                      #line 11
# `instance_data` is a pointer to instance data that the `leaf_handler`#line 12
# function may want whenever it is invoked again.      #line 13#line 14
# Eh_States :: enum { idle, active }                   #line 15
class Eh:
    def __init__ (self):                               #line 16
        self.name =  ""                                #line 17
        self.inq =  deque ([])                         #line 18
        self.outq =  deque ([])                        #line 19
        self.owner =  None                             #line 20
        self.children = []                             #line 21
        self.visit_ordering =  deque ([])              #line 22
        self.connections = []                          #line 23
        self.handler =  None                           #line 24
        self.finject =  None                           #line 25
        self.reset =  None                             #line 26
        self.instance_data =  None                     #line 27# arg needed for probe support #line 28
        self.arg =  ""                                 #line 29
        self.state =  "idle"                           #line 30
        self.special =  False                          #line 31#line 32
                                                       #line 33
def injector (eh,mevent):                              #line 34
    eh.handler ( eh, mevent)                           #line 35#line 36#line 37
digits = [ "₀", "₁", "₂", "₃", "₄", "₅", "₆", "₇", "₈", "₉", "₁₀", "₁₁", "₁₂", "₁₃", "₁₄", "₁₅", "₁₆", "₁₇", "₁₈", "₁₉", "₂₀", "₂₁", "₂₂", "₂₃", "₂₄", "₂₅", "₂₆", "₂₇", "₂₈", "₂₉"]#line 7#line 8#line 9
def subscripted_digit (n):                             #line 10
    global digits                                      #line 11
    if ( n >=  0 and  n <=  29):                       #line 12
        return  digits [ n]                            #line 13
    else:                                              #line 14
        return  str( "₊") + str ( n)                   #line 15#line 16#line 17#line 18

counter =  0                                           #line 19#line 20
def gensymbol (s):                                     #line 21
    global counter                                     #line 22
    name_with_id =  str( s) + subscripted_digit ( counter) #line 23
    counter =  counter+ 1                              #line 24
    return  name_with_id                               #line 25#line 26
#line 1
class Datum:
    def __init__ (self):                               #line 2
        self.v =  None                                 #line 3
        self.clone =  None                             #line 4
        self.reclaim =  None                           #line 5#line 6
                                                       #line 7#line 8
# Mevent passed to a leaf component.                   #line 9
#                                                      #line 10
# `port` refers to the name of the incoming or outgoing port of this component.#line 11
# `payload` is the data attached to this mevent.       #line 12
class Mevent:
    def __init__ (self):                               #line 13
        self.port =  None                              #line 14
        self.payload =  None                           #line 15#line 16
                                                       #line 17
def clone_port (s):                                    #line 18
    return clone_string ( s)                           #line 19#line 20#line 21

# Utility for making a `Mevent`. Used to safely "seed“ mevents#line 22
# entering the very top of a network.                  #line 23
def make_mevent (port,datum):                          #line 24
    p = clone_string ( port)                           #line 25
    m =  Mevent ()                                     #line 26
    m.port =  p                                        #line 27
    m.payload =  datum.clone ()                        #line 28
    return  m                                          #line 29#line 30#line 31

# Clones a mevent. Primarily used internally for “fanning out“ a mevent to multiple destinations.#line 32
def mevent_clone (mev):                                #line 33
    m =  Mevent ()                                     #line 34
    m.port = clone_port ( mev.port)                    #line 35
    m.payload =  mev.payload.clone ()                  #line 36
    return  m                                          #line 37#line 38#line 39

# Frees a mevent.                                      #line 40
def destroy_mevent (mev):                              #line 41
    # during debug, dont destroy any mevent, since we want to trace mevents, thus, we need to persist ancestor mevents#line 42
    pass                                               #line 43#line 44#line 45

def destroy_datum (mev):                               #line 46
    pass                                               #line 47#line 48#line 49

def destroy_port (mev):                                #line 50
    pass                                               #line 51#line 52#line 53

#                                                      #line 54
def format_mevent (m):                                 #line 55
    if  m ==  None:                                    #line 56
        return  "{}"                                   #line 57
    else:                                              #line 58
        return  str( "{%5C”") +  str( m.port) +  str( "%5C”:%5C”") +  str( m.payload.v) +  "%5C”}"    #line 59#line 60#line 61

def format_mevent_raw (m):                             #line 62
    if  m ==  None:                                    #line 63
        return  ""                                     #line 64
    else:                                              #line 65
        return  m.payload.v                            #line 66#line 67#line 68
#line 1
enumDown =  0                                          #line 2
enumAcross =  1                                        #line 3
enumUp =  2                                            #line 4
enumThrough =  3                                       #line 5#line 6#line 7
# Routing connection for a container component. The `direction` field has#line 8
# no affect on the default mevent routing system _ it is there for debugging#line 9
# purposes, or for reading by other tools.             #line 10#line 11
class Connector:
    def __init__ (self):                               #line 12
        self.direction =  None # down, across, up, through#line 13
        self.sender =  None                            #line 14
        self.receiver =  None                          #line 15#line 16
                                                       #line 17
# `Sender` is used to "pattern match“ which `Receiver` a mevent should go to,#line 18
# based on component ID (pointer) and port name.       #line 19#line 20
class Sender:
    def __init__ (self):                               #line 21
        self.name =  None                              #line 22
        self.component =  None                         #line 23
        self.port =  None                              #line 24#line 25
                                                       #line 26#line 27#line 28
# `Receiver` is a handle to a destination queue, and a `port` name to assign#line 29
# to incoming mevents to this queue.                   #line 30#line 31
class Receiver:
    def __init__ (self):                               #line 32
        self.name =  None                              #line 33
        self.queue =  None                             #line 34
        self.port =  None                              #line 35
        self.component =  None                         #line 36#line 37
                                                       #line 38
def mkSender (name,component,port):                    #line 39
    s =  Sender ()                                     #line 40
    s.name =  name                                     #line 41
    s.component =  component                           #line 42
    s.port =  port                                     #line 43
    return  s                                          #line 44#line 45#line 46

def mkReceiver (name,component,port,q):                #line 47
    r =  Receiver ()                                   #line 48
    r.name =  name                                     #line 49
    r.component =  component                           #line 50
    r.port =  port                                     #line 51
    # We need a way to determine which queue to target. "Down" and "Across" go to inq, "Up" and "Through" go to outq.#line 52
    r.queue =  q                                       #line 53
    return  r                                          #line 54#line 55
class Component_Registry:
    def __init__ (self):                               #line 1
        self.templates = {}                            #line 2#line 3
                                                       #line 4
class Template:
    def __init__ (self):                               #line 5
        self.name =  None                              #line 6
        self.container =  None                         #line 7
        self.instantiator =  None                      #line 8#line 9
                                                       #line 10
def mkTemplate (name,template_data,instantiator):      #line 11
    templ =  Template ()                               #line 12
    templ.name =  name                                 #line 13
    templ.template_data =  template_data               #line 14
    templ.instantiator =  instantiator                 #line 15
    return  templ                                      #line 16#line 17#line 18
                                                       #line 19
# convert a little-network to internal form (an object data structure created by json parser) ... #line 20
# the actual data structure depends on the json parser library used by the target language #line 21
# the form of the data structure doesn;t matter here, as long as we use lookup operators "@" in this .rt code #line 22#line 23
# ... by reading the little-net from an external file  #line 24
def lnet2internal_from_file (container_xml):           #line 25
    pathname = os.getenv('PBPWD', '<none>')            #line 26
    filename =  os.path.basename ( container_xml)      #line 27

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
                                                       #line 28#line 29#line 30

# ... by reading the little-net from an embedded string (an aspect of creating t2t tool code) #line 31
def lnet2internal_from_string (lnet):                  #line 32

    try:
        routings = json.loads(lnet)
        return routings
    except json.JSONDecodeError as e:
        print ("Error decoding JSON from string 'lnet': '{e}'")
        return None
                                                       #line 33#line 34#line 35

def make_component_registry ():                        #line 36
    return  Component_Registry ()                      #line 37#line 38#line 39

def register_component (reg,template):
    return abstracted_register_component ( reg, template, False)#line 40

def register_component_allow_overwriting (reg,template):
    return abstracted_register_component ( reg, template, True)#line 41#line 42

def abstracted_register_component (reg,template,ok_to_overwrite):#line 43
    name = mangle_name ( template.name)                #line 44
    if  reg!= None and  name in  reg.templates and not  ok_to_overwrite:#line 45
        load_error ( str( "Component /") +  str( template.name) +  "/ already declared"  )#line 46
        return  reg                                    #line 47
    else:                                              #line 48
        reg.templates [name] =  template               #line 49
        return  reg                                    #line 50#line 51#line 52#line 53

def get_component_instance (reg,full_name,owner):      #line 54
    # If a part name begins with ":", it is treated as a JIT part and we let the runtime factory generate it on-the-fly (see kernel_external.rt and external.rt) else it is assumed to be a regular AOT part and assumed to have been registered before runtime, so we just pull its template out of the registry and instantiate it. #line 55
    # ":?<string>" is a probe part that is tagged with <string> #line 56
    # ":$ <command>" is a shell-out part that sends <command> to the operating system shell #line 57
    # ":<string>" else, it's just treated as a string part that produces <string> on its output #line 58
    template_name = mangle_name ( full_name)           #line 59
    if  ":" ==   full_name[0] :                        #line 60
        instance_name = generate_instance_name ( owner, template_name)#line 61
        instance = jit_instantiate ( reg, owner, instance_name, full_name)#line 62
        return  instance                               #line 63
    else:                                              #line 64
        if  template_name in  reg.templates:           #line 65
            template =  reg.templates [template_name]  #line 66
            if ( template ==  None):                   #line 67
                load_error ( str( "Registry Error (A): Can't find component /") +  str( template_name) +  "/"  )#line 68
                return  None                           #line 69
            else:                                      #line 70
                instance_name = generate_instance_name ( owner, template_name)#line 71
                instance =  template.instantiator ( reg, owner, instance_name, template.template_data, "")#line 72
                return  instance                       #line 73#line 74
        else:                                          #line 75
            load_error ( str( "Registry Error (B): Can't find component /") +  str( template_name) +  "/"  )#line 76
            return  None                               #line 77#line 78#line 79#line 80#line 81

def generate_instance_name (owner,template_name):      #line 82
    owner_name =  ""                                   #line 83
    instance_name =  template_name                     #line 84
    if  None!= owner:                                  #line 85
        owner_name =  owner.name                       #line 86
        instance_name =  str( owner_name) +  str( "▹") +  template_name  #line 87
    else:                                              #line 88
        instance_name =  template_name                 #line 89#line 90
    return  instance_name                              #line 91#line 92#line 93

def mangle_name (s):                                   #line 94
    # trim name to remove code from Container component names _ deferred until later (or never)#line 95
    return  s                                          #line 96#line 97
def create_down_connector (container,proto_conn,connectors,children_by_id):#line 1
    # JSON: {;dir': 0, 'source': {'name': '', 'id': 0}, 'source_port': '', 'target': {'name': 'Echo', 'id': 12}, 'target_port': ''},#line 2
    connector =  Connector ()                          #line 3
    connector.direction =  "down"                      #line 4
    connector.sender = mkSender ( container.name, container, proto_conn ["source_port"])#line 5
    target_proto =  proto_conn ["target"]              #line 6
    id_proto =  target_proto ["id"]                    #line 7
    target_component =  children_by_id [id_proto]      #line 8
    if ( target_component ==  None):                   #line 9
        load_error ( str( "internal error: .Down connection target internal error ") + ( proto_conn ["target"]) ["name"] )#line 10
    else:                                              #line 11
        connector.receiver = mkReceiver ( target_component.name, target_component, proto_conn ["target_port"], target_component.inq)#line 12#line 13
    return  connector                                  #line 14#line 15#line 16

def create_across_connector (container,proto_conn,connectors,children_by_id):#line 17
    connector =  Connector ()                          #line 18
    connector.direction =  "across"                    #line 19
    sid = ( proto_conn ["source"]) ["id"]              #line 20
    source_component =  children_by_id [sid]           #line 21
    tid = ( proto_conn ["target"]) ["id"]              #line 22
    target_component =  children_by_id [tid]           #line 23
    if  source_component ==  None:                     #line 24
        load_error ( str( "internal error: .Across connection source not ok ") + ( proto_conn ["source"]) ["name"] )#line 25
    else:                                              #line 26
        connector.sender = mkSender ( source_component.name, source_component, proto_conn ["source_port"])#line 27
        if  target_component ==  None:                 #line 28
            load_error ( str( "internal error: .Across connection target not ok ") + ( proto_conn ["target"]) ["name"] )#line 29
        else:                                          #line 30
            connector.receiver = mkReceiver ( target_component.name, target_component, proto_conn ["target_port"], target_component.inq)#line 31#line 32#line 33
    return  connector                                  #line 34#line 35#line 36

def create_up_connector (container,proto_conn,connectors,children_by_id):#line 37
    connector =  Connector ()                          #line 38
    connector.direction =  "up"                        #line 39
    sid = ( proto_conn ["source"]) ["id"]              #line 40
    source_component =  children_by_id [sid]           #line 41
    if  source_component ==  None:                     #line 42
        load_error ( str( "internal error: .Up connection source not ok ") + ( proto_conn ["source"]) ["name"] )#line 43
    else:                                              #line 44
        connector.sender = mkSender ( source_component.name, source_component, proto_conn ["source_port"])#line 45
        connector.receiver = mkReceiver ( container.name, container, proto_conn ["target_port"], container.outq)#line 46#line 47
    return  connector                                  #line 48#line 49#line 50

def create_through_connector (container,proto_conn,connectors,children_by_id):#line 51
    connector =  Connector ()                          #line 52
    connector.direction =  "through"                   #line 53
    connector.sender = mkSender ( container.name, container, proto_conn ["source_port"])#line 54
    connector.receiver = mkReceiver ( container.name, container, proto_conn ["target_port"], container.outq)#line 55
    return  connector                                  #line 56#line 57#line 58
                                                       #line 59
def container_instantiator (reg,owner,container_name,desc,arg):#line 60
    global enumDown                                    #line 61
    global enumUp                                      #line 62
    global enumAcross                                  #line 63
    global enumThrough                                 #line 64
    container = make_container ( container_name, owner)#line 65
    children = []                                      #line 66
    children_by_id = {}
    # not strictly necessary, but, we can remove 1 runtime lookup by "compiling it out“ here#line 67
    # collect children                                 #line 68
    for child_desc in  desc ["children"]:              #line 69
        child_instance = get_component_instance ( reg, child_desc ["name"], container)#line 70
        children.append ( child_instance)              #line 71
        id =  child_desc ["id"]                        #line 72
        children_by_id [id] =  child_instance          #line 73#line 74#line 75
    container.children =  children                     #line 76#line 77
    connectors = []                                    #line 78
    for proto_conn in  desc ["connections"]:           #line 79
        connector =  Connector ()                      #line 80
        if  proto_conn ["dir"] ==  enumDown:           #line 81
            connectors.append (create_down_connector ( container, proto_conn, connectors, children_by_id)) #line 82
        elif  proto_conn ["dir"] ==  enumAcross:       #line 83
            connectors.append (create_across_connector ( container, proto_conn, connectors, children_by_id)) #line 84
        elif  proto_conn ["dir"] ==  enumUp:           #line 85
            connectors.append (create_up_connector ( container, proto_conn, connectors, children_by_id)) #line 86
        elif  proto_conn ["dir"] ==  enumThrough:      #line 87
            connectors.append (create_through_connector ( container, proto_conn, connectors, children_by_id)) #line 88#line 89#line 90
    container.connections =  connectors                #line 91
    return  container                                  #line 92#line 93#line 94

# The default handler for container components.        #line 95
def container_handler (container,mevent):              #line 96
    route ( container, container, mevent)
    # references to 'self' are replaced by the container during instantiation#line 97
    while any_child_ready ( container):                #line 98
        step_children ( container, mevent)             #line 99#line 100#line 101

# Stop all children. Reset to a known state. Hit the big red button. #line 102
def container_reset (container):                       #line 103
    for child in  container.children:                  #line 104
        child.reset ( child)                           #line 105#line 106

    container.visit_ordering.clear ()                  #line 107

    container.inq.clear ()                             #line 108

    container.outq.clear ()                            #line 109
    container.state =  "idle"                          #line 110#line 111#line 112

# Frees the given container and associated data.       #line 113
def destroy_container (eh):                            #line 114
    pass                                               #line 115#line 116

# Checks if two senders match, by pointer equality and port name matching.#line 117
def sender_eq (s1,s2):                                 #line 118
    same_components = ( s1.component ==  s2.component) #line 119
    same_ports = ( s1.port ==  s2.port)                #line 120
    return  same_components and  same_ports            #line 121#line 122#line 123

# Delivers the given mevent to the receiver of this connector.#line 124#line 125
def deposit (parent,conn,mevent):                      #line 126
    new_mevent = make_mevent ( conn.receiver.port, mevent.payload)#line 127
    push_mevent ( parent, conn.receiver.component, conn.receiver.queue, new_mevent)#line 128#line 129#line 130

def force_tick (parent,eh):                            #line 131
    tick_mev = make_mevent ( ".",new_datum_bang ())    #line 132
    push_mevent ( parent, eh, eh.inq, tick_mev)        #line 133
    return  tick_mev                                   #line 134#line 135#line 136

def push_mevent (parent,receiver,inq,m):               #line 137
    inq.append ( m)                                    #line 138
    if ( receiver.special):                            #line 139
        parent.visit_ordering.appendleft ( receiver)   #line 140
    else:                                              #line 141
        parent.visit_ordering.append ( receiver)       #line 142#line 143#line 144#line 145#line 146

def is_self (child,container):                         #line 147
    # in an earlier version “self“ was denoted as ϕ    #line 148
    return  child ==  container                        #line 149#line 150#line 151

def step_child_once (child,mev):                       #line 152
    if ( ("PBPSTEPPING" in os.environ) ):              #line 153
        print ( str( "-- stepping ❮") +  str( child.name) +  "❯"  , file=sys.stderr)#line 154
                                                       #line 155#line 156
    child.handler ( child, mev)                        #line 157#line 158#line 159

def step_children (container,causingMevent):           #line 160
    container.state =  "idle"                          #line 161#line 162
    # phase 1 - loop through children and process inputs or children that not "idle" #line 163
    for child in  list ( container.visit_ordering):    #line 164
        # child = container represents self, skip it   #line 165
        if (not (is_self ( child, container))):        #line 166
            if (not ((0==len( child.inq)))):           #line 167
                mev =  child.inq.popleft ()            #line 168
                step_child_once ( child, mev)          #line 169#line 170
                destroy_mevent ( mev)                  #line 171
            else:                                      #line 172
                if  child.state ==  "idle":            #line 173
                    pass                               #line 174
                else:                                  #line 175
                    mev = force_tick ( container, child)#line 176
                    step_child_once ( child, mev)      #line 177
                    destroy_mevent ( mev)              #line 178#line 179#line 180#line 181#line 182

    container.visit_ordering.clear ()                  #line 183#line 184
    # phase 2 - loop through children and route their outputs to appropriate receiver queues based on .connections #line 185
    for child in  container.children:                  #line 186
        if  child.state ==  "active":                  #line 187
            # if child remains active, then the container must remain active and must propagate “ticks“ to child#line 188
            container.state =  "active"                #line 189#line 190#line 191
        while (not ((0==len( child.outq)))):           #line 192
            mev =  child.outq.popleft ()               #line 193
            route ( container, child, mev)             #line 194
            destroy_mevent ( mev)                      #line 195#line 196#line 197#line 198#line 199

def attempt_tick (parent,eh):                          #line 200
    if  eh.state!= "idle":                             #line 201
        force_tick ( parent, eh)                       #line 202#line 203#line 204#line 205

def is_tick (mev):                                     #line 206
    return  "." ==  mev.port
    # assume that any mevent that is sent to port "." is a tick #line 207#line 208#line 209

# Routes a single mevent to all matching destinations, according to#line 210
# the container's connection network.                  #line 211#line 212
def route (container,from_component,mevent):           #line 213
    was_sent =  False
    # for checking that output went somewhere (at least during bootstrap)#line 214
    fromname =  ""                                     #line 215
    global ticktime                                    #line 216
    ticktime =  ticktime+ 1                            #line 217
    if is_tick ( mevent):                              #line 218
        for child in  container.children:              #line 219
            attempt_tick ( container, child)           #line 220
        was_sent =  True                               #line 221
    else:                                              #line 222
        if (not (is_self ( from_component, container))):#line 223
            fromname =  from_component.name            #line 224#line 225
        from_sender = mkSender ( fromname, from_component, mevent.port)#line 226#line 227
        for connector in  container.connections:       #line 228
            if sender_eq ( from_sender, connector.sender):#line 229
                deposit ( container, connector, mevent)#line 230
                was_sent =  True                       #line 231#line 232#line 233#line 234
    if not ( was_sent):                                #line 235
        live_update ( "internal error",  str( container.name) +  str( ": mevent on port '") +  str( mevent.port) +  str( "' from ") +  str( fromname) +  " dropped on floor..."     )#line 236#line 237#line 238#line 239

def any_child_ready (container):                       #line 240
    for child in  container.children:                  #line 241
        if child_is_ready ( child):                    #line 242
            return  True                               #line 243#line 244#line 245
    return  False                                      #line 246#line 247#line 248

def child_is_ready (eh):                               #line 249
    return (not ((0==len( eh.outq)))) or (not ((0==len( eh.inq)))) or ( eh.state!= "idle") or (any_child_ready ( eh))#line 250#line 251#line 252
                                                       #line 253
# Creates a component that acts as a container. It is the same as a `Eh` instance#line 254
# whose handler function is `container_handler`.       #line 255
def make_container (name,owner):                       #line 256
    eh =  Eh ()                                        #line 257
    eh.name =  name                                    #line 258
    eh.owner =  owner                                  #line 259
    eh.handler =  container_handler                    #line 260
    eh.finject =  injector                             #line 261
    eh.reset =  container_reset                        #line 262
    eh.state =  "idle"                                 #line 263
    eh.kind =  "container"                             #line 264
    return  eh                                         #line 265#line 266#line 267

# Sends a mevent on the given `port` with `data`, placing it on the output#line 268
# of the given component.                              #line 269#line 270
def send (eh,port,obj,causingMevent):                  #line 271
    d =  Datum ()                                      #line 272
    d.v =  obj                                         #line 273
    d.clone =  lambda : obj_clone ( d)                 #line 274
    d.reclaim =  None                                  #line 275
    mev = make_mevent ( port, d)                       #line 276
    put_output ( eh, mev)                              #line 277#line 278#line 279

def forward (eh,port,mev):                             #line 280
    fwdmev = make_mevent ( port, mev.payload)          #line 281
    put_output ( eh, fwdmev)                           #line 282#line 283#line 284

def inject_mevent (eh,mev):                            #line 285
    eh.finject ( eh, mev)                              #line 286#line 287#line 288

def set_active (eh):                                   #line 289
    eh.state =  "active"                               #line 290#line 291#line 292

def set_idle (eh):                                     #line 293
    eh.state =  "idle"                                 #line 294#line 295#line 296

def put_output (eh,mev):                               #line 297
    eh.outq.append ( mev)                              #line 298#line 299#line 300

def obj_clone (obj):                                   #line 301
    return  obj                                        #line 302#line 303
# Creates a new leaf component out of a handler function, and a data parameter#line 1
# that will be passed back to your handler when called.#line 2#line 3
def make_leaf (name,owner,instance_data,arg,handler,reset_handler):#line 4
    eh =  Eh ()                                        #line 5
    nm =  ""                                           #line 6
    if  None!= owner:                                  #line 7
        nm =  owner.name                               #line 8#line 9
    eh.name =  str( nm) +  str( "▹") +  name           #line 10
    eh.owner =  owner                                  #line 11
    eh.handler =  handler                              #line 12
    eh.reset_handler =  reset_handler                  #line 13
    eh.finject =  injector                             #line 14
    eh.reset =  leaf_reset                             #line 15
    eh.instance_data =  instance_data                  #line 16
    eh.arg =  arg                                      #line 17
    eh.state =  "idle"                                 #line 18
    return  eh                                         #line 19#line 20#line 21

# Reset Leaf part to a known, idle state. Hit the big red button. #line 22
def leaf_reset (part):                                 #line 23

    part.inq.clear ()                                  #line 24

    part.outq.clear ()                                 #line 25
    if ( part.reset_handler!= None):                   #line 26
        part.reset_handler ( part)                     #line 27#line 28
    part.state =  "idle"                               #line 29#line 30
# (This used to be called `external` due to historical reasons). This has evolved into 2 kinds of Leaf parts: AOT and JIT (statically generated before runtime, vs. dynamically generated at runtime). If a part name begins with ;:', it is treated specially as a JIT part, else the part is assumed to have been pre-loaded into the register in the regular way. #line 1#line 2
def jit_instantiate (reg,owner,name,arg):              #line 3
    name_with_id = gensymbol ( name)                   #line 4
    inst = make_leaf ( name_with_id, owner, None, arg, handle_jit, None)#line 5
    firstc =  name [ 1]                                #line 6
    if ( firstc!= "$"):                                #line 7
        # probes get to go to the front of the line    #line 8
        inst.special =  True                           #line 9#line 10
    return  inst                                       #line 11#line 12#line 13

def handle_jit (eh,mev):                               #line 14
    s =  eh.arg                                        #line 15
    firstc =  s [ 1]                                   #line 16
    if  firstc ==  "$":                                #line 17
        shell_out_handler ( eh,    s[1:] [1:] [1:] , mev)#line 18
    elif  firstc ==  "?":                              #line 19
        probe_handler ( eh,  s[1:] , mev)              #line 20
    else:                                              #line 21
        # just a string, send it out                   #line 22
        send ( eh, "",  s[1:] , mev)                   #line 23#line 24#line 25#line 26

def probe_handler (eh,tag,mev):                        #line 27
    global ticktime                                    #line 28
    s =  mev.payload.v                                 #line 29
    live_update ( "Info",  str( "  @") +  str(str ( ticktime)) +  str( "  ") +  str( "probe ") +  str( eh.name) +  str( ": ") + str ( s)      )#line 37#line 38#line 39

def shell_out_handler (eh,cmd,mev):                    #line 40
    s =  mev.payload.v                                 #line 41
    ret =  None                                        #line 42
    rc =  None                                         #line 43
    stdout =  None                                     #line 44
    stderr =  None                                     #line 45
    command =  cmd                                     #line 46
    pbpRoot = os.getenv('PBP', '<none>')               #line 47
    if  pbpRoot!= "":                                  #line 48
        command = re.sub ( "_/",  str( pbpRoot) +  "/" ,  command)#line 51#line 52
    if ( ("PBPSHELLUT" in os.environ) ):               #line 53
        print ( str( "- --- shell-out: ") +  command , file=sys.stderr)#line 54
                                                       #line 55#line 56

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
                                                       #line 57
    if  rc ==  0:                                      #line 58
        send ( eh, "", str( stdout) +  stderr , mev)   #line 59
    else:                                              #line 60
        send ( eh, "✗", str( stdout) +  stderr , mev)  #line 61#line 62#line 63#line 64
def clone_string (s):                                  #line 1
    return  s                                          #line 2#line 3#line 4
                                                       #line 5
def trash_instantiate (reg,owner,name,template_data,arg):#line 6
    name_with_id = gensymbol ( "trash")                #line 7
    return make_leaf ( name_with_id, owner, None, "", trash_handler, None)#line 8#line 9#line 10

def trash_handler (eh,mev):                            #line 11
    # to appease dumped_on_floor checker               #line 12
    pass                                               #line 13#line 14

class TwoMevents:
    def __init__ (self):                               #line 15
        self.firstmev =  None                          #line 16
        self.secondmev =  None                         #line 17#line 18
                                                       #line 19
# Deracer_States :: enum { idle, waitingForFirstmev, waitingForSecondmev }#line 20
class Deracer_Instance_Data:
    def __init__ (self):                               #line 21
        self.state =  None                             #line 22
        self.buffer =  None                            #line 23#line 24
                                                       #line 25
def reclaim_Buffers_from_heap (inst):                  #line 26
    pass                                               #line 27#line 28#line 29

def deracer_reset_handler (eh):                        #line 30
    inst =  eh.instance_data                           #line 31
    inst.state =  "idle"                               #line 32
    inst.buffer =  TwoMevents ()                       #line 33#line 34#line 35

def deracer_instantiate (reg,owner,name,template_data,arg):#line 36
    name_with_id = gensymbol ( "deracer")              #line 37
    inst =  Deracer_Instance_Data ()                   #line 38
    inst.state =  "idle"                               #line 39
    inst.buffer =  TwoMevents ()                       #line 40
    eh = make_leaf ( name_with_id, owner, inst, "", deracer_handler, deracer_reset_handler)#line 41
    return  eh                                         #line 42#line 43#line 44

def send_firstmev_then_secondmev (eh,inst):            #line 45
    forward ( eh, "1", inst.buffer.firstmev)           #line 46
    forward ( eh, "2", inst.buffer.secondmev)          #line 47
    reclaim_Buffers_from_heap ( inst)                  #line 48#line 49#line 50

def deracer_handler (eh,mev):                          #line 51
    inst =  eh.instance_data                           #line 52
    if  inst.state ==  "idle":                         #line 53
        if  "1" ==  mev.port:                          #line 54
            inst.buffer.firstmev =  mev                #line 55
            inst.state =  "waitingForSecondmev"        #line 56
        elif  "2" ==  mev.port:                        #line 57
            inst.buffer.secondmev =  mev               #line 58
            inst.state =  "waitingForFirstmev"         #line 59
        else:                                          #line 60
            runtime_error ( str( "bad mev.port (case A) for deracer ") +  mev.port )#line 61#line 62
    elif  inst.state ==  "waitingForFirstmev":         #line 63
        if  "1" ==  mev.port:                          #line 64
            inst.buffer.firstmev =  mev                #line 65
            send_firstmev_then_secondmev ( eh, inst)   #line 66
            inst.state =  "idle"                       #line 67
        else:                                          #line 68
            runtime_error ( str( "deracer: waiting for 1 but got [") +  str( mev.port) +  "] (case B)"  )#line 69#line 70
    elif  inst.state ==  "waitingForSecondmev":        #line 71
        if  "2" ==  mev.port:                          #line 72
            inst.buffer.secondmev =  mev               #line 73
            send_firstmev_then_secondmev ( eh, inst)   #line 74
            inst.state =  "idle"                       #line 75
        else:                                          #line 76
            runtime_error ( str( "deracer: waiting for 2 but got [") +  str( mev.port) +  "] (case C)"  )#line 77#line 78
    else:                                              #line 79
        runtime_error ( "bad state for deracer {eh.state}")#line 80#line 81#line 82#line 83

def low_level_read_text_file_instantiate (reg,owner,name,template_data,arg):#line 84
    name_with_id = gensymbol ( "Low Level Read Text File")#line 85
    return make_leaf ( name_with_id, owner, None, "", low_level_read_text_file_handler, None)#line 86#line 87#line 88

def low_level_read_text_file_handler (eh,mev):         #line 89
    fname =  mev.payload.v                             #line 90

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
                                                       #line 91#line 92#line 93

def ensure_string_datum_instantiate (reg,owner,name,template_data,arg):#line 94
    name_with_id = gensymbol ( "Ensure String Datum")  #line 95
    return make_leaf ( name_with_id, owner, None, "", ensure_string_datum_handler, None)#line 96#line 97#line 98

def ensure_string_datum_handler (eh,mev):              #line 99
    if  "string" ==  mev.payload.kind ():              #line 100
        forward ( eh, "", mev)                         #line 101
    else:                                              #line 102
        emev =  str( "*** ensure: type error (expected a string payload) but got ") +  mev.payload #line 103
        send ( eh, "✗", emev, mev)                     #line 104#line 105#line 106#line 107

class Syncfilewrite_Data:
    def __init__ (self):                               #line 108
        self.filename =  ""                            #line 109#line 110
                                                       #line 111
def syncfilewrite_reset_handler (eh):                  #line 112
    eh.instance_data =  Syncfilewrite_Data ()          #line 113#line 114#line 115

# temp copy for bootstrap, sends "done“ (error during bootstrap if not wired)#line 116
def syncfilewrite_instantiate (reg,owner,name,template_data,arg):#line 117
    name_with_id = gensymbol ( "syncfilewrite")        #line 118
    inst =  Syncfilewrite_Data ()                      #line 119
    return make_leaf ( name_with_id, owner, inst, "", syncfilewrite_handler, syncfilewrite_reset_handler)#line 120#line 121#line 122

def syncfilewrite_handler (eh,mev):                    #line 123
    inst =  eh.instance_data                           #line 124
    if  "filename" ==  mev.port:                       #line 125
        inst.filename =  mev.payload.v                 #line 126
    elif  "input" ==  mev.port:                        #line 127
        contents =  mev.payload.v                      #line 128
        f = open ( inst.filename, "w")                 #line 129
        if  f!= None:                                  #line 130
            f.write ( mev.payload.v)                   #line 131
            f.close ()                                 #line 132
            send ( eh, "done",new_datum_bang (), mev)  #line 133
        else:                                          #line 134
            send ( eh, "✗", str( "open error on file ") +  inst.filename , mev)#line 135#line 136#line 137#line 138#line 139

class StringConcat_Instance_Data:
    def __init__ (self):                               #line 140
        self.buffer1 =  None                           #line 141
        self.buffer2 =  None                           #line 142#line 143
                                                       #line 144
def stringconcat_reset_handler (eh):                   #line 145
    inst =  eh.instance_data                           #line 146
    inst.buffer1 =  None                               #line 147
    inst.buffer2 =  None                               #line 148#line 149#line 150

def stringconcat_instantiate (reg,owner,name,template_data,arg):#line 151
    name_with_id = gensymbol ( "stringconcat")         #line 152
    instp =  StringConcat_Instance_Data ()             #line 153
    return make_leaf ( name_with_id, owner, instp, "", stringconcat_handler, stringconcat_reset_handler)#line 154#line 155#line 156

def stringconcat_handler (eh,mev):                     #line 157
    inst =  eh.instance_data                           #line 158
    if  "1" ==  mev.port:                              #line 159
        inst.buffer1 = clone_string ( mev.payload.v)   #line 160
        maybe_stringconcat ( eh, inst, mev)            #line 161
    elif  "2" ==  mev.port:                            #line 162
        inst.buffer2 = clone_string ( mev.payload.v)   #line 163
        maybe_stringconcat ( eh, inst, mev)            #line 164
    elif  "reset" ==  mev.port:                        #line 165
        inst.buffer1 =  None                           #line 166
        inst.buffer2 =  None                           #line 167
    else:                                              #line 168
        runtime_error ( str( "bad mev.port for stringconcat: ") +  mev.port )#line 169#line 170#line 171#line 172

def maybe_stringconcat (eh,inst,mev):                  #line 173
    if  inst.buffer1!= None and  inst.buffer2!= None:  #line 174
        concatenated_string =  ""                      #line 175
        if  0 == len ( inst.buffer1):                  #line 176
            concatenated_string =  inst.buffer2        #line 177
        elif  0 == len ( inst.buffer2):                #line 178
            concatenated_string =  inst.buffer1        #line 179
        else:                                          #line 180
            concatenated_string =  inst.buffer1+ inst.buffer2#line 181#line 182
        send ( eh, "", concatenated_string, mev)       #line 183
        inst.buffer1 =  None                           #line 184
        inst.buffer2 =  None                           #line 185#line 186#line 187#line 188

#                                                      #line 189#line 190
projectRoot =  "."                                     #line 191#line 192
def string_constant_instantiate (reg,owner,name,template_data,arg):#line 193
    global projectRoot                                 #line 194
    name_with_id = gensymbol ( "strconst")             #line 195
    s =  template_data                                 #line 196
    if  projectRoot!= "":                              #line 197
        s = re.sub ( "_00_",  projectRoot,  s)         #line 198#line 199
    return make_leaf ( name_with_id, owner, s, "", string_constant_handler, None)#line 200#line 201#line 202

def string_constant_handler (eh,mev):                  #line 203
    s =  eh.instance_data                              #line 204
    send ( eh, "", s, mev)                             #line 205#line 206#line 207

def fakepipename_instantiate (reg,owner,name,template_data,arg):#line 208
    instance_name = gensymbol ( "fakepipe")            #line 209
    return make_leaf ( instance_name, owner, None, "", fakepipename_handler, None)#line 210#line 211#line 212

rand =  0                                              #line 213#line 214
def fakepipename_handler (eh,mev):                     #line 215
    global rand                                        #line 216
    rand =  rand+ 1
    # not very random, but good enough _ ;rand' must be unique within a single run#line 217
    send ( eh, "", str( "/tmp/fakepipe") +  rand , mev)#line 218#line 219#line 220
                                                       #line 221
class Switch1star_Instance_Data:
    def __init__ (self):                               #line 222
        self.state =  "1"                              #line 223#line 224
                                                       #line 225
def switch1star_reset_handler (eh):                    #line 226
    inst =  eh.instance_data                           #line 227
    inst =  Switch1star_Instance_Data ()               #line 228#line 229#line 230

def switch1star_instantiate (reg,owner,name,template_data,arg):#line 231
    name_with_id = gensymbol ( "switch1*")             #line 232
    instp =  Switch1star_Instance_Data ()              #line 233
    return make_leaf ( name_with_id, owner, instp, "", switch1star_handler, switch1star_reset_handler)#line 234#line 235#line 236

def switch1star_handler (eh,mev):                      #line 237
    inst =  eh.instance_data                           #line 238
    whichOutput =  inst.state                          #line 239
    if  "" ==  mev.port:                               #line 240
        if  "1" ==  whichOutput:                       #line 241
            forward ( eh, "1", mev)                    #line 242
            inst.state =  "*"                          #line 243
        elif  "*" ==  whichOutput:                     #line 244
            forward ( eh, "*", mev)                    #line 245
        else:                                          #line 246
            send ( eh, "✗", "internal error bad state in switch1*", mev)#line 247#line 248
    elif  "reset" ==  mev.port:                        #line 249
        inst.state =  "1"                              #line 250
    else:                                              #line 251
        send ( eh, "✗", "internal error bad mevent for switch1*", mev)#line 252#line 253#line 254#line 255

class StringAccumulator:
    def __init__ (self):                               #line 256
        self.s =  ""                                   #line 257#line 258
                                                       #line 259
def strcatstar_reset_handler (eh):                     #line 260
    eh.instance_data =  StringAccumulator ()           #line 261#line 262#line 263

def strcatstar_instantiate (reg,owner,name,template_data,arg):#line 264
    name_with_id = gensymbol ( "String Concat *")      #line 265
    instp =  StringAccumulator ()                      #line 266
    return make_leaf ( name_with_id, owner, instp, "", strcatstar_handler, strcatstar_reset_handler)#line 267#line 268#line 269

def strcatstar_handler (eh,mev):                       #line 270
    accum =  eh.instance_data                          #line 271
    if  "" ==  mev.port:                               #line 272
        accum.s =  str( accum.s) +  mev.payload.v      #line 273
    elif  "fini" ==  mev.port:                         #line 274
        send ( eh, "", accum.s, mev)                   #line 275
    else:                                              #line 276
        send ( eh, "✗", "internal error bad mevent for String Concat *", mev)#line 277#line 278#line 279#line 280

def stop_instantiate (reg,owner,name,template_data,arg):#line 281
    name_with_id = gensymbol ( "Stop")                 #line 282
    inst =  None                                       #line 283
    return make_leaf ( name_with_id, owner, inst, "", stop_handler, None)#line 284#line 285#line 286

def stop_handler (eh,mev):                             #line 287
    inst =  eh.instance_data                           #line 288
    parent =  eh.owner                                 #line 289
    s =  str( "   !!! stopping: '") +  str( parent.name) +  "'"  #line 290
    print ( s, file=sys.stderr)                        #line 291
                                                       #line 292
    parent.reset ( parent)                             #line 293
    send ( eh, "", mev.payload.v, mev)                 #line 294#line 295#line 296

# all of the the built_in leaves are listed here       #line 297
# future: refactor this such that programmers can pick and choose which (lumps of) builtins are used in a specific project#line 298#line 299
def initialize_stock_components (reg):                 #line 300
    register_component ( reg,mkTemplate ( "1then2", None, deracer_instantiate))#line 301
    register_component ( reg,mkTemplate ( "1→2", None, deracer_instantiate))#line 302
    register_component ( reg,mkTemplate ( "trash", None, trash_instantiate))#line 303
    register_component ( reg,mkTemplate ( "🗑️", None, trash_instantiate))#line 304
    register_component ( reg,mkTemplate ( "🚫", None, stop_instantiate))#line 305#line 306#line 307
    register_component ( reg,mkTemplate ( "Read Text File", None, low_level_read_text_file_instantiate))#line 308
    register_component ( reg,mkTemplate ( "Ensure String Datum", None, ensure_string_datum_instantiate))#line 309#line 310
    register_component ( reg,mkTemplate ( "syncfilewrite", None, syncfilewrite_instantiate))#line 311
    register_component ( reg,mkTemplate ( "String Concat", None, stringconcat_instantiate))#line 312
    register_component ( reg,mkTemplate ( "switch1*", None, switch1star_instantiate))#line 313
    register_component ( reg,mkTemplate ( "String Concat *", None, strcatstar_instantiate))#line 314
    # for fakepipe                                     #line 315
    register_component ( reg,mkTemplate ( "fakepipename", None, fakepipename_instantiate))#line 316#line 317#line 318
load_errors =  False                                   #line 1
runtime_errors =  False                                #line 2
ticktime =  0                                          #line 3#line 4
def load_error (s):                                    #line 5
    global load_errors                                 #line 6
    print ( s, file=sys.stderr)                        #line 7
                                                       #line 8
    load_errors =  True                                #line 9#line 10#line 11

def runtime_error (s):                                 #line 12
    global runtime_errors                              #line 13
    print ( s, file=sys.stderr)                        #line 14
    exit (1)                                           #line 15
    runtime_errors =  True                             #line 16#line 17#line 18
                                                       #line 19
def initialize_component_palette_from_files (diagram_source_files):#line 20
    reg = make_component_registry ()                   #line 21
    for diagram_source in  diagram_source_files:       #line 22
        all_containers_within_single_file = lnet2internal_from_file ( diagram_source)#line 23
        for container in  all_containers_within_single_file:#line 24
            register_component ( reg,mkTemplate ( container ["name"], container, container_instantiator))#line 25#line 26#line 27
    initialize_stock_components ( reg)                 #line 28
    return  reg                                        #line 29#line 30#line 31

def initialize_component_palette_from_string (lnet):   #line 32
    reg = make_component_registry ()                   #line 33
    all_containers = lnet2internal_from_string ( lnet) #line 34
    for container in  all_containers:                  #line 35
        register_component ( reg,mkTemplate ( container ["name"], container, container_instantiator))#line 36#line 37
    initialize_stock_components ( reg)                 #line 38
    return  reg                                        #line 39#line 40

def initialize_from_files (diagram_names):             #line 41
    arg =  None                                        #line 42
    palette = initialize_component_palette_from_files ( diagram_names)#line 43
    return [ palette,[ diagram_names, arg]]            #line 44#line 45#line 46

def initialize_from_string ():                         #line 47
    arg =  None                                        #line 48
    palette = initialize_component_palette_from_string ()#line 49
    return [ palette,[ None, arg]]                     #line 50#line 51#line 52

def start (arg,part_name,palette,env):                 #line 53
    part = start_bare ( part_name, palette, env)       #line 54
    inject ( part, "", arg)                            #line 55
    finalize ( part)                                   #line 56#line 57#line 58

def start_bare (part_name,palette,env):                #line 59
    diagram_names =  env [ 0]                          #line 60
    # get entrypoint container                         #line 61
    part = get_component_instance ( palette, part_name, None)#line 62
    if  None ==  part:                                 #line 63
        load_error ( str( "Couldn;t find container with page name /") +  str( part_name) +  str( "/ in files ") +  str(str ( diagram_names)) +  " (check tab names, or disable compression?)"    )#line 67#line 68
    return  part                                       #line 69#line 70#line 71

def inject (part,port,payload):                        #line 72
    global load_errors                                 #line 73
    if not  load_errors:                               #line 74
        d =  Datum ()                                  #line 75
        d.v =  payload                                 #line 76
        d.clone =  lambda : obj_clone ( d)             #line 77
        d.reclaim =  None                              #line 78
        mev = make_mevent ( port, d)                   #line 79
        inject_mevent ( part, mev)                     #line 80
    else:                                              #line 81
        exit (1)                                       #line 82#line 83#line 84#line 85

def finalize (part):                                   #line 86
    print (deque_to_json ( part.outq))                 #line 87#line 88#line 89

def new_datum_bang ():                                 #line 90
    d =  Datum ()                                      #line 91
    d.v =  "!"                                         #line 92
    d.clone =  lambda : obj_clone ( d)                 #line 93
    d.reclaim =  None                                  #line 94
    return  d                                          #line 95#line 96
