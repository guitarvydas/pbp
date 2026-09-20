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
        self.routings =  deque ([])                    #line 26
        self.handler =  None                           #line 27
        self.reset_instance_data =  None               #line 28
        self.finject =  None                           #line 29
        self.stop =  None                              #line 30
        self.instance_data =  None                     #line 31# arg needed for probe support #line 32
        self.arg =  ""                                 #line 33
        self.state =  "idle"                           #line 34
        self.special =  False                          #line 35# bootstrap debugging#line 36
        self.kind =  None # enum { container, leaf, }  #line 37#line 38
                                                       #line 39
def injector (eh,mevent):                              #line 40
    eh.handler ( eh, mevent)                           #line 41#line 42#line 43
