(defparameter  load_errors  t)                              #|line 1|#
(defparameter  runtime_errors  t)                           #|line 2|# #|line 3|#
(defun load_error (&optional  s)
  (declare (ignorable  s))                                  #|line 4|# #|line 5|#
  (format *error-output* "~a~%"  s)                         #|line 6|#
  (format *error-output* "
  ")                                                        #|line 7|#
  (setf  load_errors  t)                                    #|line 8|# #|line 9|#
  )
(defun runtime_error (&optional  s)
  (declare (ignorable  s))                                  #|line 11|# #|line 12|#
  (format *error-output* "~a~%"  s)                         #|line 13|#
  (break)                                                   #|line 14|#
  (setf  runtime_errors  t)                                 #|line 15|# #|line 16|#
  )                                                         #|line 18|#
(defun initialize_component_palette_from_files (&optional  diagram_source_files)
  (declare (ignorable  diagram_source_files))               #|line 19|#
  (let (( reg (funcall (quote make_component_registry) )))
    (declare (ignorable  reg))                              #|line 20|#
    (loop for diagram_source in  diagram_source_files
      do
        (progn
          diagram_source                                    #|line 21|#
          (let ((all_containers_within_single_file (funcall (quote lnet2internal_from_file)   diagram_source  #|line 22|#)))
            (declare (ignorable all_containers_within_single_file))
            (loop for container in  all_containers_within_single_file
              do
                (progn
                  container                                 #|line 23|#
                  (funcall (quote register_component)   reg (funcall (quote mkTemplate)  (gethash  "name"  container)  #| container= |# container  #| instantiator= |# #'container_instantiator )  #|line 24|#) #|line 25|#
                  )))                                       #|line 26|#
          ))
    (funcall (quote initialize_stock_components)   reg      #|line 27|#)
    (return-from initialize_component_palette_from_files  reg) #|line 28|#) #|line 29|#
  )
(defun initialize_component_palette_from_string (&optional  lnet)
  (declare (ignorable  lnet))                               #|line 31|#
  (let (( reg (funcall (quote make_component_registry) )))
    (declare (ignorable  reg))                              #|line 32|#
    (let ((all_containers (funcall (quote lnet2internal_from_string)   lnet  #|line 33|#)))
      (declare (ignorable all_containers))
      (loop for container in  all_containers
        do
          (progn
            container                                       #|line 34|#
            (funcall (quote register_component)   reg (funcall (quote mkTemplate)  (gethash  "name"  container)  #| container= |# container  #| instantiator= |# #'container_instantiator )  #|line 35|#) #|line 36|#
            ))
      (funcall (quote initialize_stock_components)   reg    #|line 37|#)
      (return-from initialize_component_palette_from_string  reg) #|line 38|#)) #|line 39|#
  )
(defun initialize_from_files (&optional  diagram_names)
  (declare (ignorable  diagram_names))                      #|line 40|#
  (let ((arg  nil))
    (declare (ignorable arg))                               #|line 41|#
    (let ((palette (funcall (quote initialize_component_palette_from_files)   diagram_names  #|line 42|#)))
      (declare (ignorable palette))
      (return-from initialize_from_files (values  palette (list   diagram_names  arg ))) #|line 43|#)) #|line 44|#
  )
(defun initialize_from_string (&optional )
  (declare (ignorable ))                                    #|line 46|#
  (let ((arg  nil))
    (declare (ignorable arg))                               #|line 47|#
    (let ((palette (funcall (quote initialize_component_palette_from_string) )))
      (declare (ignorable palette))                         #|line 48|#
      (return-from initialize_from_string (values  palette (list   nil  arg ))) #|line 49|#)) #|line 50|#
  )
(defun start (&optional  arg  part_name  palette  env)
  (declare (ignorable  arg  part_name  palette  env))       #|line 52|#
  (let ((part (funcall (quote start_bare)   part_name  palette  env  #|line 53|#)))
    (declare (ignorable part))
    (funcall (quote inject)   part  ""  arg                 #|line 54|#)
    (funcall (quote finalize)   part                        #|line 55|#)) #|line 56|#
  )
(defun start_bare (&optional  part_name  palette  env)
  (declare (ignorable  part_name  palette  env))            #|line 58|#
  (let ((diagram_names (nth  0  env)))
    (declare (ignorable diagram_names))                     #|line 59|#
    #|  get entrypoint container |#                         #|line 60|#
    (let (( part (funcall (quote get_component_instance)   palette  part_name  nil  #|line 61|#)))
      (declare (ignorable  part))
      (cond
        (( equal    nil  part)                              #|line 62|#
          (funcall (quote load_error)   (concatenate 'string  "Couldn;t find container with page name /"  (concatenate 'string  part_name  (concatenate 'string  "/ in files "  (concatenate 'string (format nil "~a"  diagram_names)  " (check tab names, or disable compression?)"))))  #|line 66|#) #|line 67|#
          ))
      (return-from start_bare  part)                        #|line 68|#)) #|line 69|#
  )
(defun inject (&optional  part  port  payload)
  (declare (ignorable  part  port  payload))                #|line 71|#
  (cond
    ((not  load_errors)                                     #|line 72|#
      (let (( d  (make-instance 'Datum)                     #|line 73|#))
        (declare (ignorable  d))
        (setf (slot-value  d 'v)  payload)                  #|line 74|#
        (setf (slot-value  d 'clone)  #'(lambda (&optional )(funcall (quote obj_clone)   d  #|line 75|#)))
        (setf (slot-value  d 'reclaim)  nil)                #|line 76|#
        (let (( mev (funcall (quote make_mevent)   port  d  #|line 77|#)))
          (declare (ignorable  mev))
          (funcall (quote inject_mevent)   part  mev        #|line 78|#)))
      )
    (t                                                      #|line 79|#
      (break)                                               #|line 80|# #|line 81|#
      ))                                                    #|line 82|#
  )
(defun finalize (&optional  part)
  (declare (ignorable  part))                               #|line 84|#
  (queue-as-json-to-stdout (slot-value  part 'outq))        #|line 85|# #|line 86|#
  )
(defun new_datum_bang (&optional )
  (declare (ignorable ))                                    #|line 88|#
  (let (( d  (make-instance 'Datum)                         #|line 89|#))
    (declare (ignorable  d))
    (setf (slot-value  d 'v)  "!")                          #|line 90|#
    (setf (slot-value  d 'clone)  #'(lambda (&optional )(funcall (quote obj_clone)   d  #|line 91|#)))
    (setf (slot-value  d 'reclaim)  nil)                    #|line 92|#
    (return-from new_datum_bang  d                          #|line 93|# #|line 94|#))
  )
