#|line 1|#
(defun load_error (&optional  s)
  (declare (ignorable  s))                                  #|line 2|# #|line 3|#
  (format *error-output* "~a~%"  s)                         #|line 4|#
  (format *error-output* "
  ")                                                        #|line 5|#
  (setf  load_errors  t)                                    #|line 6|# #|line 7|#
  )
(defun runtime_error (&optional  s)
  (declare (ignorable  s))                                  #|line 9|# #|line 10|#
  (format *error-output* "~a~%"  s)                         #|line 11|#
  (break)                                                   #|line 12|#
  (setf  runtime_errors  t)                                 #|line 13|# #|line 14|#
  )                                                         #|line 16|#
(defun initialize_component_palette_from_files (&optional  diagram_source_files)
  (declare (ignorable  diagram_source_files))               #|line 17|#
  (let (( reg (funcall (quote make_component_registry) )))
    (declare (ignorable  reg))                              #|line 18|#
    (loop for diagram_source in  diagram_source_files
      do
        (progn
          diagram_source                                    #|line 19|#
          (let ((all_containers_within_single_file (funcall (quote lnet2internal_from_file)   diagram_source  #|line 20|#)))
            (declare (ignorable all_containers_within_single_file))
            (loop for container in  all_containers_within_single_file
              do
                (progn
                  container                                 #|line 21|#
                  (funcall (quote register_component)   reg (funcall (quote mkTemplate)  (gethash  "name"  container)  #| container= |# container  #| instantiator= |# #'container_instantiator )  #|line 22|#) #|line 23|#
                  )))                                       #|line 24|#
          ))
    (funcall (quote initialize_stock_components)   reg      #|line 25|#)
    (return-from initialize_component_palette_from_files  reg) #|line 26|#) #|line 27|#
  )
(defun initialize_component_palette_from_string (&optional  lnet)
  (declare (ignorable  lnet))                               #|line 29|#
  (let (( reg (funcall (quote make_component_registry) )))
    (declare (ignorable  reg))                              #|line 30|#
    (let ((all_containers (funcall (quote lnet2internal_from_string)   lnet  #|line 31|#)))
      (declare (ignorable all_containers))
      (loop for container in  all_containers
        do
          (progn
            container                                       #|line 32|#
            (funcall (quote register_component)   reg (funcall (quote mkTemplate)  (gethash  "name"  container)  #| container= |# container  #| instantiator= |# #'container_instantiator )  #|line 33|#) #|line 34|#
            ))
      (funcall (quote initialize_stock_components)   reg    #|line 35|#)
      (return-from initialize_component_palette_from_string  reg) #|line 36|#)) #|line 37|#
  )
(defun initialize_from_files (&optional  diagram_names)
  (declare (ignorable  diagram_names))                      #|line 38|#
  (let ((arg  nil))
    (declare (ignorable arg))                               #|line 39|#
    (let ((palette (funcall (quote initialize_component_palette_from_files)   diagram_names  #|line 40|#)))
      (declare (ignorable palette))
      (return-from initialize_from_files (values  palette (list   diagram_names  arg ))) #|line 41|#)) #|line 42|#
  )
(defun initialize_from_string (&optional )
  (declare (ignorable ))                                    #|line 44|#
  (let ((arg  nil))
    (declare (ignorable arg))                               #|line 45|#
    (let ((palette (funcall (quote initialize_component_palette_from_string) )))
      (declare (ignorable palette))                         #|line 46|#
      (return-from initialize_from_string (values  palette (list   nil  arg ))) #|line 47|#)) #|line 48|#
  )
(defun start (&optional  arg  part_name  palette  env)
  (declare (ignorable  arg  part_name  palette  env))       #|line 50|#
  (let ((part (funcall (quote start_bare)   part_name  palette  env  #|line 51|#)))
    (declare (ignorable part))
    (funcall (quote inject)   part  ""  arg                 #|line 52|#)
    (funcall (quote finalize)   part                        #|line 53|#)) #|line 54|#
  )
(defun start_bare (&optional  part_name  palette  env)
  (declare (ignorable  part_name  palette  env))            #|line 56|#
  (let ((diagram_names (nth  0  env)))
    (declare (ignorable diagram_names))                     #|line 57|#
    #|  get entrypoint container |#                         #|line 58|#
    (let (( part (funcall (quote get_component_instance)   palette  part_name  nil  #|line 59|#)))
      (declare (ignorable  part))
      (cond
        (( equal    nil  part)                              #|line 60|#
          (funcall (quote load_error)   (concatenate 'string  "Couldn;t find container with page name /"  (concatenate 'string  part_name  (concatenate 'string  "/ in files "  (concatenate 'string (format nil "~a"  diagram_names)  " (check tab names, or disable compression?)"))))  #|line 64|#) #|line 65|#
          ))
      (return-from start_bare  part)                        #|line 66|#)) #|line 67|#
  )
(defun inject (&optional  part  port  payload)
  (declare (ignorable  part  port  payload))                #|line 69|#
  (cond
    ((not  load_errors)                                     #|line 70|#
      (let (( d  (make-instance 'Datum)                     #|line 71|#))
        (declare (ignorable  d))
        (setf (slot-value  d 'v)  payload)                  #|line 72|#
        (setf (slot-value  d 'clone)  #'(lambda (&optional )(funcall (quote obj_clone)   d  #|line 73|#)))
        (setf (slot-value  d 'reclaim)  nil)                #|line 74|#
        (let (( mev (funcall (quote make_mevent)   port  d  #|line 75|#)))
          (declare (ignorable  mev))
          (funcall (quote inject_mevent)   part  mev        #|line 76|#)))
      )
    (t                                                      #|line 77|#
      (break)                                               #|line 78|# #|line 79|#
      ))                                                    #|line 80|#
  )
(defun finalize (&optional  part)
  (declare (ignorable  part))                               #|line 82|#
  (queue-as-json-to-stdout (slot-value  part 'outq))        #|line 83|# #|line 84|#
  )
(defun new_datum_bang (&optional )
  (declare (ignorable ))                                    #|line 86|#
  (let (( d  (make-instance 'Datum)                         #|line 87|#))
    (declare (ignorable  d))
    (setf (slot-value  d 'v)  "!")                          #|line 88|#
    (setf (slot-value  d 'clone)  #'(lambda (&optional )(funcall (quote obj_clone)   d  #|line 89|#)))
    (setf (slot-value  d 'reclaim)  nil)                    #|line 90|#
    (return-from new_datum_bang  d                          #|line 91|# #|line 92|#))
  )
