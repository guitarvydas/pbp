(defun clone_string (&optional  s)
  (declare (ignorable  s))                                  #|line 1|#
  (return-from clone_string  s)                             #|line 2|# #|line 3|#
  )                                                         #|line 5|#
(defun trash_instantiate (&optional  reg  owner  name  template_data  arg)
  (declare (ignorable  reg  owner  name  template_data  arg)) #|line 6|#
  (let ((name_with_id (funcall (quote gensymbol)   "trash"  #|line 7|#)))
    (declare (ignorable name_with_id))
    (return-from trash_instantiate (funcall (quote make_leaf)   name_with_id  owner  nil  ""  #'trash_handler  nil  #|line 8|#))) #|line 9|#
  )
(defun trash_handler (&optional  eh  mev)
  (declare (ignorable  eh  mev))                            #|line 11|#
  #|  to appease dumped_on_floor checker |#                 #|line 12|#
  #| pass |#                                                #|line 13|# #|line 14|#
  )
(defclass TwoMevents ()                                     #|line 15|#
  (
    (firstmev :accessor firstmev :initarg :firstmev :initform  nil)  #|line 16|#
    (secondmev :accessor secondmev :initarg :secondmev :initform  nil)  #|line 17|#)) #|line 18|#

                                                            #|line 19|# #|  Deracer_States :: enum { idle, waitingForFirstmev, waitingForSecondmev } |# #|line 20|#
(defclass Deracer_Instance_Data ()                          #|line 21|#
  (
    (state :accessor state :initarg :state :initform  nil)  #|line 22|#
    (buffer :accessor buffer :initarg :buffer :initform  nil)  #|line 23|#)) #|line 24|#

                                                            #|line 25|#
(defun reclaim_Buffers_from_heap (&optional  inst)
  (declare (ignorable  inst))                               #|line 26|#
  #| pass |#                                                #|line 27|# #|line 28|#
  )
(defun deracer_reset_handler (&optional  eh)
  (declare (ignorable  eh))                                 #|line 30|#
  (let (( inst (slot-value  eh 'instance_data)))
    (declare (ignorable  inst))                             #|line 31|#
    (setf (slot-value  inst 'state)  "idle")                #|line 32|#
    (setf (slot-value  inst 'buffer)  (make-instance 'TwoMevents) #|line 33|#)) #|line 34|#
  )
(defun deracer_instantiate (&optional  reg  owner  name  template_data  arg)
  (declare (ignorable  reg  owner  name  template_data  arg)) #|line 36|#
  (let ((name_with_id (funcall (quote gensymbol)   "deracer"  #|line 37|#)))
    (declare (ignorable name_with_id))
    (let (( inst  (make-instance 'Deracer_Instance_Data)    #|line 38|#))
      (declare (ignorable  inst))
      (setf (slot-value  inst 'state)  "idle")              #|line 39|#
      (setf (slot-value  inst 'buffer)  (make-instance 'TwoMevents) #|line 40|#)
      (let ((eh (funcall (quote make_leaf)   name_with_id  owner  inst  ""  #'deracer_handler  #'deracer_reset_handler  #|line 41|#)))
        (declare (ignorable eh))
        (return-from deracer_instantiate  eh)               #|line 42|#))) #|line 43|#
  )
(defun send_firstmev_then_secondmev (&optional  eh  inst)
  (declare (ignorable  eh  inst))                           #|line 45|#
  (funcall (quote forward)   eh  "1" (slot-value (slot-value  inst 'buffer) 'firstmev)  #|line 46|#)
  (funcall (quote forward)   eh  "2" (slot-value (slot-value  inst 'buffer) 'secondmev)  #|line 47|#)
  (funcall (quote reclaim_Buffers_from_heap)   inst         #|line 48|#) #|line 49|#
  )
(defun deracer_handler (&optional  eh  mev)
  (declare (ignorable  eh  mev))                            #|line 51|#
  (let (( inst (slot-value  eh 'instance_data)))
    (declare (ignorable  inst))                             #|line 52|#
    (cond
      (( equal   (slot-value  inst 'state)  "idle")         #|line 53|#
        (cond
          (( equal    "1" (slot-value  mev 'port))          #|line 54|#
            (setf (slot-value (slot-value  inst 'buffer) 'firstmev)  mev) #|line 55|#
            (setf (slot-value  inst 'state)  "waitingForSecondmev") #|line 56|#
            )
          (( equal    "2" (slot-value  mev 'port))          #|line 57|#
            (setf (slot-value (slot-value  inst 'buffer) 'secondmev)  mev) #|line 58|#
            (setf (slot-value  inst 'state)  "waitingForFirstmev") #|line 59|#
            )
          (t                                                #|line 60|#
            (funcall (quote runtime_error)   (concatenate 'string  "bad mev.port (case A) for deracer " (slot-value  mev 'port))  #|line 61|#) #|line 62|#
            ))
        )
      (( equal   (slot-value  inst 'state)  "waitingForFirstmev") #|line 63|#
        (cond
          (( equal    "1" (slot-value  mev 'port))          #|line 64|#
            (setf (slot-value (slot-value  inst 'buffer) 'firstmev)  mev) #|line 65|#
            (funcall (quote send_firstmev_then_secondmev)   eh  inst  #|line 66|#)
            (setf (slot-value  inst 'state)  "idle")        #|line 67|#
            )
          (t                                                #|line 68|#
            (funcall (quote runtime_error)   (concatenate 'string  "deracer: waiting for 1 but got ["  (concatenate 'string (slot-value  mev 'port)  "] (case B)"))  #|line 69|#) #|line 70|#
            ))
        )
      (( equal   (slot-value  inst 'state)  "waitingForSecondmev") #|line 71|#
        (cond
          (( equal    "2" (slot-value  mev 'port))          #|line 72|#
            (setf (slot-value (slot-value  inst 'buffer) 'secondmev)  mev) #|line 73|#
            (funcall (quote send_firstmev_then_secondmev)   eh  inst  #|line 74|#)
            (setf (slot-value  inst 'state)  "idle")        #|line 75|#
            )
          (t                                                #|line 76|#
            (funcall (quote runtime_error)   (concatenate 'string  "deracer: waiting for 2 but got ["  (concatenate 'string (slot-value  mev 'port)  "] (case C)"))  #|line 77|#) #|line 78|#
            ))
        )
      (t                                                    #|line 79|#
        (funcall (quote runtime_error)   "bad state for deracer {eh.state}"  #|line 80|#) #|line 81|#
        )))                                                 #|line 82|#
  )
(defun low_level_read_text_file_instantiate (&optional  reg  owner  name  template_data  arg)
  (declare (ignorable  reg  owner  name  template_data  arg)) #|line 84|#
  (let ((name_with_id (funcall (quote gensymbol)   "Low Level Read Text File"  #|line 85|#)))
    (declare (ignorable name_with_id))
    (return-from low_level_read_text_file_instantiate (funcall (quote make_leaf)   name_with_id  owner  nil  ""  #'low_level_read_text_file_handler  nil  #|line 86|#))) #|line 87|#
  )
(defun low_level_read_text_file_handler (&optional  eh  mev)
  (declare (ignorable  eh  mev))                            #|line 89|#
  (let ((fname (slot-value (slot-value  mev 'payload) 'v)))
    (declare (ignorable fname))                             #|line 90|#

    ;; read text from a named file fname, send the text out on port "" else send error info on port "✗"
    ;; given eh and mev if needed
    (handler-bind ((error #'(lambda (condition) (send_string eh "✗" (format nil "~&~A~&" condition)))))
      (with-open-file (stream fname)
        (let ((contents (make-string (file-length stream))))
          (read-sequence contents stream)
          (send_string eh "" contents))))
                                                            #|line 91|#) #|line 92|#
  )
(defun ensure_string_datum_instantiate (&optional  reg  owner  name  template_data  arg)
  (declare (ignorable  reg  owner  name  template_data  arg)) #|line 94|#
  (let ((name_with_id (funcall (quote gensymbol)   "Ensure String Datum"  #|line 95|#)))
    (declare (ignorable name_with_id))
    (return-from ensure_string_datum_instantiate (funcall (quote make_leaf)   name_with_id  owner  nil  ""  #'ensure_string_datum_handler  nil  #|line 96|#))) #|line 97|#
  )
(defun ensure_string_datum_handler (&optional  eh  mev)
  (declare (ignorable  eh  mev))                            #|line 99|#
  (cond
    (( equal    "string" (funcall (slot-value (slot-value  mev 'payload) 'kind) )) #|line 100|#
      (funcall (quote forward)   eh  ""  mev                #|line 101|#)
      )
    (t                                                      #|line 102|#
      (let ((emev  (concatenate 'string  "*** ensure: type error (expected a string payload) but got " (slot-value  mev 'payload)) #|line 103|#))
        (declare (ignorable emev))
        (funcall (quote send)   eh  "✗"  emev  mev          #|line 104|#)) #|line 105|#
      ))                                                    #|line 106|#
  )
(defclass Syncfilewrite_Data ()                             #|line 108|#
  (
    (filename :accessor filename :initarg :filename :initform  "")  #|line 109|#)) #|line 110|#

                                                            #|line 111|#
(defun syncfilewrite_reset_handler (&optional  eh)
  (declare (ignorable  eh))                                 #|line 112|#
  (setf (slot-value  eh 'instance_data)  (make-instance 'Syncfilewrite_Data) #|line 113|#) #|line 114|#
  ) #|  temp copy for bootstrap, sends "done“ (error during bootstrap if not wired) |# #|line 116|#
(defun syncfilewrite_instantiate (&optional  reg  owner  name  template_data  arg)
  (declare (ignorable  reg  owner  name  template_data  arg)) #|line 117|#
  (let ((name_with_id (funcall (quote gensymbol)   "syncfilewrite"  #|line 118|#)))
    (declare (ignorable name_with_id))
    (let ((inst  (make-instance 'Syncfilewrite_Data)        #|line 119|#))
      (declare (ignorable inst))
      (return-from syncfilewrite_instantiate (funcall (quote make_leaf)   name_with_id  owner  inst  ""  #'syncfilewrite_handler  #'syncfilewrite_reset_handler  #|line 120|#)))) #|line 121|#
  )
(defun syncfilewrite_handler (&optional  eh  mev)
  (declare (ignorable  eh  mev))                            #|line 123|#
  (let (( inst (slot-value  eh 'instance_data)))
    (declare (ignorable  inst))                             #|line 124|#
    (cond
      (( equal    "filename" (slot-value  mev 'port))       #|line 125|#
        (setf (slot-value  inst 'filename) (slot-value (slot-value  mev 'payload) 'v)) #|line 126|#
        )
      (( equal    "input" (slot-value  mev 'port))          #|line 127|#
        (let ((contents (slot-value (slot-value  mev 'payload) 'v)))
          (declare (ignorable contents))                    #|line 128|#
          (let (( f (funcall (quote open)  (slot-value  inst 'filename)  "w"  #|line 129|#)))
            (declare (ignorable  f))
            (cond
              ((not (equal   f  nil))                       #|line 130|#
                (funcall (slot-value  f 'write)  (slot-value (slot-value  mev 'payload) 'v)  #|line 131|#)
                (funcall (slot-value  f 'close) )           #|line 132|#
                (funcall (quote send)   eh  "done" (funcall (quote new_datum_bang) )  mev  #|line 133|#)
                )
              (t                                            #|line 134|#
                (funcall (quote send)   eh  "✗"  (concatenate 'string  "open error on file " (slot-value  inst 'filename))  mev  #|line 135|#) #|line 136|#
                ))))                                        #|line 137|#
        )))                                                 #|line 138|#
  )
(defclass StringConcat_Instance_Data ()                     #|line 140|#
  (
    (buffer1 :accessor buffer1 :initarg :buffer1 :initform  nil)  #|line 141|#
    (buffer2 :accessor buffer2 :initarg :buffer2 :initform  nil)  #|line 142|#)) #|line 143|#

                                                            #|line 144|#
(defun stringconcat_reset_handler (&optional  eh)
  (declare (ignorable  eh))                                 #|line 145|#
  (let (( inst (slot-value  eh 'instance_data)))
    (declare (ignorable  inst))                             #|line 146|#
    (setf (slot-value  inst 'buffer1)  nil)                 #|line 147|#
    (setf (slot-value  inst 'buffer2)  nil)                 #|line 148|#) #|line 149|#
  )
(defun stringconcat_instantiate (&optional  reg  owner  name  template_data  arg)
  (declare (ignorable  reg  owner  name  template_data  arg)) #|line 151|#
  (let ((name_with_id (funcall (quote gensymbol)   "stringconcat"  #|line 152|#)))
    (declare (ignorable name_with_id))
    (let ((instp  (make-instance 'StringConcat_Instance_Data) #|line 153|#))
      (declare (ignorable instp))
      (return-from stringconcat_instantiate (funcall (quote make_leaf)   name_with_id  owner  instp  ""  #'stringconcat_handler  #'stringconcat_reset_handler  #|line 154|#)))) #|line 155|#
  )
(defun stringconcat_handler (&optional  eh  mev)
  (declare (ignorable  eh  mev))                            #|line 157|#
  (let (( inst (slot-value  eh 'instance_data)))
    (declare (ignorable  inst))                             #|line 158|#
    (cond
      (( equal    "1" (slot-value  mev 'port))              #|line 159|#
        (setf (slot-value  inst 'buffer1) (funcall (quote clone_string)  (slot-value (slot-value  mev 'payload) 'v)  #|line 160|#))
        (funcall (quote maybe_stringconcat)   eh  inst  mev  #|line 161|#)
        )
      (( equal    "2" (slot-value  mev 'port))              #|line 162|#
        (setf (slot-value  inst 'buffer2) (funcall (quote clone_string)  (slot-value (slot-value  mev 'payload) 'v)  #|line 163|#))
        (funcall (quote maybe_stringconcat)   eh  inst  mev  #|line 164|#)
        )
      (( equal    "reset" (slot-value  mev 'port))          #|line 165|#
        (setf (slot-value  inst 'buffer1)  nil)             #|line 166|#
        (setf (slot-value  inst 'buffer2)  nil)             #|line 167|#
        )
      (t                                                    #|line 168|#
        (funcall (quote runtime_error)   (concatenate 'string  "bad mev.port for stringconcat: " (slot-value  mev 'port))  #|line 169|#) #|line 170|#
        )))                                                 #|line 171|#
  )
(defun maybe_stringconcat (&optional  eh  inst  mev)
  (declare (ignorable  eh  inst  mev))                      #|line 173|#
  (cond
    (( and  (not (equal  (slot-value  inst 'buffer1)  nil)) (not (equal  (slot-value  inst 'buffer2)  nil))) #|line 174|#
      (let (( concatenated_string  ""))
        (declare (ignorable  concatenated_string))          #|line 175|#
        (cond
          (( equal    0 (length (slot-value  inst 'buffer1))) #|line 176|#
            (setf  concatenated_string (slot-value  inst 'buffer2)) #|line 177|#
            )
          (( equal    0 (length (slot-value  inst 'buffer2))) #|line 178|#
            (setf  concatenated_string (slot-value  inst 'buffer1)) #|line 179|#
            )
          (t                                                #|line 180|#
            (setf  concatenated_string (+ (slot-value  inst 'buffer1) (slot-value  inst 'buffer2))) #|line 181|# #|line 182|#
            ))
        (funcall (quote send)   eh  ""  concatenated_string  mev  #|line 183|#)
        (setf (slot-value  inst 'buffer1)  nil)             #|line 184|#
        (setf (slot-value  inst 'buffer2)  nil)             #|line 185|#) #|line 186|#
      ))                                                    #|line 187|#
  ) #|  |#                                                  #|line 189|# #|line 190|#
(defparameter projectRoot  ".")                             #|line 191|# #|line 192|#
(defun string_constant_instantiate (&optional  reg  owner  name  template_data  arg)
  (declare (ignorable  reg  owner  name  template_data  arg)) #|line 193|# #|line 194|#
  (let ((name_with_id (funcall (quote gensymbol)   "strconst"  #|line 195|#)))
    (declare (ignorable name_with_id))
    (let (( s  template_data))
      (declare (ignorable  s))                              #|line 196|#
      (cond
        ((not (equal   projectRoot  ""))                    #|line 197|#
          (setf  s (substitute  "_00_"  projectRoot  s)     #|line 198|#) #|line 199|#
          ))
      (return-from string_constant_instantiate (funcall (quote make_leaf)   name_with_id  owner  s  ""  #'string_constant_handler  nil  #|line 200|#)))) #|line 201|#
  )
(defun string_constant_handler (&optional  eh  mev)
  (declare (ignorable  eh  mev))                            #|line 203|#
  (let ((s (slot-value  eh 'instance_data)))
    (declare (ignorable s))                                 #|line 204|#
    (funcall (quote send)   eh  ""  s  mev                  #|line 205|#)) #|line 206|#
  )
(defun fakepipename_instantiate (&optional  reg  owner  name  template_data  arg)
  (declare (ignorable  reg  owner  name  template_data  arg)) #|line 208|#
  (let ((instance_name (funcall (quote gensymbol)   "fakepipe"  #|line 209|#)))
    (declare (ignorable instance_name))
    (return-from fakepipename_instantiate (funcall (quote make_leaf)   instance_name  owner  nil  ""  #'fakepipename_handler  nil  #|line 210|#))) #|line 211|#
  )
(defparameter rand  0)                                      #|line 213|# #|line 214|#
(defun fakepipename_handler (&optional  eh  mev)
  (declare (ignorable  eh  mev))                            #|line 215|# #|line 216|#
  (setf  rand (+  rand  1))
  #|  not very random, but good enough _ ;rand' must be unique within a single run |# #|line 217|#
  (funcall (quote send)   eh  ""  (concatenate 'string  "/tmp/fakepipe"  rand)  mev  #|line 218|#) #|line 219|#
  )                                                         #|line 221|#
(defclass Switch1star_Instance_Data ()                      #|line 222|#
  (
    (state :accessor state :initarg :state :initform  "1")  #|line 223|#)) #|line 224|#

                                                            #|line 225|#
(defun switch1star_reset_handler (&optional  eh)
  (declare (ignorable  eh))                                 #|line 226|#
  (let (( inst (slot-value  eh 'instance_data)))
    (declare (ignorable  inst))                             #|line 227|#
    (setf  inst  (make-instance 'Switch1star_Instance_Data) #|line 228|#)) #|line 229|#
  )
(defun switch1star_instantiate (&optional  reg  owner  name  template_data  arg)
  (declare (ignorable  reg  owner  name  template_data  arg)) #|line 231|#
  (let ((name_with_id (funcall (quote gensymbol)   "switch1*"  #|line 232|#)))
    (declare (ignorable name_with_id))
    (let ((instp  (make-instance 'Switch1star_Instance_Data) #|line 233|#))
      (declare (ignorable instp))
      (return-from switch1star_instantiate (funcall (quote make_leaf)   name_with_id  owner  instp  ""  #'switch1star_handler  #'switch1star_reset_handler  #|line 234|#)))) #|line 235|#
  )
(defun switch1star_handler (&optional  eh  mev)
  (declare (ignorable  eh  mev))                            #|line 237|#
  (let (( inst (slot-value  eh 'instance_data)))
    (declare (ignorable  inst))                             #|line 238|#
    (let ((whichOutput (slot-value  inst 'state)))
      (declare (ignorable whichOutput))                     #|line 239|#
      (cond
        (( equal    "" (slot-value  mev 'port))             #|line 240|#
          (cond
            (( equal    "1"  whichOutput)                   #|line 241|#
              (funcall (quote forward)   eh  "1"  mev       #|line 242|#)
              (setf (slot-value  inst 'state)  "*")         #|line 243|#
              )
            (( equal    "*"  whichOutput)                   #|line 244|#
              (funcall (quote forward)   eh  "*"  mev       #|line 245|#)
              )
            (t                                              #|line 246|#
              (funcall (quote send)   eh  "✗"  "internal error bad state in switch1*"  mev  #|line 247|#) #|line 248|#
              ))
          )
        (( equal    "reset" (slot-value  mev 'port))        #|line 249|#
          (setf (slot-value  inst 'state)  "1")             #|line 250|#
          )
        (t                                                  #|line 251|#
          (funcall (quote send)   eh  "✗"  "internal error bad mevent for switch1*"  mev  #|line 252|#) #|line 253|#
          ))))                                              #|line 254|#
  )
(defclass StringAccumulator ()                              #|line 256|#
  (
    (s :accessor s :initarg :s :initform  "")               #|line 257|#)) #|line 258|#

                                                            #|line 259|#
(defun strcatstar_reset_handler (&optional  eh)
  (declare (ignorable  eh))                                 #|line 260|#
  (setf (slot-value  eh 'instance_data)  (make-instance 'StringAccumulator) #|line 261|#) #|line 262|#
  )
(defun strcatstar_instantiate (&optional  reg  owner  name  template_data  arg)
  (declare (ignorable  reg  owner  name  template_data  arg)) #|line 264|#
  (let ((name_with_id (funcall (quote gensymbol)   "String Concat *"  #|line 265|#)))
    (declare (ignorable name_with_id))
    (let ((instp  (make-instance 'StringAccumulator)        #|line 266|#))
      (declare (ignorable instp))
      (return-from strcatstar_instantiate (funcall (quote make_leaf)   name_with_id  owner  instp  ""  #'strcatstar_handler  #'strcatstar_reset_handler  #|line 267|#)))) #|line 268|#
  )
(defun strcatstar_handler (&optional  eh  mev)
  (declare (ignorable  eh  mev))                            #|line 270|#
  (let (( accum (slot-value  eh 'instance_data)))
    (declare (ignorable  accum))                            #|line 271|#
    (cond
      (( equal    "" (slot-value  mev 'port))               #|line 272|#
        (setf (slot-value  accum 's)  (concatenate 'string (slot-value  accum 's) (slot-value (slot-value  mev 'payload) 'v)) #|line 273|#)
        )
      (( equal    "fini" (slot-value  mev 'port))           #|line 274|#
        (funcall (quote send)   eh  "" (slot-value  accum 's)  mev  #|line 275|#)
        )
      (t                                                    #|line 276|#
        (funcall (quote send)   eh  "✗"  "internal error bad mevent for String Concat *"  mev  #|line 277|#) #|line 278|#
        )))                                                 #|line 279|#
  )
(defun stop_instantiate (&optional  reg  owner  name  template_data  arg)
  (declare (ignorable  reg  owner  name  template_data  arg)) #|line 281|#
  (let ((name_with_id (funcall (quote gensymbol)   "Stop"   #|line 282|#)))
    (declare (ignorable name_with_id))
    (let ((inst  nil))
      (declare (ignorable inst))                            #|line 283|#
      (return-from stop_instantiate (funcall (quote make_leaf)   name_with_id  owner  inst  ""  #'stop_handler  nil  #|line 284|#)))) #|line 285|#
  )
(defun stop_handler (&optional  eh  mev)
  (declare (ignorable  eh  mev))                            #|line 287|#
  (let (( inst (slot-value  eh 'instance_data)))
    (declare (ignorable  inst))                             #|line 288|#
    (let (( parent (slot-value  eh 'owner)))
      (declare (ignorable  parent))                         #|line 289|#
      (let (( s  (concatenate 'string  "   !!! stopping: '"  (concatenate 'string (slot-value  parent 'name)  "'")) #|line 290|#))
        (declare (ignorable  s))
        (format *error-output* "~a~%"  s)                   #|line 291|#
        (format *error-output* "
        ")                                                  #|line 292|#
        (funcall (slot-value  parent 'reset)   parent       #|line 293|#)
        (funcall (quote send)   eh  "" (slot-value (slot-value  mev 'payload) 'v)  mev  #|line 294|#)))) #|line 295|#
  ) #|  all of the the built_in leaves are listed here |#   #|line 297|# #|  future: refactor this such that programmers can pick and choose which (lumps of) builtins are used in a specific project |# #|line 298|# #|line 299|#
(defun initialize_stock_components (&optional  reg)
  (declare (ignorable  reg))                                #|line 300|#
  (funcall (quote register_component)   reg (funcall (quote mkTemplate)   "1then2"  nil  #'deracer_instantiate )  #|line 301|#)
  (funcall (quote register_component)   reg (funcall (quote mkTemplate)   "1→2"  nil  #'deracer_instantiate )  #|line 302|#)
  (funcall (quote register_component)   reg (funcall (quote mkTemplate)   "trash"  nil  #'trash_instantiate )  #|line 303|#)
  (funcall (quote register_component)   reg (funcall (quote mkTemplate)   "🗑️"  nil  #'trash_instantiate )  #|line 304|#)
  (funcall (quote register_component)   reg (funcall (quote mkTemplate)   "🚫"  nil  #'stop_instantiate )  #|line 305|#) #|line 306|# #|line 307|#
  (funcall (quote register_component)   reg (funcall (quote mkTemplate)   "Read Text File"  nil  #'low_level_read_text_file_instantiate )  #|line 308|#)
  (funcall (quote register_component)   reg (funcall (quote mkTemplate)   "Ensure String Datum"  nil  #'ensure_string_datum_instantiate )  #|line 309|#) #|line 310|#
  (funcall (quote register_component)   reg (funcall (quote mkTemplate)   "syncfilewrite"  nil  #'syncfilewrite_instantiate )  #|line 311|#)
  (funcall (quote register_component)   reg (funcall (quote mkTemplate)   "String Concat"  nil  #'stringconcat_instantiate )  #|line 312|#)
  (funcall (quote register_component)   reg (funcall (quote mkTemplate)   "switch1*"  nil  #'switch1star_instantiate )  #|line 313|#)
  (funcall (quote register_component)   reg (funcall (quote mkTemplate)   "String Concat *"  nil  #'strcatstar_instantiate )  #|line 314|#)
  #|  for fakepipe |#                                       #|line 315|#
  (funcall (quote register_component)   reg (funcall (quote mkTemplate)   "fakepipename"  nil  #'fakepipename_instantiate )  #|line 316|#) #|line 317|#
  )
