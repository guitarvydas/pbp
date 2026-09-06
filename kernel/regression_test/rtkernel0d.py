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


                                                       #line 1#line 2
counter =  0                                           #line 3
ticktime =  0                                          #line 4#line 5
digits = [ "₀", "₁", "₂", "₃", "₄", "₅", "₆", "₇", "₈", "₉", "₁₀", "₁₁", "₁₂", "₁₃", "₁₄", "₁₅", "₁₆", "₁₇", "₁₈", "₁₉", "₂₀", "₂₁", "₂₂", "₂₃", "₂₄", "₂₅", "₂₆", "₂₇", "₂₈", "₂₉"]#line 12#line 13#line 14
def gensymbol (s):                                     #line 15
    global counter                                     #line 16
    name_with_id =  str( s) + subscripted_digit ( counter) #line 17
    counter =  counter+ 1                              #line 18
    return  name_with_id                               #line 19#line 20#line 21

def subscripted_digit (n):                             #line 22
    global digits                                      #line 23
    if ( n >=  0 and  n <=  29):                       #line 24
        return  digits [ n]                            #line 25
    else:                                              #line 26
        return  str( "₊") + str ( n)                   #line 27#line 28#line 29#line 30

class Datum:
    def __init__ (self,):                              #line 31
        self.v =  None                                 #line 32
        self.clone =  None                             #line 33
        self.reclaim =  None                           #line 34
        self.other =  None # reserved for use on per-project basis #line 35#line 36
                                                       #line 37#line 38
# Mevent passed to a leaf component.                   #line 39
#                                                      #line 40
# `port` refers to the name of the incoming or outgoing port of this component.#line 41
# `payload` is the data attached to this mevent.       #line 42
class Mevent:
    def __init__ (self,):                              #line 43
        self.port =  None                              #line 44
        self.payload =  None                           #line 45#line 46
                                                       #line 47
def clone_port (s):                                    #line 48
    return clone_string ( s)                           #line 49#line 50#line 51

# Utility for making a `Mevent`. Used to safely "seed“ mevents#line 52
# entering the very top of a network.                  #line 53
def make_mevent (port,datum):                          #line 54
    p = clone_string ( port)                           #line 55
    m =  Mevent ()                                     #line 56
    m.port =  p                                        #line 57
    m.payload =  datum.clone ()                        #line 58
    return  m                                          #line 59#line 60#line 61

# Clones a mevent. Primarily used internally for “fanning out“ a mevent to multiple destinations.#line 62
def mevent_clone (mev):                                #line 63
    m =  Mevent ()                                     #line 64
    m.port = clone_port ( mev.port)                    #line 65
    m.payload =  mev.payload.clone ()                  #line 66
    return  m                                          #line 67#line 68#line 69

# Frees a mevent.                                      #line 70
def destroy_mevent (mev):                              #line 71
    # during debug, dont destroy any mevent, since we want to trace mevents, thus, we need to persist ancestor mevents#line 72
    pass                                               #line 73#line 74#line 75

def destroy_datum (mev):                               #line 76
    pass                                               #line 77#line 78#line 79

def destroy_port (mev):                                #line 80
    pass                                               #line 81#line 82#line 83

#                                                      #line 84
def format_mevent (m):                                 #line 85
    if  m ==  None:                                    #line 86
        return  "{}"                                   #line 87
    else:                                              #line 88
        return  str( "{%5C”") +  str( m.port) +  str( "%5C”:%5C”") +  str( m.payload.v) +  "%5C”}"    #line 89#line 90#line 91

def format_mevent_raw (m):                             #line 92
    if  m ==  None:                                    #line 93
        return  ""                                     #line 94
    else:                                              #line 95
        return  m.payload.v                            #line 96#line 97#line 98#line 99

enumDown =  0                                          #line 100
enumAcross =  1                                        #line 101
enumUp =  2                                            #line 102
enumThrough =  3                                       #line 103#line 104#line 105
class Component_Registry:
    def __init__ (self,):                              #line 106
        self.templates = {}                            #line 107#line 108
                                                       #line 109
class Template:
    def __init__ (self,):                              #line 110
        self.name =  None                              #line 111
        self.container =  None                         #line 112
        self.instantiator =  None                      #line 113#line 114
                                                       #line 115
# Routing connection for a container component. The `direction` field has#line 116
# no affect on the default mevent routing system _ it is there for debugging#line 117
# purposes, or for reading by other tools.             #line 118#line 119
class Connector:
    def __init__ (self,):                              #line 120
        self.direction =  None # down, across, up, through#line 121
        self.sender =  None                            #line 122
        self.receiver =  None                          #line 123#line 124
                                                       #line 125
# `Sender` is used to “pattern match“ which `Receiver` a mevent should go to,#line 126
# based on component ID (pointer) and port name.       #line 127#line 128
class Sender:
    def __init__ (self,):                              #line 129
        self.name =  None                              #line 130
        self.component =  None                         #line 131
        self.port =  None                              #line 132#line 133
                                                       #line 134#line 135#line 136
