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
# Routing connection for a container component. The `direction` field has#line 106
# no affect on the default mevent routing system _ it is there for debugging#line 107
# purposes, or for reading by other tools.             #line 108#line 109
class Connector:
    def __init__ (self,):                              #line 110
        self.direction =  None # down, across, up, through#line 111
        self.sender =  None                            #line 112
        self.receiver =  None                          #line 113#line 114
                                                       #line 115
# `Sender` is used to “pattern match“ which `Receiver` a mevent should go to,#line 116
# based on component ID (pointer) and port name.       #line 117#line 118
class Sender:
    def __init__ (self,):                              #line 119
        self.name =  None                              #line 120
        self.component =  None                         #line 121
        self.port =  None                              #line 122#line 123
                                                       #line 124#line 125#line 126
# `Receiver` is a handle to a destination queue, and a `port` name to assign#line 127
# to incoming mevents to this queue.                   #line 128#line 129
class Receiver:
    def __init__ (self,):                              #line 130
        self.name =  None                              #line 131
        self.queue =  None                             #line 132
        self.port =  None                              #line 133
        self.component =  None                         #line 134#line 135
                                                       #line 136
def mkSender (name,component,port):                    #line 137
    s =  Sender ()                                     #line 138
    s.name =  name                                     #line 139
    s.component =  component                           #line 140
    s.port =  port                                     #line 141
    return  s                                          #line 142#line 143#line 144

def mkReceiver (name,component,port,q):                #line 145
    r =  Receiver ()                                   #line 146
    r.name =  name                                     #line 147
    r.component =  component                           #line 148
    r.port =  port                                     #line 149
    # We need a way to determine which queue to target. "Down" and "Across" go to inq, "Up" and "Through" go to outq.#line 150
    r.queue =  q                                       #line 151
    return  r                                          #line 152#line 153#line 154
                                                       #line 155
class Component_Registry:
    def __init__ (self,):                              #line 156
        self.templates = {}                            #line 157#line 158
                                                       #line 159
class Template:
    def __init__ (self,):                              #line 160
        self.name =  None                              #line 161
        self.container =  None                         #line 162
        self.instantiator =  None                      #line 163#line 164
                                                       #line 165
def mkTemplate (name,template_data,instantiator):      #line 166
    templ =  Template ()                               #line 167
    templ.name =  name                                 #line 168
    templ.template_data =  template_data               #line 169
    templ.instantiator =  instantiator                 #line 170
    return  templ                                      #line 171#line 172#line 173

def make_component_registry ():                        #line 174
    return  Component_Registry ()                      #line 175#line 176#line 177

# Data for an asyncronous component _ effectively, a function with input#line 178
# and output queues of mevents.                        #line 179
#                                                      #line 180
# Components can either be a user_supplied function (“leaf“), or a “container“#line 181
# that routes mevents to child components according to a list of connections#line 182
# that serve as a mevent routing table.                #line 183
#                                                      #line 184
# Child components themselves can be leaves or other containers.#line 185
#                                                      #line 186
# `handler` invokes the code that is attached to this component.#line 187
#                                                      #line 188
# `instance_data` is a pointer to instance data that the `leaf_handler`#line 189
# function may want whenever it is invoked again.      #line 190#line 191
# TODO: what is .routings for? (is it a historical artefact that can be removed?) #line 192#line 193
# Eh_States :: enum { idle, active }                   #line 194
class Eh:
    def __init__ (self,):                              #line 195
        self.name =  ""                                #line 196
        self.inq =  deque ([])                         #line 197
        self.outq =  deque ([])                        #line 198
        self.owner =  None                             #line 199
        self.children = []                             #line 200
        self.visit_ordering =  deque ([])              #line 201
        self.connections = []                          #line 202
        self.routings =  deque ([])                    #line 203
        self.handler =  None                           #line 204
        self.reset_instance_data =  None               #line 205
        self.finject =  None                           #line 206
        self.stop =  None                              #line 207
        self.instance_data =  None                     #line 208# arg needed for probe support #line 209
        self.arg =  ""                                 #line 210
        self.state =  "idle"                           #line 211
        self.special =  False                          #line 212# bootstrap debugging#line 213
        self.kind =  None # enum { container, leaf, }  #line 214#line 215
                                                       #line 216
load_errors =  False                                   #line 217
runtime_errors =  False                                #line 218#line 219
def clone_string (s):                                  #line 220
    return  s                                          #line 221#line 222#line 223

def injector (eh,mevent):                              #line 224
    eh.handler ( eh, mevent)                           #line 225#line 226#line 227
