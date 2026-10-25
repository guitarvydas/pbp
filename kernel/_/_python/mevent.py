# A mevent has a port (the name of the incoming or outgoing port) and a payload. The payload is always a string.#line 1
class Mevent:
    def __init__ (self):                               #line 2
        self.port =  None                              #line 3
        self.payload =  None                           #line 4#line 5
                                                       #line 6
def make_mevent (port,payload):                        #line 7
    m =  Mevent ()                                     #line 8
    m.port =  port                                     #line 9
    m.payload =  payload                               #line 10
    return  m                                          #line 11#line 12