# `Receiver` is a handle to a destination queue, and a `port` name to assign#line 137
# to incoming mevents to this queue.                   #line 138#line 139
class Receiver:
    def __init__ (self,):                              #line 140
        self.name =  None                              #line 141
        self.queue =  None                             #line 142
        self.port =  None                              #line 143
        self.component =  None                         #line 144#line 145
                                                       #line 146
def mkSender (name,component,port):                    #line 147
    s =  Sender ()                                     #line 148
    s.name =  name                                     #line 149
    s.component =  component                           #line 150
    s.port =  port                                     #line 151
    return  s                                          #line 152#line 153#line 154

def mkReceiver (name,component,port,q):                #line 155
    r =  Receiver ()                                   #line 156
    r.name =  name                                     #line 157
    r.component =  component                           #line 158
    r.port =  port                                     #line 159
    # We need a way to determine which queue to target. "Down" and "Across" go to inq, "Up" and "Through" go to outq.#line 160
    r.queue =  q                                       #line 161
    return  r                                          #line 162#line 163#line 164
                                                       #line 165
class Component_Registry:
    def __init__ (self,):                              #line 166
        self.templates = {}                            #line 167#line 168
                                                       #line 169
class Template:
    def __init__ (self,):                              #line 170
        self.name =  None                              #line 171
        self.container =  None                         #line 172
        self.instantiator =  None                      #line 173#line 174
                                                       #line 175
def mkTemplate (name,template_data,instantiator):      #line 176
    templ =  Template ()                               #line 177
    templ.name =  name                                 #line 178
    templ.template_data =  template_data               #line 179
    templ.instantiator =  instantiator                 #line 180
    return  templ                                      #line 181#line 182#line 183

def make_component_registry ():                        #line 184
    return  Component_Registry ()                      #line 185#line 186#line 187

# Data for an asyncronous component _ effectively, a function with input#line 188
# and output queues of mevents.                        #line 189
#                                                      #line 190
# Components can either be a user_supplied function (“leaf“), or a “container“#line 191
# that routes mevents to child components according to a list of connections#line 192
# that serve as a mevent routing table.                #line 193
#                                                      #line 194
# Child components themselves can be leaves or other containers.#line 195
#                                                      #line 196
# `handler` invokes the code that is attached to this component.#line 197
#                                                      #line 198
# `instance_data` is a pointer to instance data that the `leaf_handler`#line 199
# function may want whenever it is invoked again.      #line 200#line 201
# TODO: what is .routings for? (is it a historical artefact that can be removed?) #line 202#line 203
# Eh_States :: enum { idle, active }                   #line 204
class Eh:
    def __init__ (self,):                              #line 205
        self.name =  ""                                #line 206
        self.inq =  deque ([])                         #line 207
        self.outq =  deque ([])                        #line 208
        self.owner =  None                             #line 209
        self.children = []                             #line 210
        self.visit_ordering =  deque ([])              #line 211
        self.connections = []                          #line 212
        self.routings =  deque ([])                    #line 213
        self.handler =  None                           #line 214
        self.reset_instance_data =  None               #line 215
        self.finject =  None                           #line 216
        self.stop =  None                              #line 217
        self.instance_data =  None                     #line 218# arg needed for probe support #line 219
        self.arg =  ""                                 #line 220
        self.state =  "idle"                           #line 221
        self.special =  False                          #line 222# bootstrap debugging#line 223
        self.kind =  None # enum { container, leaf, }  #line 224#line 225
                                                       #line 226
load_errors =  False                                   #line 227
runtime_errors =  False                                #line 228#line 229
def clone_string (s):                                  #line 230
    return  s                                          #line 231#line 232#line 233

def injector (eh,mevent):                              #line 234
    eh.handler ( eh, mevent)                           #line 235#line 236#line 237
def mkTemplate (name,template_data,instantiator):      #line 1
    templ =  Template ()                               #line 2
    templ.name =  name                                 #line 3
    templ.template_data =  template_data               #line 4
    templ.instantiator =  instantiator                 #line 5
    return  templ                                      #line 6#line 7#line 8
                                                       #line 9
# convert a little-network to internal form (an object data structure created by json parser) ... #line 10
# the actual data structure depends on the json parser library used by the target language #line 11
# the form of the data structure doesn;t matter here, as long as we use lookup operators "@" in this .rt code #line 12#line 13
# ... by reading the little-net from an external file  #line 14
def lnet2internal_from_file (container_xml):           #line 15
    pathname = os.getenv('PBPWD', '<none>')            #line 16
    filename =  os.path.basename ( container_xml)      #line 17

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
                                                       #line 18#line 19#line 20

