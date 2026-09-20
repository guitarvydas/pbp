(defparameter  load_errors  nil)                            #|line 1|#
(defparameter  runtime_errors  nil)                         #|line 2|#
(defparameter  ticktime  0)                                 #|line 3|# #|line 4|#
(defun load_error (&optional  s)
  (declare (ignorable  s))                                  #|line 5|# #|line 6|#
  (format *error-output* "~a~%"  s)                         #|line 7|#
  (format *error-output* "
  ")                                                        #|line 8|#
  (setf  load_errors  t)                                    #|line 9|# #|line 10|#
  )
(defun runtime_error (&optional  s)
  (declare (ignorable  s))                                  #|line 12|# #|line 13|#
  (format *error-output* "~a~%"  s)                         #|line 14|#
  (break)                                                   #|line 15|#
  (setf  runtime_errors  t)                                 #|line 16|# #|line 17|#
  )                                                         #|line 19|#
(defun initialize_component_palette_from_files (&optional  diagram_source_files)
  (declare (ignorable  diagram_source_files))               #|line 20|#
  (let (( reg (funcall (quote make_component_registry) )))
    (declare (ignorable  reg))                              #|line 21|#
    (loop for diagram_source in  diagram_source_files
      do
        (progn
          diagram_source                                    #|line 22|#
          (let ((all_containers_within_single_file (funcall (quote lnet2internal_from_file)   diagram_source  #|line 23|#)))
            (declare (ignorable all_containers_within_single_file))
            (loop for container in  all_containers_within_single_file
              do
                (progn
                  container                                 #|line 24|#
                  (funcall (quote register_component)   reg (funcall (quote mkTemplate)  (gethash  "name"  container)  #| container= |# container  #| instantiator= |# #'container_instantiator )  #|line 25|#) #|line 26|#
                  )))                                       #|line 27|#
          ))
    (funcall (quote initialize_stock_components)   reg      #|line 28|#)
    (return-from initialize_component_palette_from_files  reg) #|line 29|#) #|line 30|#
  )
(defun initialize_component_palette_from_string (&optional  lnet)
  (declare (ignorable  lnet))                               #|line 32|#
  (let (( reg (funcall (quote make_component_registry) )))
    (declare (ignorable  reg))                              #|line 33|#
    (let ((all_containers (funcall (quote lnet2internal_from_string)   lnet  #|line 34|#)))
      (declare (ignorable all_containers))
      (loop for container in  all_containers
        do
          (progn
            container                                       #|line 35|#
            (funcall (quote register_component)   reg (funcall (quote mkTemplate)  (gethash  "name"  container)  #| container= |# container  #| instantiator= |# #'container_instantiator )  #|line 36|#) #|line 37|#
            ))
      (funcall (quote initialize_stock_components)   reg    #|line 38|#)
      (return-from initialize_component_palette_from_string  reg) #|line 39|#)) #|line 40|#
  )
(defun initialize_from_files (&optional  diagram_names)
  (declare (ignorable  diagram_names))                      #|line 41|#
  (let ((arg  nil))
    (declare (ignorable arg))                               #|line 42|#
    (let ((palette (funcall (quote initialize_component_palette_from_files)   diagram_names  #|line 43|#)))
      (declare (ignorable palette))
      (return-from initialize_from_files (values  palette (list   diagram_names  arg ))) #|line 44|#)) #|line 45|#
  )
(defun initialize_from_string (&optional )
  (declare (ignorable ))                                    #|line 47|#
  (let ((arg  nil))
    (declare (ignorable arg))                               #|line 48|#
    (let ((palette (funcall (quote initialize_component_palette_from_string) )))
      (declare (ignorable palette))                         #|line 49|#
      (return-from initialize_from_string (values  palette (list   nil  arg ))) #|line 50|#)) #|line 51|#
  )
(defun start (&optional  arg  part_name  palette  env)
  (declare (ignorable  arg  part_name  palette  env))       #|line 53|#
  (let ((part (funcall (quote start_bare)   part_name  palette  env  #|line 54|#)))
    (declare (ignorable part))
    (funcall (quote inject)   part  ""  arg                 #|line 55|#)
    (funcall (quote finalize)   part                        #|line 56|#)) #|line 57|#
  )
(defun start_bare (&optional  part_name  palette  env)
  (declare (ignorable  part_name  palette  env))            #|line 59|#
  (let ((diagram_names (nth  0  env)))
    (declare (ignorable diagram_names))                     #|line 60|#
    #|  get entrypoint container |#                         #|line 61|#
    (let (( part (funcall (quote get_component_instance)   palette  part_name  nil  #|line 62|#)))
      (declare (ignorable  part))
      (cond
        (( equal    nil  part)                              #|line 63|#
          (funcall (quote load_error)   (concatenate 'string  "Couldn;t find container with page name /"  (concatenate 'string  part_name  (concatenate 'string  "/ in files "  (concatenate 'string (format nil "~a"  diagram_names)  " (check tab names, or disable compression?)"))))  #|line 67|#) #|line 68|#
          ))
      (return-from start_bare  part)                        #|line 69|#)) #|line 70|#
  )
(defun inject (&optional  part  port  payload)
  (declare (ignorable  part  port  payload))                #|line 72|#
  (cond
    ((not  load_errors)                                     #|line 73|#
      (let (( d  (make-instance 'Datum)                     #|line 74|#))
        (declare (ignorable  d))
        (setf (slot-value  d 'v)  payload)                  #|line 75|#
        (setf (slot-value  d 'clone)  #'(lambda (&optional )(funcall (quote obj_clone)   d  #|line 76|#)))
        (setf (slot-value  d 'reclaim)  nil)                #|line 77|#
        (let (( mev (funcall (quote make_mevent)   port  d  #|line 78|#)))
          (declare (ignorable  mev))
          (funcall (quote inject_mevent)   part  mev        #|line 79|#)))
      )
    (t                                                      #|line 80|#
      (break)                                               #|line 81|# #|line 82|#
      ))                                                    #|line 83|#
  )
(defun finalize (&optional  part)
  (declare (ignorable  part))                               #|line 85|#
  (queue-as-json-to-stdout (slot-value  part 'outq))        #|line 86|# #|line 87|#
  )
(defun new_datum_bang (&optional )
  (declare (ignorable ))                                    #|line 89|#
  (let (( d  (make-instance 'Datum)                         #|line 90|#))
    (declare (ignorable  d))
    (setf (slot-value  d 'v)  "!")                          #|line 91|#
    (setf (slot-value  d 'clone)  #'(lambda (&optional )(funcall (quote obj_clone)   d  #|line 92|#)))
    (setf (slot-value  d 'reclaim)  nil)                    #|line 93|#
    (return-from new_datum_bang  d                          #|line 94|# #|line 95|#))
  )
