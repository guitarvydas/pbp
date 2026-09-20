(defclass Component_Registry ()                             #|line 1|#
  (
    (templates :accessor templates :initarg :templates :initform  (dict-fresh))  #|line 2|#)) #|line 3|#

                                                            #|line 4|#
(defun mkTemplate (&optional  name  template_data  instantiator)
  (declare (ignorable  name  template_data  instantiator))  #|line 5|#
  (let (( templ  (make-instance 'Template)                  #|line 6|#))
    (declare (ignorable  templ))
    (setf (slot-value  templ 'name)  name)                  #|line 7|#
    (setf (slot-value  templ 'template_data)  template_data) #|line 8|#
    (setf (slot-value  templ 'instantiator)  instantiator)  #|line 9|#
    (return-from mkTemplate  templ)                         #|line 10|#) #|line 11|#
  )                                                         #|line 13|# #|  convert a little-network to internal form (an object data structure created by json parser) ...  |# #|line 14|# #|  the actual data structure depends on the json parser library used by the target language  |# #|line 15|# #|  the form of the data structure doesn;t matter here, as long as we use lookup operators "@" in this .rt code  |# #|line 16|# #|line 17|# #|  ... by reading the little-net from an external file  |# #|line 18|#
(defun lnet2internal_from_file (&optional  container_xml)
  (declare (ignorable  container_xml))                      #|line 19|#
  (let ((pathname (uiop:getenv "PBPWD")                     #|line 20|#))
    (declare (ignorable pathname))
    (let ((filename  container_xml                          #|line 21|#))
      (declare (ignorable filename))

      ;; read json from a named file and convert it into internal form (a list of Container alists)
      (json2dict (merge-pathnames pathname filename))
                                                            #|line 22|#)) #|line 23|#
  ) #|  ... by reading the little-net from an embedded string (an aspect of creating t2t tool code)  |# #|line 25|#
(defun lnet2internal_from_string (&optional  lnet)
  (declare (ignorable  lnet))                               #|line 26|#

  (internalize-lnet-from-JSON *lnet*)
                                                            #|line 27|# #|line 28|#
  )
(defun delete_decls (&optional  d)
  (declare (ignorable  d))                                  #|line 30|#
  #| pass |#                                                #|line 31|# #|line 32|#
  )
(defun make_component_registry (&optional )
  (declare (ignorable ))                                    #|line 34|#
  (return-from make_component_registry  (make-instance 'Component_Registry) #|line 35|#) #|line 36|#
  )
(defun register_component (&optional  reg  template)
  (declare (ignorable  reg  template))
  (return-from register_component (funcall (quote abstracted_register_component)   reg  template  nil )) #|line 38|#
  )
(defun register_component_allow_overwriting (&optional  reg  template)
  (declare (ignorable  reg  template))
  (return-from register_component_allow_overwriting (funcall (quote abstracted_register_component)   reg  template  t )) #|line 39|#
  )
(defun abstracted_register_component (&optional  reg  template  ok_to_overwrite)
  (declare (ignorable  reg  template  ok_to_overwrite))     #|line 41|#
  (let ((name (funcall (quote mangle_name)  (slot-value  template 'name)  #|line 42|#)))
    (declare (ignorable name))
    (cond
      (( and  ( dict-in?  ( and  (not (equal   reg  nil))  name) (slot-value  reg 'templates)) (not  ok_to_overwrite)) #|line 43|#
        (funcall (quote load_error)   (concatenate 'string  "Component /"  (concatenate 'string (slot-value  template 'name)  "/ already declared"))  #|line 44|#)
        (return-from abstracted_register_component  reg)    #|line 45|#
        )
      (t                                                    #|line 46|#
        (setf (gethash name (slot-value  reg 'templates))  template) #|line 47|#
        (return-from abstracted_register_component  reg)    #|line 48|# #|line 49|#
        )))                                                 #|line 50|#
  )
(defun get_component_instance (&optional  reg  full_name  owner)
  (declare (ignorable  reg  full_name  owner))              #|line 52|#
  #|  If a part name begins with ":", it is treated as a JIT part and we let the runtime factory generate it on-the-fly (see kernel_external.rt and external.rt) else it is assumed to be a regular AOT part and assumed to have been registered before runtime, so we just pull its template out of the registry and instantiate it.  |# #|line 53|#
  #|  ":?<string>" is a probe part that is tagged with <string>  |# #|line 54|#
  #|  ":$ <command>" is a shell-out part that sends <command> to the operating system shell  |# #|line 55|#
  #|  ":<string>" else, it's just treated as a string part that produces <string> on its output  |# #|line 56|#
  (let ((template_name (funcall (quote mangle_name)   full_name  #|line 57|#)))
    (declare (ignorable template_name))
    (cond
      (( equal    ":"  (string (char  full_name 0)))        #|line 58|#
        (let ((instance_name (funcall (quote generate_instance_name)   owner  template_name  #|line 59|#)))
          (declare (ignorable instance_name))
          (let ((instance (funcall (quote jit_instantiate)   reg  owner  instance_name  full_name  #|line 60|#)))
            (declare (ignorable instance))
            (return-from get_component_instance  instance)  #|line 61|#))
        )
      (t                                                    #|line 62|#
        (cond
          (( dict-in?   template_name (slot-value  reg 'templates)) #|line 63|#
            (let ((template (gethash template_name (slot-value  reg 'templates))))
              (declare (ignorable template))                #|line 64|#
              (cond
                (( equal    template  nil)                  #|line 65|#
                  (funcall (quote load_error)   (concatenate 'string  "Registry Error (A): Can't find component /"  (concatenate 'string  template_name  "/"))  #|line 66|#)
                  (return-from get_component_instance  nil) #|line 67|#
                  )
                (t                                          #|line 68|#
                  (let ((instance_name (funcall (quote generate_instance_name)   owner  template_name  #|line 69|#)))
                    (declare (ignorable instance_name))
                    (let ((instance (funcall (slot-value  template 'instantiator)   reg  owner  instance_name (slot-value  template 'template_data)  ""  #|line 70|#)))
                      (declare (ignorable instance))
                      (return-from get_component_instance  instance) #|line 71|#)) #|line 72|#
                  )))
            )
          (t                                                #|line 73|#
            (funcall (quote load_error)   (concatenate 'string  "Registry Error (B): Can't find component /"  (concatenate 'string  template_name  "/"))  #|line 74|#)
            (return-from get_component_instance  nil)       #|line 75|# #|line 76|#
            ))                                              #|line 77|#
        )))                                                 #|line 78|#
  )
(defun generate_instance_name (&optional  owner  template_name)
  (declare (ignorable  owner  template_name))               #|line 80|#
  (let ((owner_name  ""))
    (declare (ignorable owner_name))                        #|line 81|#
    (let ((instance_name  template_name))
      (declare (ignorable instance_name))                   #|line 82|#
      (cond
        ((not (equal   nil  owner))                         #|line 83|#
          (setf  owner_name (slot-value  owner 'name))      #|line 84|#
          (setf  instance_name  (concatenate 'string  owner_name  (concatenate 'string  "▹"  template_name)) #|line 85|#)
          )
        (t                                                  #|line 86|#
          (setf  instance_name  template_name)              #|line 87|# #|line 88|#
          ))
      (return-from generate_instance_name  instance_name)   #|line 89|#)) #|line 90|#
  )
(defun mangle_name (&optional  s)
  (declare (ignorable  s))                                  #|line 92|#
  #|  trim name to remove code from Container component names _ deferred until later (or never) |# #|line 93|#
  (return-from mangle_name  s)                              #|line 94|# #|line 95|#
  )
