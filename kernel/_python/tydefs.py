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
# Routing connection for a container component. The `direction` field has#line 110
# no affect on the default mevent routing system _ it is there for debugging#line 111
# purposes, or for reading by other tools.             #line 112#line 113
class Connector:
    def __init__ (self,):                              #line 114
        self.direction =  None # down, across, up, through#line 115
        self.sender =  None                            #line 116
        self.receiver =  None                          #line 117#line 118
                                                       #line 119
# `Sender` is used to “pattern match“ which `Receiver` a mevent should go to,#line 120
# based on component ID (pointer) and port name.       #line 121#line 122
class Sender:
    def __init__ (self,):                              #line 123
        self.name =  None                              #line 124
        self.component =  None                         #line 125
        self.port =  None                              #line 126#line 127
                                                       #line 128#line 129#line 130
# `Receiver` is a handle to a destination queue, and a `port` name to assign#line 131
# to incoming mevents to this queue.                   #line 132#line 133
class Receiver:
    def __init__ (self,):                              #line 134
        self.name =  None                              #line 135
        self.queue =  None                             #line 136
        self.port =  None                              #line 137
        self.component =  None                         #line 138#line 139
                                                       #line 140
def mkSender (name,component,port):                    #line 141
    s =  Sender ()                                     #line 142
    s.name =  name                                     #line 143
    s.component =  component                           #line 144
    s.port =  port                                     #line 145
    return  s                                          #line 146#line 147#line 148

def mkReceiver (name,component,port,q):                #line 149
    r =  Receiver ()                                   #line 150
    r.name =  name                                     #line 151
    r.component =  component                           #line 152
    r.port =  port                                     #line 153
    # We need a way to determine which queue to target. "Down" and "Across" go to inq, "Up" and "Through" go to outq.#line 154
    r.queue =  q                                       #line 155
    return  r                                          #line 156#line 157#line 158
                                                       #line 159
class Component_Registry:
    def __init__ (self,):                              #line 160
        self.templates = {}                            #line 161#line 162
                                                       #line 163
class Template:
    def __init__ (self,):                              #line 164
        self.name =  None                              #line 165
        self.container =  None                         #line 166
        self.instantiator =  None                      #line 167#line 168
                                                       #line 169
def mkTemplate (name,template_data,instantiator):      #line 170
    templ =  Template ()                               #line 171
    templ.name =  name                                 #line 172
    templ.template_data =  template_data               #line 173
    templ.instantiator =  instantiator                 #line 174
    return  templ                                      #line 175#line 176#line 177

def make_component_registry ():                        #line 178
    return  Component_Registry ()                      #line 179#line 180#line 181

# Data for an asyncronous component _ effectively, a function with input#line 182
# and output queues of mevents.                        #line 183
#                                                      #line 184
# Components can either be a user_supplied function (“leaf“), or a “container“#line 185
# that routes mevents to child components according to a list of connections#line 186
# that serve as a mevent routing table.                #line 187
#                                                      #line 188
# Child components themselves can be leaves or other containers.#line 189
#                                                      #line 190
# `handler` invokes the code that is attached to this component.#line 191
#                                                      #line 192
# `instance_data` is a pointer to instance data that the `leaf_handler`#line 193
# function may want whenever it is invoked again.      #line 194#line 195
# TODO: what is .routings for? (is it a historical artefact that can be removed?) #line 196#line 197
# Eh_States :: enum { idle, active }                   #line 198
class Eh:
    def __init__ (self,):                              #line 199
        self.name =  ""                                #line 200
        self.inq =  deque ([])                         #line 201
        self.outq =  deque ([])                        #line 202
        self.owner =  None                             #line 203
        self.children = []                             #line 204
        self.visit_ordering =  deque ([])              #line 205
        self.connections = []                          #line 206
        self.routings =  deque ([])                    #line 207
        self.handler =  None                           #line 208
        self.reset_instance_data =  None               #line 209
        self.finject =  None                           #line 210
        self.stop =  None                              #line 211
        self.instance_data =  None                     #line 212# arg needed for probe support #line 213
        self.arg =  ""                                 #line 214
        self.state =  "idle"                           #line 215
        self.special =  False                          #line 216# bootstrap debugging#line 217
        self.kind =  None # enum { container, leaf, }  #line 218#line 219
                                                       #line 220
load_errors =  False                                   #line 221
runtime_errors =  False                                #line 222#line 223
def clone_string (s):                                  #line 224
    return  s                                          #line 225#line 226#line 227

def injector (eh,mevent):                              #line 228
    eh.handler ( eh, mevent)                           #line 229#line 230#line 231