# ... by reading the little-net from an embedded string (an aspect of creating t2t tool code) #line 21
def lnet2internal_from_string (lnet):                  #line 22

    try:
        routings = json.loads(lnet)
        return routings
    except json.JSONDecodeError as e:
        print ("Error decoding JSON from string 'lnet': '{e}'")
        return None
                                                       #line 23#line 24#line 25

def delete_decls (d):                                  #line 26
    pass                                               #line 27#line 28#line 29

def make_component_registry ():                        #line 30
    return  Component_Registry ()                      #line 31#line 32#line 33

def register_component (reg,template):
    return abstracted_register_component ( reg, template, False)#line 34

def register_component_allow_overwriting (reg,template):
    return abstracted_register_component ( reg, template, True)#line 35#line 36

def abstracted_register_component (reg,template,ok_to_overwrite):#line 37
    name = mangle_name ( template.name)                #line 38
    if  reg!= None and  name in  reg.templates and not  ok_to_overwrite:#line 39
        load_error ( str( "Component /") +  str( template.name) +  "/ already declared"  )#line 40
        return  reg                                    #line 41
    else:                                              #line 42
        reg.templates [name] =  template               #line 43
        return  reg                                    #line 44#line 45#line 46#line 47

def get_component_instance (reg,full_name,owner):      #line 48
    # If a part name begins with ":", it is treated as a JIT part and we let the runtime factory generate it on-the-fly (see kernel_external.rt and external.rt) else it is assumed to be a regular AOT part and assumed to have been registered before runtime, so we just pull its template out of the registry and instantiate it. #line 49
    # ":?<string>" is a probe part that is tagged with <string> #line 50
    # ":$ <command>" is a shell-out part that sends <command> to the operating system shell #line 51
    # ":<string>" else, it's just treated as a string part that produces <string> on its output #line 52
    template_name = mangle_name ( full_name)           #line 53
    if  ":" ==   full_name[0] :                        #line 54
        instance_name = generate_instance_name ( owner, template_name)#line 55
        instance = jit_instantiate ( reg, owner, instance_name, full_name)#line 56
        return  instance                               #line 57
    else:                                              #line 58
        if  template_name in  reg.templates:           #line 59
            template =  reg.templates [template_name]  #line 60
            if ( template ==  None):                   #line 61
                load_error ( str( "Registry Error (A): Can't find component /") +  str( template_name) +  "/"  )#line 62
                return  None                           #line 63
            else:                                      #line 64
                instance_name = generate_instance_name ( owner, template_name)#line 65
                instance =  template.instantiator ( reg, owner, instance_name, template.template_data, "")#line 66
                return  instance                       #line 67#line 68
        else:                                          #line 69
            load_error ( str( "Registry Error (B): Can't find component /") +  str( template_name) +  "/"  )#line 70
            return  None                               #line 71#line 72#line 73#line 74#line 75

def generate_instance_name (owner,template_name):      #line 76
    owner_name =  ""                                   #line 77
    instance_name =  template_name                     #line 78
    if  None!= owner:                                  #line 79
        owner_name =  owner.name                       #line 80
        instance_name =  str( owner_name) +  str( "▹") +  template_name  #line 81
    else:                                              #line 82
        instance_name =  template_name                 #line 83#line 84
    return  instance_name                              #line 85#line 86#line 87

def mangle_name (s):                                   #line 88
    # trim name to remove code from Container component names _ deferred until later (or never)#line 89
    return  s                                          #line 90#line 91
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

    container.routings.clear ()                        #line 102

    container.inq.clear ()                             #line 103

    container.outq.clear ()                            #line 104
    container.state =  "idle"                          #line 105#line 106#line 107

# Frees the given container and associated data.       #line 108
def destroy_container (eh):                            #line 109
    pass                                               #line 110#line 111

# Checks if two senders match, by pointer equality and port name matching.#line 112
def sender_eq (s1,s2):                                 #line 113
    same_components = ( s1.component ==  s2.component) #line 114
    same_ports = ( s1.port ==  s2.port)                #line 115
    return  same_components and  same_ports            #line 116#line 117#line 118

# Delivers the given mevent to the receiver of this connector.#line 119#line 120
def deposit (parent,conn,mevent):                      #line 121
    new_mevent = make_mevent ( conn.receiver.port, mevent.payload)#line 122
    push_mevent ( parent, conn.receiver.component, conn.receiver.queue, new_mevent)#line 123#line 124#line 125

def force_tick (parent,eh):                            #line 126
    tick_mev = make_mevent ( ".",new_datum_bang ())    #line 127
    push_mevent ( parent, eh, eh.inq, tick_mev)        #line 128
    return  tick_mev                                   #line 129#line 130#line 131

def push_mevent (parent,receiver,inq,m):               #line 132
    inq.append ( m)                                    #line 133
    if ( receiver.special):                            #line 134
        parent.visit_ordering.appendleft ( receiver)   #line 135
    else:                                              #line 136
        parent.visit_ordering.append ( receiver)       #line 137#line 138#line 139#line 140#line 141

