from plib import parse, StringBuffer, fail, accept, begin, parse_ignore, parse_string, parse_one_or_more, parse_either, parse_upto, parse_range, parse_set_input

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
    except Exception as e:
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
    except Exception as e:
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
    except Exception as e:
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
        fail ("s_KV")

def l_string ():
    begin ("l_string")
    try:
        parse_string ('"')
        parse_upto ('"')
        parse_string ('"')
        accept ("l_string")
    except Exception as e:
        fail ("l_string")
        
def l_keystring ():
    begin ("l_keystring")
    try:
        parse (l_string)
        accept ("l_keystring")
    except Exception as e:
        fail ("l_keystring")

def l_emptyString ():
    begin ("l_emptystring")
    try:
        parse_string ("\"")
        parse_string ("\"")
        accept ("l_emptystring")
    except Exception as e:
        fail ("l_emptystring")

def l_number ():
    begin ("l_number")
    try:
        parse_range ("0", "9")
        accept ("l_number")
    except Exception as e:
        fail ("l_number")


stest = '"name" : "hello" ]'
parse_set_input (stest)
begin ("test")
try:
    parse (s_KV)
    # parse_ignore ()
    # print (f'c')
    # parse_string (":")
    # print (f'd')
    # parse_ignore ()
    # print (f'e')
    # parse (l_string)
    # print (f'f')
    # parse_ignore ()
    # print (f'g')
    # parse_string ("]")
    # print (f'h')
    accept ("test")
    print (f'\nsuccess')
except Exception as e:
    fail ("test")
    print (f'\nfail {e}')
