(defun mkTemplate (&optional  name  template_data  instantiator)
  (declare (ignorable  name  template_data  instantiator))  #|line 1|#
  (let (( templ  (make-instance 'Template)                  #|line 2|#))
    (declare (ignorable  templ))
    (setf (slot-value  templ 'name)  name)                  #|line 3|#
    (setf (slot-value  templ 'template_data)  template_data) #|line 4|#
    (setf (slot-value  templ 'instantiator)  instantiator)  #|line 5|#
    (return-from mkTemplate  templ)                         #|line 6|#) #|line 7|#
  )                                                         #|line 9|# #|  convert a little-network to internal form (an object data structure created by json parser) ...  |# #|line 10|# #|  the actual data structure depends on the json parser library used by the target language  |# #|line 11|# #|  the form of the data structure doesn;t matter here, as long as we use lookup operators "@" in this .rt code  |# #|line 12|# #|line 13|# #|  ... by reading the little-net from an external file  |# #|line 14|#
(defun lnet2internal_from_file (&optional  container_xml)
  (declare (ignorable  container_xml))                      #|line 15|#
  (let ((pathname (uiop:getenv "PBPWD")                     #|line 16|#))
    (declare (ignorable pathname))
    (let ((filename  container_xml                          #|line 17|#))
      (declare (ignorable filename))

      ;; read json from a named file and convert it into internal form (a list of Container alists)
      (json2dict (merge-pathnames pathname filename))
                                                            #|line 18|#)) #|line 19|#
  ) #|  ... by reading the little-net from an embedded string (an aspect of creating t2t tool code)  |# #|line 21|#
(defun lnet2internal_from_string (&optional  lnet)
  (declare (ignorable  lnet))                               #|line 22|#

  (internalize-lnet-from-JSON *lnet*)
                                                            #|line 23|# #|line 24|#
  )
(defun delete_decls (&optional  d)
  (declare (ignorable  d))                                  #|line 26|#
  #| pass |#                                                #|line 27|# #|line 28|#
  )
(defun make_component_registry (&optional )
  (declare (ignorable ))                                    #|line 30|#
  (return-from make_component_registry  (make-instance 'Component_Registry) #|line 31|#) #|line 32|#
  )
(defun register_component (&optional  reg  template)
  (declare (ignorable  reg  template))
  (return-from register_component (funcall (quote abstracted_register_component)   reg  template  nil )) #|line 34|#
  )
(defun register_component_allow_overwriting (&optional  reg  template)
  (declare (ignorable  reg  template))
  (return-from register_component_allow_overwriting (funcall (quote abstracted_register_component)   reg  template  t )) #|line 35|#
  )
(defun abstracted_register_component (&optional  reg  template  ok_to_overwrite)
  (declare (ignorable  reg  template  ok_to_overwrite))     #|line 37|#
  (let ((name (funcall (quote mangle_name)  (slot-value  template 'name)  #|line 38|#)))
    (declare (ignorable name))
    (cond
      (( and  ( dict-in?  ( and  (not (equal   reg  nil))  name) (slot-value  reg 'templates)) (not  ok_to_overwrite)) #|line 39|#
        (funcall (quote load_error)   (concatenate 'string  "Component /"  (concatenate 'string (slot-value  template 'name)  "/ already declared"))  #|line 40|#)
        (return-from abstracted_register_component  reg)    #|line 41|#
        )
      (t                                                    #|line 42|#
        (setf (gethash name (slot-value  reg 'templates))  template) #|line 43|#
        (return-from abstracted_register_component  reg)    #|line 44|# #|line 45|#
        )))                                                 #|line 46|#
  )
(defun get_component_instance (&optional  reg  full_name  owner)
  (declare (ignorable  reg  full_name  owner))              #|line 48|#
  #|  If a part name begins with ":", it is treated as a JIT part and we let the runtime factory generate it on-the-fly (see kernel_external.rt and external.rt) else it is assumed to be a regular AOT part and assumed to have been registered before runtime, so we just pull its template out of the registry and instantiate it.  |# #|line 49|#
  #|  ":?<string>" is a probe part that is tagged with <string>  |# #|line 50|#
  #|  ":$ <command>" is a shell-out part that sends <command> to the operating system shell  |# #|line 51|#
  #|  ":<string>" else, it's just treated as a string part that produces <string> on its output  |# #|line 52|#
  (let ((template_name (funcall (quote mangle_name)   full_name  #|line 53|#)))
    (declare (ignorable template_name))
    (cond
      (( equal    ":"  (string (char  full_name 0)))        #|line 54|#
        (let ((instance_name (funcall (quote generate_instance_name)   owner  template_name  #|line 55|#)))
          (declare (ignorable instance_name))
          (let ((instance (funcall (quote jit_instantiate)   reg  owner  instance_name  full_name  #|line 56|#)))
            (declare (ignorable instance))
            (return-from get_component_instance  instance)  #|line 57|#))
        )
      (t                                                    #|line 58|#
        (cond
          (( dict-in?   template_name (slot-value  reg 'templates)) #|line 59|#
            (let ((template (gethash template_name (slot-value  reg 'templates))))
              (declare (ignorable template))                #|line 60|#
              (cond
                (( equal    template  nil)                  #|line 61|#
                  (funcall (quote load_error)   (concatenate 'string  "Registry Error (A): Can't find component /"  (concatenate 'string  template_name  "/"))  #|line 62|#)
                  (return-from get_component_instance  nil) #|line 63|#
                  )
                (t                                          #|line 64|#
                  (let ((instance_name (funcall (quote generate_instance_name)   owner  template_name  #|line 65|#)))
                    (declare (ignorable instance_name))
                    (let ((instance (funcall (slot-value  template 'instantiator)   reg  owner  instance_name (slot-value  template 'template_data)  ""  #|line 66|#)))
                      (declare (ignorable instance))
                      (return-from get_component_instance  instance) #|line 67|#)) #|line 68|#
                  )))
            )
          (t                                                #|line 69|#
            (funcall (quote load_error)   (concatenate 'string  "Registry Error (B): Can't find component /"  (concatenate 'string  template_name  "/"))  #|line 70|#)
            (return-from get_component_instance  nil)       #|line 71|# #|line 72|#
            ))                                              #|line 73|#
        )))                                                 #|line 74|#
  )
(defun generate_instance_name (&optional  owner  template_name)
  (declare (ignorable  owner  template_name))               #|line 76|#
  (let ((owner_name  ""))
    (declare (ignorable owner_name))                        #|line 77|#
    (let ((instance_name  template_name))
      (declare (ignorable instance_name))                   #|line 78|#
      (cond
        ((not (equal   nil  owner))                         #|line 79|#
          (setf  owner_name (slot-value  owner 'name))      #|line 80|#
          (setf  instance_name  (concatenate 'string  owner_name  (concatenate 'string  "▹"  template_name)) #|line 81|#)
          )
        (t                                                  #|line 82|#
          (setf  instance_name  template_name)              #|line 83|# #|line 84|#
          ))
      (return-from generate_instance_name  instance_name)   #|line 85|#)) #|line 86|#
  )
(defun mangle_name (&optional  s)
  (declare (ignorable  s))                                  #|line 88|#
  #|  trim name to remove code from Container component names _ deferred until later (or never) |# #|line 89|#
  (return-from mangle_name  s)                              #|line 90|# #|line 91|#
  )