def is_self (child,container):                         #line 142
    # in an earlier version “self“ was denoted as ϕ    #line 143
    return  child ==  container                        #line 144#line 145#line 146

def step_child_once (child,mev):                       #line 147
    if ( ("PBPSTEPPING" in os.environ) ):              #line 148
        print ( str( "-- stepping ❮") +  str( child.name) +  "❯"  , file=sys.stderr)#line 149
                                                       #line 150#line 151
    child.handler ( child, mev)                        #line 152#line 153#line 154

def step_children (container,causingMevent):           #line 155
    container.state =  "idle"                          #line 156#line 157
    # phase 1 - loop through children and process inputs or children that not "idle" #line 158
    for child in  list ( container.visit_ordering):    #line 159
        # child = container represents self, skip it   #line 160
        if (not (is_self ( child, container))):        #line 161
            if (not ((0==len( child.inq)))):           #line 162
                mev =  child.inq.popleft ()            #line 163
                step_child_once ( child, mev)          #line 164#line 165
                destroy_mevent ( mev)                  #line 166
            else:                                      #line 167
                if  child.state ==  "idle":            #line 168
                    pass                               #line 169
                else:                                  #line 170
                    mev = force_tick ( container, child)#line 171
                    step_child_once ( child, mev)      #line 172
                    destroy_mevent ( mev)              #line 173#line 174#line 175#line 176#line 177

    container.visit_ordering.clear ()                  #line 178#line 179
    # phase 2 - loop through children and route their outputs to appropriate receiver queues based on .connections #line 180
    for child in  container.children:                  #line 181
        if  child.state ==  "active":                  #line 182
            # if child remains active, then the container must remain active and must propagate “ticks“ to child#line 183
            container.state =  "active"                #line 184#line 185#line 186
        while (not ((0==len( child.outq)))):           #line 187
            mev =  child.outq.popleft ()               #line 188
            route ( container, child, mev)             #line 189
            destroy_mevent ( mev)                      #line 190#line 191#line 192#line 193#line 194

def attempt_tick (parent,eh):                          #line 195
    if  eh.state!= "idle":                             #line 196
        force_tick ( parent, eh)                       #line 197#line 198#line 199#line 200

def is_tick (mev):                                     #line 201
    return  "." ==  mev.port
    # assume that any mevent that is sent to port "." is a tick #line 202#line 203#line 204

# Routes a single mevent to all matching destinations, according to#line 205
# the container's connection network.                  #line 206#line 207
def route (container,from_component,mevent):           #line 208
    was_sent =  False
    # for checking that output went somewhere (at least during bootstrap)#line 209
    fromname =  ""                                     #line 210
    global ticktime                                    #line 211
    ticktime =  ticktime+ 1                            #line 212
    if is_tick ( mevent):                              #line 213
        for child in  container.children:              #line 214
            attempt_tick ( container, child)           #line 215
        was_sent =  True                               #line 216
    else:                                              #line 217
        if (not (is_self ( from_component, container))):#line 218
            fromname =  from_component.name            #line 219#line 220
        from_sender = mkSender ( fromname, from_component, mevent.port)#line 221#line 222
        for connector in  container.connections:       #line 223
            if sender_eq ( from_sender, connector.sender):#line 224
                deposit ( container, connector, mevent)#line 225
                was_sent =  True                       #line 226#line 227#line 228#line 229
    if not ( was_sent):                                #line 230
        live_update ( "internal error",  str( container.name) +  str( ": mevent on port '") +  str( mevent.port) +  str( "' from ") +  str( fromname) +  " dropped on floor..."     )#line 231#line 232#line 233#line 234

def any_child_ready (container):                       #line 235
    for child in  container.children:                  #line 236
        if child_is_ready ( child):                    #line 237
            return  True                               #line 238#line 239#line 240
    return  False                                      #line 241#line 242#line 243

def child_is_ready (eh):                               #line 244
    return (not ((0==len( eh.outq)))) or (not ((0==len( eh.inq)))) or ( eh.state!= "idle") or (any_child_ready ( eh))#line 245#line 246#line 247

def append_routing_descriptor (container,desc):        #line 248
    container.routings.append ( desc)                  #line 249#line 250#line 251
                                                       #line 252
# Creates a component that acts as a container. It is the same as a `Eh` instance#line 253
# whose handler function is `container_handler`.       #line 254
def make_container (name,owner):                       #line 255
    eh =  Eh ()                                        #line 256
    eh.name =  name                                    #line 257
    eh.owner =  owner                                  #line 258
    eh.handler =  container_handler                    #line 259
    eh.finject =  injector                             #line 260
    eh.stop =  container_reset_children                #line 261
    eh.state =  "idle"                                 #line 262
    eh.kind =  "container"                             #line 263
    return  eh                                         #line 264#line 265#line 266

