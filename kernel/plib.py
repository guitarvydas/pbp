## builtins
def debug ():
    return False

class StringBuffer ():
    def __init__ (self):
        self.buff = ""
        self.bx = 0
        self.marks = []
    def set_input (self, s):
        self.buff = s
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
            bifail (e)
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
            raise


class Indentation ():
    def __init__ (self):
        self.indent = 0
        self.prefix = ''
    def ind (self):
        self.indent += 1
    def out (self):
        self.indent -= 1
    def pr (self):
        if debug ():
            print (" " * self.indent, end="")
            print (self.prefix, end="")
        pass

    
buffer = StringBuffer ()
indent = Indentation ()

def parse_debug_prefix (s):
    global indent
    indent.prefix = s
    
def fail (s):
    global buffer
    global indent
    indent.out ()
    indent.pr ()
    if debug ():
        print (f'fail {s}')
    buffer.rewind ()

def begin (s):
    global buffer
    global indent
    indent.pr ()
    indent.ind ()
    if debug ():
        print (f'begin {s} {buffer.buff[buffer.bx:]}')
    buffer.push_mark ()

def accept (s):
    global buffer
    global indent
    indent.out ()
    indent.pr ()
    prev = buffer.pop_mark ()
    if debug ():
        print (f'accept {s}  {buffer.buff[prev:buffer.bx]}')

def bifail (e):
    raise Exception (f'Builtin Fail {e}')

def parse (f):
    try:
        f ()
    except:
        raise

def parse_string (s):
    global buffer
    i = 0
    try:
        while i < len (s):
            try:
                buffer.parse_char (s [i])
                i += 1
            except:
                raise
    except:
        raise
    
def parse_upto (c):
    global buffer
    while True:
        if buffer.overrun ():
            bifail ("overrun")
        else:
            if c == buffer.look ():
                break
            else:
                buffer.accept_char ()
                
def parse_range (first, last):
    global buffer
    try:
        if buffer.look () >= first and buffer.look () <= last:
            buffer.accept_char ()
        else:
            raise
    except:
        raise

def parse_ignore ():
    while (buffer.is_space ()):
        buffer.accept_char ()

def parse_one_or_more (f):
    parse_debug_prefix ('+')
    try:
        parse (f)
        while True:
            try:
                parse (f)
            except:
                break
    except:
        parse_debug_prefix ('')
        raise
    parse_debug_prefix ('')
    
def parse_either (flist):
    parse_debug_prefix ('|')
    while True:
        try:
            f = flist [0]
            parse (f)
            break
        except:
            if flist:
                if len (flist) > 0:
                    flist = flist [1:]
                else:
                    parse_debug_prefix ('')
                    raise
            else:
                parse_debug_prefix ('')
                raise
    parse_debug_prefix ('')

def parse_set_input (s):
    buffer.set_input (s)
