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
