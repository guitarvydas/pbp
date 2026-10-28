
## builtins
class StringBuffer ():
    def __init__ (self, input):
        self.buff = input
        self.bx = 0
        self.marks = []
    def push_mark (self):
        self.marks.append (self.bx)
    def pop_mark (self):
        self.marks.pop ()
    def rewind (self):
        self.bx = self.marks.pop ()
    def accept_char (self):
        print (f'accept_char c={self.buff [self.bx]}')
        self.bx += 1
    def overrun (self):
        return self.bx >= len (self.buff)
    def is_space (self):
        if self.overrun ():
            return False
        elif self.buff [self.bx] == " " or self.buff [self.bx] == "," or self.buff [self.bx] == "\n" :
            return True
    def parse_char (self, c):
        try:
            if self.overrun ():
                raise Exception ("Fail")
            if c == self.buff [self.bx]:
                self.accept_char ()
            else:
                raise Exception ("Fail")
        except:
            raise Exception ("Fail")

buffer = StringBuffer ("")

def fail ():
    global buffer
    buffer.rewind ()
    raise Exception ("Fail")

def begin ():
    global buffer
    buffer.push_mark ()

def accept ():
    global buffer
    buffer.pop_mark ()

def parse (f):
    try:
        f ()
    except:
        fail ()

def parse_string (s):
    global buffer
    i = 0
    try:
        while i < len (s):
            try:
                buffer.parse_char (s [i])
                i += 1
            except:
                fail ()
    except:
        fail ()
    
def parse_range (first, last):
    global buffer
    try:
        if buffer.next_char >= first and buffer.next_char <= last:
            buffer.accept_char ()
        else:
            fail ()
    except:
        fail ()

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
        fail ()
    
def parse_either (flist):
    while True:
        try:
            f = flist [0]
            flist = flist [1:]
            parse (f)
        except:
            if flist:
                flist = flist [1:]
            else:
                fail ()
    fail ()


## Parser
def s_Main ():
    begin ()
    try:
        parse_ignore ()
        s_Array ()
        parse_ignore ()
        accept ()
    except:
        raise Exception("Fail")

def s_Array ():
    begin ()
    try:
        parse_ignore ()
        parse_string ("[")
        parse_ignore ()
        parse_one_or_more (parse_either ([s_KV, s_Object]))
        parse_ignore ()
        parse_string ("]")
        parse_ignore ()
        accept ()
    except:
        raise Exception("Fail")

def s_Object ():
    begin ()
    try:
        parse_ignore ()
        parse_string ("{")
        parse_ignore ()
        parse_one_or_more (s_KV)
        parse_ignore ()
        parse_string ("}")
        parse_ignore ()
        accept ()
    except:
        raise Exception("Fail")

def s_KV ():
    begin ()
    try:
        parse_ignore ()
        parse (l_keystring)
        parse_ignore ()
        parse_string (":")
        parse_ignore ()
        parse_either ([s_Array, s_Object, l_string, l_emptyString, l_number])
        parse_ignore ()
        accept ()
    except:
        raise Exception("Fail")

def l_string ():
    begin ()
    try:
        parse_string ('"')
        parse_upto ('"')
        parse_string ('"')
        accept ()
    except:
        fail ()
        
def l_keystring ():
    begin ()
    try:
        parse (l_string)
        accept ()
    except:
        fail ()

def l_keystring ():
    begin ()
    try:
        parse_string ("\"")
        parse_string ("\"")
        accept ()
    except:
        fail ()

def l_number ():
    begin ()
    try:
        parse_range ("0", "9")
        accept ()
    except:
        fail ()


stest = '[ "name" : "hello" ]'
buffer = StringBuffer (stest)
try:
    parse (s_Main)
    print (f'\nsuccess')
except:
    print (f'\nfail {buffer.buff [buffer.bx:]}')
