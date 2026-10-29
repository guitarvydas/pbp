# Manual encoding of the following simple grammar, in Python

# d2j {
#   Main = Array
#   Array = "[" (KV | Object)+ "]"
#   Object = "{" KV+ "}"
#   KV = keystring ":" (Array | Object | string | emptyString | number)
#   string = "\"" (~"\"" any)+ "\""
#   keystring = string
#   emptyString = "\"" "\""
#   number = digit+
#   space += ","
# }



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
        raise

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
        raise

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
        raise

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
        raise

def l_string ():
    begin ("l_string")
    try:
        parse_string ('"')
        parse_upto ('"')
        parse_string ('"')
        accept ("l_string")
    except Exception as e:
        fail ("l_string")
        raise
        
def l_keystring ():
    begin ("l_keystring")
    try:
        parse (l_string)
        accept ("l_keystring")
    except Exception as e:
        fail ("l_keystring")
        raise

def l_emptyString ():
    begin ("l_emptystring")
    try:
        parse_string ("\"")
        parse_string ("\"")
        accept ("l_emptystring")
    except Exception as e:
        fail ("l_emptystring")
        raise

def l_number ():
    begin ("l_number")
    try:
        parse_one_or_more (lambda: parse_range ("0", "9"))
        accept ("l_number")
    except Exception as e:
        fail ("l_number")
        raise


with open("test.json", "r", encoding="utf-8") as f:
    stest = f.read()
    parse_set_input (stest)
    begin ("test")
    try:
        parse (s_Array)
        accept ("test")
        print (f'success')
    except Exception as e:
        fail ("test")
        print (f'fail')

        
