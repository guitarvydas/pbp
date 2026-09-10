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
ticktime =  0                                          #line 4#line 5#line 6
class Component_Registry:
    def __init__ (self,):                              #line 7
        self.templates = {}                            #line 8#line 9
                                                       #line 10
class Template:
    def __init__ (self,):                              #line 11
        self.name =  None                              #line 12
        self.container =  None                         #line 13
        self.instantiator =  None                      #line 14#line 15
                                                       #line 16
def mkTemplate (name,template_data,instantiator):      #line 17
    templ =  Template ()                               #line 18
    templ.name =  name                                 #line 19
    templ.template_data =  template_data               #line 20
    templ.instantiator =  instantiator                 #line 21
    return  templ                                      #line 22#line 23#line 24

def make_component_registry ():                        #line 25
    return  Component_Registry ()                      #line 26#line 27#line 28

# Data for an asyncronous component _ effectively, a function with input#line 29
# and output queues of mevents.                        #line 30
#                                                      #line 31
# Components can either be a user_supplied function ("leaf“), or a “container“#line 32
# that routes mevents to child components according to a list of connections#line 33
# that serve as a mevent routing table.                #line 34
#                                                      #line 35
# Child components themselves can be leaves or other containers.#line 36
#                                                      #line 37
# `handler` invokes the code that is attached to this component.#line 38
#                                                      #line 39
# `instance_data` is a pointer to instance data that the `leaf_handler`#line 40
# function may want whenever it is invoked again.      #line 41#line 42
# TODO: what is .routings for? (is it a historical artefact that can be removed?) #line 43#line 44
# Eh_States :: enum { idle, active }                   #line 45
class Eh:
    def __init__ (self,):                              #line 46
        self.name =  ""                                #line 47
        self.inq =  deque ([])                         #line 48
        self.outq =  deque ([])                        #line 49
        self.owner =  None                             #line 50
        self.children = []                             #line 51
        self.visit_ordering =  deque ([])              #line 52
        self.connections = []                          #line 53
        self.routings =  deque ([])                    #line 54
        self.handler =  None                           #line 55
        self.reset_instance_data =  None               #line 56
        self.finject =  None                           #line 57
        self.stop =  None                              #line 58
        self.instance_data =  None                     #line 59# arg needed for probe support #line 60
        self.arg =  ""                                 #line 61
        self.state =  "idle"                           #line 62
        self.special =  False                          #line 63# bootstrap debugging#line 64
        self.kind =  None # enum { container, leaf, }  #line 65#line 66
                                                       #line 67
load_errors =  False                                   #line 68
runtime_errors =  False                                #line 69#line 70
def clone_string (s):                                  #line 71
    return  s                                          #line 72#line 73#line 74

def injector (eh,mevent):                              #line 75
    eh.handler ( eh, mevent)                           #line 76#line 77#line 78