# Sends a mevent on the given `port` with `data`, placing it on the output#line 267
# of the given component.                              #line 268#line 269
def send (eh,port,obj,causingMevent):                  #line 270
    d =  Datum ()                                      #line 271
    d.v =  obj                                         #line 272
    d.clone =  lambda : obj_clone ( d)                 #line 273
    d.reclaim =  None                                  #line 274
    mev = make_mevent ( port, d)                       #line 275
    put_output ( eh, mev)                              #line 276#line 277#line 278

def forward (eh,port,mev):                             #line 279
    fwdmev = make_mevent ( port, mev.payload)          #line 280
    put_output ( eh, fwdmev)                           #line 281#line 282#line 283

def inject_mevent (eh,mev):                            #line 284
    eh.finject ( eh, mev)                              #line 285#line 286#line 287

def set_active (eh):                                   #line 288
    eh.state =  "active"                               #line 289#line 290#line 291

def set_idle (eh):                                     #line 292
    eh.state =  "idle"                                 #line 293#line 294#line 295

def put_output (eh,mev):                               #line 296
    eh.outq.append ( mev)                              #line 297#line 298#line 299

def obj_clone (obj):                                   #line 300
    return  obj                                        #line 301#line 302
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
#line 1
def trash_instantiate (reg,owner,name,template_data,arg):#line 2
    name_with_id = gensymbol ( "trash")                #line 3
    return make_leaf ( name_with_id, owner, None, "", trash_handler, None)#line 4#line 5#line 6

def trash_handler (eh,mev):                            #line 7
    # to appease dumped_on_floor checker               #line 8
    pass                                               #line 9#line 10

class TwoMevents:
    def __init__ (self,):                              #line 11
        self.firstmev =  None                          #line 12
        self.secondmev =  None                         #line 13#line 14
                                                       #line 15
# Deracer_States :: enum { idle, waitingForFirstmev, waitingForSecondmev }#line 16
class Deracer_Instance_Data:
    def __init__ (self,):                              #line 17
        self.state =  None                             #line 18
        self.buffer =  None                            #line 19#line 20
                                                       #line 21
def reclaim_Buffers_from_heap (inst):                  #line 22
    pass                                               #line 23#line 24#line 25

def deracer_reset_handler (eh):                        #line 26
    inst =  eh.instance_data                           #line 27
    inst.state =  "idle"                               #line 28
    inst.buffer =  TwoMevents ()                       #line 29#line 30#line 31

def deracer_instantiate (reg,owner,name,template_data,arg):#line 32
    name_with_id = gensymbol ( "deracer")              #line 33
    inst =  Deracer_Instance_Data ()                   #line 34
    inst.state =  "idle"                               #line 35
    inst.buffer =  TwoMevents ()                       #line 36
    eh = make_leaf ( name_with_id, owner, inst, "", deracer_handler, deracer_reset_handler)#line 37
    return  eh                                         #line 38#line 39#line 40

def send_firstmev_then_secondmev (eh,inst):            #line 41
    forward ( eh, "1", inst.buffer.firstmev)           #line 42
    forward ( eh, "2", inst.buffer.secondmev)          #line 43
    reclaim_Buffers_from_heap ( inst)                  #line 44#line 45#line 46

def deracer_handler (eh,mev):                          #line 47
    inst =  eh.instance_data                           #line 48
    if  inst.state ==  "idle":                         #line 49
        if  "1" ==  mev.port:                          #line 50
            inst.buffer.firstmev =  mev                #line 51
            inst.state =  "waitingForSecondmev"        #line 52
        elif  "2" ==  mev.port:                        #line 53
            inst.buffer.secondmev =  mev               #line 54
            inst.state =  "waitingForFirstmev"         #line 55
        else:                                          #line 56
            runtime_error ( str( "bad mev.port (case A) for deracer ") +  mev.port )#line 57#line 58
    elif  inst.state ==  "waitingForFirstmev":         #line 59
        if  "1" ==  mev.port:                          #line 60
            inst.buffer.firstmev =  mev                #line 61
            send_firstmev_then_secondmev ( eh, inst)   #line 62
            inst.state =  "idle"                       #line 63
        else:                                          #line 64
            runtime_error ( str( "deracer: waiting for 1 but got [") +  str( mev.port) +  "] (case B)"  )#line 65#line 66
    elif  inst.state ==  "waitingForSecondmev":        #line 67
        if  "2" ==  mev.port:                          #line 68
            inst.buffer.secondmev =  mev               #line 69
            send_firstmev_then_secondmev ( eh, inst)   #line 70
            inst.state =  "idle"                       #line 71
        else:                                          #line 72
            runtime_error ( str( "deracer: waiting for 2 but got [") +  str( mev.port) +  "] (case C)"  )#line 73#line 74
    else:                                              #line 75
        runtime_error ( "bad state for deracer {eh.state}")#line 76#line 77#line 78#line 79

