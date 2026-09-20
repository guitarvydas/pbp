load_errors =  True                                    #line 1
runtime_errors =  True                                 #line 2#line 3
def load_error (s):                                    #line 4
    global load_errors                                 #line 5
    print ( s, file=sys.stderr)                        #line 6
                                                       #line 7
    load_errors =  True                                #line 8#line 9#line 10

def runtime_error (s):                                 #line 11
    global runtime_errors                              #line 12
    print ( s, file=sys.stderr)                        #line 13
    exit (1)                                           #line 14
    runtime_errors =  True                             #line 15#line 16#line 17
                                                       #line 18
def initialize_component_palette_from_files (diagram_source_files):#line 19
    reg = make_component_registry ()                   #line 20
    for diagram_source in  diagram_source_files:       #line 21
        all_containers_within_single_file = lnet2internal_from_file ( diagram_source)#line 22
        for container in  all_containers_within_single_file:#line 23
            register_component ( reg,mkTemplate ( container [ "name"], container, container_instantiator))#line 24#line 25#line 26
    initialize_stock_components ( reg)                 #line 27
    return  reg                                        #line 28#line 29#line 30

def initialize_component_palette_from_string (lnet):   #line 31
    reg = make_component_registry ()                   #line 32
    all_containers = lnet2internal_from_string ( lnet) #line 33
    for container in  all_containers:                  #line 34
        register_component ( reg,mkTemplate ( container [ "name"], container, container_instantiator))#line 35#line 36
    initialize_stock_components ( reg)                 #line 37
    return  reg                                        #line 38#line 39

def initialize_from_files (diagram_names):             #line 40
    arg =  None                                        #line 41
    palette = initialize_component_palette_from_files ( diagram_names)#line 42
    return [ palette,[ diagram_names, arg]]            #line 43#line 44#line 45

def initialize_from_string ():                         #line 46
    arg =  None                                        #line 47
    palette = initialize_component_palette_from_string ()#line 48
    return [ palette,[ None, arg]]                     #line 49#line 50#line 51

def start (arg,part_name,palette,env):                 #line 52
    part = start_bare ( part_name, palette, env)       #line 53
    inject ( part, "", arg)                            #line 54
    finalize ( part)                                   #line 55#line 56#line 57

def start_bare (part_name,palette,env):                #line 58
    diagram_names =  env [ 0]                          #line 59
    # get entrypoint container                         #line 60
    part = get_component_instance ( palette, part_name, None)#line 61
    if  None ==  part:                                 #line 62
        load_error ( str( "Couldn;t find container with page name /") +  str( part_name) +  str( "/ in files ") +  str(str ( diagram_names)) +  " (check tab names, or disable compression?)"    )#line 66#line 67
    return  part                                       #line 68#line 69#line 70

def inject (part,port,payload):                        #line 71
    if not  load_errors:                               #line 72
        d =  Datum ()                                  #line 73
        d.v =  payload                                 #line 74
        d.clone =  lambda : obj_clone ( d)             #line 75
        d.reclaim =  None                              #line 76
        mev = make_mevent ( port, d)                   #line 77
        inject_mevent ( part, mev)                     #line 78
    else:                                              #line 79
        exit (1)                                       #line 80#line 81#line 82#line 83

def finalize (part):                                   #line 84
    print (deque_to_json ( part.outq))                 #line 85#line 86#line 87

def new_datum_bang ():                                 #line 88
    d =  Datum ()                                      #line 89
    d.v =  "!"                                         #line 90
    d.clone =  lambda : obj_clone ( d)                 #line 91
    d.reclaim =  None                                  #line 92
    return  d                                          #line 93#line 94
