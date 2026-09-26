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
# TODO: what is .routings for? (is it a historical artefact that can be removed?) #line 15#line 16
# Eh_States :: enum { idle, active }                   #line 17
class Eh:
    def __init__ (self,):                              #line 18
        self.name =  ""                                #line 19
        self.inq =  deque ([])                         #line 20
        self.outq =  deque ([])                        #line 21
        self.owner =  None                             #line 22
        self.children = []                             #line 23
        self.visit_ordering =  deque ([])              #line 24
        self.connections = []                          #line 25
        self.handler =  None                           #line 26
        self.reset_instance_data =  None               #line 27
        self.finject =  None                           #line 28
        self.stop =  None                              #line 29
        self.instance_data =  None                     #line 30# arg needed for probe support #line 31
        self.arg =  ""                                 #line 32
        self.state =  "idle"                           #line 33
        self.special =  False                          #line 34# bootstrap debugging#line 35
        self.kind =  None # enum { container, leaf, }  #line 36#line 37
                                                       #line 38
def injector (eh,mevent):                              #line 39
    eh.handler ( eh, mevent)                           #line 40#line 41#line 42
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
    return  name_with_id                               #line 25#line 26#line 27
#line 1
class Datum:
    def __init__ (self,):                              #line 2
        self.v =  None                                 #line 3
        self.clone =  None                             #line 4
        self.reclaim =  None                           #line 5
        self.other =  None # reserved for use on per-project basis #line 6#line 7
                                                       #line 8#line 9
# Mevent passed to a leaf component.                   #line 10
#                                                      #line 11
# `port` refers to the name of the incoming or outgoing port of this component.#line 12
# `payload` is the data attached to this mevent.       #line 13
class Mevent:
    def __init__ (self,):                              #line 14
        self.port =  None                              #line 15
        self.payload =  None                           #line 16#line 17
                                                       #line 18
def clone_port (s):                                    #line 19
    return clone_string ( s)                           #line 20#line 21#line 22

# Utility for making a `Mevent`. Used to safely "seed“ mevents#line 23
# entering the very top of a network.                  #line 24
def make_mevent (port,datum):                          #line 25
    p = clone_string ( port)                           #line 26
    m =  Mevent ()                                     #line 27
    m.port =  p                                        #line 28
    m.payload =  datum.clone ()                        #line 29
    return  m                                          #line 30#line 31#line 32

# Clones a mevent. Primarily used internally for “fanning out“ a mevent to multiple destinations.#line 33
def mevent_clone (mev):                                #line 34
    m =  Mevent ()                                     #line 35
    m.port = clone_port ( mev.port)                    #line 36
    m.payload =  mev.payload.clone ()                  #line 37
    return  m                                          #line 38#line 39#line 40

# Frees a mevent.                                      #line 41
def destroy_mevent (mev):                              #line 42
    # during debug, dont destroy any mevent, since we want to trace mevents, thus, we need to persist ancestor mevents#line 43
    pass                                               #line 44#line 45#line 46

def destroy_datum (mev):                               #line 47
    pass                                               #line 48#line 49#line 50

def destroy_port (mev):                                #line 51
    pass                                               #line 52#line 53#line 54

#                                                      #line 55
def format_mevent (m):                                 #line 56
    if  m ==  None:                                    #line 57
        return  "{}"                                   #line 58
    else:                                              #line 59
        return  str( "{%5C”") +  str( m.port) +  str( "%5C”:%5C”") +  str( m.payload.v) +  "%5C”}"    #line 60#line 61#line 62

def format_mevent_raw (m):                             #line 63
    if  m ==  None:                                    #line 64
        return  ""                                     #line 65
    else:                                              #line 66
        return  m.payload.v                            #line 67#line 68#line 69
#line 1
enumDown =  0                                          #line 2
enumAcross =  1                                        #line 3
enumUp =  2                                            #line 4
enumThrough =  3                                       #line 5#line 6#line 7
# Routing connection for a container component. The `direction` field has#line 8
# no affect on the default mevent routing system _ it is there for debugging#line 9
# purposes, or for reading by other tools.             #line 10#line 11
class Connector:
    def __init__ (self,):                              #line 12
        self.direction =  None # down, across, up, through#line 13
        self.sender =  None                            #line 14
        self.receiver =  None                          #line 15#line 16
                                                       #line 17
# `Sender` is used to "pattern match“ which `Receiver` a mevent should go to,#line 18
# based on component ID (pointer) and port name.       #line 19#line 20
class Sender:
    def __init__ (self,):                              #line 21
        self.name =  None                              #line 22
        self.component =  None                         #line 23
        self.port =  None                              #line 24#line 25
                                                       #line 26#line 27#line 28