def low_level_read_text_file_instantiate (reg,owner,name,template_data,arg):#line 80
    name_with_id = gensymbol ( "Low Level Read Text File")#line 81
    return make_leaf ( name_with_id, owner, None, "", low_level_read_text_file_handler, None)#line 82#line 83#line 84

def low_level_read_text_file_handler (eh,mev):         #line 85
    fname =  mev.payload.v                             #line 86

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
                                                       #line 87#line 88#line 89

def ensure_string_datum_instantiate (reg,owner,name,template_data,arg):#line 90
    name_with_id = gensymbol ( "Ensure String Datum")  #line 91
    return make_leaf ( name_with_id, owner, None, "", ensure_string_datum_handler, None)#line 92#line 93#line 94

def ensure_string_datum_handler (eh,mev):              #line 95
    if  "string" ==  mev.payload.kind ():              #line 96
        forward ( eh, "", mev)                         #line 97
    else:                                              #line 98
        emev =  str( "*** ensure: type error (expected a string payload) but got ") +  mev.payload #line 99
        send ( eh, "✗", emev, mev)                     #line 100#line 101#line 102#line 103

class Syncfilewrite_Data:
    def __init__ (self,):                              #line 104
        self.filename =  ""                            #line 105#line 106
                                                       #line 107
def syncfilewrite_reset_handler (eh):                  #line 108
    eh.instance_data =  Syncfilewrite_Data ()          #line 109#line 110#line 111

# temp copy for bootstrap, sends "done“ (error during bootstrap if not wired)#line 112
def syncfilewrite_instantiate (reg,owner,name,template_data,arg):#line 113
    name_with_id = gensymbol ( "syncfilewrite")        #line 114
    inst =  Syncfilewrite_Data ()                      #line 115
    return make_leaf ( name_with_id, owner, inst, "", syncfilewrite_handler, syncfilewrite_reset_handler)#line 116#line 117#line 118

def syncfilewrite_handler (eh,mev):                    #line 119
    inst =  eh.instance_data                           #line 120
    if  "filename" ==  mev.port:                       #line 121
        inst.filename =  mev.payload.v                 #line 122
    elif  "input" ==  mev.port:                        #line 123
        contents =  mev.payload.v                      #line 124
        f = open ( inst.filename, "w")                 #line 125
        if  f!= None:                                  #line 126
            f.write ( mev.payload.v)                   #line 127
            f.close ()                                 #line 128
            send ( eh, "done",new_datum_bang (), mev)  #line 129
        else:                                          #line 130
            send ( eh, "✗", str( "open error on file ") +  inst.filename , mev)#line 131#line 132#line 133#line 134#line 135

class StringConcat_Instance_Data:
    def __init__ (self,):                              #line 136
        self.buffer1 =  None                           #line 137
        self.buffer2 =  None                           #line 138#line 139
                                                       #line 140
def stringconcat_reset_handler (eh):                   #line 141
    inst =  eh.instance_data                           #line 142
    inst.buffer1 =  None                               #line 143
    inst.buffer2 =  None                               #line 144#line 145#line 146

def stringconcat_instantiate (reg,owner,name,template_data,arg):#line 147
    name_with_id = gensymbol ( "stringconcat")         #line 148
    instp =  StringConcat_Instance_Data ()             #line 149
    return make_leaf ( name_with_id, owner, instp, "", stringconcat_handler, stringconcat_reset_handler)#line 150#line 151#line 152

def stringconcat_handler (eh,mev):                     #line 153
    inst =  eh.instance_data                           #line 154
    if  "1" ==  mev.port:                              #line 155
        inst.buffer1 = clone_string ( mev.payload.v)   #line 156
        maybe_stringconcat ( eh, inst, mev)            #line 157
    elif  "2" ==  mev.port:                            #line 158
        inst.buffer2 = clone_string ( mev.payload.v)   #line 159
        maybe_stringconcat ( eh, inst, mev)            #line 160
    elif  "reset" ==  mev.port:                        #line 161
        inst.buffer1 =  None                           #line 162
        inst.buffer2 =  None                           #line 163
    else:                                              #line 164
        runtime_error ( str( "bad mev.port for stringconcat: ") +  mev.port )#line 165#line 166#line 167#line 168

def maybe_stringconcat (eh,inst,mev):                  #line 169
    if  inst.buffer1!= None and  inst.buffer2!= None:  #line 170
        concatenated_string =  ""                      #line 171
        if  0 == len ( inst.buffer1):                  #line 172
            concatenated_string =  inst.buffer2        #line 173
        elif  0 == len ( inst.buffer2):                #line 174
            concatenated_string =  inst.buffer1        #line 175
        else:                                          #line 176
            concatenated_string =  inst.buffer1+ inst.buffer2#line 177#line 178
        send ( eh, "", concatenated_string, mev)       #line 179
        inst.buffer1 =  None                           #line 180
        inst.buffer2 =  None                           #line 181#line 182#line 183#line 184

