#line 1
def load_error (s):                                    #line 2
    global load_errors                                 #line 3
    print ( s, file=sys.stderr)                        #line 4
                                                       #line 5
    load_errors =  True                                #line 6#line 7#line 8

def runtime_error (s):                                 #line 9
    global runtime_errors                              #line 10
    print ( s, file=sys.stderr)                        #line 11
    exit (1)                                           #line 12
    runtime_errors =  True                             #line 13#line 14#line 15
                                                       #line 16
def initialize_component_palette_from_files (diagram_source_files):#line 17
    reg = make_component_registry ()                   #line 18
    for diagram_source in  diagram_source_files:       #line 19
        all_containers_within_single_file = lnet2internal_from_file ( diagram_source)#line 20
        for container in  all_containers_within_single_file:#line 21
            register_component ( reg,mkTemplate ( container [ "name"], container, container_instantiator))#line 22#line 23#line 24
    initialize_stock_components ( reg)                 #line 25
    return  reg                                        #line 26#line 27#line 28

def initialize_component_palette_from_string (lnet):   #line 29
    reg = make_component_registry ()                   #line 30
    all_containers = lnet2internal_from_string ( lnet) #line 31
    for container in  all_containers:                  #line 32
        register_component ( reg,mkTemplate ( container [ "name"], container, container_instantiator))#line 33#line 34
    initialize_stock_components ( reg)                 #line 35
    return  reg                                        #line 36#line 37

def initialize_from_files (diagram_names):             #line 38
    arg =  None                                        #line 39
    palette = initialize_component_palette_from_files ( diagram_names)#line 40
    return [ palette,[ diagram_names, arg]]            #line 41#line 42#line 43

def initialize_from_string ():                         #line 44
    arg =  None                                        #line 45
    palette = initialize_component_palette_from_string ()#line 46
    return [ palette,[ None, arg]]                     #line 47#line 48#line 49

def start (arg,part_name,palette,env):                 #line 50
    part = start_bare ( part_name, palette, env)       #line 51
    inject ( part, "", arg)                            #line 52
    finalize ( part)                                   #line 53#line 54#line 55

def start_bare (part_name,palette,env):                #line 56
    diagram_names =  env [ 0]                          #line 57
    # get entrypoint container                         #line 58
    part = get_component_instance ( palette, part_name, None)#line 59
    if  None ==  part:                                 #line 60
        load_error ( str( "Couldn;t find container with page name /") +  str( part_name) +  str( "/ in files ") +  str(str ( diagram_names)) +  " (check tab names, or disable compression?)"    )#line 64#line 65
    return  part                                       #line 66#line 67#line 68

def inject (part,port,payload):                        #line 69
    if not  load_errors:                               #line 70
        d =  Datum ()                                  #line 71
        d.v =  payload                                 #line 72
        d.clone =  lambda : obj_clone ( d)             #line 73
        d.reclaim =  None                              #line 74
        mev = make_mevent ( port, d)                   #line 75
        inject_mevent ( part, mev)                     #line 76
    else:                                              #line 77
        exit (1)                                       #line 78#line 79#line 80#line 81

def finalize (part):                                   #line 82
    print (deque_to_json ( part.outq))                 #line 83#line 84#line 85

def new_datum_bang ():                                 #line 86
    d =  Datum ()                                      #line 87
    d.v =  "!"                                         #line 88
    d.clone =  lambda : obj_clone ( d)                 #line 89
    d.reclaim =  None                                  #line 90
    return  d                                          #line 91#line 92
