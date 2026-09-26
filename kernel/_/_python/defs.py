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
enumDown =  0                                          #line 6
enumAcross =  1                                        #line 7
enumUp =  2                                            #line 8
enumThrough =  3                                       #line 9#line 10#line 11
# Routing connection for a container component. The `direction` field has#line 12
# no affect on the default mevent routing system _ it is there for debugging#line 13
# purposes, or for reading by other tools.             #line 14#line 15
class Connector:
    def __init__ (self,):                              #line 16
        self.direction =  None # down, across, up, through#line 17
        self.sender =  None                            #line 18
        self.receiver =  None                          #line 19#line 20
                                                       #line 21
# `Sender` is used to "pattern match“ which `Receiver` a mevent should go to,#line 22
# based on component ID (pointer) and port name.       #line 23#line 24
class Sender:
    def __init__ (self,):                              #line 25
        self.name =  None                              #line 26
        self.component =  None                         #line 27
        self.port =  None                              #line 28#line 29
                                                       #line 30#line 31#line 32
# `Receiver` is a handle to a destination queue, and a `port` name to assign#line 33
# to incoming mevents to this queue.                   #line 34#line 35
class Receiver:
    def __init__ (self,):                              #line 36
        self.name =  None                              #line 37
        self.queue =  None                             #line 38
        self.port =  None                              #line 39
        self.component =  None                         #line 40#line 41
                                                       #line 42
def mkSender (name,component,port):                    #line 43
    s =  Sender ()                                     #line 44
    s.name =  name                                     #line 45
    s.component =  component                           #line 46
    s.port =  port                                     #line 47
    return  s                                          #line 48#line 49#line 50

def mkReceiver (name,component,port,q):                #line 51
    r =  Receiver ()                                   #line 52
    r.name =  name                                     #line 53
    r.component =  component                           #line 54
    r.port =  port                                     #line 55
    # We need a way to determine which queue to target. "Down" and "Across" go to inq, "Up" and "Through" go to outq.#line 56
    r.queue =  q                                       #line 57
    return  r                                          #line 58#line 59#line 60
                                                       #line 61
class Component_Registry:
    def __init__ (self,):                              #line 62
        self.templates = {}                            #line 63#line 64
                                                       #line 65
class Template:
    def __init__ (self,):                              #line 66
        self.name =  None                              #line 67
        self.container =  None                         #line 68
        self.instantiator =  None                      #line 69#line 70
                                                       #line 71
def mkTemplate (name,template_data,instantiator):      #line 72
    templ =  Template ()                               #line 73
    templ.name =  name                                 #line 74
    templ.template_data =  template_data               #line 75
    templ.instantiator =  instantiator                 #line 76
    return  templ                                      #line 77#line 78#line 79

def make_component_registry ():                        #line 80
    return  Component_Registry ()                      #line 81#line 82#line 83

# Data for an asyncronous component _ effectively, a function with input#line 84
# and output queues of mevents.                        #line 85
#                                                      #line 86
# Components can either be a user_supplied function (“leaf“), or a “container“#line 87
# that routes mevents to child components according to a list of connections#line 88
# that serve as a mevent routing table.                #line 89
#                                                      #line 90
# Child components themselves can be leaves or other containers.#line 91
#                                                      #line 92
# `handler` invokes the code that is attached to this component.#line 93
#                                                      #line 94
# `instance_data` is a pointer to instance data that the `leaf_handler`#line 95
# function may want whenever it is invoked again.      #line 96#line 97
# TODO: what is .routings for? (is it a historical artefact that can be removed?) #line 98#line 99
# Eh_States :: enum { idle, active }                   #line 100
class Eh:
    def __init__ (self,):                              #line 101
        self.name =  ""                                #line 102
        self.inq =  deque ([])                         #line 103
        self.outq =  deque ([])                        #line 104
        self.owner =  None                             #line 105
        self.children = []                             #line 106
        self.visit_ordering =  deque ([])              #line 107
        self.connections = []                          #line 108
        self.routings =  deque ([])                    #line 109
        self.handler =  None                           #line 110
        self.reset_instance_data =  None               #line 111
        self.finject =  None                           #line 112
        self.stop =  None                              #line 113
        self.instance_data =  None                     #line 114# arg needed for probe support #line 115
        self.arg =  ""                                 #line 116
        self.state =  "idle"                           #line 117
        self.special =  False                          #line 118# bootstrap debugging#line 119
        self.kind =  None # enum { container, leaf, }  #line 120#line 121
                                                       #line 122
load_errors =  False                                   #line 123
runtime_errors =  False                                #line 124#line 125
def clone_string (s):                                  #line 126
    return  s                                          #line 127#line 128#line 129

def injector (eh,mevent):                              #line 130
    eh.handler ( eh, mevent)                           #line 131#line 132#line 133