#                                                      #line 185#line 186
def string_constant_instantiate (reg,owner,name,template_data,arg):#line 187
    global projectRoot                                 #line 188
    name_with_id = gensymbol ( "strconst")             #line 189
    s =  template_data                                 #line 190
    if  projectRoot!= "":                              #line 191
        s = re.sub ( "_00_",  projectRoot,  s)         #line 192#line 193
    return make_leaf ( name_with_id, owner, s, "", string_constant_handler, None)#line 194#line 195#line 196

def string_constant_handler (eh,mev):                  #line 197
    s =  eh.instance_data                              #line 198
    send ( eh, "", s, mev)                             #line 199#line 200#line 201

def fakepipename_instantiate (reg,owner,name,template_data,arg):#line 202
    instance_name = gensymbol ( "fakepipe")            #line 203
    return make_leaf ( instance_name, owner, None, "", fakepipename_handler, None)#line 204#line 205#line 206

rand =  0                                              #line 207#line 208
def fakepipename_handler (eh,mev):                     #line 209
    global rand                                        #line 210
    rand =  rand+ 1
    # not very random, but good enough _ ;rand' must be unique within a single run#line 211
    send ( eh, "", str( "/tmp/fakepipe") +  rand , mev)#line 212#line 213#line 214
                                                       #line 215
class Switch1star_Instance_Data:
    def __init__ (self,):                              #line 216
        self.state =  "1"                              #line 217#line 218
                                                       #line 219
def switch1star_reset_handler (eh):                    #line 220
    inst =  eh.instance_data                           #line 221
    inst =  Switch1star_Instance_Data ()               #line 222#line 223#line 224

def switch1star_instantiate (reg,owner,name,template_data,arg):#line 225
    name_with_id = gensymbol ( "switch1*")             #line 226
    instp =  Switch1star_Instance_Data ()              #line 227
    return make_leaf ( name_with_id, owner, instp, "", switch1star_handler, switch1star_reset_handler)#line 228#line 229#line 230

def switch1star_handler (eh,mev):                      #line 231
    inst =  eh.instance_data                           #line 232
    whichOutput =  inst.state                          #line 233
    if  "" ==  mev.port:                               #line 234
        if  "1" ==  whichOutput:                       #line 235
            forward ( eh, "1", mev)                    #line 236
            inst.state =  "*"                          #line 237
        elif  "*" ==  whichOutput:                     #line 238
            forward ( eh, "*", mev)                    #line 239
        else:                                          #line 240
            send ( eh, "✗", "internal error bad state in switch1*", mev)#line 241#line 242
    elif  "reset" ==  mev.port:                        #line 243
        inst.state =  "1"                              #line 244
    else:                                              #line 245
        send ( eh, "✗", "internal error bad mevent for switch1*", mev)#line 246#line 247#line 248#line 249

class StringAccumulator:
    def __init__ (self,):                              #line 250
        self.s =  ""                                   #line 251#line 252
                                                       #line 253
def strcatstar_reset_handler (eh):                     #line 254
    eh.instance_data =  StringAccumulator ()           #line 255#line 256#line 257

def strcatstar_instantiate (reg,owner,name,template_data,arg):#line 258
    name_with_id = gensymbol ( "String Concat *")      #line 259
    instp =  StringAccumulator ()                      #line 260
    return make_leaf ( name_with_id, owner, instp, "", strcatstar_handler, strcatstar_reset_handler)#line 261#line 262#line 263

def strcatstar_handler (eh,mev):                       #line 264
    accum =  eh.instance_data                          #line 265
    if  "" ==  mev.port:                               #line 266
        accum.s =  str( accum.s) +  mev.payload.v      #line 267
    elif  "fini" ==  mev.port:                         #line 268
        send ( eh, "", accum.s, mev)                   #line 269
    else:                                              #line 270
        send ( eh, "✗", "internal error bad mevent for String Concat *", mev)#line 271#line 272#line 273#line 274

def stop_instantiate (reg,owner,name,template_data,arg):#line 275
    name_with_id = gensymbol ( "Stop")                 #line 276
    inst =  None                                       #line 277
    return make_leaf ( name_with_id, owner, inst, "", stop_handler, None)#line 278#line 279#line 280

def stop_handler (eh,mev):                             #line 281
    inst =  eh.instance_data                           #line 282
    parent =  eh.owner                                 #line 283
    s =  str( "   !!! stopping: '") +  str( parent.name) +  "'"  #line 284
    print ( s, file=sys.stderr)                        #line 285
                                                       #line 286
    parent.stop ( parent)                              #line 287
    send ( eh, "", mev.payload.v, mev)                 #line 288#line 289#line 290

