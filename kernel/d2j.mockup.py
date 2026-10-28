
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
        self.bx += 1
    def overrun (self):
        return self.bx >= len (self.buff)
    def is_space (self):
        if self.overrun ():
            return False
        elif self.buff [self.bx] == " " or self.buff [self.bx] == "," or self.buff [self.bx] == "\n" :
            return True
    def look (self):
        if self.overrun ():
            bifail ()
        else:
            return self.buff [self.bx]
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


class Indentation ():
    def __init__ (self):
        self.indent = 0
    def ind (self):
        self.indent += 1
    def out (self):
        self.indent -= 1
    def pr (self):
        print (" " * self.indent, end="")
        
buffer = StringBuffer ("")
indent = Indentation ()

def fail (s):
    global buffer
    global indent
    indent.out ()
    indent.pr ()
    print (f'fail {s}')
    buffer.rewind ()
    raise Exception ("Parse Failure")

def begin (s):
    global buffer
    global indent
    indent.pr ()
    indent.ind ()
    print (f'begin {s}')
    buffer.push_mark ()

def accept (s):
    global buffer
    global indent
    indent.out ()
    indent.pr ()
    prev = buffer.pop_mark ()
    print (f'accept {s} {buffer.buff[prev:buffer.bx]}')

def bifail ():
    raise Exception ("Builtin Fail")

def parse (f):
    try:
        f ()
    except:
        bifail ()

def parse_string (s):
    global buffer
    i = 0
    try:
        while i < len (s):
            try:
                buffer.parse_char (s [i])
                i += 1
            except:
                bifail ()
    except:
        bifail ()
    
def parse_upto (c):
    global buffer
    while True:
        if buffer.overrun ():
            bifail ()
        else:
            if c == buffer.look ():
                return True
            else:
                buffer.accept_char ()

def parse_range (first, last):
    global buffer
    try:
        if buffer.next_char >= first and buffer.next_char <= last:
            buffer.accept_char ()
        else:
            bifail ()
    except:
        bifail ()

def parse_ignore ():
    while (buffer.is_space ()):
        buffer.accept_char ()

def parse_one_or_more (f):
    try:
        parse (f)
        while True:
            try:
                parse (f)
            except:
                pass
    except:
        bifail ()
    
def parse_either (flist):
    print (f'either {flist}')
    while True:
        try:
            f = flist [0]
            print (f'either trying {f.__name__}')
            parse (f)
            return
        except:
            if flist:
                flist = flist [1:]
            else:
                bifail ()

    


##########
## Parser
##########
def s_Main ():
    begin ("s_Main")
    try:
        parse_ignore ()
        s_Array ()
        parse_ignore ()
        accept ("s_Main")
    except:
        fail ("s_Main")

def s_Array ():
    begin ("s_Array")
    try:
        parse_ignore ()
        parse_string ("[")
        parse_ignore ()
        parse_one_or_more (lambda : parse_either ([s_KV, s_Object]))
        parse_ignore ()
        parse_string ("]")
        parse_ignore ()
        accept ("s_Array")
    except:
        fail ("s_Array")

def s_Object ():
    begin ("s_Object")
    try:
        parse_ignore ()
        parse_string ("{")
        parse_ignore ()
        parse_one_or_more (s_KV)
        parse_ignore ()
        parse_string ("}")
        parse_ignore ()
        accept ("s_Object")
    except:
        fail ("s_Object")

def s_KV ():
    begin ("s_KV")
    try:
        parse_ignore ()
        parse (l_keystring)
        parse_ignore ()
        parse_string (":")
        parse_ignore ()
        parse_either ([s_Array, s_Object, l_emptyString, l_string, l_number])
        parse_ignore ()
        accept ("s_KV")
    except Exception as e:
        print(e)
        fail ("s_KV")

def l_string ():
    begin ("l_string")
    try:
        parse_string ('"')
        parse_upto ('"')
        parse_string ('"')
        accept ("l_string")
    except:
        fail ("l_string")
        
def l_keystring ():
    begin ("l_keystring")
    try:
        parse (l_string)
        accept ("l_keystring")
    except:
        fail ("l_keystring")

def l_emptyString ():
    begin ("l_emptystring")
    try:
        parse_string ("\"")
        parse_string ("\"")
        accept ("l_emptystring")
    except:
        fail ("l_emptystring")

def l_number ():
    begin ("l_number")
    try:
        parse_range ("0", "9")
        accept ("l_number")
    except:
        fail ("l_number")


stest = '"name" : "hello"'
buffer = StringBuffer (stest)
try:
    parse (s_KV)
    print (f'\nsuccess')
except:
    print (f'\nfail {buffer.buff [buffer.bx:]}')
