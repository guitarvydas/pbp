load_errors =  False                                   #line 1
runtime_errors =  False                                #line 2
ticktime =  0                                          #line 3#line 4
def load_error (s):                                    #line 5
    global load_errors                                 #line 6
    print ( s, file=sys.stderr)                        #line 7
                                                       #line 8
    load_errors =  True                                #line 9#line 10#line 11

def runtime_error (s):                                 #line 12
    global runtime_errors                              #line 13
    print ( s, file=sys.stderr)                        #line 14
    exit (1)                                           #line 15
    runtime_errors =  True                             #line 16#line 17#line 18
                                                       #line 19
def initialize_component_palette_from_files (diagram_source_files):#line 20
    reg = make_component_registry ()                   #line 21
    for diagram_source in  diagram_source_files:       #line 22
        all_containers_within_single_file = lnet2internal_from_file ( diagram_source)#line 23
        for container in  all_containers_within_single_file:#line 24
            register_component ( reg,mkTemplate ( container [ "name"], container, container_instantiator))#line 25#line 26#line 27
    initialize_stock_components ( reg)                 #line 28
    return  reg                                        #line 29#line 30#line 31

def initialize_component_palette_from_string (lnet):   #line 32
    reg = make_component_registry ()                   #line 33
    all_containers = lnet2internal_from_string ( lnet) #line 34
    for container in  all_containers:                  #line 35
        register_component ( reg,mkTemplate ( container [ "name"], container, container_instantiator))#line 36#line 37
    initialize_stock_components ( reg)                 #line 38
    return  reg                                        #line 39#line 40

def initialize_from_files (diagram_names):             #line 41
    arg =  None                                        #line 42
    palette = initialize_component_palette_from_files ( diagram_names)#line 43
    return [ palette,[ diagram_names, arg]]            #line 44#line 45#line 46

def initialize_from_string ():                         #line 47
    arg =  None                                        #line 48
    palette = initialize_component_palette_from_string ()#line 49
    return [ palette,[ None, arg]]                     #line 50#line 51#line 52

def start (arg,part_name,palette,env):                 #line 53
    part = start_bare ( part_name, palette, env)       #line 54
    inject ( part, "", arg)                            #line 55
    finalize ( part)                                   #line 56#line 57#line 58

def start_bare (part_name,palette,env):                #line 59
    diagram_names =  env [ 0]                          #line 60
    # get entrypoint container                         #line 61
    part = get_component_instance ( palette, part_name, None)#line 62
    if  None ==  part:                                 #line 63
        load_error ( str( "Couldn;t find container with page name /") +  str( part_name) +  str( "/ in files ") +  str(str ( diagram_names)) +  " (check tab names, or disable compression?)"    )#line 67#line 68
    return  part                                       #line 69#line 70#line 71

def inject (part,port,payload):                        #line 72
    if not  load_errors:                               #line 73
        d =  Datum ()                                  #line 74
        d.v =  payload                                 #line 75
        d.clone =  lambda : obj_clone ( d)             #line 76
        d.reclaim =  None                              #line 77
        mev = make_mevent ( port, d)                   #line 78
        inject_mevent ( part, mev)                     #line 79
    else:                                              #line 80
        exit (1)                                       #line 81#line 82#line 83#line 84

def finalize (part):                                   #line 85
    print (deque_to_json ( part.outq))                 #line 86#line 87#line 88

def new_datum_bang ():                                 #line 89
    d =  Datum ()                                      #line 90
    d.v =  "!"                                         #line 91
    d.clone =  lambda : obj_clone ( d)                 #line 92
    d.reclaim =  None                                  #line 93
    return  d                                          #line 94#line 95