# `Receiver` is a handle to a destination queue, and a `port` name to assign#line 29
# to incoming mevents to this queue.                   #line 30#line 31
class Receiver:
    def __init__ (self,):                              #line 32
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
    def __init__ (self,):                              #line 1
        self.templates = {}                            #line 2#line 3
                                                       #line 4
class Template:
    def __init__ (self,):                              #line 5
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

def delete_decls (d):                                  #line 36
    pass                                               #line 37#line 38#line 39

def make_component_registry ():                        #line 40
    return  Component_Registry ()                      #line 41#line 42#line 43

def register_component (reg,template):
    return abstracted_register_component ( reg, template, False)#line 44

def register_component_allow_overwriting (reg,template):
    return abstracted_register_component ( reg, template, True)#line 45#line 46

def abstracted_register_component (reg,template,ok_to_overwrite):#line 47
    name = mangle_name ( template.name)                #line 48
    if  reg!= None and  name in  reg.templates and not  ok_to_overwrite:#line 49
        load_error ( str( "Component /") +  str( template.name) +  "/ already declared"  )#line 50
        return  reg                                    #line 51
    else:                                              #line 52
        reg.templates [name] =  template               #line 53
        return  reg                                    #line 54#line 55#line 56#line 57

def get_component_instance (reg,full_name,owner):      #line 58
    # If a part name begins with ":", it is treated as a JIT part and we let the runtime factory generate it on-the-fly (see kernel_external.rt and external.rt) else it is assumed to be a regular AOT part and assumed to have been registered before runtime, so we just pull its template out of the registry and instantiate it. #line 59
    # ":?<string>" is a probe part that is tagged with <string> #line 60
    # ":$ <command>" is a shell-out part that sends <command> to the operating system shell #line 61
    # ":<string>" else, it's just treated as a string part that produces <string> on its output #line 62
    template_name = mangle_name ( full_name)           #line 63
    if  ":" ==   full_name[0] :                        #line 64
        instance_name = generate_instance_name ( owner, template_name)#line 65
        instance = jit_instantiate ( reg, owner, instance_name, full_name)#line 66
        return  instance                               #line 67
    else:                                              #line 68
        if  template_name in  reg.templates:           #line 69
            template =  reg.templates [template_name]  #line 70
            if ( template ==  None):                   #line 71
                load_error ( str( "Registry Error (A): Can't find component /") +  str( template_name) +  "/"  )#line 72
                return  None                           #line 73
            else:                                      #line 74
                instance_name = generate_instance_name ( owner, template_name)#line 75
                instance =  template.instantiator ( reg, owner, instance_name, template.template_data, "")#line 76
                return  instance                       #line 77#line 78
        else:                                          #line 79
            load_error ( str( "Registry Error (B): Can't find component /") +  str( template_name) +  "/"  )#line 80
            return  None                               #line 81#line 82#line 83#line 84#line 85

def generate_instance_name (owner,template_name):      #line 86
    owner_name =  ""                                   #line 87
    instance_name =  template_name                     #line 88
    if  None!= owner:                                  #line 89
        owner_name =  owner.name                       #line 90
        instance_name =  str( owner_name) +  str( "▹") +  template_name  #line 91
    else:                                              #line 92
        instance_name =  template_name                 #line 93#line 94
    return  instance_name                              #line 95#line 96#line 97

def mangle_name (s):                                   #line 98
    # trim name to remove code from Container component names _ deferred until later (or never)#line 99
    return  s                                          #line 100#line 101
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
    eh.stop =  leaf_reset                              #line 15
    eh.instance_data =  instance_data                  #line 16
    eh.arg =  arg                                      #line 17
    eh.state =  "idle"                                 #line 18
    eh.kind =  "leaf"                                  #line 19
    return  eh                                         #line 20#line 21#line 22

# Reset Leaf part to a known, idle state. Hit the big red button. #line 23
def leaf_reset (part):                                 #line 24

    part.inq.clear ()                                  #line 25

    part.outq.clear ()                                 #line 26
    if ( part.reset_handler!= None):                   #line 27
        part.reset_handler ( part)                     #line 28#line 29
    part.state =  "idle"                               #line 30#line 31
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
    s =  mev.payload.v                                 #line 28
    live_update ( "Info",  str( "  @") +  str(str ( ticktime)) +  str( "  ") +  str( "probe ") +  str( eh.name) +  str( ": ") + str ( s)      )#line 36#line 37#line 38

