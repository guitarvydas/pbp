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
