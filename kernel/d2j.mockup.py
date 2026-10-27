
## builtins
class StringBuffer ():
    def __init__ (self, input):
        self.buff = input
        self.bx = 0
    def accept_char (self):
        self.bx += 1
    def overrun (self):
        return self.bx >= len (self.buff)
    def parse_char (c):
        try:
            if self.overrun ():
                throw Exception ("Fail")
            if c == self.buff [buff.bx]:
                self.accept_char ()
            else:
                throw Exception ("Fail")

buffer = StringBuffer ()

def parse (f):
    try:
        f ()
    except:
        throw Exception ("Fail")

def parse_string (s):
    global buffer
    i = 0
    try:
        while i < len (s):
            try:
                buffer.parse_char (s [i])
                i += 1
            except:
                throw ("Fail")
    except:
        throw ("Fail")
    
def parse_range (first, last):
    global buffer
    try:
        if buffer.next_char >= first and buffer.next_char <= last:
            buffer.accept_char ()
        else:
            throw Exception ("Fail")

def parse_ignore ():
    while (buffer.is_space ()):
        buffer.accept_char ()

def parse_one_or_more (f):
    try:
        parse (f)
        try:
            parse (f)
        except:
            pass
    except:
        throw Exception ("Fail")

def parse_either (flist):
    f = first (flist)
    flist = rest (flist)
    while True:
        try:
            parse (f)
            return True
        except:
            if flist:
                flist = rest (flist)
            else:
                throw Exception ("Fail")
    throw Exception ("Fail")


## Parser
def s_Main ():
    try:
        parse_ignore ()
        s_Array ()
        parse_ignore ()
        return #True
    except:
        throw Exception("Fail")

def s_Array ():
    try:
        parse_ignore ()
        parse_string ("[")
        parse_ignore ()
        parse_one_os_more (parse_either ([s_KV, s_Object]))
        parse_ignore ()
        parse_string ("]")
        parse_ignore ()
        return #True
    except:
        throw Exception("Fail")

def s_Object ():
    try:
        parse_ignore ()
        parse_string ("{")
        parse_ignore ()
        parse_one_or_more (s_KV)
        parse_ignore ()
        parse_string ("}")
        parse_ignore ()
    except:
        throw Exception("Fail")

def s_KV ():
    try:
        parse_ignore ()
        parse (s_keystring)
        parse_ignore ()
        parse_string (":")
        parse_ignore ()
        parse_either ([s_Array, s_Object, l_string, l_emptyString, l_number])
        parse_ignore ()
    except:
        throw Exception("Fail")

def l_string ():
    try:
        parse_string ('"')
        parse_upto ('"')
        parse_string ('"')
    except:
        throw Exception ("Fail")
        
def l_keystring ():
    try:
        parse (l_string)
    except:
        throw Exception ("Fail")

def l_keystring ():
    try:
        parse_string ("\"")
        parse_string ("\"")
    except:
        throw Exception ("Fail")

def l_number ():
    try:
        parse_range ("0", "9")
    except:
        throw Exception ("Fail")


stest = '[ "name" : "hello" ]'
buffer = StringBuffer (stest)
parse (s_Main)
