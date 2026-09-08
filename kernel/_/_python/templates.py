def mkTemplate (name,template_data,instantiator):      #line 1
    templ =  Template ()                               #line 2
    templ.name =  name                                 #line 3
    templ.template_data =  template_data               #line 4
    templ.instantiator =  instantiator                 #line 5
    return  templ                                      #line 6#line 7#line 8
                                                       #line 9
# convert a little-network to internal form (an object data structure created by json parser) ... #line 10
# the actual data structure depends on the json parser library used by the target language #line 11
# the form of the data structure doesn;t matter here, as long as we use lookup operators "@" in this .rt code #line 12#line 13
# ... by reading the little-net from an external file  #line 14
def lnet2internal_from_file (container_xml):           #line 15
    pathname = os.getenv('PBPWD', '<none>')            #line 16
    filename =  os.path.basename ( container_xml)      #line 17

    try:
        fil = open(filename, "r")
        json_data = fil.read()
        routings = json.loads(json_data)
        fil.close ()
        return routings
    except FileNotFoundError:
        print (f"File not found: '{filename}'", file=sys.stderr)
        return None
    except json.JSONDecodeError as e:
        print (f"Error decoding JSON in path /{pathname}/: '{e}'", file=sys.stderr)
        return None
                                                       #line 18#line 19#line 20

# ... by reading the little-net from an embedded string (an aspect of creating t2t tool code) #line 21
def lnet2internal_from_string (lnet):                  #line 22

    try:
        routings = json.loads(lnet)
        return routings
    except json.JSONDecodeError as e:
        print ("Error decoding JSON from string 'lnet': '{e}'")
        return None
                                                       #line 23#line 24#line 25

def delete_decls (d):                                  #line 26
    pass                                               #line 27#line 28#line 29

def make_component_registry ():                        #line 30
    return  Component_Registry ()                      #line 31#line 32#line 33

def register_component (reg,template):
    return abstracted_register_component ( reg, template, False)#line 34

def register_component_allow_overwriting (reg,template):
    return abstracted_register_component ( reg, template, True)#line 35#line 36

def abstracted_register_component (reg,template,ok_to_overwrite):#line 37
    name = mangle_name ( template.name)                #line 38
    if  reg!= None and  name in  reg.templates and not  ok_to_overwrite:#line 39
        load_error ( str( "Component /") +  str( template.name) +  "/ already declared"  )#line 40
        return  reg                                    #line 41
    else:                                              #line 42
        reg.templates [name] =  template               #line 43
        return  reg                                    #line 44#line 45#line 46#line 47

def get_component_instance (reg,full_name,owner):      #line 48
    # If a part name begins with ":", it is treated as a JIT part and we let the runtime factory generate it on-the-fly (see kernel_external.rt and external.rt) else it is assumed to be a regular AOT part and assumed to have been registered before runtime, so we just pull its template out of the registry and instantiate it. #line 49
    # ":?<string>" is a probe part that is tagged with <string> #line 50
    # ":$ <command>" is a shell-out part that sends <command> to the operating system shell #line 51
    # ":<string>" else, it's just treated as a string part that produces <string> on its output #line 52
    template_name = mangle_name ( full_name)           #line 53
    if  ":" ==   full_name[0] :                        #line 54
        instance_name = generate_instance_name ( owner, template_name)#line 55
        instance = jit_instantiate ( reg, owner, instance_name, full_name)#line 56
        return  instance                               #line 57
    else:                                              #line 58
        if  template_name in  reg.templates:           #line 59
            template =  reg.templates [template_name]  #line 60
            if ( template ==  None):                   #line 61
                load_error ( str( "Registry Error (A): Can't find component /") +  str( template_name) +  "/"  )#line 62
                return  None                           #line 63
            else:                                      #line 64
                instance_name = generate_instance_name ( owner, template_name)#line 65
                instance =  template.instantiator ( reg, owner, instance_name, template.template_data, "")#line 66
                return  instance                       #line 67#line 68
        else:                                          #line 69
            load_error ( str( "Registry Error (B): Can't find component /") +  str( template_name) +  "/"  )#line 70
            return  None                               #line 71#line 72#line 73#line 74#line 75

def generate_instance_name (owner,template_name):      #line 76
    owner_name =  ""                                   #line 77
    instance_name =  template_name                     #line 78
    if  None!= owner:                                  #line 79
        owner_name =  owner.name                       #line 80
        instance_name =  str( owner_name) +  str( "▹") +  template_name  #line 81
    else:                                              #line 82
        instance_name =  template_name                 #line 83#line 84
    return  instance_name                              #line 85#line 86#line 87

def mangle_name (s):                                   #line 88
    # trim name to remove code from Container component names _ deferred until later (or never)#line 89
    return  s                                          #line 90#line 91
