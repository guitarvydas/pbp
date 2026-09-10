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
class Datum:
    def __init__ (self,):                              #line 6
        self.v =  None                                 #line 7
        self.clone =  None                             #line 8
        self.reclaim =  None                           #line 9
        self.other =  None # reserved for use on per-project basis #line 10#line 11
                                                       #line 12#line 13
# Mevent passed to a leaf component.                   #line 14
#                                                      #line 15
# `port` refers to the name of the incoming or outgoing port of this component.#line 16
# `payload` is the data attached to this mevent.       #line 17
class Mevent:
    def __init__ (self,):                              #line 18
        self.port =  None                              #line 19
        self.payload =  None                           #line 20#line 21
                                                       #line 22
def clone_port (s):                                    #line 23
    return clone_string ( s)                           #line 24#line 25#line 26

# Utility for making a `Mevent`. Used to safely "seed“ mevents#line 27
# entering the very top of a network.                  #line 28
def make_mevent (port,datum):                          #line 29
    p = clone_string ( port)                           #line 30
    m =  Mevent ()                                     #line 31
    m.port =  p                                        #line 32
    m.payload =  datum.clone ()                        #line 33
    return  m                                          #line 34#line 35#line 36

# Clones a mevent. Primarily used internally for “fanning out“ a mevent to multiple destinations.#line 37
def mevent_clone (mev):                                #line 38
    m =  Mevent ()                                     #line 39
    m.port = clone_port ( mev.port)                    #line 40
    m.payload =  mev.payload.clone ()                  #line 41
    return  m                                          #line 42#line 43#line 44

# Frees a mevent.                                      #line 45
def destroy_mevent (mev):                              #line 46
    # during debug, dont destroy any mevent, since we want to trace mevents, thus, we need to persist ancestor mevents#line 47
    pass                                               #line 48#line 49#line 50

def destroy_datum (mev):                               #line 51
    pass                                               #line 52#line 53#line 54

def destroy_port (mev):                                #line 55
    pass                                               #line 56#line 57#line 58

#                                                      #line 59
def format_mevent (m):                                 #line 60
    if  m ==  None:                                    #line 61
        return  "{}"                                   #line 62
    else:                                              #line 63
        return  str( "{%5C”") +  str( m.port) +  str( "%5C”:%5C”") +  str( m.payload.v) +  "%5C”}"    #line 64#line 65#line 66

def format_mevent_raw (m):                             #line 67
    if  m ==  None:                                    #line 68
        return  ""                                     #line 69
    else:                                              #line 70
        return  m.payload.v                            #line 71#line 72#line 73#line 74

enumDown =  0                                          #line 75
enumAcross =  1                                        #line 76
enumUp =  2                                            #line 77
enumThrough =  3                                       #line 78#line 79#line 80
# Routing connection for a container component. The `direction` field has#line 81
# no affect on the default mevent routing system _ it is there for debugging#line 82
# purposes, or for reading by other tools.             #line 83#line 84
class Connector:
    def __init__ (self,):                              #line 85
        self.direction =  None # down, across, up, through#line 86
        self.sender =  None                            #line 87
        self.receiver =  None                          #line 88#line 89
                                                       #line 90
# `Sender` is used to “pattern match“ which `Receiver` a mevent should go to,#line 91
# based on component ID (pointer) and port name.       #line 92#line 93
class Sender:
    def __init__ (self,):                              #line 94
        self.name =  None                              #line 95
        self.component =  None                         #line 96
        self.port =  None                              #line 97#line 98
                                                       #line 99#line 100#line 101
# `Receiver` is a handle to a destination queue, and a `port` name to assign#line 102
# to incoming mevents to this queue.                   #line 103#line 104
class Receiver:
    def __init__ (self,):                              #line 105
        self.name =  None                              #line 106
        self.queue =  None                             #line 107
        self.port =  None                              #line 108
        self.component =  None                         #line 109#line 110
                                                       #line 111
def mkSender (name,component,port):                    #line 112
    s =  Sender ()                                     #line 113
    s.name =  name                                     #line 114
    s.component =  component                           #line 115
    s.port =  port                                     #line 116
    return  s                                          #line 117#line 118#line 119

def mkReceiver (name,component,port,q):                #line 120
    r =  Receiver ()                                   #line 121
    r.name =  name                                     #line 122
    r.component =  component                           #line 123
    r.port =  port                                     #line 124
    # We need a way to determine which queue to target. "Down" and "Across" go to inq, "Up" and "Through" go to outq.#line 125
    r.queue =  q                                       #line 126
    return  r                                          #line 127#line 128#line 129
                                                       #line 130
class Component_Registry:
    def __init__ (self,):                              #line 131
        self.templates = {}                            #line 132#line 133
                                                       #line 134
class Template:
    def __init__ (self,):                              #line 135
        self.name =  None                              #line 136
        self.container =  None                         #line 137
        self.instantiator =  None                      #line 138#line 139
                                                       #line 140
def mkTemplate (name,template_data,instantiator):      #line 141
    templ =  Template ()                               #line 142
    templ.name =  name                                 #line 143
    templ.template_data =  template_data               #line 144
    templ.instantiator =  instantiator                 #line 145
    return  templ                                      #line 146#line 147#line 148

def make_component_registry ():                        #line 149
    return  Component_Registry ()                      #line 150#line 151#line 152

# Data for an asyncronous component _ effectively, a function with input#line 153
# and output queues of mevents.                        #line 154
#                                                      #line 155
# Components can either be a user_supplied function (“leaf“), or a “container“#line 156
# that routes mevents to child components according to a list of connections#line 157
# that serve as a mevent routing table.                #line 158
#                                                      #line 159
# Child components themselves can be leaves or other containers.#line 160
#                                                      #line 161
# `handler` invokes the code that is attached to this component.#line 162
#                                                      #line 163
# `instance_data` is a pointer to instance data that the `leaf_handler`#line 164
# function may want whenever it is invoked again.      #line 165#line 166
# TODO: what is .routings for? (is it a historical artefact that can be removed?) #line 167#line 168
# Eh_States :: enum { idle, active }                   #line 169
class Eh:
    def __init__ (self,):                              #line 170
        self.name =  ""                                #line 171
        self.inq =  deque ([])                         #line 172
        self.outq =  deque ([])                        #line 173
        self.owner =  None                             #line 174
        self.children = []                             #line 175
        self.visit_ordering =  deque ([])              #line 176
        self.connections = []                          #line 177
        self.routings =  deque ([])                    #line 178
        self.handler =  None                           #line 179
        self.reset_instance_data =  None               #line 180
        self.finject =  None                           #line 181
        self.stop =  None                              #line 182
        self.instance_data =  None                     #line 183# arg needed for probe support #line 184
        self.arg =  ""                                 #line 185
        self.state =  "idle"                           #line 186
        self.special =  False                          #line 187# bootstrap debugging#line 188
        self.kind =  None # enum { container, leaf, }  #line 189#line 190
                                                       #line 191
load_errors =  False                                   #line 192
runtime_errors =  False                                #line 193#line 194
def clone_string (s):                                  #line 195
    return  s                                          #line 196#line 197#line 198

def injector (eh,mevent):                              #line 199
    eh.handler ( eh, mevent)                           #line 200#line 201#line 202