def shell_out_handler (eh,cmd,mev):                    #line 39
    s =  mev.payload.v                                 #line 40
    ret =  None                                        #line 41
    rc =  None                                         #line 42
    stdout =  None                                     #line 43
    stderr =  None                                     #line 44
    command =  cmd                                     #line 45
    pbpRoot = os.getenv('PBP', '<none>')               #line 46
    if  pbpRoot!= "":                                  #line 47
        command = re.sub ( "_/",  str( pbpRoot) +  "/" ,  command)#line 50#line 51
    if ( ("PBPSHELLUT" in os.environ) ):               #line 52
        print ( str( "- --- shell-out: ") +  command , file=sys.stderr)#line 53
                                                       #line 54#line 55

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
                                                       #line 56
    if  rc ==  0:                                      #line 57
        send ( eh, "", str( stdout) +  stderr , mev)   #line 58
    else:                                              #line 59
        send ( eh, "✗", str( stdout) +  stderr , mev)  #line 60#line 61#line 62#line 63
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
    def __init__ (self,):                              #line 15
        self.firstmev =  None                          #line 16
        self.secondmev =  None                         #line 17#line 18
                                                       #line 19
# Deracer_States :: enum { idle, waitingForFirstmev, waitingForSecondmev }#line 20
class Deracer_Instance_Data:
    def __init__ (self,):                              #line 21
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
    def __init__ (self,):                              #line 108
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
    def __init__ (self,):                              #line 140
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
def string_constant_instantiate (reg,owner,name,template_data,arg):#line 191
    global projectRoot                                 #line 192
    name_with_id = gensymbol ( "strconst")             #line 193
    s =  template_data                                 #line 194
    if  projectRoot!= "":                              #line 195
        s = re.sub ( "_00_",  projectRoot,  s)         #line 196#line 197
    return make_leaf ( name_with_id, owner, s, "", string_constant_handler, None)#line 198#line 199#line 200

def string_constant_handler (eh,mev):                  #line 201
    s =  eh.instance_data                              #line 202
    send ( eh, "", s, mev)                             #line 203#line 204#line 205

def fakepipename_instantiate (reg,owner,name,template_data,arg):#line 206
    instance_name = gensymbol ( "fakepipe")            #line 207
    return make_leaf ( instance_name, owner, None, "", fakepipename_handler, None)#line 208#line 209#line 210

rand =  0                                              #line 211#line 212
def fakepipename_handler (eh,mev):                     #line 213
    global rand                                        #line 214
    rand =  rand+ 1
    # not very random, but good enough _ ;rand' must be unique within a single run#line 215
    send ( eh, "", str( "/tmp/fakepipe") +  rand , mev)#line 216#line 217#line 218
                                                       #line 219
class Switch1star_Instance_Data:
    def __init__ (self,):                              #line 220
        self.state =  "1"                              #line 221#line 222
                                                       #line 223
def switch1star_reset_handler (eh):                    #line 224
    inst =  eh.instance_data                           #line 225
    inst =  Switch1star_Instance_Data ()               #line 226#line 227#line 228

def switch1star_instantiate (reg,owner,name,template_data,arg):#line 229
    name_with_id = gensymbol ( "switch1*")             #line 230
    instp =  Switch1star_Instance_Data ()              #line 231
    return make_leaf ( name_with_id, owner, instp, "", switch1star_handler, switch1star_reset_handler)#line 232#line 233#line 234

def switch1star_handler (eh,mev):                      #line 235
    inst =  eh.instance_data                           #line 236
    whichOutput =  inst.state                          #line 237
    if  "" ==  mev.port:                               #line 238
        if  "1" ==  whichOutput:                       #line 239
            forward ( eh, "1", mev)                    #line 240
            inst.state =  "*"                          #line 241
        elif  "*" ==  whichOutput:                     #line 242
            forward ( eh, "*", mev)                    #line 243
        else:                                          #line 244
            send ( eh, "✗", "internal error bad state in switch1*", mev)#line 245#line 246
    elif  "reset" ==  mev.port:                        #line 247
        inst.state =  "1"                              #line 248
    else:                                              #line 249
        send ( eh, "✗", "internal error bad mevent for switch1*", mev)#line 250#line 251#line 252#line 253

class StringAccumulator:
    def __init__ (self,):                              #line 254
        self.s =  ""                                   #line 255#line 256
                                                       #line 257
def strcatstar_reset_handler (eh):                     #line 258
    eh.instance_data =  StringAccumulator ()           #line 259#line 260#line 261

