(defclass Component_Registry ()                             #|line 1|#
  (
    (templates :accessor templates :initarg :templates :initform  (dict-fresh))  #|line 2|#)) #|line 3|#

                                                            #|line 4|#
(defclass Template ()                                       #|line 5|#
  (
    (name :accessor name :initarg :name :initform  nil)     #|line 6|#
    (container :accessor container :initarg :container :initform  nil)  #|line 7|#
    (instantiator :accessor instantiator :initarg :instantiator :initform  nil)  #|line 8|#)) #|line 9|#

                                                            #|line 10|#
(defun mkTemplate (&optional  name  template_data  instantiator)
  (declare (ignorable  name  template_data  instantiator))  #|line 11|#
  (let (( templ  (make-instance 'Template)                  #|line 12|#))
    (declare (ignorable  templ))
    (setf (slot-value  templ 'name)  name)                  #|line 13|#
    (setf (slot-value  templ 'template_data)  template_data) #|line 14|#
    (setf (slot-value  templ 'instantiator)  instantiator)  #|line 15|#
    (return-from mkTemplate  templ)                         #|line 16|#) #|line 17|#
  )                                                         #|line 19|# #|  convert a little-network to internal form (an object data structure created by json parser) ...  |# #|line 20|# #|  the actual data structure depends on the json parser library used by the target language  |# #|line 21|# #|  the form of the data structure doesn;t matter here, as long as we use lookup operators "@" in this .rt code  |# #|line 22|# #|line 23|# #|  ... by reading the little-net from an external file  |# #|line 24|#
(defun lnet2internal_from_file (&optional  container_xml)
  (declare (ignorable  container_xml))                      #|line 25|#
  (let ((pathname (uiop:getenv "PBPWD")                     #|line 26|#))
    (declare (ignorable pathname))
    (let ((filename  container_xml                          #|line 27|#))
      (declare (ignorable filename))

      ;; read json from a named file and convert it into internal form (a list of Container alists)
      (json2dict (merge-pathnames pathname filename))
                                                            #|line 28|#)) #|line 29|#
  ) #|  ... by reading the little-net from an embedded string (an aspect of creating t2t tool code)  |# #|line 31|#
(defun lnet2internal_from_string (&optional  lnet)
  (declare (ignorable  lnet))                               #|line 32|#

  (internalize-lnet-from-JSON *lnet*)
                                                            #|line 33|# #|line 34|#
  )
(defun make_component_registry (&optional )
  (declare (ignorable ))                                    #|line 36|#
  (return-from make_component_registry  (make-instance 'Component_Registry) #|line 37|#) #|line 38|#
  )
(defun register_component (&optional  reg  template)
  (declare (ignorable  reg  template))
  (return-from register_component (funcall (quote abstracted_register_component)   reg  template  nil )) #|line 40|#
  )
(defun register_component_allow_overwriting (&optional  reg  template)
  (declare (ignorable  reg  template))
  (return-from register_component_allow_overwriting (funcall (quote abstracted_register_component)   reg  template  t )) #|line 41|#
  )
(defun abstracted_register_component (&optional  reg  template  ok_to_overwrite)
  (declare (ignorable  reg  template  ok_to_overwrite))     #|line 43|#
  (let ((name (funcall (quote mangle_name)  (slot-value  template 'name)  #|line 44|#)))
    (declare (ignorable name))
    (cond
      (( and  ( dict-in?  ( and  (not (equal   reg  nil))  name) (slot-value  reg 'templates)) (not  ok_to_overwrite)) #|line 45|#
        (funcall (quote load_error)   (concatenate 'string  "Component /"  (concatenate 'string (slot-value  template 'name)  "/ already declared"))  #|line 46|#)
        (return-from abstracted_register_component  reg)    #|line 47|#
        )
      (t                                                    #|line 48|#
        (setf (gethash name (slot-value  reg 'templates))  template) #|line 49|#
        (return-from abstracted_register_component  reg)    #|line 50|# #|line 51|#
        )))                                                 #|line 52|#
  )
(defun get_component_instance (&optional  reg  full_name  owner)
  (declare (ignorable  reg  full_name  owner))              #|line 54|#
  #|  If a part name begins with ":", it is treated as a JIT part and we let the runtime factory generate it on-the-fly (see kernel_external.rt and external.rt) else it is assumed to be a regular AOT part and assumed to have been registered before runtime, so we just pull its template out of the registry and instantiate it.  |# #|line 55|#
  #|  ":?<string>" is a probe part that is tagged with <string>  |# #|line 56|#
  #|  ":$ <command>" is a shell-out part that sends <command> to the operating system shell  |# #|line 57|#
  #|  ":<string>" else, it's just treated as a string part that produces <string> on its output  |# #|line 58|#
  (let ((template_name (funcall (quote mangle_name)   full_name  #|line 59|#)))
    (declare (ignorable template_name))
    (cond
      (( equal    ":"  (string (char  full_name 0)))        #|line 60|#
        (let ((instance_name (funcall (quote generate_instance_name)   owner  template_name  #|line 61|#)))
          (declare (ignorable instance_name))
          (let ((instance (funcall (quote jit_instantiate)   reg  owner  instance_name  full_name  #|line 62|#)))
            (declare (ignorable instance))
            (return-from get_component_instance  instance)  #|line 63|#))
        )
      (t                                                    #|line 64|#
        (cond
          (( dict-in?   template_name (slot-value  reg 'templates)) #|line 65|#
            (let ((template (gethash template_name (slot-value  reg 'templates))))
              (declare (ignorable template))                #|line 66|#
              (cond
                (( equal    template  nil)                  #|line 67|#
                  (funcall (quote load_error)   (concatenate 'string  "Registry Error (A): Can't find component /"  (concatenate 'string  template_name  "/"))  #|line 68|#)
                  (return-from get_component_instance  nil) #|line 69|#
                  )
                (t                                          #|line 70|#
                  (let ((instance_name (funcall (quote generate_instance_name)   owner  template_name  #|line 71|#)))
                    (declare (ignorable instance_name))
                    (let ((instance (funcall (slot-value  template 'instantiator)   reg  owner  instance_name (slot-value  template 'template_data)  ""  #|line 72|#)))
                      (declare (ignorable instance))
                      (return-from get_component_instance  instance) #|line 73|#)) #|line 74|#
                  )))
            )
          (t                                                #|line 75|#
            (funcall (quote load_error)   (concatenate 'string  "Registry Error (B): Can't find component /"  (concatenate 'string  template_name  "/"))  #|line 76|#)
            (return-from get_component_instance  nil)       #|line 77|# #|line 78|#
            ))                                              #|line 79|#
        )))                                                 #|line 80|#
  )
(defun generate_instance_name (&optional  owner  template_name)
  (declare (ignorable  owner  template_name))               #|line 82|#
  (let ((owner_name  ""))
    (declare (ignorable owner_name))                        #|line 83|#
    (let ((instance_name  template_name))
      (declare (ignorable instance_name))                   #|line 84|#
      (cond
        ((not (equal   nil  owner))                         #|line 85|#
          (setf  owner_name (slot-value  owner 'name))      #|line 86|#
          (setf  instance_name  (concatenate 'string  owner_name  (concatenate 'string  "▹"  template_name)) #|line 87|#)
          )
        (t                                                  #|line 88|#
          (setf  instance_name  template_name)              #|line 89|# #|line 90|#
          ))
      (return-from generate_instance_name  instance_name)   #|line 91|#)) #|line 92|#
  )
(defun mangle_name (&optional  s)
  (declare (ignorable  s))                                  #|line 94|#
  #|  trim name to remove code from Container component names _ deferred until later (or never) |# #|line 95|#
  (return-from mangle_name  s)                              #|line 96|# #|line 97|#
  )