# all of the the built_in leaves are listed here       #line 291
# future: refactor this such that programmers can pick and choose which (lumps of) builtins are used in a specific project#line 292#line 293
def initialize_stock_components (reg):                 #line 294
    register_component ( reg,mkTemplate ( "1then2", None, deracer_instantiate))#line 295
    register_component ( reg,mkTemplate ( "1→2", None, deracer_instantiate))#line 296
    register_component ( reg,mkTemplate ( "trash", None, trash_instantiate))#line 297
    register_component ( reg,mkTemplate ( "🗑️", None, trash_instantiate))#line 298
    register_component ( reg,mkTemplate ( "🚫", None, stop_instantiate))#line 299#line 300#line 301
    register_component ( reg,mkTemplate ( "Read Text File", None, low_level_read_text_file_instantiate))#line 302
    register_component ( reg,mkTemplate ( "Ensure String Datum", None, ensure_string_datum_instantiate))#line 303#line 304
    register_component ( reg,mkTemplate ( "syncfilewrite", None, syncfilewrite_instantiate))#line 305
    register_component ( reg,mkTemplate ( "String Concat", None, stringconcat_instantiate))#line 306
    register_component ( reg,mkTemplate ( "switch1*", None, switch1star_instantiate))#line 307
    register_component ( reg,mkTemplate ( "String Concat *", None, strcatstar_instantiate))#line 308
    # for fakepipe                                     #line 309
    register_component ( reg,mkTemplate ( "fakepipename", None, fakepipename_instantiate))#line 310#line 311#line 312
#line 1
def load_error (s):                                    #line 2
    global load_errors                                 #line 3
    print ( s, file=sys.stderr)                        #line 4
                                                       #line 5
    load_errors =  True                                #line 6#line 7#line 8

def runtime_error (s):                                 #line 9
    global runtime_errors                              #line 10
    print ( s, file=sys.stderr)                        #line 11
    exit (1)                                           #line 12
    runtime_errors =  True                             #line 13#line 14#line 15
                                                       #line 16
def initialize_component_palette_from_files (diagram_source_files):#line 17
    reg = make_component_registry ()                   #line 18
    for diagram_source in  diagram_source_files:       #line 19
        all_containers_within_single_file = lnet2internal_from_file ( diagram_source)#line 20
        for container in  all_containers_within_single_file:#line 21
            register_component ( reg,mkTemplate ( container [ "name"], container, container_instantiator))#line 22#line 23#line 24
    initialize_stock_components ( reg)                 #line 25
    return  reg                                        #line 26#line 27#line 28

def initialize_component_palette_from_string (lnet):   #line 29
    reg = make_component_registry ()                   #line 30
    all_containers = lnet2internal_from_string ( lnet) #line 31
    for container in  all_containers:                  #line 32
        register_component ( reg,mkTemplate ( container [ "name"], container, container_instantiator))#line 33#line 34
    initialize_stock_components ( reg)                 #line 35
    return  reg                                        #line 36#line 37

def initialize_from_files (diagram_names):             #line 38
    arg =  None                                        #line 39
    palette = initialize_component_palette_from_files ( diagram_names)#line 40
    return [ palette,[ diagram_names, arg]]            #line 41#line 42#line 43

def initialize_from_string ():                         #line 44
    arg =  None                                        #line 45
    palette = initialize_component_palette_from_string ()#line 46
    return [ palette,[ None, arg]]                     #line 47#line 48#line 49

def start (arg,part_name,palette,env):                 #line 50
    part = start_bare ( part_name, palette, env)       #line 51
    inject ( part, "", arg)                            #line 52
    finalize ( part)                                   #line 53#line 54#line 55

def start_bare (part_name,palette,env):                #line 56
    diagram_names =  env [ 0]                          #line 57
    # get entrypoint container                         #line 58
    part = get_component_instance ( palette, part_name, None)#line 59
    if  None ==  part:                                 #line 60
        load_error ( str( "Couldn;t find container with page name /") +  str( part_name) +  str( "/ in files ") +  str(str ( diagram_names)) +  " (check tab names, or disable compression?)"    )#line 64#line 65
    return  part                                       #line 66#line 67#line 68

def inject (part,port,payload):                        #line 69
    if not  load_errors:                               #line 70
        d =  Datum ()                                  #line 71
        d.v =  payload                                 #line 72
        d.clone =  lambda : obj_clone ( d)             #line 73
        d.reclaim =  None                              #line 74
        mev = make_mevent ( port, d)                   #line 75
        inject_mevent ( part, mev)                     #line 76
    else:                                              #line 77
        exit (1)                                       #line 78#line 79#line 80#line 81

def finalize (part):                                   #line 82
    print (deque_to_json ( part.outq))                 #line 83#line 84#line 85

def new_datum_bang ():                                 #line 86
    d =  Datum ()                                      #line 87
    d.v =  "!"                                         #line 88
    d.clone =  lambda : obj_clone ( d)                 #line 89
    d.reclaim =  None                                  #line 90
    return  d                                          #line 91#line 92