def strcatstar_instantiate (reg,owner,name,template_data,arg):#line 262
    name_with_id = gensymbol ( "String Concat *")      #line 263
    instp =  StringAccumulator ()                      #line 264
    return make_leaf ( name_with_id, owner, instp, "", strcatstar_handler, strcatstar_reset_handler)#line 265#line 266#line 267

def strcatstar_handler (eh,mev):                       #line 268
    accum =  eh.instance_data                          #line 269
    if  "" ==  mev.port:                               #line 270
        accum.s =  str( accum.s) +  mev.payload.v      #line 271
    elif  "fini" ==  mev.port:                         #line 272
        send ( eh, "", accum.s, mev)                   #line 273
    else:                                              #line 274
        send ( eh, "✗", "internal error bad mevent for String Concat *", mev)#line 275#line 276#line 277#line 278

def stop_instantiate (reg,owner,name,template_data,arg):#line 279
    name_with_id = gensymbol ( "Stop")                 #line 280
    inst =  None                                       #line 281
    return make_leaf ( name_with_id, owner, inst, "", stop_handler, None)#line 282#line 283#line 284

def stop_handler (eh,mev):                             #line 285
    inst =  eh.instance_data                           #line 286
    parent =  eh.owner                                 #line 287
    s =  str( "   !!! stopping: '") +  str( parent.name) +  "'"  #line 288
    print ( s, file=sys.stderr)                        #line 289
                                                       #line 290
    parent.stop ( parent)                              #line 291
    send ( eh, "", mev.payload.v, mev)                 #line 292#line 293#line 294

# all of the the built_in leaves are listed here       #line 295
# future: refactor this such that programmers can pick and choose which (lumps of) builtins are used in a specific project#line 296#line 297
def initialize_stock_components (reg):                 #line 298
    register_component ( reg,mkTemplate ( "1then2", None, deracer_instantiate))#line 299
    register_component ( reg,mkTemplate ( "1→2", None, deracer_instantiate))#line 300
    register_component ( reg,mkTemplate ( "trash", None, trash_instantiate))#line 301
    register_component ( reg,mkTemplate ( "🗑️", None, trash_instantiate))#line 302
    register_component ( reg,mkTemplate ( "🚫", None, stop_instantiate))#line 303#line 304#line 305
    register_component ( reg,mkTemplate ( "Read Text File", None, low_level_read_text_file_instantiate))#line 306
    register_component ( reg,mkTemplate ( "Ensure String Datum", None, ensure_string_datum_instantiate))#line 307#line 308
    register_component ( reg,mkTemplate ( "syncfilewrite", None, syncfilewrite_instantiate))#line 309
    register_component ( reg,mkTemplate ( "String Concat", None, stringconcat_instantiate))#line 310
    register_component ( reg,mkTemplate ( "switch1*", None, switch1star_instantiate))#line 311
    register_component ( reg,mkTemplate ( "String Concat *", None, strcatstar_instantiate))#line 312
    # for fakepipe                                     #line 313
    register_component ( reg,mkTemplate ( "fakepipename", None, fakepipename_instantiate))#line 314#line 315#line 316
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
            register_component ( reg,mkTemplate ( container [ "name"], container, container_instantiator))#line 25#line 26#line 27
    initialize_stock_components ( reg)                 #line 28
    return  reg                                        #line 29#line 30#line 31

def initialize_component_palette_from_string (lnet):   #line 32
    reg = make_component_registry ()                   #line 33
    all_containers = lnet2internal_from_string ( lnet) #line 34
    for container in  all_containers:                  #line 35
        register_component ( reg,mkTemplate ( container [ "name"], container, container_instantiator))#line 36#line 37
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
    if not  load_errors:                               #line 73
        d =  Datum ()                                  #line 74
        d.v =  payload                                 #line 75
        d.clone =  lambda : obj_clone ( d)             #line 76
        d.reclaim =  None                              #line 77
        mev = make_mevent ( port, d)                   #line 78
        inject_mevent ( part, mev)                     #line 79
    else:                                              #line 80
        exit (1)                                       #line 81#line 82#line 83#line 84

def finalize (part):                                   #line 85
    print (deque_to_json ( part.outq))                 #line 86#line 87#line 88

def new_datum_bang ():                                 #line 89
    d =  Datum ()                                      #line 90
    d.v =  "!"                                         #line 91
    d.clone =  lambda : obj_clone ( d)                 #line 92
    d.reclaim =  None                                  #line 93
    return  d                                          #line 94#line 95
