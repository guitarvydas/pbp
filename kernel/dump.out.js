{
  _global: {
    '❲enumDown❳': {
      name: '❲enumDown❳',
      indir: 1,
      type: 'Dir',
      scope: '_global',
      kind: 'variable'
    },
    '❲enumAcross❳': {
      name: '❲enumAcross❳',
      indir: 1,
      type: 'Dir',
      scope: '_global',
      kind: 'variable'
    },
    '❲enumUp❳': {
      name: '❲enumUp❳',
      indir: 1,
      type: 'Dir',
      scope: '_global',
      kind: 'variable'
    },
    '❲enumThrough❳': {
      name: '❲enumThrough❳',
      indir: 1,
      type: 'Dir',
      scope: '_global',
      kind: 'variable'
    },
    '❲Connector❳': {
      name: '❲Connector❳',
      indir: 1,
      type: 'obj',
      scope: '_global',
      kind: 'obj'
    },
    '❲Sender❳': {
      name: '❲Sender❳',
      indir: 1,
      type: 'obj',
      scope: '_global',
      kind: 'obj'
    },
    '❲Receiver❳': {
      name: '❲Receiver❳',
      indir: 1,
      type: 'obj',
      scope: '_global',
      kind: 'obj'
    },
    '❲mkSender❳': {
      name: '❲mkSender❳',
      indir: 2,
      type: 'Sender',
      kind: 'retproc',
      scope: '_global'
    },
    '❲mkReceiver❳': {
      name: '❲mkReceiver❳',
      indir: 2,
      type: 'Receiver',
      kind: 'retproc',
      scope: '_global'
    },
    '❲create_down_connector❳': {
      name: '❲create_down_connector❳',
      indir: 2,
      type: 'Wire',
      kind: 'retproc',
      scope: '_global'
    },
    '❲create_across_connector❳': {
      name: '❲create_across_connector❳',
      indir: 2,
      type: 'Wire',
      kind: 'retproc',
      scope: '_global'
    },
    '❲create_up_connector❳': {
      name: '❲create_up_connector❳',
      indir: 2,
      type: 'Wire',
      kind: 'retproc',
      scope: '_global'
    },
    '❲create_through_connector❳': {
      name: '❲create_through_connector❳',
      indir: 2,
      type: 'Wire',
      kind: 'retproc',
      scope: '_global'
    },
    '❲container_instantiator❳': {
      name: '❲container_instantiator❳',
      indir: 2,
      type: 'Container',
      kind: 'retproc',
      scope: '_global'
    },
    '❲container_handler❳': {
      name: '❲container_handler❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲container_reset_children❳': {
      name: '❲container_reset_children❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲destroy_container❳': {
      name: '❲destroy_container❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲sender_eq❳': {
      name: '❲sender_eq❳',
      indir: 1,
      type: 'BOOL',
      kind: 'retproc',
      scope: '_global'
    },
    '❲deposit❳': {
      name: '❲deposit❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲force_tick❳': {
      name: '❲force_tick❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲push_mevent❳': {
      name: '❲push_mevent❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲is_self❳': {
      name: '❲is_self❳',
      indir: 1,
      type: 'Bool',
      kind: 'retproc',
      scope: '_global'
    },
    '❲step_child_once❳': {
      name: '❲step_child_once❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲step_children❳': {
      name: '❲step_children❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲attempt_tick❳': {
      name: '❲attempt_tick❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲is_tick❳': {
      name: '❲is_tick❳',
      indir: 1,
      type: 'Bool',
      kind: 'retproc',
      scope: '_global'
    },
    '❲route❳': {
      name: '❲route❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲any_child_ready❳': {
      name: '❲any_child_ready❳',
      indir: 1,
      type: 'Bool',
      kind: 'retproc',
      scope: '_global'
    },
    '❲child_is_ready❳': {
      name: '❲child_is_ready❳',
      indir: 1,
      type: 'Bool',
      kind: 'retproc',
      scope: '_global'
    },
    '❲append_routing_descriptor❳': {
      name: '❲append_routing_descriptor❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲make_container❳': {
      name: '❲make_container❳',
      indir: 2,
      type: 'Container',
      kind: 'retproc',
      scope: '_global'
    },
    '❲send❳': {
      name: '❲send❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲forward❳': {
      name: '❲forward❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲inject_mevent❳': {
      name: '❲inject_mevent❳',
      indir: 2,
      type: 'Mevent',
      kind: 'retproc',
      scope: '_global'
    },
    '❲set_active❳': {
      name: '❲set_active❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲set_idle❳': {
      name: '❲set_idle❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲put_output❳': {
      name: '❲put_output❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲clone_payload❳': {
      name: '❲clone_payload❳',
      indir: 2,
      type: 'Payload',
      kind: 'retproc',
      scope: '_global'
    },
    '❲Eh❳': {
      name: '❲Eh❳',
      indir: 1,
      type: 'obj',
      scope: '_global',
      kind: 'obj'
    },
    '❲injector❳': {
      name: '❲injector❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲digits❳': {
      name: '❲digits❳',
      indir: 1,
      type: 'Array_of_Str',
      scope: '_global',
      kind: 'variable'
    },
    '❲subscripted_digit❳': {
      name: '❲subscripted_digit❳',
      indir: 2,
      type: 'Str',
      kind: 'retproc',
      scope: '_global'
    },
    '❲counter❳': {
      name: '❲counter❳',
      indir: 1,
      type: 'Int',
      scope: '_global',
      kind: 'variable'
    },
    '❲gensymbol❳': {
      name: '❲gensymbol❳',
      indir: 2,
      type: 'Str',
      kind: 'retproc',
      scope: '_global'
    },
    '❲jit_instantiate❳': {
      name: '❲jit_instantiate❳',
      indir: 2,
      type: 'Part',
      kind: 'retproc',
      scope: '_global'
    },
    '❲handle_jit❳': {
      name: '❲handle_jit❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲probe_handler❳': {
      name: '❲probe_handler❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    'shell_out_handler❳': {
      name: 'shell_out_handler❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲make_leaf❳': {
      name: '❲make_leaf❳',
      indir: 2,
      type: 'Leaf',
      kind: 'retproc',
      scope: '_global'
    },
    '❲leaf_reset❳': {
      name: '❲leaf_reset❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲Datum❳': {
      name: '❲Datum❳',
      indir: 1,
      type: 'obj',
      scope: '_global',
      kind: 'obj'
    },
    '❲Mevent❳': {
      name: '❲Mevent❳',
      indir: 1,
      type: 'obj',
      scope: '_global',
      kind: 'obj'
    },
    '❲clone_port❳': {
      name: '❲clone_port❳',
      indir: 1,
      type: 'Port',
      kind: 'retproc',
      scope: '_global'
    },
    '❲make_mevent❳': {
      name: '❲make_mevent❳',
      indir: 2,
      type: 'Mevent',
      kind: 'retproc',
      scope: '_global'
    },
    '❲mevent_clone❳': {
      name: '❲mevent_clone❳',
      indir: 2,
      type: 'Mevent',
      kind: 'retproc',
      scope: '_global'
    },
    '❲destroy_mevent❳': {
      name: '❲destroy_mevent❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲destroy_datum❳': {
      name: '❲destroy_datum❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲destroy_port❳': {
      name: '❲destroy_port❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲format_mevent❳': {
      name: '❲format_mevent❳',
      indir: 2,
      type: 'Str',
      kind: 'retproc',
      scope: '_global'
    },
    '❲format_mevent_raw❳': {
      name: '❲format_mevent_raw❳',
      indir: 2,
      type: 'Str',
      kind: 'retproc',
      scope: '_global'
    },
    'load_errors❳': {
      name: 'load_errors❳',
      indir: 1,
      type: 'Bool',
      scope: '_global',
      kind: 'variable'
    },
    'runtime_errors❳': {
      name: 'runtime_errors❳',
      indir: 1,
      type: 'Bool',
      scope: '_global',
      kind: 'variable'
    },
    '❲load_error❳': {
      name: '❲load_error❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲runtime_error❳': {
      name: '❲runtime_error❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲initialize_component_palette_from_files❳': {
      name: '❲initialize_component_palette_from_files❳',
      indir: 2,
      type: 'Component_Registry',
      kind: 'retproc',
      scope: '_global'
    },
    '❲initialize_component_palette_from_string❳': {
      name: '❲initialize_component_palette_from_string❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲initialize_from_files❳': {
      name: '❲initialize_from_files❳',
      indir: 2,
      type: 'Tuple_Palette_DiagramNames_ArgStr',
      kind: 'retproc',
      scope: '_global'
    },
    '❲initialize_from_string❳': {
      name: '❲initialize_from_string❳',
      indir: 2,
      type: 'Tuple_Palette_DiagramNames_ArgStr',
      kind: 'retproc',
      scope: '_global'
    },
    '❲start❳': {
      name: '❲start❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    'start_bare❳': {
      name: 'start_bare❳',
      indir: 2,
      type: 'Part',
      kind: 'retproc',
      scope: '_global'
    },
    '❲inject❳': {
      name: '❲inject❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲finalize❳': {
      name: '❲finalize❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲new_datum_bang❳': {
      name: '❲new_datum_bang❳',
      indir: 2,
      type: 'Datum',
      kind: 'retproc',
      scope: '_global'
    },
    '❲trash_instantiate❳': {
      name: '❲trash_instantiate❳',
      indir: 2,
      type: 'Leaf',
      kind: 'retproc',
      scope: '_global'
    },
    'trash_handler❳': {
      name: 'trash_handler❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲TwoMevents❳': {
      name: '❲TwoMevents❳',
      indir: 1,
      type: 'obj',
      scope: '_global',
      kind: 'obj'
    },
    '❲Deracer_Instance_Data❳': {
      name: '❲Deracer_Instance_Data❳',
      indir: 1,
      type: 'obj',
      scope: '_global',
      kind: 'obj'
    },
    '❲reclaim_Buffers_from_heap❳': {
      name: '❲reclaim_Buffers_from_heap❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲deracer_reset_handler❳': {
      name: '❲deracer_reset_handler❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲deracer_instantiate❳': {
      name: '❲deracer_instantiate❳',
      indir: 2,
      type: 'Leaf',
      kind: 'retproc',
      scope: '_global'
    },
    'send_firstmev_then_secondmev❳': {
      name: 'send_firstmev_then_secondmev❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲deracer_handler❳': {
      name: '❲deracer_handler❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲low_level_read_text_file_instantiate❳': {
      name: '❲low_level_read_text_file_instantiate❳',
      indir: 2,
      type: 'Leaf',
      kind: 'retproc',
      scope: '_global'
    },
    '❲low_level_read_text_file_handler❳': {
      name: '❲low_level_read_text_file_handler❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲ensure_string_datum_instantiate❳': {
      name: '❲ensure_string_datum_instantiate❳',
      indir: 2,
      type: 'Leaf',
      kind: 'retproc',
      scope: '_global'
    },
    '❲ensure_string_datum_handler❳': {
      name: '❲ensure_string_datum_handler❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲Syncfilewrite_Data❳': {
      name: '❲Syncfilewrite_Data❳',
      indir: 1,
      type: 'obj',
      scope: '_global',
      kind: 'obj'
    },
    '❲syncfilewrite_reset_handler❳': {
      name: '❲syncfilewrite_reset_handler❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲syncfilewrite_instantiate❳': {
      name: '❲syncfilewrite_instantiate❳',
      indir: 2,
      type: 'Leaf',
      kind: 'retproc',
      scope: '_global'
    },
    '❲syncfilewrite_handler❳': {
      name: '❲syncfilewrite_handler❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲StringConcat_Instance_Data❳': {
      name: '❲StringConcat_Instance_Data❳',
      indir: 1,
      type: 'obj',
      scope: '_global',
      kind: 'obj'
    },
    '❲stringconcat_reset_handler❳': {
      name: '❲stringconcat_reset_handler❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲stringconcat_instantiate❳': {
      name: '❲stringconcat_instantiate❳',
      indir: 2,
      type: 'Leaf',
      kind: 'retproc',
      scope: '_global'
    },
    '❲stringconcat_handler❳': {
      name: '❲stringconcat_handler❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲maybe_stringconcat❳': {
      name: '❲maybe_stringconcat❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲string_constant_instantiate❳': {
      name: '❲string_constant_instantiate❳',
      indir: 2,
      type: 'Leaf',
      kind: 'retproc',
      scope: '_global'
    },
    '❲string_constant_handler❳': {
      name: '❲string_constant_handler❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲fakepipename_instantiate❳': {
      name: '❲fakepipename_instantiate❳',
      indir: 2,
      type: 'Leaf',
      kind: 'retproc',
      scope: '_global'
    },
    '❲rand❳': {
      name: '❲rand❳',
      indir: 1,
      type: 'Number',
      scope: '_global',
      kind: 'variable'
    },
    '❲fakepipename_handler❳': {
      name: '❲fakepipename_handler❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲Switch1star_Instance_Data❳': {
      name: '❲Switch1star_Instance_Data❳',
      indir: 1,
      type: 'obj',
      scope: '_global',
      kind: 'obj'
    },
    '❲switch1star_reset_handler❳': {
      name: '❲switch1star_reset_handler❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲switch1star_instantiate❳': {
      name: '❲switch1star_instantiate❳',
      indir: 2,
      type: 'Leaf',
      kind: 'retproc',
      scope: '_global'
    },
    '❲switch1star_handler❳': {
      name: '❲switch1star_handler❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲StringAccumulator❳': {
      name: '❲StringAccumulator❳',
      indir: 1,
      type: 'obj',
      scope: '_global',
      kind: 'obj'
    },
    '❲strcatstar_reset_handler❳': {
      name: '❲strcatstar_reset_handler❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲strcatstar_instantiate❳': {
      name: '❲strcatstar_instantiate❳',
      indir: 2,
      type: 'Leaf',
      kind: 'retproc',
      scope: '_global'
    },
    '❲strcatstar_handler❳': {
      name: '❲strcatstar_handler❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲stop_instantiate❳': {
      name: '❲stop_instantiate❳',
      indir: 2,
      type: 'Leaf',
      kind: 'retproc',
      scope: '_global'
    },
    '❲stop_handler❳': {
      name: '❲stop_handler❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲initialize_stock_components❳': {
      name: '❲initialize_stock_components❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲Component_Registry❳': {
      name: '❲Component_Registry❳',
      indir: 1,
      type: 'obj',
      scope: '_global',
      kind: 'obj'
    },
    '❲Template❳': {
      name: '❲Template❳',
      indir: 1,
      type: 'obj',
      scope: '_global',
      kind: 'obj'
    },
    '❲mkTemplate❳': {
      name: '❲mkTemplate❳',
      indir: 2,
      type: 'Template',
      kind: 'retproc',
      scope: '_global'
    },
    '❲lnet2internal_from_file❳': {
      name: '❲lnet2internal_from_file❳',
      indir: 2,
      type: 'Collection_of_Wire',
      kind: 'retproc',
      scope: '_global'
    },
    '❲lnet2internal_from_string❳': {
      name: '❲lnet2internal_from_string❳',
      indir: 2,
      type: 'Collection_of_Wire',
      kind: 'retproc',
      scope: '_global'
    },
    '❲make_component_registry❳': {
      name: '❲make_component_registry❳',
      indir: 2,
      type: 'Component_Registry',
      kind: 'retproc',
      scope: '_global'
    },
    '❲register_component❳': {
      name: '❲register_component❳',
      indir: 2,
      type: 'Component_Registry',
      kind: 'retproc',
      scope: '_global'
    },
    '❲register_component_allow_overwriting❳': {
      name: '❲register_component_allow_overwriting❳',
      indir: 2,
      type: 'Component_Registry',
      kind: 'retproc',
      scope: '_global'
    },
    '❲abstracted_register_component❳': {
      name: '❲abstracted_register_component❳',
      indir: 0,
      type: 'void',
      kind: 'proc',
      scope: '_global'
    },
    '❲get_component_instance❳': {
      name: '❲get_component_instance❳',
      indir: 2,
      type: 'Part',
      kind: 'retproc',
      scope: '_global'
    },
    '❲generate_instance_name❳': {
      name: '❲generate_instance_name❳',
      indir: 2,
      type: 'Str',
      kind: 'retproc',
      scope: '_global'
    },
    '❲mangle_name❳': {
      name: '❲mangle_name❳',
      indir: 2,
      type: 'Str',
      kind: 'retproc',
      scope: '_global'
    }
  },
  '❲Connector❳': {
    '❲direction❳': {
      name: '❲direction❳',
      indir: 1,
      type: 'Dir',
      scope: '❲Connector❳',
      kind: 'variable'
    },
    '❲sender❳': {
      name: '❲sender❳',
      indir: 2,
      type: 'Sender',
      scope: '❲Connector❳',
      kind: 'variable'
    },
    '❲receiver❳': {
      name: '❲receiver❳',
      indir: 2,
      type: 'Receiver',
      scope: '❲Connector❳',
      kind: 'variable'
    }
  },
  '❲Sender❳': {
    '❲name❳': {
      name: '❲name❳',
      indir: 2,
      type: 'Str',
      scope: '❲Sender❳',
      kind: 'variable'
    },
    '❲component❳': {
      name: '❲component❳',
      indir: 2,
      type: 'Eh',
      scope: '❲Sender❳',
      kind: 'variable'
    },
    '❲port❳': {
      name: '❲port❳',
      indir: 1,
      type: 'Port',
      scope: '❲Sender❳',
      kind: 'variable'
    }
  },
  '❲Receiver❳': {
    '❲name❳': {
      name: '❲name❳',
      indir: 2,
      type: 'Str',
      scope: '❲Receiver❳',
      kind: 'variable'
    },
    '❲queue❳': {
      name: '❲queue❳',
      indir: 2,
      type: 'Queue',
      scope: '❲Receiver❳',
      kind: 'variable'
    },
    '❲port❳': {
      name: '❲port❳',
      indir: 1,
      type: 'Port',
      scope: '❲Receiver❳',
      kind: 'variable'
    },
    '❲component❳': {
      name: '❲component❳',
      indir: 2,
      type: 'Eh',
      scope: '❲Receiver❳',
      kind: 'variable'
    }
  },
  '❲mkSender❳': {
    '❲name❳': {
      name: '❲name❳',
      indir: 2,
      type: 'Str',
      scope: '❲mkSender❳',
      kind: 'parameter'
    },
    '❲component❳': {
      name: '❲component❳',
      indir: 2,
      type: 'Eh',
      scope: '❲mkSender❳',
      kind: 'parameter'
    },
    '❲port❳': {
      name: '❲port❳',
      indir: 1,
      type: 'Port',
      scope: '❲mkSender❳',
      kind: 'parameter'
    },
    '❲s❳': {
      name: '❲s❳',
      indir: 2,
      type: 'Sender',
      scope: '❲mkSender❳',
      kind: 'variable'
    }
  },
  '❲mkReceiver❳': {
    '❲name❳': {
      name: '❲name❳',
      indir: 2,
      type: 'Str',
      scope: '❲mkReceiver❳',
      kind: 'parameter'
    },
    '❲component❳': {
      name: '❲component❳',
      indir: 2,
      type: 'Eh',
      scope: '❲mkReceiver❳',
      kind: 'parameter'
    },
    '❲port❳': {
      name: '❲port❳',
      indir: 1,
      type: 'Port',
      scope: '❲mkReceiver❳',
      kind: 'parameter'
    },
    '❲q❳': {
      name: '❲q❳',
      indir: 2,
      type: 'Queue',
      scope: '❲mkReceiver❳',
      kind: 'parameter'
    },
    '❲r❳': {
      name: '❲r❳',
      indir: 2,
      type: 'Receiver',
      scope: '❲mkReceiver❳',
      kind: 'variable'
    }
  },
  '❲create_down_connector❳': {
    '❲container❳': {
      name: '❲container❳',
      indir: 2,
      type: 'Container',
      scope: '❲create_down_connector❳',
      kind: 'parameter'
    },
    '❲proto_conn❳': {
      name: '❲proto_conn❳',
      indir: 2,
      type: 'Wire_Proto',
      scope: '❲create_down_connector❳',
      kind: 'parameter'
    },
    '❲connectors❳': {
      name: '❲connectors❳',
      indir: 2,
      type: 'Collection_of_Wire',
      scope: '❲create_down_connector❳',
      kind: 'parameter'
    },
    '❲children_by_id❳': {
      name: '❲children_by_id❳',
      indir: 2,
      type: 'Table_by_ID_of_Part',
      scope: '❲create_down_connector❳',
      kind: 'parameter'
    },
    '❲connector❳': {
      name: '❲connector❳',
      indir: 2,
      type: 'Wire',
      scope: '❲create_down_connector❳',
      kind: 'variable'
    },
    '❲target_proto❳': {
      name: '❲target_proto❳',
      indir: 2,
      type: 'Part',
      scope: '❲create_down_connector❳',
      kind: 'variable'
    },
    '❲id_proto❳': {
      name: '❲id_proto❳',
      indir: 1,
      type: 'ID',
      scope: '❲create_down_connector❳',
      kind: 'variable'
    },
    '❲target_component❳': {
      name: '❲target_component❳',
      indir: 2,
      type: 'Part',
      scope: '❲create_down_connector❳',
      kind: 'variable'
    }
  },
  '❲create_across_connector❳': {
    '❲container❳': {
      name: '❲container❳',
      indir: 2,
      type: 'Container',
      scope: '❲create_across_connector❳',
      kind: 'parameter'
    },
    '❲proto_conn❳': {
      name: '❲proto_conn❳',
      indir: 2,
      type: 'Wire_Proto',
      scope: '❲create_across_connector❳',
      kind: 'parameter'
    },
    '❲connectors❳': {
      name: '❲connectors❳',
      indir: 2,
      type: 'Collection_of_Wire',
      scope: '❲create_across_connector❳',
      kind: 'parameter'
    },
    '❲children_by_id❳': {
      name: '❲children_by_id❳',
      indir: 2,
      type: 'Table_by_ID_of_Part',
      scope: '❲create_across_connector❳',
      kind: 'parameter'
    },
    '❲connector❳': {
      name: '❲connector❳',
      indir: 2,
      type: 'Wire',
      scope: '❲create_across_connector❳',
      kind: 'variable'
    },
    '❲source_component❳': {
      name: '❲source_component❳',
      indir: 2,
      type: 'Part',
      scope: '❲create_across_connector❳',
      kind: 'variable'
    },
    '❲target_component❳': {
      name: '❲target_component❳',
      indir: 2,
      type: 'Part',
      scope: '❲create_across_connector❳',
      kind: 'variable'
    }
  },
  '❲create_up_connector❳': {
    '❲container❳': {
      name: '❲container❳',
      indir: 2,
      type: 'Container',
      scope: '❲create_up_connector❳',
      kind: 'parameter'
    },
    '❲proto_conn❳': {
      name: '❲proto_conn❳',
      indir: 2,
      type: 'Wire_Proto',
      scope: '❲create_up_connector❳',
      kind: 'parameter'
    },
    '❲connectors❳': {
      name: '❲connectors❳',
      indir: 2,
      type: 'Collection_of_Wire',
      scope: '❲create_up_connector❳',
      kind: 'parameter'
    },
    '❲children_by_id❳': {
      name: '❲children_by_id❳',
      indir: 2,
      type: 'Table_by_ID_of_Part',
      scope: '❲create_up_connector❳',
      kind: 'parameter'
    },
    '❲connector❳': {
      name: '❲connector❳',
      indir: 2,
      type: 'Wire',
      scope: '❲create_up_connector❳',
      kind: 'variable'
    },
    '❲source_component❳': {
      name: '❲source_component❳',
      indir: 2,
      type: 'Part',
      scope: '❲create_up_connector❳',
      kind: 'variable'
    }
  },
  '❲create_through_connector❳': {
    '❲container❳': {
      name: '❲container❳',
      indir: 2,
      type: 'Container',
      scope: '❲create_through_connector❳',
      kind: 'parameter'
    },
    '❲proto_conn❳': {
      name: '❲proto_conn❳',
      indir: 2,
      type: 'Wire_Proto',
      scope: '❲create_through_connector❳',
      kind: 'parameter'
    },
    '❲connectors❳': {
      name: '❲connectors❳',
      indir: 2,
      type: 'Collection_of_Wire',
      scope: '❲create_through_connector❳',
      kind: 'parameter'
    },
    '❲children_by_id❳': {
      name: '❲children_by_id❳',
      indir: 2,
      type: 'Table_by_ID_of_Part',
      scope: '❲create_through_connector❳',
      kind: 'parameter'
    },
    '❲connector❳': {
      name: '❲connector❳',
      indir: 2,
      type: 'Wire',
      scope: '❲create_through_connector❳',
      kind: 'variable'
    }
  },
  '❲container_instantiator❳': {
    '❲reg❳': {
      name: '❲reg❳',
      indir: 2,
      type: 'Component_Registry',
      scope: '❲container_instantiator❳',
      kind: 'parameter'
    },
    '❲owner❳': {
      name: '❲owner❳',
      indir: 2,
      type: 'Container',
      scope: '❲container_instantiator❳',
      kind: 'parameter'
    },
    '❲container_name❳': {
      name: '❲container_name❳',
      indir: 2,
      type: 'Str',
      scope: '❲container_instantiator❳',
      kind: 'parameter'
    },
    '❲desc❳': {
      name: '❲desc❳',
      indir: 2,
      type: 'Container_Template',
      scope: '❲container_instantiator❳',
      kind: 'parameter'
    },
    '❲arg❳': {
      name: '❲arg❳',
      indir: 2,
      type: 'Str',
      scope: '❲container_instantiator❳',
      kind: 'parameter'
    }
  },
  '❲container_handler❳': {
    '❲container❳': {
      name: '❲container❳',
      indir: 2,
      type: 'Container',
      scope: '❲container_handler❳',
      kind: 'parameter'
    },
    '❲mevent❳': {
      name: '❲mevent❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲container_handler❳',
      kind: 'parameter'
    }
  },
  '❲container_reset_children❳': {
    '❲container❳': {
      name: '❲container❳',
      indir: 2,
      type: 'Container',
      scope: '❲container_reset_children❳',
      kind: 'parameter'
    }
  },
  '❲destroy_container❳': {
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Part',
      scope: '❲destroy_container❳',
      kind: 'parameter'
    }
  },
  '❲sender_eq❳': {
    '❲s1❳': {
      name: '❲s1❳',
      indir: 2,
      type: 'Part',
      scope: '❲sender_eq❳',
      kind: 'parameter'
    },
    '❲s2❳': {
      name: '❲s2❳',
      indir: 2,
      type: 'Part',
      scope: '❲sender_eq❳',
      kind: 'parameter'
    },
    '❲same_components❳': {
      name: '❲same_components❳',
      indir: 1,
      type: 'BOOL',
      scope: '❲sender_eq❳',
      kind: 'variable'
    },
    '❲same_ports❳': {
      name: '❲same_ports❳',
      indir: 1,
      type: 'BOOL',
      scope: '❲sender_eq❳',
      kind: 'variable'
    }
  },
  '❲deposit❳': {
    '❲parent❳': {
      name: '❲parent❳',
      indir: 2,
      type: 'Container',
      scope: '❲deposit❳',
      kind: 'parameter'
    },
    '❲conn❳': {
      name: '❲conn❳',
      indir: 2,
      type: 'Wire',
      scope: '❲deposit❳',
      kind: 'parameter'
    },
    '❲mevent❳': {
      name: '❲mevent❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲deposit❳',
      kind: 'parameter'
    },
    '❲new_mevent❳': {
      name: '❲new_mevent❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲deposit❳',
      kind: 'variable'
    }
  },
  '❲force_tick❳': {
    '❲parent❳': {
      name: '❲parent❳',
      indir: 2,
      type: 'Container',
      scope: '❲force_tick❳',
      kind: 'parameter'
    },
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Part',
      scope: '❲force_tick❳',
      kind: 'parameter'
    },
    '❲tick_mev❳': {
      name: '❲tick_mev❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲force_tick❳',
      kind: 'variable'
    }
  },
  '❲push_mevent❳': {
    '❲parent❳': {
      name: '❲parent❳',
      indir: 2,
      type: 'Container',
      scope: '❲push_mevent❳',
      kind: 'parameter'
    },
    '❲receiver❳': {
      name: '❲receiver❳',
      indir: 2,
      type: 'Part',
      scope: '❲push_mevent❳',
      kind: 'parameter'
    },
    '❲inq❳': {
      name: '❲inq❳',
      indir: 2,
      type: 'Queue',
      scope: '❲push_mevent❳',
      kind: 'parameter'
    },
    '❲m❳': {
      name: '❲m❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲push_mevent❳',
      kind: 'parameter'
    }
  },
  '❲is_self❳': {
    '❲child❳': {
      name: '❲child❳',
      indir: 2,
      type: 'Part',
      scope: '❲is_self❳',
      kind: 'parameter'
    },
    '❲container❳': {
      name: '❲container❳',
      indir: 2,
      type: 'Container',
      scope: '❲is_self❳',
      kind: 'parameter'
    }
  },
  '❲step_child_once❳': {
    '❲child❳': {
      name: '❲child❳',
      indir: 2,
      type: 'Part',
      scope: '❲step_child_once❳',
      kind: 'parameter'
    },
    '❲mev❳': {
      name: '❲mev❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲step_child_once❳',
      kind: 'parameter'
    }
  },
  '❲step_children❳': {
    '❲container❳': {
      name: '❲container❳',
      indir: 2,
      type: 'Container',
      scope: '❲step_children❳',
      kind: 'parameter'
    },
    '❲causingMevent❳': {
      name: '❲causingMevent❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲step_children❳',
      kind: 'parameter'
    },
    '❲child❳': {
      name: '❲child❳',
      indir: 2,
      type: 'Part',
      scope: '❲step_children❳',
      kind: 'variable'
    },
    '❲mev❳': {
      name: '❲mev❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲step_children❳',
      kind: 'variable'
    }
  },
  '❲attempt_tick❳': {
    '❲parent❳': {
      name: '❲parent❳',
      indir: 2,
      type: 'Container',
      scope: '❲attempt_tick❳',
      kind: 'parameter'
    },
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Part',
      scope: '❲attempt_tick❳',
      kind: 'parameter'
    }
  },
  '❲is_tick❳': {
    '❲mev❳': {
      name: '❲mev❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲is_tick❳',
      kind: 'parameter'
    }
  },
  '❲route❳': {
    '❲container❳': {
      name: '❲container❳',
      indir: 2,
      type: 'Container',
      scope: '❲route❳',
      kind: 'parameter'
    },
    '❲from_component❳': {
      name: '❲from_component❳',
      indir: 2,
      type: 'Part',
      scope: '❲route❳',
      kind: 'parameter'
    },
    '❲mevent❳': {
      name: '❲mevent❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲route❳',
      kind: 'parameter'
    },
    '❲was_sent❳': {
      name: '❲was_sent❳',
      indir: 1,
      type: 'Bool',
      scope: '❲route❳',
      kind: 'variable'
    },
    '❲fromname❳': {
      name: '❲fromname❳',
      indir: 2,
      type: 'Str',
      scope: '❲route❳',
      kind: 'variable'
    }
  },
  '❲any_child_ready❳': {
    '❲container❳': {
      name: '❲container❳',
      indir: 2,
      type: 'Container',
      scope: '❲any_child_ready❳',
      kind: 'parameter'
    },
    '❲child❳': {
      name: '❲child❳',
      indir: 2,
      type: 'Part',
      scope: '❲any_child_ready❳',
      kind: 'variable'
    }
  },
  '❲child_is_ready❳': {
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Part',
      scope: '❲child_is_ready❳',
      kind: 'parameter'
    }
  },
  '❲append_routing_descriptor❳': {
    '❲container❳': {
      name: '❲container❳',
      indir: 2,
      type: 'Container',
      scope: '❲append_routing_descriptor❳',
      kind: 'parameter'
    },
    '❲desc❳': {
      name: '❲desc❳',
      indir: 2,
      type: 'Wire',
      scope: '❲append_routing_descriptor❳',
      kind: 'parameter'
    }
  },
  '❲make_container❳': {
    '❲name❳': {
      name: '❲name❳',
      indir: 2,
      type: 'Str',
      scope: '❲make_container❳',
      kind: 'parameter'
    },
    '❲owner❳': {
      name: '❲owner❳',
      indir: 2,
      type: 'Container',
      scope: '❲make_container❳',
      kind: 'parameter'
    },
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Container',
      scope: '❲make_container❳',
      kind: 'variable'
    }
  },
  '❲send❳': {
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Part',
      scope: '❲send❳',
      kind: 'parameter'
    },
    '❲port❳': {
      name: '❲port❳',
      indir: 1,
      type: 'Port',
      scope: '❲send❳',
      kind: 'parameter'
    },
    '❲obj❳': {
      name: '❲obj❳',
      indir: 2,
      type: 'Part',
      scope: '❲send❳',
      kind: 'parameter'
    },
    '❲causingMevent❳': {
      name: '❲causingMevent❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲send❳',
      kind: 'parameter'
    },
    '❲d❳': {
      name: '❲d❳',
      indir: 2,
      type: 'Payload',
      scope: '❲send❳',
      kind: 'variable'
    },
    '❲mev❳': {
      name: '❲mev❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲send❳',
      kind: 'variable'
    }
  },
  '❲forward❳': {
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Part',
      scope: '❲forward❳',
      kind: 'parameter'
    },
    '❲port❳': {
      name: '❲port❳',
      indir: 1,
      type: 'Port',
      scope: '❲forward❳',
      kind: 'parameter'
    },
    '❲mev❳': {
      name: '❲mev❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲forward❳',
      kind: 'parameter'
    },
    '❲fwdmev❳': {
      name: '❲fwdmev❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲forward❳',
      kind: 'variable'
    }
  },
  '❲inject_mevent❳': {
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Part',
      scope: '❲inject_mevent❳',
      kind: 'parameter'
    },
    '❲mev❳': {
      name: '❲mev❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲inject_mevent❳',
      kind: 'parameter'
    }
  },
  '❲set_active❳': {
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Part',
      scope: '❲set_active❳',
      kind: 'parameter'
    }
  },
  '❲set_idle❳': {
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Part',
      scope: '❲set_idle❳',
      kind: 'parameter'
    }
  },
  '❲put_output❳': {
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Part',
      scope: '❲put_output❳',
      kind: 'parameter'
    },
    '❲mev❳': {
      name: '❲mev❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲put_output❳',
      kind: 'parameter'
    }
  },
  '❲clone_payload❳': {
    '❲clone_payload❳': {
      name: '❲clone_payload❳',
      indir: 2,
      type: 'Payload',
      scope: '❲clone_payload❳',
      kind: 'parameter'
    }
  },
  '❲Eh❳': {
    '❲name❳': {
      name: '❲name❳',
      indir: 2,
      type: 'Str',
      scope: '❲Eh❳',
      kind: 'variable'
    },
    '❲inq❳': {
      name: '❲inq❳',
      indir: 2,
      type: 'Queue',
      scope: '❲Eh❳',
      kind: 'variable'
    },
    '❲outq❳': {
      name: '❲outq❳',
      indir: 2,
      type: 'Queue',
      scope: '❲Eh❳',
      kind: 'variable'
    },
    '❲owner❳': {
      name: '❲owner❳',
      indir: 2,
      type: 'Container',
      scope: '❲Eh❳',
      kind: 'variable'
    },
    '❲children❳': {
      name: '❲children❳',
      indir: 2,
      type: 'Collection_of_Part',
      scope: '❲Eh❳',
      kind: 'variable'
    },
    '❲visit_ordering❳': {
      name: '❲visit_ordering❳',
      indir: 2,
      type: 'Queue_of_Part',
      scope: '❲Eh❳',
      kind: 'variable'
    },
    '❲connections❳': {
      name: '❲connections❳',
      indir: 2,
      type: 'Collection_ofWire',
      scope: '❲Eh❳',
      kind: 'variable'
    },
    '❲handler❳': {
      name: '❲handler❳',
      indir: 1,
      type: 'Fhandler',
      scope: '❲Eh❳',
      kind: 'variable'
    },
    '❲finject❳': {
      name: '❲finject❳',
      indir: 1,
      type: 'Finject',
      scope: '❲Eh❳',
      kind: 'variable'
    },
    '❲reset❳': {
      name: '❲reset❳',
      indir: 1,
      type: 'Freset',
      scope: '❲Eh❳',
      kind: 'variable'
    },
    '❲instance_data❳': {
      name: '❲instance_data❳',
      indir: 2,
      type: 'any',
      scope: '❲Eh❳',
      kind: 'variable'
    },
    '❲arg❳': {
      name: '❲arg❳',
      indir: 2,
      type: 'Str',
      scope: '❲Eh❳',
      kind: 'variable'
    },
    '❲state❳': {
      name: '❲state❳',
      indir: 2,
      type: 'Str',
      scope: '❲Eh❳',
      kind: 'variable'
    },
    '❲special❳': {
      name: '❲special❳',
      indir: 1,
      type: 'Bool',
      scope: '❲Eh❳',
      kind: 'variable'
    }
  },
  '❲injector❳': {
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Eh',
      scope: '❲injector❳',
      kind: 'parameter'
    },
    '❲mevent❳': {
      name: '❲mevent❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲injector❳',
      kind: 'parameter'
    }
  },
  '❲subscripted_digit❳': {
    '❲n❳': {
      name: '❲n❳',
      indir: 1,
      type: 'Int',
      scope: '❲subscripted_digit❳',
      kind: 'parameter'
    }
  },
  '❲gensymbol❳': {
    '❲s❳': {
      name: '❲s❳',
      indir: 2,
      type: 'Str',
      scope: '❲gensymbol❳',
      kind: 'parameter'
    }
  },
  '❲jit_instantiate❳': {
    '❲reg❳': {
      name: '❲reg❳',
      indir: 2,
      type: 'Component_Registry',
      scope: '❲jit_instantiate❳',
      kind: 'parameter'
    },
    '❲owner❳': {
      name: '❲owner❳',
      indir: 2,
      type: 'Container',
      scope: '❲jit_instantiate❳',
      kind: 'parameter'
    },
    '❲name❳': {
      name: '❲name❳',
      indir: 2,
      type: 'Str',
      scope: '❲jit_instantiate❳',
      kind: 'parameter'
    },
    '❲arg❳': {
      name: '❲arg❳',
      indir: 2,
      type: 'Str',
      scope: '❲jit_instantiate❳',
      kind: 'parameter'
    },
    '❲name_with_id❳': {
      name: '❲name_with_id❳',
      indir: 2,
      type: 'Str',
      scope: '❲jit_instantiate❳',
      kind: 'variable'
    },
    '❲inst❳': {
      name: '❲inst❳',
      indir: 2,
      type: 'Part',
      scope: '❲jit_instantiate❳',
      kind: 'variable'
    },
    '❲firstc❳': {
      name: '❲firstc❳',
      indir: 1,
      type: 'Char',
      scope: '❲jit_instantiate❳',
      kind: 'variable'
    }
  },
  '❲handle_jit❳': {
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Part',
      scope: '❲handle_jit❳',
      kind: 'parameter'
    },
    '❲mev❳': {
      name: '❲mev❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲handle_jit❳',
      kind: 'parameter'
    },
    '❲s❳': {
      name: '❲s❳',
      indir: 2,
      type: 'Str',
      scope: '❲handle_jit❳',
      kind: 'variable'
    },
    '❲firstc❳': {
      name: '❲firstc❳',
      indir: 1,
      type: 'Char',
      scope: '❲handle_jit❳',
      kind: 'variable'
    }
  },
  '❲probe_handler❳': {
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Part',
      scope: '❲probe_handler❳',
      kind: 'parameter'
    },
    '❲tag❳': {
      name: '❲tag❳',
      indir: 1,
      type: 'Port',
      scope: '❲probe_handler❳',
      kind: 'parameter'
    },
    '❲mev❳': {
      name: '❲mev❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲probe_handler❳',
      kind: 'parameter'
    },
    '❲s❳': {
      name: '❲s❳',
      indir: 2,
      type: 'Str',
      scope: '❲probe_handler❳',
      kind: 'variable'
    }
  },
  'shell_out_handler❳': {
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Part',
      scope: 'shell_out_handler❳',
      kind: 'parameter'
    },
    '❲tag❳': {
      name: '❲tag❳',
      indir: 1,
      type: 'Port',
      scope: 'shell_out_handler❳',
      kind: 'parameter'
    },
    '❲mev❳': {
      name: '❲mev❳',
      indir: 2,
      type: 'Mevent',
      scope: 'shell_out_handler❳',
      kind: 'parameter'
    },
    '❲s❳': {
      name: '❲s❳',
      indir: 2,
      type: 'Str',
      scope: 'shell_out_handler❳',
      kind: 'variable'
    },
    '❲ret❳': {
      name: '❲ret❳',
      indir: 1,
      type: 'Int',
      scope: 'shell_out_handler❳',
      kind: 'variable'
    },
    '❲rc❳': {
      name: '❲rc❳',
      indir: 1,
      type: 'Int',
      scope: 'shell_out_handler❳',
      kind: 'variable'
    },
    '❲stdout❳': {
      name: '❲stdout❳',
      indir: 2,
      type: 'Str',
      scope: 'shell_out_handler❳',
      kind: 'variable'
    },
    '❲stderr❳': {
      name: '❲stderr❳',
      indir: 2,
      type: 'Str',
      scope: 'shell_out_handler❳',
      kind: 'variable'
    },
    '❲command❳': {
      name: '❲command❳',
      indir: 2,
      type: 'Str',
      scope: 'shell_out_handler❳',
      kind: 'variable'
    },
    '❲pbpRoot❳': {
      name: '❲pbpRoot❳',
      indir: 2,
      type: 'Pathname',
      scope: 'shell_out_handler❳',
      kind: 'variable'
    }
  },
  '❲make_leaf❳': {
    '❲name❳': {
      name: '❲name❳',
      indir: 2,
      type: 'Str',
      scope: '❲make_leaf❳',
      kind: 'parameter'
    },
    '❲owner❳': {
      name: '❲owner❳',
      indir: 2,
      type: 'Container',
      scope: '❲make_leaf❳',
      kind: 'parameter'
    },
    '❲instance_data❳': {
      name: '❲instance_data❳',
      indir: 2,
      type: 'any',
      scope: '❲make_leaf❳',
      kind: 'parameter'
    },
    '❲arg❳': {
      name: '❲arg❳',
      indir: 2,
      type: 'Str',
      scope: '❲make_leaf❳',
      kind: 'parameter'
    },
    '❲handler❳': {
      name: '❲handler❳',
      indir: 1,
      type: 'Fhandler',
      scope: '❲make_leaf❳',
      kind: 'parameter'
    },
    '❲reset_handler❳': {
      name: '❲reset_handler❳',
      indir: 1,
      type: 'Freset',
      scope: '❲make_leaf❳',
      kind: 'parameter'
    },
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Leaf',
      scope: '❲make_leaf❳',
      kind: 'variable'
    },
    '❲nm❳': {
      name: '❲nm❳',
      indir: 2,
      type: 'Part',
      scope: '❲make_leaf❳',
      kind: 'variable'
    }
  },
  '❲leaf_reset❳': {
    '❲part❳': {
      name: '❲part❳',
      indir: 2,
      type: 'Leaf',
      scope: '❲leaf_reset❳',
      kind: 'parameter'
    }
  },
  '❲Datum❳': {
    '❲v❳': {
      name: '❲v❳',
      indir: 2,
      type: 'Payload',
      scope: '❲Datum❳',
      kind: 'variable'
    },
    '❲clone❳': {
      name: '❲clone❳',
      indir: 1,
      type: 'Fclone',
      scope: '❲Datum❳',
      kind: 'variable'
    },
    '❲reclaim❳': {
      name: '❲reclaim❳',
      indir: 1,
      type: 'Freclaim',
      scope: '❲Datum❳',
      kind: 'variable'
    },
    '❲other❳': {
      name: '❲other❳',
      indir: 2,
      type: 'any',
      scope: '❲Datum❳',
      kind: 'variable'
    }
  },
  '❲Mevent❳': {
    '❲port❳': {
      name: '❲port❳',
      indir: 1,
      type: 'Port',
      scope: '❲Mevent❳',
      kind: 'variable'
    },
    '❲payload❳': {
      name: '❲payload❳',
      indir: 2,
      type: 'Payload',
      scope: '❲Mevent❳',
      kind: 'variable'
    }
  },
  '❲clone_port❳': {
    '❲s❳': {
      name: '❲s❳',
      indir: 1,
      type: 'Port',
      scope: '❲clone_port❳',
      kind: 'parameter'
    }
  },
  '❲make_mevent❳': {
    '❲port❳': {
      name: '❲port❳',
      indir: 1,
      type: 'Port',
      scope: '❲make_mevent❳',
      kind: 'parameter'
    },
    '❲datum❳': {
      name: '❲datum❳',
      indir: 2,
      type: 'Datum',
      scope: '❲make_mevent❳',
      kind: 'parameter'
    },
    '❲p❳': {
      name: '❲p❳',
      indir: 1,
      type: 'Port',
      scope: '❲make_mevent❳',
      kind: 'variable'
    },
    '❲m❳': {
      name: '❲m❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲make_mevent❳',
      kind: 'variable'
    }
  },
  '❲mevent_clone❳': {
    '❲mev❳': {
      name: '❲mev❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲mevent_clone❳',
      kind: 'parameter'
    },
    '❲m❳': {
      name: '❲m❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲mevent_clone❳',
      kind: 'variable'
    }
  },
  '❲destroy_mevent❳': {
    '❲mev❳': {
      name: '❲mev❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲destroy_mevent❳',
      kind: 'parameter'
    }
  },
  '❲destroy_datum❳': {
    '❲mev❳': {
      name: '❲mev❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲destroy_datum❳',
      kind: 'parameter'
    }
  },
  '❲destroy_port❳': {
    '❲mev❳': {
      name: '❲mev❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲destroy_port❳',
      kind: 'parameter'
    }
  },
  '❲format_mevent❳': {
    '❲m❳': {
      name: '❲m❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲format_mevent❳',
      kind: 'parameter'
    }
  },
  '❲format_mevent_raw❳': {
    '❲m❳': {
      name: '❲m❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲format_mevent_raw❳',
      kind: 'parameter'
    }
  },
  '❲load_error❳': {
    '❲s❳': {
      name: '❲s❳',
      indir: 2,
      type: 'Str',
      scope: '❲load_error❳',
      kind: 'parameter'
    }
  },
  '❲runtime_error❳': {
    '❲s❳': {
      name: '❲s❳',
      indir: 2,
      type: 'Str',
      scope: '❲runtime_error❳',
      kind: 'parameter'
    }
  },
  '❲initialize_component_palette_from_files❳': {
    '❲diagram_source_files❳': {
      name: '❲diagram_source_files❳',
      indir: 2,
      type: 'Collection_of_Pathname',
      scope: '❲initialize_component_palette_from_files❳',
      kind: 'parameter'
    },
    '❲reg❳': {
      name: '❲reg❳',
      indir: 2,
      type: 'Component_Registry',
      scope: '❲initialize_component_palette_from_files❳',
      kind: 'variable'
    },
    '❲diagram_source❳': {
      name: '❲diagram_source❳',
      indir: 2,
      type: 'Pathname',
      scope: '❲initialize_component_palette_from_files❳',
      kind: 'variable'
    },
    '❲all_containers_within_single_file❳': {
      name: '❲all_containers_within_single_file❳',
      indir: 2,
      type: 'Collection_of_Container',
      scope: '❲initialize_component_palette_from_files❳',
      kind: 'variable'
    },
    '❲container❳': {
      name: '❲container❳',
      indir: 2,
      type: 'Container',
      scope: '❲initialize_component_palette_from_files❳',
      kind: 'variable'
    }
  },
  '❲initialize_component_palette_from_string❳': {
    '❲lnet❳': {
      name: '❲lnet❳',
      indir: 2,
      type: 'JSONStr',
      scope: '❲initialize_component_palette_from_string❳',
      kind: 'parameter'
    },
    '❲reg❳': {
      name: '❲reg❳',
      indir: 2,
      type: 'Component_Registry',
      scope: '❲initialize_component_palette_from_string❳',
      kind: 'variable'
    },
    '❲all_containers❳': {
      name: '❲all_containers❳',
      indir: 2,
      type: 'Collection_of_Container',
      scope: '❲initialize_component_palette_from_string❳',
      kind: 'variable'
    },
    '❲container❳': {
      name: '❲container❳',
      indir: 2,
      type: 'Container',
      scope: '❲initialize_component_palette_from_string❳',
      kind: 'variable'
    }
  },
  '❲initialize_from_files❳': {
    '❲diagram_names❳': {
      name: '❲diagram_names❳',
      indir: 2,
      type: 'Collection_of_DiagramsName',
      scope: '❲initialize_from_files❳',
      kind: 'parameter'
    },
    '❲arg❳': {
      name: '❲arg❳',
      indir: 2,
      type: 'Str',
      scope: '❲initialize_from_files❳',
      kind: 'variable'
    },
    '❲palette❳': {
      name: '❲palette❳',
      indir: 2,
      type: 'Component_Registry',
      scope: '❲initialize_from_files❳',
      kind: 'variable'
    }
  },
  '❲initialize_from_string❳': {
    '❲arg❳': {
      name: '❲arg❳',
      indir: 2,
      type: 'Str',
      scope: '❲initialize_from_string❳',
      kind: 'variable'
    },
    '❲palette❳': {
      name: '❲palette❳',
      indir: 2,
      type: 'Component_Registry',
      scope: '❲initialize_from_string❳',
      kind: 'variable'
    }
  },
  '❲start❳': {
    '❲arg❳': {
      name: '❲arg❳',
      indir: 2,
      type: 'Str',
      scope: '❲start❳',
      kind: 'parameter'
    },
    '❲part_name❳': {
      name: '❲part_name❳',
      indir: 2,
      type: 'Str',
      scope: '❲start❳',
      kind: 'parameter'
    },
    '❲palette❳': {
      name: '❲palette❳',
      indir: 2,
      type: 'Component_Registry',
      scope: '❲start❳',
      kind: 'parameter'
    },
    '❲env❳': {
      name: '❲env❳',
      indir: 2,
      type: 'Tuple_Palette_DiagramNames_ArgStr',
      scope: '❲start❳',
      kind: 'parameter'
    },
    '❲part❳': {
      name: '❲part❳',
      indir: 2,
      type: 'Part',
      scope: '❲start❳',
      kind: 'variable'
    }
  },
  'start_bare❳': {
    '❲part_name❳': {
      name: '❲part_name❳',
      indir: 2,
      type: 'Str',
      scope: 'start_bare❳',
      kind: 'parameter'
    },
    '❲palette❳': {
      name: '❲palette❳',
      indir: 2,
      type: 'Component_Registry',
      scope: 'start_bare❳',
      kind: 'parameter'
    },
    '❲env❳': {
      name: '❲env❳',
      indir: 2,
      type: 'Tuple_Palette_DiagramNames_ArgStr',
      scope: 'start_bare❳',
      kind: 'parameter'
    },
    '❲diagram_names❳': {
      name: '❲diagram_names❳',
      indir: 2,
      type: 'Collection_of_DiagramsName',
      scope: 'start_bare❳',
      kind: 'variable'
    },
    '❲part❳': {
      name: '❲part❳',
      indir: 2,
      type: 'Part',
      scope: 'start_bare❳',
      kind: 'variable'
    }
  },
  '❲inject❳': {
    '❲part❳': {
      name: '❲part❳',
      indir: 2,
      type: 'Part',
      scope: '❲inject❳',
      kind: 'parameter'
    },
    '❲port❳': {
      name: '❲port❳',
      indir: 1,
      type: 'Port',
      scope: '❲inject❳',
      kind: 'parameter'
    },
    '❲payload❳': {
      name: '❲payload❳',
      indir: 2,
      type: 'Payload',
      scope: '❲inject❳',
      kind: 'parameter'
    },
    '❲d❳': {
      name: '❲d❳',
      indir: 2,
      type: 'Datum',
      scope: '❲inject❳',
      kind: 'variable'
    },
    '❲mev❳': {
      name: '❲mev❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲inject❳',
      kind: 'variable'
    }
  },
  '❲finalize❳': {
    '❲part❳': {
      name: '❲part❳',
      indir: 2,
      type: 'Part',
      scope: '❲finalize❳',
      kind: 'parameter'
    }
  },
  '❲new_datum_bang❳': {
    '❲d❳': {
      name: '❲d❳',
      indir: 2,
      type: 'Datum',
      scope: '❲new_datum_bang❳',
      kind: 'variable'
    }
  },
  '❲trash_instantiate❳': {
    '❲reg❳': {
      name: '❲reg❳',
      indir: 2,
      type: 'Component_Registry',
      scope: '❲trash_instantiate❳',
      kind: 'parameter'
    },
    '❲owner❳': {
      name: '❲owner❳',
      indir: 2,
      type: 'Container',
      scope: '❲trash_instantiate❳',
      kind: 'parameter'
    },
    '❲name❳': {
      name: '❲name❳',
      indir: 2,
      type: 'Str',
      scope: '❲trash_instantiate❳',
      kind: 'parameter'
    },
    '❲template_data❳': {
      name: '❲template_data❳',
      indir: 2,
      type: 'Template',
      scope: '❲trash_instantiate❳',
      kind: 'parameter'
    },
    '❲arg❳': {
      name: '❲arg❳',
      indir: 2,
      type: 'Str',
      scope: '❲trash_instantiate❳',
      kind: 'parameter'
    },
    '❲name_with_id❳': {
      name: '❲name_with_id❳',
      indir: 2,
      type: 'Str',
      scope: '❲trash_instantiate❳',
      kind: 'variable'
    }
  },
  'trash_handler❳': {
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Part',
      scope: 'trash_handler❳',
      kind: 'parameter'
    },
    '❲mev❳': {
      name: '❲mev❳',
      indir: 2,
      type: 'Mevent',
      scope: 'trash_handler❳',
      kind: 'parameter'
    }
  },
  '❲TwoMevents❳': {
    '❲firstmev❳': {
      name: '❲firstmev❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲TwoMevents❳',
      kind: 'variable'
    },
    '❲secondmev❳': {
      name: '❲secondmev❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲TwoMevents❳',
      kind: 'variable'
    }
  },
  '❲Deracer_Instance_Data❳': {
    '❲state❳': {
      name: '❲state❳',
      indir: 1,
      type: 'State',
      scope: '❲Deracer_Instance_Data❳',
      kind: 'variable'
    },
    '❲buffer❳': {
      name: '❲buffer❳',
      indir: 2,
      type: 'TwoMevents',
      scope: '❲Deracer_Instance_Data❳',
      kind: 'variable'
    }
  },
  '❲reclaim_Buffers_from_heap❳': {
    '❲inst❳': {
      name: '❲inst❳',
      indir: 2,
      type: 'Leaf',
      scope: '❲reclaim_Buffers_from_heap❳',
      kind: 'parameter'
    }
  },
  '❲deracer_reset_handler❳': {
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Leaf',
      scope: '❲deracer_reset_handler❳',
      kind: 'parameter'
    },
    '❲inst❳': {
      name: '❲inst❳',
      indir: 2,
      type: 'Deracer_Instance_Data',
      scope: '❲deracer_reset_handler❳',
      kind: 'variable'
    }
  },
  '❲deracer_instantiate❳': {
    '❲reg❳': {
      name: '❲reg❳',
      indir: 2,
      type: 'Component_Registry',
      scope: '❲deracer_instantiate❳',
      kind: 'parameter'
    },
    '❲owner❳': {
      name: '❲owner❳',
      indir: 2,
      type: 'Container',
      scope: '❲deracer_instantiate❳',
      kind: 'parameter'
    },
    '❲name❳': {
      name: '❲name❳',
      indir: 2,
      type: 'Str',
      scope: '❲deracer_instantiate❳',
      kind: 'parameter'
    },
    '❲template_data❳': {
      name: '❲template_data❳',
      indir: 1,
      type: 'Ignored',
      scope: '❲deracer_instantiate❳',
      kind: 'parameter'
    },
    '❲arg❳': {
      name: '❲arg❳',
      indir: 1,
      type: 'Ignored',
      scope: '❲deracer_instantiate❳',
      kind: 'parameter'
    },
    '❲name_with_id❳': {
      name: '❲name_with_id❳',
      indir: 2,
      type: 'Str',
      scope: '❲deracer_instantiate❳',
      kind: 'variable'
    },
    '❲inst❳': {
      name: '❲inst❳',
      indir: 2,
      type: 'Deracer_Instance_Data',
      scope: '❲deracer_instantiate❳',
      kind: 'variable'
    },
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Leaf',
      scope: '❲deracer_instantiate❳',
      kind: 'variable'
    }
  },
  'send_firstmev_then_secondmev❳': {
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Leaf',
      scope: 'send_firstmev_then_secondmev❳',
      kind: 'parameter'
    },
    '❲inst❳': {
      name: '❲inst❳',
      indir: 2,
      type: 'Deracer_Instance_Data',
      scope: 'send_firstmev_then_secondmev❳',
      kind: 'parameter'
    }
  },
  '❲deracer_handler❳': {
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Leaf',
      scope: '❲deracer_handler❳',
      kind: 'parameter'
    },
    '❲mev❳': {
      name: '❲mev❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲deracer_handler❳',
      kind: 'parameter'
    },
    '❲inst❳': {
      name: '❲inst❳',
      indir: 2,
      type: 'Deracer_Instance_Data',
      scope: '❲deracer_handler❳',
      kind: 'variable'
    }
  },
  '❲low_level_read_text_file_instantiate❳': {
    '❲reg❳': {
      name: '❲reg❳',
      indir: 2,
      type: 'Component_Registry',
      scope: '❲low_level_read_text_file_instantiate❳',
      kind: 'parameter'
    },
    '❲owner❳': {
      name: '❲owner❳',
      indir: 2,
      type: 'Container',
      scope: '❲low_level_read_text_file_instantiate❳',
      kind: 'parameter'
    },
    '❲name❳': {
      name: '❲name❳',
      indir: 2,
      type: 'Str',
      scope: '❲low_level_read_text_file_instantiate❳',
      kind: 'parameter'
    },
    '❲template_data❳': {
      name: '❲template_data❳',
      indir: 2,
      type: 'Template',
      scope: '❲low_level_read_text_file_instantiate❳',
      kind: 'parameter'
    },
    '❲arg❳': {
      name: '❲arg❳',
      indir: 2,
      type: 'Str',
      scope: '❲low_level_read_text_file_instantiate❳',
      kind: 'parameter'
    },
    '❲name_with_id❳': {
      name: '❲name_with_id❳',
      indir: 2,
      type: 'Str',
      scope: '❲low_level_read_text_file_instantiate❳',
      kind: 'variable'
    }
  },
  '❲low_level_read_text_file_handler❳': {
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Leaf',
      scope: '❲low_level_read_text_file_handler❳',
      kind: 'parameter'
    },
    '❲mev❳': {
      name: '❲mev❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲low_level_read_text_file_handler❳',
      kind: 'parameter'
    },
    '❲fname❳': {
      name: '❲fname❳',
      indir: 2,
      type: 'Payload',
      scope: '❲low_level_read_text_file_handler❳',
      kind: 'variable'
    }
  },
  '❲ensure_string_datum_instantiate❳': {
    '❲reg❳': {
      name: '❲reg❳',
      indir: 2,
      type: 'Component_Registry',
      scope: '❲ensure_string_datum_instantiate❳',
      kind: 'parameter'
    },
    '❲owner❳': {
      name: '❲owner❳',
      indir: 2,
      type: 'Container',
      scope: '❲ensure_string_datum_instantiate❳',
      kind: 'parameter'
    },
    '❲name❳': {
      name: '❲name❳',
      indir: 2,
      type: 'Str',
      scope: '❲ensure_string_datum_instantiate❳',
      kind: 'parameter'
    },
    '❲template_data❳': {
      name: '❲template_data❳',
      indir: 1,
      type: 'Ignored',
      scope: '❲ensure_string_datum_instantiate❳',
      kind: 'parameter'
    },
    '❲arg❳': {
      name: '❲arg❳',
      indir: 1,
      type: 'Ignored',
      scope: '❲ensure_string_datum_instantiate❳',
      kind: 'parameter'
    },
    '❲name_with_id❳': {
      name: '❲name_with_id❳',
      indir: 2,
      type: 'Str',
      scope: '❲ensure_string_datum_instantiate❳',
      kind: 'variable'
    }
  },
  '❲ensure_string_datum_handler❳': {
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Leaf',
      scope: '❲ensure_string_datum_handler❳',
      kind: 'parameter'
    },
    '❲mev❳': {
      name: '❲mev❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲ensure_string_datum_handler❳',
      kind: 'parameter'
    },
    '❲emev❳': {
      name: '❲emev❳',
      indir: 2,
      type: 'Str',
      scope: '❲ensure_string_datum_handler❳',
      kind: 'variable'
    }
  },
  '❲Syncfilewrite_Data❳': {
    '❲filename❳': {
      name: '❲filename❳',
      indir: 2,
      type: 'Str',
      scope: '❲Syncfilewrite_Data❳',
      kind: 'variable'
    }
  },
  '❲syncfilewrite_reset_handler❳': {
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Leaf',
      scope: '❲syncfilewrite_reset_handler❳',
      kind: 'parameter'
    }
  },
  '❲syncfilewrite_instantiate❳': {
    '❲reg❳': {
      name: '❲reg❳',
      indir: 2,
      type: 'Component_Registry',
      scope: '❲syncfilewrite_instantiate❳',
      kind: 'parameter'
    },
    '❲owner❳': {
      name: '❲owner❳',
      indir: 2,
      type: 'Container',
      scope: '❲syncfilewrite_instantiate❳',
      kind: 'parameter'
    },
    '❲name❳': {
      name: '❲name❳',
      indir: 2,
      type: 'Str',
      scope: '❲syncfilewrite_instantiate❳',
      kind: 'parameter'
    },
    '❲template_data❳': {
      name: '❲template_data❳',
      indir: 1,
      type: 'Ignored',
      scope: '❲syncfilewrite_instantiate❳',
      kind: 'parameter'
    },
    '❲arg❳': {
      name: '❲arg❳',
      indir: 1,
      type: 'Ignored',
      scope: '❲syncfilewrite_instantiate❳',
      kind: 'parameter'
    },
    '❲name_with_id❳': {
      name: '❲name_with_id❳',
      indir: 2,
      type: 'Str',
      scope: '❲syncfilewrite_instantiate❳',
      kind: 'variable'
    },
    '❲inst❳': {
      name: '❲inst❳',
      indir: 2,
      type: 'SyncFilewrite_Data',
      scope: '❲syncfilewrite_instantiate❳',
      kind: 'variable'
    }
  },
  '❲syncfilewrite_handler❳': {
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Leaf',
      scope: '❲syncfilewrite_handler❳',
      kind: 'parameter'
    },
    '❲mev❳': {
      name: '❲mev❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲syncfilewrite_handler❳',
      kind: 'parameter'
    },
    '❲inst❳': {
      name: '❲inst❳',
      indir: 2,
      type: 'Synfilewrite_Data',
      scope: '❲syncfilewrite_handler❳',
      kind: 'variable'
    },
    '❲contents❳': {
      name: '❲contents❳',
      indir: 2,
      type: 'Payload',
      scope: '❲syncfilewrite_handler❳',
      kind: 'variable'
    },
    '❲f❳': {
      name: '❲f❳',
      indir: 2,
      type: 'FileDescriptor',
      scope: '❲syncfilewrite_handler❳',
      kind: 'variable'
    }
  },
  '❲StringConcat_Instance_Data❳': {
    '❲buffer1❳': {
      name: '❲buffer1❳',
      indir: 2,
      type: 'Str',
      scope: '❲StringConcat_Instance_Data❳',
      kind: 'variable'
    },
    '❲buffer2❳': {
      name: '❲buffer2❳',
      indir: 2,
      type: 'Str',
      scope: '❲StringConcat_Instance_Data❳',
      kind: 'variable'
    }
  },
  '❲stringconcat_reset_handler❳': {
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Leaf',
      scope: '❲stringconcat_reset_handler❳',
      kind: 'parameter'
    },
    '❲inst❳': {
      name: '❲inst❳',
      indir: 2,
      type: 'StringConcat_Instance_Data',
      scope: '❲stringconcat_reset_handler❳',
      kind: 'variable'
    }
  },
  '❲stringconcat_instantiate❳': {
    '❲reg❳': {
      name: '❲reg❳',
      indir: 2,
      type: 'Component_Registry',
      scope: '❲stringconcat_instantiate❳',
      kind: 'parameter'
    },
    '❲owner❳': {
      name: '❲owner❳',
      indir: 2,
      type: 'Container',
      scope: '❲stringconcat_instantiate❳',
      kind: 'parameter'
    },
    '❲name❳': {
      name: '❲name❳',
      indir: 2,
      type: 'Str',
      scope: '❲stringconcat_instantiate❳',
      kind: 'parameter'
    },
    '❲template_data❳': {
      name: '❲template_data❳',
      indir: 1,
      type: 'Ignored',
      scope: '❲stringconcat_instantiate❳',
      kind: 'parameter'
    },
    '❲arg❳': {
      name: '❲arg❳',
      indir: 1,
      type: 'Ignored',
      scope: '❲stringconcat_instantiate❳',
      kind: 'parameter'
    },
    '❲name_with_id❳': {
      name: '❲name_with_id❳',
      indir: 2,
      type: 'Str',
      scope: '❲stringconcat_instantiate❳',
      kind: 'variable'
    },
    '❲instp❳': {
      name: '❲instp❳',
      indir: 2,
      type: 'StringConcat_Instance_Data',
      scope: '❲stringconcat_instantiate❳',
      kind: 'variable'
    }
  },
  '❲stringconcat_handler❳': {
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Leaf',
      scope: '❲stringconcat_handler❳',
      kind: 'parameter'
    },
    '❲mev❳': {
      name: '❲mev❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲stringconcat_handler❳',
      kind: 'parameter'
    },
    '❲inst❳': {
      name: '❲inst❳',
      indir: 2,
      type: 'StringConcat_Instance_Data',
      scope: '❲stringconcat_handler❳',
      kind: 'variable'
    }
  },
  '❲maybe_stringconcat❳': {
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Leaf',
      scope: '❲maybe_stringconcat❳',
      kind: 'parameter'
    },
    '❲inst❳': {
      name: '❲inst❳',
      indir: 2,
      type: 'StringConcat_Instance_Dat',
      scope: '❲maybe_stringconcat❳',
      kind: 'parameter'
    },
    '❲mev❳': {
      name: '❲mev❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲maybe_stringconcat❳',
      kind: 'parameter'
    },
    '❲concatenated_string❳': {
      name: '❲concatenated_string❳',
      indir: 2,
      type: 'Str',
      scope: '❲maybe_stringconcat❳',
      kind: 'variable'
    }
  },
  '❲string_constant_instantiate❳': {
    '❲reg❳': {
      name: '❲reg❳',
      indir: 2,
      type: 'Component_Registry',
      scope: '❲string_constant_instantiate❳',
      kind: 'parameter'
    },
    '❲owner❳': {
      name: '❲owner❳',
      indir: 2,
      type: 'Container',
      scope: '❲string_constant_instantiate❳',
      kind: 'parameter'
    },
    '❲name❳': {
      name: '❲name❳',
      indir: 2,
      type: 'Str',
      scope: '❲string_constant_instantiate❳',
      kind: 'parameter'
    },
    '❲template_data❳': {
      name: '❲template_data❳',
      indir: 1,
      type: 'Ignored',
      scope: '❲string_constant_instantiate❳',
      kind: 'parameter'
    },
    '❲arg❳': {
      name: '❲arg❳',
      indir: 1,
      type: 'Ignored',
      scope: '❲string_constant_instantiate❳',
      kind: 'parameter'
    }
  },
  '❲string_constant_handler❳': {
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Leaf',
      scope: '❲string_constant_handler❳',
      kind: 'parameter'
    },
    '❲mev❳': {
      name: '❲mev❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲string_constant_handler❳',
      kind: 'parameter'
    },
    '❲s❳': {
      name: '❲s❳',
      indir: 2,
      type: 'Str',
      scope: '❲string_constant_handler❳',
      kind: 'variable'
    }
  },
  '❲fakepipename_instantiate❳': {
    '❲reg❳': {
      name: '❲reg❳',
      indir: 2,
      type: 'Component_Registry',
      scope: '❲fakepipename_instantiate❳',
      kind: 'parameter'
    },
    '❲owner❳': {
      name: '❲owner❳',
      indir: 2,
      type: 'Container',
      scope: '❲fakepipename_instantiate❳',
      kind: 'parameter'
    },
    '❲name❳': {
      name: '❲name❳',
      indir: 2,
      type: 'Str',
      scope: '❲fakepipename_instantiate❳',
      kind: 'parameter'
    },
    '❲template_data❳': {
      name: '❲template_data❳',
      indir: 1,
      type: 'Ignored',
      scope: '❲fakepipename_instantiate❳',
      kind: 'parameter'
    },
    '❲arg❳': {
      name: '❲arg❳',
      indir: 1,
      type: 'Ignored',
      scope: '❲fakepipename_instantiate❳',
      kind: 'parameter'
    },
    '❲instance_name❳': {
      name: '❲instance_name❳',
      indir: 2,
      type: 'Str',
      scope: '❲fakepipename_instantiate❳',
      kind: 'variable'
    }
  },
  '❲fakepipename_handler❳': {
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Leaf',
      scope: '❲fakepipename_handler❳',
      kind: 'parameter'
    },
    '❲mev❳': {
      name: '❲mev❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲fakepipename_handler❳',
      kind: 'parameter'
    }
  },
  '❲Switch1star_Instance_Data❳': {
    '❲state❳': {
      name: '❲state❳',
      indir: 2,
      type: 'Str',
      scope: '❲Switch1star_Instance_Data❳',
      kind: 'variable'
    }
  },
  '❲switch1star_reset_handler❳': {
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Leaf',
      scope: '❲switch1star_reset_handler❳',
      kind: 'parameter'
    },
    '❲inst❳': {
      name: '❲inst❳',
      indir: 2,
      type: 'Switch1star_Instance_Data',
      scope: '❲switch1star_reset_handler❳',
      kind: 'variable'
    }
  },
  '❲switch1star_instantiate❳': {
    '❲reg❳': {
      name: '❲reg❳',
      indir: 2,
      type: 'Component_Registry',
      scope: '❲switch1star_instantiate❳',
      kind: 'parameter'
    },
    '❲owner❳': {
      name: '❲owner❳',
      indir: 2,
      type: 'Container',
      scope: '❲switch1star_instantiate❳',
      kind: 'parameter'
    },
    '❲name❳': {
      name: '❲name❳',
      indir: 2,
      type: 'Str',
      scope: '❲switch1star_instantiate❳',
      kind: 'parameter'
    },
    '❲template_data❳': {
      name: '❲template_data❳',
      indir: 1,
      type: 'Ignored',
      scope: '❲switch1star_instantiate❳',
      kind: 'parameter'
    },
    '❲arg❳': {
      name: '❲arg❳',
      indir: 1,
      type: 'Ignored',
      scope: '❲switch1star_instantiate❳',
      kind: 'parameter'
    },
    '❲name_with_id❳': {
      name: '❲name_with_id❳',
      indir: 2,
      type: 'Str',
      scope: '❲switch1star_instantiate❳',
      kind: 'variable'
    },
    '❲instp❳': {
      name: '❲instp❳',
      indir: 2,
      type: 'Switch1star_Instance_Data',
      scope: '❲switch1star_instantiate❳',
      kind: 'variable'
    }
  },
  '❲switch1star_handler❳': {
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Leaf',
      scope: '❲switch1star_handler❳',
      kind: 'parameter'
    },
    '❲mev❳': {
      name: '❲mev❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲switch1star_handler❳',
      kind: 'parameter'
    },
    '❲inst❳': {
      name: '❲inst❳',
      indir: 2,
      type: 'Switch1star_Instance_Data',
      scope: '❲switch1star_handler❳',
      kind: 'variable'
    },
    '❲whichOutput❳': {
      name: '❲whichOutput❳',
      indir: 2,
      type: 'Str',
      scope: '❲switch1star_handler❳',
      kind: 'variable'
    }
  },
  '❲StringAccumulator❳': {
    '❲s❳': {
      name: '❲s❳',
      indir: 2,
      type: 'Str',
      scope: '❲StringAccumulator❳',
      kind: 'variable'
    }
  },
  '❲strcatstar_reset_handler❳': {
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Leaf',
      scope: '❲strcatstar_reset_handler❳',
      kind: 'parameter'
    }
  },
  '❲strcatstar_instantiate❳': {
    '❲reg❳': {
      name: '❲reg❳',
      indir: 2,
      type: 'Component_Registry',
      scope: '❲strcatstar_instantiate❳',
      kind: 'parameter'
    },
    '❲owner❳': {
      name: '❲owner❳',
      indir: 2,
      type: 'Container',
      scope: '❲strcatstar_instantiate❳',
      kind: 'parameter'
    },
    '❲name❳': {
      name: '❲name❳',
      indir: 2,
      type: 'Str',
      scope: '❲strcatstar_instantiate❳',
      kind: 'parameter'
    },
    '❲template_data❳': {
      name: '❲template_data❳',
      indir: 1,
      type: 'Ignored',
      scope: '❲strcatstar_instantiate❳',
      kind: 'parameter'
    },
    '❲arg❳': {
      name: '❲arg❳',
      indir: 1,
      type: 'Ignored',
      scope: '❲strcatstar_instantiate❳',
      kind: 'parameter'
    },
    '❲name_with_id❳': {
      name: '❲name_with_id❳',
      indir: 2,
      type: 'Str',
      scope: '❲strcatstar_instantiate❳',
      kind: 'variable'
    },
    'instp❳': {
      name: 'instp❳',
      indir: 2,
      type: 'Switch1star_Instance_Data',
      scope: '❲strcatstar_instantiate❳',
      kind: 'variable'
    }
  },
  '❲strcatstar_handler❳': {
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Leaf',
      scope: '❲strcatstar_handler❳',
      kind: 'parameter'
    },
    '❲mev❳': {
      name: '❲mev❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲strcatstar_handler❳',
      kind: 'parameter'
    },
    '❲accum❳': {
      name: '❲accum❳',
      indir: 2,
      type: 'Switch1star_Instance_Data',
      scope: '❲strcatstar_handler❳',
      kind: 'variable'
    }
  },
  '❲stop_instantiate❳': {
    '❲reg❳': {
      name: '❲reg❳',
      indir: 2,
      type: 'Component_Registry',
      scope: '❲stop_instantiate❳',
      kind: 'parameter'
    },
    '❲owner❳': {
      name: '❲owner❳',
      indir: 2,
      type: 'Container',
      scope: '❲stop_instantiate❳',
      kind: 'parameter'
    },
    '❲name❳': {
      name: '❲name❳',
      indir: 2,
      type: 'Str',
      scope: '❲stop_instantiate❳',
      kind: 'parameter'
    },
    '❲template_data❳': {
      name: '❲template_data❳',
      indir: 1,
      type: 'Ignored',
      scope: '❲stop_instantiate❳',
      kind: 'parameter'
    },
    '❲arg❳': {
      name: '❲arg❳',
      indir: 1,
      type: 'Ignored',
      scope: '❲stop_instantiate❳',
      kind: 'parameter'
    },
    '❲name_with_id❳': {
      name: '❲name_with_id❳',
      indir: 2,
      type: 'Str',
      scope: '❲stop_instantiate❳',
      kind: 'variable'
    },
    '❲inst❳': {
      name: '❲inst❳',
      indir: 2,
      type: 'Switch1star_Instance_Data',
      scope: '❲stop_instantiate❳',
      kind: 'variable'
    }
  },
  '❲stop_handler❳': {
    '❲eh❳': {
      name: '❲eh❳',
      indir: 2,
      type: 'Leaf',
      scope: '❲stop_handler❳',
      kind: 'parameter'
    },
    '❲mev❳': {
      name: '❲mev❳',
      indir: 2,
      type: 'Mevent',
      scope: '❲stop_handler❳',
      kind: 'parameter'
    },
    '❲inst❳': {
      name: '❲inst❳',
      indir: 2,
      type: 'any',
      scope: '❲stop_handler❳',
      kind: 'variable'
    },
    '❲parent❳': {
      name: '❲parent❳',
      indir: 2,
      type: 'Container',
      scope: '❲stop_handler❳',
      kind: 'variable'
    },
    '❲s❳': {
      name: '❲s❳',
      indir: 2,
      type: 'Str',
      scope: '❲stop_handler❳',
      kind: 'variable'
    }
  },
  '❲initialize_stock_components❳': {
    '❲reg❳': {
      name: '❲reg❳',
      indir: 2,
      type: 'Component_Registry',
      scope: '❲initialize_stock_components❳',
      kind: 'parameter'
    }
  },
  '❲Component_Registry❳': {
    '❲templates❳': {
      name: '❲templates❳',
      indir: 2,
      type: 'Dict_of_Template',
      scope: '❲Component_Registry❳',
      kind: 'variable'
    }
  },
  '❲Template❳': {
    '❲name❳': {
      name: '❲name❳',
      indir: 2,
      type: 'Str',
      scope: '❲Template❳',
      kind: 'variable'
    },
    '❲container❳': {
      name: '❲container❳',
      indir: 2,
      type: 'Container',
      scope: '❲Template❳',
      kind: 'variable'
    },
    '❲instantiator❳': {
      name: '❲instantiator❳',
      indir: 1,
      type: 'Finstantiator',
      scope: '❲Template❳',
      kind: 'variable'
    }
  },
  '❲mkTemplate❳': {
    '❲name❳': {
      name: '❲name❳',
      indir: 2,
      type: 'Str',
      scope: '❲mkTemplate❳',
      kind: 'parameter'
    },
    '❲template_data❳': {
      name: '❲template_data❳',
      indir: 2,
      type: 'Container',
      scope: '❲mkTemplate❳',
      kind: 'parameter'
    },
    '❲instantiator❳': {
      name: '❲instantiator❳',
      indir: 1,
      type: 'Finstantiator',
      scope: '❲mkTemplate❳',
      kind: 'parameter'
    },
    '❲templ❳': {
      name: '❲templ❳',
      indir: 2,
      type: 'Template',
      scope: '❲mkTemplate❳',
      kind: 'variable'
    }
  },
  '❲lnet2internal_from_file❳': {
    '❲json_filename❳': {
      name: '❲json_filename❳',
      indir: 2,
      type: 'Pathname',
      scope: '❲lnet2internal_from_file❳',
      kind: 'parameter'
    },
    '❲pathname❳': {
      name: '❲pathname❳',
      indir: 2,
      type: 'Pathname',
      scope: '❲lnet2internal_from_file❳',
      kind: 'variable'
    },
    '❲filename❳': {
      name: '❲filename❳',
      indir: 2,
      type: 'Str',
      scope: '❲lnet2internal_from_file❳',
      kind: 'variable'
    }
  },
  '❲lnet2internal_from_string❳': {
    '❲lnet❳': {
      name: '❲lnet❳',
      indir: 2,
      type: 'Str',
      scope: '❲lnet2internal_from_string❳',
      kind: 'parameter'
    }
  },
  '❲make_component_registry❳': {},
  '❲register_component❳': {
    '❲reg❳': {
      name: '❲reg❳',
      indir: 2,
      type: 'Component_Registry',
      scope: '❲register_component❳',
      kind: 'parameter'
    },
    '❲template❳': {
      name: '❲template❳',
      indir: 2,
      type: 'Template',
      scope: '❲register_component❳',
      kind: 'parameter'
    }
  },
  '❲register_component_allow_overwriting❳': {
    '❲reg❳': {
      name: '❲reg❳',
      indir: 2,
      type: 'Component_Registry',
      scope: '❲register_component_allow_overwriting❳',
      kind: 'parameter'
    },
    '❲template❳': {
      name: '❲template❳',
      indir: 2,
      type: 'Template',
      scope: '❲register_component_allow_overwriting❳',
      kind: 'parameter'
    }
  },
  '❲abstracted_register_component❳': {
    '❲reg❳': {
      name: '❲reg❳',
      indir: 2,
      type: 'Component_Registry',
      scope: '❲abstracted_register_component❳',
      kind: 'parameter'
    },
    '❲template❳': {
      name: '❲template❳',
      indir: 2,
      type: 'Template',
      scope: '❲abstracted_register_component❳',
      kind: 'parameter'
    },
    '❲ok_to_overwrite❳': {
      name: '❲ok_to_overwrite❳',
      indir: 1,
      type: 'Bool',
      scope: '❲abstracted_register_component❳',
      kind: 'parameter'
    },
    '❲name❳': {
      name: '❲name❳',
      indir: 2,
      type: 'Str',
      scope: '❲abstracted_register_component❳',
      kind: 'variable'
    }
  },
  '❲get_component_instance❳': {
    '❲reg❳': {
      name: '❲reg❳',
      indir: 2,
      type: 'Component_Registry',
      scope: '❲get_component_instance❳',
      kind: 'parameter'
    },
    '❲full_name❳': {
      name: '❲full_name❳',
      indir: 2,
      type: 'Str',
      scope: '❲get_component_instance❳',
      kind: 'parameter'
    },
    '❲owner❳': {
      name: '❲owner❳',
      indir: 2,
      type: 'Container',
      scope: '❲get_component_instance❳',
      kind: 'parameter'
    },
    '❲template_name❳': {
      name: '❲template_name❳',
      indir: 2,
      type: 'Str',
      scope: '❲get_component_instance❳',
      kind: 'variable'
    },
    '❲instance_name❳': {
      name: '❲instance_name❳',
      indir: 2,
      type: 'Str',
      scope: '❲get_component_instance❳',
      kind: 'variable'
    },
    '❲instance❳': {
      name: '❲instance❳',
      indir: 2,
      type: 'PART',
      scope: '❲get_component_instance❳',
      kind: 'variable'
    },
    '❲template❳': {
      name: '❲template❳',
      indir: 2,
      type: 'Template',
      scope: '❲get_component_instance❳',
      kind: 'variable'
    }
  },
  '❲generate_instance_name❳': {
    '❲owner❳': {
      name: '❲owner❳',
      indir: 2,
      type: 'Container',
      scope: '❲generate_instance_name❳',
      kind: 'parameter'
    },
    '❲template_name❳': {
      name: '❲template_name❳',
      indir: 2,
      type: 'Str',
      scope: '❲generate_instance_name❳',
      kind: 'parameter'
    },
    '❲owner_name❳': {
      name: '❲owner_name❳',
      indir: 2,
      type: 'Str',
      scope: '❲generate_instance_name❳',
      kind: 'variable'
    },
    '❲instance_name❳': {
      name: '❲instance_name❳',
      indir: 2,
      type: 'Str',
      scope: '❲generate_instance_name❳',
      kind: 'variable'
    }
  },
  '❲mangle_name❳': {
    '❲s❳': {
      name: '❲s❳',
      indir: 2,
      type: 'Str',
      scope: '❲mangle_name❳',
      kind: 'parameter'
    }
  }
}
